#include "Core/Actions/ModuleActionHandler.h"

#include <cstddef>

#include "Core/Init/InitTemplateFooter.h"
#include "Core/Init/MatrixModInitService.h"
#include "Core/Init/PatchModuleInitService.h"
#include "Core/MIDI/MasterEditGate.h"
#include "Core/MIDI/MatrixModBusParameterSysExDispatcher.h"
#include "Core/MIDI/PatchParameterSysExDispatcher.h"
#include "Core/Models/ApvtsPatchMapper.h"
#include "Core/Models/PatchModel.h"
#include "Core/Services/ClipboardPasteEnabledResolver.h"
#include "Core/Services/DeviceMemoryLimits.h"
#include "Core/Services/DeviceTypeRegistry.h"
#include "Shared/Definitions/Matrix1000Limits.h"
#include "Shared/Definitions/MatrixDeviceTypes.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"

namespace Core
{

namespace
{
    juce::String patchModuleDisplayName(PatchModuleKind kind)
    {
        using namespace PluginDisplayNames::PatchEditSection;

        switch (kind)
        {
            case PatchModuleKind::Dco1: return Dco1Module::kName;
            case PatchModuleKind::Dco2: return Dco2Module::kName;
            case PatchModuleKind::Env1: return Envelope1Module::kName;
            case PatchModuleKind::Env2: return Envelope2Module::kName;
            case PatchModuleKind::Env3: return Envelope3Module::kName;
            case PatchModuleKind::Lfo1: return Lfo1Module::kName;
            case PatchModuleKind::Lfo2: return Lfo2Module::kName;
        }

        return {};
    }

    void publishFooter(juce::AudioProcessorValueTreeState& apvts,
                       const juce::String& text,
                       const juce::String& severity)
    {
        apvts.state.setProperty("uiMessageText", text, nullptr);
        apvts.state.setProperty("uiMessageSeverity", severity, nullptr);
    }

    juce::String currentInternalPatchLabel(juce::AudioProcessorValueTreeState& apvts)
    {
        const auto deviceType = DeviceTypeRegistry::fromApvtsProperty(
            apvts.state.getProperty(MatrixDeviceTypes::kApvtsPropertyName));
        const auto limits = DeviceMemoryLimits::resolve(deviceType);

        namespace Internal = PluginIDs::PatchManagerSection::InternalPatchesModule::StandaloneWidgets;
        const int bank = static_cast<int>(apvts.state.getProperty(
            Internal::kCurrentBankNumber, limits.minBankNumber()));
        const int patch = static_cast<int>(apvts.state.getProperty(
            Internal::kCurrentPatchNumber, limits.minPatchNumber()));

        return PluginDisplayNames::ClipboardMessages::formatInternalPatchLocation(
            limits.hasBankConcept(), bank, patch);
    }

    juce::String clipboardSourceLabelForWarnings(const ClipboardService& clipboard)
    {
        namespace ClipboardMsg = PluginDisplayNames::ClipboardMessages;

        switch (clipboard.getMode())
        {
            case ClipboardMode::Empty:
                return {};
            case ClipboardMode::Module:
            {
                const auto kind = clipboard.getSourceModuleKind();
                if (! kind.has_value())
                    return {};

                auto label = patchModuleDisplayName(*kind);
                if (clipboard.isEnvelopeShapeOnly())
                    label += " envelope shape";
                return label;
            }
            case ClipboardMode::FullPatch:
            {
                const auto label = clipboard.getFullPatchSourceLabel();
                return label.isNotEmpty() ? label : juce::String("Patch");
            }
            case ClipboardMode::MatrixModulation:
                return ClipboardMsg::kMatrixModulationName;
        }

        return {};
    }
} // namespace

    ModuleActionHandler::ModuleActionHandler(Dependencies dependencies, ActionExecutionHooks hooks)
        : apvts_(dependencies.apvts)
        , patchModel_(dependencies.patchModel)
        , apvtsPatchMapper_(dependencies.apvtsPatchMapper)
        , clipboardService_(dependencies.clipboardService)
        , matrixModInitService_(dependencies.matrixModInitService)
        , masterModuleInitService_(dependencies.masterModuleInitService)
        , patchModuleInitService_(dependencies.patchModuleInitService)
        , patchParameterSysExDispatcher_(dependencies.patchParameterSysExDispatcher)
        , matrixModBusParameterSysExDispatcher_(dependencies.matrixModBusParameterSysExDispatcher)
        , refreshPasteMirrors_(std::move(dependencies.refreshPasteMirrors))
        , hooks_(std::move(hooks))
    {
    }

    bool ModuleActionHandler::hasClipboardPipeline() const
    {
        return clipboardService_ != nullptr
            && patchModel_ != nullptr
            && apvtsPatchMapper_ != nullptr;
    }

    void ModuleActionHandler::handleAction(const juce::String& propertyId, const juce::var&)
    {
        if (handleMatrixModInit(propertyId)) return;
        if (handleMasterModuleInit(propertyId)) return;
        if (handleClipboardCopy(propertyId)) return;
        if (handleClipboardPaste(propertyId)) return;
        if (handlePatchModuleInit(propertyId)) return;
    }

    bool ModuleActionHandler::handleMatrixModInit(const juce::String& propertyId)
    {
        using namespace PluginIDs::MatrixModulationSection;

        if (matrixModInitService_ == nullptr)
            return false;

        const int busIndex = parseMatrixModBusInitIndex(propertyId);
        const bool isSectionInit = propertyId == StandaloneWidgets::kMatrixModulationInit;

        if (!isSectionInit && busIndex < 0)
            return false;

        if (hooks_.setSuppressMatrixModSysEx)
            hooks_.setSuppressMatrixModSysEx(true);

        if (isSectionInit)
            matrixModInitService_->initAllBuses();
        else
            matrixModInitService_->initBus(busIndex);

        if (hooks_.setSuppressMatrixModSysEx)
            hooks_.setSuppressMatrixModSysEx(false);

        return true;
    }

    bool ModuleActionHandler::handleMasterModuleInit(const juce::String& propertyId)
    {
        using namespace PluginIDs::MasterEditSection;

        if (masterModuleInitService_ == nullptr)
            return false;

        std::optional<MasterModuleKind> moduleKind;

        if (propertyId == MidiModule::StandaloneWidgets::kInit)
            moduleKind = MasterModuleKind::kMidi;
        else if (propertyId == VibratoModule::StandaloneWidgets::kInit)
            moduleKind = MasterModuleKind::kVibrato;
        else if (propertyId == MiscModule::StandaloneWidgets::kInit)
            moduleKind = MasterModuleKind::kMisc;

        if (!moduleKind.has_value())
            return false;

        const bool deviceDetected = static_cast<bool>(apvts_.state.getProperty("deviceDetected", false));
        const auto deviceType = DeviceTypeRegistry::fromApvtsProperty(
            apvts_.state.getProperty(MatrixDeviceTypes::kApvtsPropertyName));

        // MASTER EDIT is grayed when unavailable — consume the action with no footer.
        if (! isMasterEditAllowed(deviceDetected, deviceType))
            return true;

        if (hooks_.setSuppressMasterSysEx)
            hooks_.setSuppressMasterSysEx(true);

        const auto result = masterModuleInitService_->initModule(*moduleKind);

        if (hooks_.setSuppressMasterSysEx)
            hooks_.setSuppressMasterSysEx(false);

        propagateInitTemplateFooterMessage(result);
        return true;
    }

    void ModuleActionHandler::clearEnvelopeShapeOnlyFlag()
    {
        apvts_.state.setProperty(PluginIDs::ClipboardFeedback::kCopyEnvelopeShapeOnly, false, nullptr);
    }

    bool ModuleActionHandler::copyFullInternalPatch()
    {
        namespace ClipboardMsg = PluginDisplayNames::ClipboardMessages;

        clearEnvelopeShapeOnlyFlag();
        apvtsPatchMapper_->apvtsToBuffer();
        const auto sourceLabel = currentInternalPatchLabel(apvts_);
        clipboardService_->copyFullPatch(*patchModel_, sourceLabel);
        if (refreshPasteMirrors_)
            refreshPasteMirrors_();
        publishFooter(apvts_, ClipboardMsg::formatPatchCopied(sourceLabel), "info");
        return true;
    }

    bool ModuleActionHandler::copyMatrixModulationSection()
    {
        namespace ClipboardMsg = PluginDisplayNames::ClipboardMessages;

        clearEnvelopeShapeOnlyFlag();
        apvtsPatchMapper_->apvtsToBuffer();
        clipboardService_->copyMatrixModulation(*patchModel_);
        if (refreshPasteMirrors_)
            refreshPasteMirrors_();
        publishFooter(apvts_, ClipboardMsg::formatMatrixModulationCopied(), "info");
        return true;
    }

    bool ModuleActionHandler::copyPatchModule(PatchModuleKind moduleKind, bool shapeOnlyIntent)
    {
        namespace ClipboardMsg = PluginDisplayNames::ClipboardMessages;

        clearEnvelopeShapeOnlyFlag();
        apvtsPatchMapper_->apvtsToBuffer();
        clipboardService_->copyModule(moduleKind, *patchModel_, shapeOnlyIntent);
        if (refreshPasteMirrors_)
            refreshPasteMirrors_();

        const auto moduleName = patchModuleDisplayName(moduleKind);
        if (clipboardService_->isEnvelopeShapeOnly())
            publishFooter(apvts_, ClipboardMsg::formatEnvelopeShapeCopied(moduleName), "info");
        else
            publishFooter(apvts_, ClipboardMsg::formatModuleCopied(moduleName), "info");

        return true;
    }

    bool ModuleActionHandler::handleClipboardCopy(const juce::String& propertyId)
    {
        if (! hasClipboardPipeline())
            return false;

        namespace InternalPatches = PluginIDs::PatchManagerSection::InternalPatchesModule::StandaloneWidgets;
        namespace MatrixMod = PluginIDs::MatrixModulationSection::StandaloneWidgets;

        const bool shapeOnlyIntent = static_cast<bool>(
            apvts_.state.getProperty(PluginIDs::ClipboardFeedback::kCopyEnvelopeShapeOnly, false));

        if (propertyId == InternalPatches::kCopyPatch)
            return copyFullInternalPatch();

        if (propertyId == MatrixMod::kMatrixModulationCopy)
            return copyMatrixModulationSection();

        if (!propertyId.endsWith("Copy"))
            return false;

        const auto moduleKind = patchModuleKindFromWidgetId(propertyId);
        if (!moduleKind.has_value())
        {
            clearEnvelopeShapeOnlyFlag();
            return false;
        }

        return copyPatchModule(*moduleKind, shapeOnlyIntent);
    }

    void ModuleActionHandler::publishPasteBlockedFooter(const juce::String& sourceLabel,
                                                        const juce::String& targetName)
    {
        namespace ClipboardMsg = PluginDisplayNames::ClipboardMessages;
        namespace ShortLabels = PluginDisplayNames::ShortLabels;

        publishFooter(apvts_,
                      clipboardService_->hasContent()
                          ? ClipboardMsg::formatIncompatiblePaste(sourceLabel, targetName)
                          : juce::String(ShortLabels::kNothingToPasteFooter),
                      "warning");
    }

    void ModuleActionHandler::pushMatrixModulationToApvtsAndSysEx()
    {
        if (hooks_.setSuppressMatrixModSysEx)
            hooks_.setSuppressMatrixModSysEx(true);

        for (int bus = 0; bus < Matrix1000Limits::kModulationBusCount; ++bus)
            apvtsPatchMapper_->pushBusToApvts(bus);

        if (hooks_.setSuppressMatrixModSysEx)
            hooks_.setSuppressMatrixModSysEx(false);

        if (matrixModBusParameterSysExDispatcher_ != nullptr)
        {
            for (int bus = 0; bus < Matrix1000Limits::kModulationBusCount; ++bus)
                matrixModBusParameterSysExDispatcher_->dispatchBus(bus);
        }
    }

    void ModuleActionHandler::pushModuleToApvtsAndSysEx(const juce::String& moduleGroupId)
    {
        if (hooks_.setSuppressPatchSysEx)
            hooks_.setSuppressPatchSysEx(true);

        apvtsPatchMapper_->pushModuleToApvts(moduleGroupId);

        if (hooks_.setSuppressPatchSysEx)
            hooks_.setSuppressPatchSysEx(false);

        if (patchParameterSysExDispatcher_ != nullptr)
            patchParameterSysExDispatcher_->dispatchModule(moduleGroupId);
    }

    bool ModuleActionHandler::pasteMatrixModulationSection()
    {
        namespace ClipboardMsg = PluginDisplayNames::ClipboardMessages;
        const auto sourceLabel = clipboardSourceLabelForWarnings(*clipboardService_);

        if (!clipboardService_->canPasteMatrixModulation())
        {
            publishPasteBlockedFooter(sourceLabel, ClipboardMsg::kMatrixModulationName);
            return true;
        }

        if (!clipboardService_->pasteMatrixModulation(*patchModel_))
        {
            publishFooter(apvts_,
                          ClipboardMsg::formatPasteFailed(
                              sourceLabel, ClipboardMsg::kMatrixModulationName),
                          "error");
            return true;
        }

        pushMatrixModulationToApvtsAndSysEx();

        if (hooks_.disarmClipboardFeedback)
            hooks_.disarmClipboardFeedback();

        publishFooter(apvts_, ClipboardMsg::formatMatrixModulationPasted(), "info");
        return true;
    }

    bool ModuleActionHandler::pastePatchModule(PatchModuleKind moduleKind)
    {
        namespace ClipboardMsg = PluginDisplayNames::ClipboardMessages;

        const auto targetName = patchModuleDisplayName(moduleKind);
        const auto sourceLabel = clipboardSourceLabelForWarnings(*clipboardService_);

        if (!clipboardService_->canPasteModule(moduleKind))
        {
            publishPasteBlockedFooter(sourceLabel, targetName);
            return true;
        }

        const bool shapeOnly = clipboardService_->isEnvelopeShapeOnly();
        const auto sourceKind = clipboardService_->getSourceModuleKind();
        const auto sourceName = sourceKind.has_value()
            ? patchModuleDisplayName(*sourceKind)
            : sourceLabel;

        if (!clipboardService_->pasteModule(moduleKind, *patchModel_))
        {
            publishFooter(apvts_,
                          ClipboardMsg::formatPasteFailed(sourceName, targetName),
                          "error");
            return true;
        }

        if (hooks_.disarmClipboardFeedback)
            hooks_.disarmClipboardFeedback();

        if (shapeOnly)
            publishFooter(apvts_,
                          ClipboardMsg::formatEnvelopeShapePasted(sourceName, targetName),
                          "info");
        else
            publishFooter(apvts_,
                          ClipboardMsg::formatModulePasted(sourceName, targetName),
                          "info");

        const auto moduleGroupId = PatchModuleInitService::moduleGroupIdFromPatchModuleKind(moduleKind);
        if (moduleGroupId.isEmpty())
            return true;

        pushModuleToApvtsAndSysEx(moduleGroupId);
        return true;
    }

    bool ModuleActionHandler::handleClipboardPaste(const juce::String& propertyId)
    {
        if (! hasClipboardPipeline())
            return false;

        namespace MatrixMod = PluginIDs::MatrixModulationSection::StandaloneWidgets;

        if (propertyId == MatrixMod::kMatrixModulationPaste)
            return pasteMatrixModulationSection();

        if (!propertyId.endsWith("Paste"))
            return false;

        const auto moduleKind = patchModuleKindFromWidgetId(propertyId);
        if (!moduleKind.has_value())
            return false;

        return pastePatchModule(*moduleKind);
    }

    bool ModuleActionHandler::handlePatchModuleInit(const juce::String& propertyId)
    {
        if (patchModuleInitService_ == nullptr)
            return false;

        const auto moduleGroupId = PatchModuleInitService::moduleGroupIdFromInitPropertyId(propertyId);
        if (moduleGroupId.isEmpty())
            return false;

        if (hooks_.setSuppressPatchSysEx)
            hooks_.setSuppressPatchSysEx(true);

        const auto result = patchModuleInitService_->initModule(moduleGroupId);

        if (hooks_.setSuppressPatchSysEx)
            hooks_.setSuppressPatchSysEx(false);

        propagateInitTemplateFooterMessage(result);
        return true;
    }

    void ModuleActionHandler::propagateInitTemplateFooterMessage(const InitTemplateLoadResult& result)
    {
        InitTemplateFooter::propagateMessage(apvts_, result);
    }

    int ModuleActionHandler::parseMatrixModBusInitIndex(const juce::String& propertyId) const
    {
        using namespace PluginIDs::MatrixModulationSection::ModulationBus::StandaloneWidgets;

        static constexpr const char* kBusInitIds[] = {
            kBus0Init, kBus1Init, kBus2Init, kBus3Init, kBus4Init,
            kBus5Init, kBus6Init, kBus7Init, kBus8Init, kBus9Init
        };

        for (std::size_t i = 0; i < std::size(kBusInitIds); ++i)
            if (propertyId == kBusInitIds[i])
                return static_cast<int>(i);

        return -1;
    }

} // namespace Core
