// Extracted from PluginProcessor.cpp for modular maintenance.
// ValueTree::Listener callbacks, their per-domain dispatch helpers, and session logging.

#include "PluginProcessor.h"
#include "PluginProcessorInternal.h"

#include "Core/Actions/ActionDispatcher.h"
#include "Core/Actions/ActionPropertyRegistry.h"
#include "Core/Actions/MutatorActionHandler.h"
#include "Core/MIDI/MasterParameterSysExDispatcher.h"
#include "Core/MIDI/MatrixModBusParameterSysExDispatcher.h"
#include "Core/MIDI/MatrixModBusReorderService.h"
#include "Core/MIDI/PatchParameterSysExDispatcher.h"
#include "Core/Models/ApvtsMasterMapper.h"
#include "Core/Models/ApvtsPatchMapper.h"
#include "Core/Models/PatchNameSyncer.h"
#include "Core/Services/PatchMutator/MutatorSessionPersistence.h"
#include "Core/Services/PatchMutator/PatchMutatorEngine.h"
#include "Loggers/ApvtsLogger.h"
#include "Loggers/MidiLogger.h"
#include "Shared/Definitions/MatrixDeviceTypes.h"
#include "Shared/Definitions/PluginIDs.h"

using namespace PluginProcessorInternal;

void PluginProcessor::ensureDevelopmentLoggingStarted()
{
    if (!shouldUseDevelopmentLogging() || developmentLoggingStarted_)
        return;

    developmentLoggingStarted_ = true;
    enableFileLoggingForSession();
    enableApvtsLogging();
}

void PluginProcessor::enableFileLoggingForSession()
{
    MidiLogger::getInstance().setLogLevel(MidiLogger::LogLevel::kDebug);
    MidiLogger::getInstance().setLogToFile(true);
}

void PluginProcessor::closeLogFileForSession()
{
    MidiLogger::getInstance().setLogToFile(false);
}

void PluginProcessor::enableApvtsLogging()
{
    ApvtsLogger::getInstance().setLogLevel(ApvtsLogger::LogLevel::kDebug);
    ApvtsLogger::getInstance().setLogToConsole(true);
    ApvtsLogger::getInstance().setLogToFile(true);
    ApvtsLogger::getInstance().logInfo("APVTS logging enabled");
}

void PluginProcessor::disableApvtsLogging()
{
    ApvtsLogger::getInstance().setLogToFile(false);
}

juce::String PluginProcessor::getThreadNameForLogging() const
{
    juce::String threadName;
    if (juce::Thread::getCurrentThread() != nullptr)
        threadName = juce::Thread::getCurrentThread()->getThreadName();
    else if (juce::MessageManager::getInstance()->isThisTheMessageThread())
        threadName = "MessageThread";
    else
        threadName = "Unknown";
    return simplifyThreadNameForLogging(threadName);
}

juce::String PluginProcessor::simplifyThreadNameForLogging(const juce::String& threadName)
{
    if (threadName == "MessageThread")
        return "Message";
    if (threadName.startsWith("Audio"))
        return "Audio";
    if (threadName.startsWith("MIDI") || threadName.startsWith("Midi"))
        return "MIDI";
    return threadName;
}

void PluginProcessor::swapMatrixModBusContents(int fromBus, int toBus)
{
    if (matrixModBusReorderService_ == nullptr)
        return;

    suppressMatrixModParameterSysEx_ = true;
    matrixModBusReorderService_->swapBusContents(fromBus, toBus);
    suppressMatrixModParameterSysEx_ = false;
}

void PluginProcessor::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged,
                                              const juce::Identifier& property)
{
    juce::var newValue = treeWhosePropertyHasChanged.getProperty(property);
    juce::String threadName = getThreadNameForLogging();
    juce::String parameterId = resolveParameterIdFromTree(treeWhosePropertyHasChanged, property);
    juce::String choiceLabel = getChoiceLabelForNumericValue(parameterId, newValue);

    if (shouldUseDevelopmentLogging())
    {
        ApvtsLogger::getInstance().logValueTreePropertyChanged({
            juce::Identifier(parameterId),
            juce::var(),
            newValue,
            threadName,
            choiceLabel
        });
    }

    dispatchPatchOrMatrixModParameterChange(parameterId);
    dispatchMasterParameterChange(parameterId);

    if (parameterId == PluginIDs::PatchEditSection::PatchNameModule::kPatchName)
        patchNameSyncer_->apvtsToBuffer();

    handleBankNumberChange(parameterId);
    handlePatchNumberChange(parameterId);

    if (Core::ActionPropertyRegistry::isActionProperty(parameterId))
        actionDispatcher_->onActionPropertyChanged(parameterId, newValue);

    dispatchMutatorHistorySelectionChange(parameterId);

    if (patchMutatorEngine_ != nullptr
        && Core::MutatorSessionPersistence::isRecipeModuleEnableProperty(parameterId))
    {
        patchMutatorEngine_->refreshActionEnabledMirrors(apvts);
    }

    handleDeviceTypePropertyChange(property.toString());
}

void PluginProcessor::dispatchPatchOrMatrixModParameterChange(const juce::String& parameterId)
{
    if (patchParameterIds_.count(parameterId) == 0)
        return;

    if (matrixModParameterIds_.count(parameterId) > 0)
    {
        if (suppressMatrixModParameterSysEx_)
            return;

        apvtsPatchMapper_->apvtsToBuffer();
        matrixModSysExCoalesceTimer_->noteParameterChanged(parameterId);
        return;
    }

    if (suppressPatchParameterSysEx_)
        return;

    apvtsPatchMapper_->apvtsToBuffer();
    patchParameterSysExDispatcher_->dispatch(parameterId);
}

void PluginProcessor::dispatchMasterParameterChange(const juce::String& parameterId)
{
    if (masterParameterIds_.count(parameterId) == 0)
        return;

    if (suppressMasterParameterSysEx_)
        return;

    apvtsMasterMapper_->apvtsToBuffer();

    if (isMasterEditOutboundAllowed())
        masterParameterSysExDispatcher_->dispatch(parameterId);
}

void PluginProcessor::dispatchMutatorHistorySelectionChange(const juce::String& parameterId)
{
    namespace MutatorState = PluginIDs::PatchManagerSection::PatchMutatorModule::StateProperties;

    if (suppressMutatorHistorySelectionDebounce_)
        return;

    // INITIAL toggles the audition target without changing M/R, so it also rebuilds the
    // mirrors (RETRY / DELETE enablement).
    const bool isInitialSelection = parameterId == MutatorState::kInitialSelected;
    const bool rebuildsMirrors = isInitialSelection
                                 || parameterId == MutatorState::kSelectedMutateRootIndex;

    if (! rebuildsMirrors && parameterId != MutatorState::kSelectedRetryIndex)
        return;

    if (mutatorActionHandler_ != nullptr)
        mutatorActionHandler_->onHistorySelectionChanged(rebuildsMirrors);
}

void PluginProcessor::handleDeviceTypePropertyChange(const juce::String& propertyName)
{
    if (propertyName != MatrixDeviceTypes::kApvtsPropertyName && propertyName != "deviceDetected")
        return;

    reconcilePatchManagerCoordinatesForDeviceType();
    applyPreferredStandaloneAudioFromForDeviceType();
}

void PluginProcessor::valueTreeChildAdded(juce::ValueTree& parentTree,
                                         juce::ValueTree& childWhichHasBeenAdded)
{
    juce::ignoreUnused(parentTree, childWhichHasBeenAdded);

    if (shouldUseDevelopmentLogging())
        ApvtsLogger::getInstance().logDebug("ValueTree child added: " + childWhichHasBeenAdded.getType().toString());
}

void PluginProcessor::valueTreeChildRemoved(juce::ValueTree& parentTree,
                                           juce::ValueTree& childWhichHasBeenRemoved,
                                           int indexFromWhichChildWasRemoved)
{
    juce::ignoreUnused(parentTree, childWhichHasBeenRemoved, indexFromWhichChildWasRemoved);

    if (shouldUseDevelopmentLogging())
        ApvtsLogger::getInstance().logDebug("ValueTree child removed: " + childWhichHasBeenRemoved.getType().toString());
}

void PluginProcessor::valueTreeChildOrderChanged(juce::ValueTree& parentTreeWhoseChildrenHaveChanged,
                                                int oldIndex, int newIndex)
{
    juce::ignoreUnused(parentTreeWhoseChildrenHaveChanged, oldIndex, newIndex);

    if (shouldUseDevelopmentLogging())
        ApvtsLogger::getInstance().logDebug("ValueTree child order changed");
}

void PluginProcessor::valueTreeParentChanged(juce::ValueTree& treeWhoseParentHasChanged)
{
    juce::ignoreUnused(treeWhoseParentHasChanged);

    if (shouldUseDevelopmentLogging())
        ApvtsLogger::getInstance().logDebug("ValueTree parent changed");
}

void PluginProcessor::valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged)
{
    juce::ignoreUnused(treeWhichHasBeenChanged);

    if (shouldUseDevelopmentLogging())
        ApvtsLogger::getInstance().logStateReplaced();

    apvtsPatchMapper_->apvtsToBuffer();
    apvtsMasterMapper_->apvtsToBuffer();
    patchNameSyncer_->apvtsToBuffer();
    syncAudioRuntimeFromState();
    refreshClipboardPasteEnabledProperties();
    disarmClipboardFeedbackSession();
    applyPreferredStandaloneAudioFromForDeviceType();
}
