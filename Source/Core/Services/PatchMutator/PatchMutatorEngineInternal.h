#pragma once

#include "Core/Actions/ActionExecutionHooks.h"
#include "Core/Models/ApvtsPatchMapper.h"
#include "Core/Models/PatchNameSyncer.h"
#include "Core/Services/PatchFileService.h"
#include "Core/Services/PatchMutator/MutationHistoryStore.h"
#include "Core/Services/PatchMutator/PatchMutatorEngine.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

// Free helpers shared by PatchMutatorEngine.cpp and its companion .cpp files.
namespace PatchMutatorEngineInternal
{
    namespace PatchMutator = PluginIDs::PatchManagerSection::PatchMutatorModule::StandaloneWidgets;
    namespace MutatorState = PluginIDs::PatchManagerSection::PatchMutatorModule::StateProperties;
    namespace CompareMessages = PluginDisplayNames::PatchManagerSection::PatchMutatorModule::Messages;

    constexpr const char* kNoOpRecipeFooterMessage = "Set Amount and Random above 0 to mutate.";
    constexpr const char* kNoModuleScopeFooterMessage = "Enable at least one module to mutate.";
    constexpr const char* kNoMutationChangeFooterMessage =
        "Mutation made no changes. Try higher Amount or Random.";
    constexpr const char* kHistoryLimitFooterMessage = "Mutation history is full. Defrag to continue.";
    constexpr const char* kEmptyHistoryFooterMessage = "Mutation history is empty.";
    constexpr const char* kNoSelectionFooterMessage = "No valid mutation history entry selected.";
    constexpr const char* kNoInitialSnapshotFooterMessage =
        "No initial patch snapshot available for compare.";
    constexpr const char* kRootDeleteCascadeFooterPrefix = "Deleted ";
    constexpr const char* kRootDeleteCascadeFooterSuffix = " and all retries.";
    constexpr const char* kDefragCompleteFooterMessage = "Mutation history renumbered.";
    constexpr const char* kExportFolderNotWritableFooterMessage = "Export folder is not writable.";
    constexpr const char* kExportFailedFooterMessage = "Mutation export failed.";
    constexpr const char* kFooterSeverityWarning = "warning";
    constexpr const char* kFooterSeverityInfo = "info";

    inline void clearCompareLockedFooterIfPresent(juce::AudioProcessorValueTreeState& apvts)
    {
        if (apvts.state.getProperty("uiMessageText").toString()
            == juce::String(CompareMessages::kCompareLockedFooter))
        {
            apvts.state.setProperty("uiMessageText", juce::String(), nullptr);
            apvts.state.setProperty("uiMessageSeverity", juce::String(), nullptr);
        }
    }

    inline juce::String formatExportCompleteFooterMessage(int filesWritten)
    {
        return "Exported " + juce::String(filesWritten) + " mutation file(s).";
    }

    inline Core::MutatorActionResult makeExportWarningResult(const char* message)
    {
        Core::MutatorActionResult result;
        result.footerMessage = message;
        result.footerSeverity = kFooterSeverityWarning;
        return result;
    }

    inline Core::MutatorActionResult makeExportHistoryResult(const Core::PatchFileExportResult& exportResult)
    {
        Core::MutatorActionResult result;

        if (! exportResult.success)
        {
            result.footerMessage = exportResult.errorMessage.isNotEmpty()
                ? exportResult.errorMessage
                : juce::String(kExportFailedFooterMessage);
            result.footerSeverity = kFooterSeverityWarning;
            return result;
        }

        result.success = true;
        result.footerMessage = formatExportCompleteFooterMessage(exportResult.filesWritten);
        result.footerSeverity = kFooterSeverityInfo;
        return result;
    }

    class SuppressMutatorHistorySelectionDebounceGuard
    {
    public:
        explicit SuppressMutatorHistorySelectionDebounceGuard(Core::ActionExecutionHooks& hooks)
            : hooks_(hooks)
        {
            if (hooks_.setSuppressMutatorHistorySelectionDebounce)
                hooks_.setSuppressMutatorHistorySelectionDebounce(true);
        }

        ~SuppressMutatorHistorySelectionDebounceGuard()
        {
            if (hooks_.setSuppressMutatorHistorySelectionDebounce)
                hooks_.setSuppressMutatorHistorySelectionDebounce(false);
        }

    private:
        Core::ActionExecutionHooks& hooks_;
    };

    inline void setPatchLoadSuppressHooks(Core::ActionExecutionHooks& hooks, bool suppress)
    {
        if (hooks.setSuppressPatchSysEx)
            hooks.setSuppressPatchSysEx(suppress);

        if (hooks.setSuppressMatrixModSysEx)
            hooks.setSuppressMatrixModSysEx(suppress);
    }

    inline void flushDeferredApvtsParameterSync(juce::AudioProcessorValueTreeState& apvts)
    {
        (void) apvts.copyState();
    }

    // Pipe-separated root/retry labels — e.g. "M00|M02|M05", "—|R00|R02".
    inline juce::String joinLabels(const juce::StringArray& labels)
    {
        return labels.joinIntoString("|");
    }

    inline juce::String historyRootSentinelLabel()
    {
        return PluginDisplayNames::PatchManagerSection::PatchMutatorModule::StandaloneWidgets::kHistoryRootSentinel;
    }

    inline int readPatchMutatorPercent(juce::AudioProcessorValueTreeState& apvts,
                                       const juce::Identifier& propertyId,
                                       int defaultValue)
    {
        auto* rawValue = apvts.getRawParameterValue(propertyId);
        if (rawValue != nullptr)
            return juce::jlimit(0, 100, juce::roundToInt(rawValue->load()));

        return juce::jlimit(0, 100, static_cast<int>(apvts.state.getProperty(propertyId, defaultValue)));
    }

    inline void pushPatchModelToApvtsWithSuppress(juce::AudioProcessorValueTreeState& apvts,
                                                  Core::ActionExecutionHooks& hooks,
                                                  Core::ApvtsPatchMapper& mapper,
                                                  Core::PatchNameSyncer* patchNameSyncer)
    {
        setPatchLoadSuppressHooks(hooks, true);
        mapper.bufferToApvts();

        if (patchNameSyncer != nullptr)
            patchNameSyncer->bufferToApvts();

        flushDeferredApvtsParameterSync(apvts);
        setPatchLoadSuppressHooks(hooks, false);
    }

    inline bool computeMutateEnabled(const Core::MutationHistoryStore& store,
                                     const Core::MutationRecipe& recipe)
    {
        return store.peekNextRootIndex().has_value() && recipe.hasAnyModuleEnabled();
    }

    inline bool computeExportEnabled(const Core::MutationHistoryStore& store)
    {
        return ! store.isEmpty();
    }

    inline bool computeDeleteEnabled(const Core::MutationHistoryStore& store, int selectedRoot)
    {
        return ! store.isEmpty() && selectedRoot >= 0;
    }

    inline bool computeRetryEnabled(const Core::MutationHistoryStore& store, int selectedRoot)
    {
        if (store.isEmpty() || selectedRoot < 0)
            return false;

        return store.peekNextRetryIndex(selectedRoot).has_value();
    }
} // namespace PatchMutatorEngineInternal
