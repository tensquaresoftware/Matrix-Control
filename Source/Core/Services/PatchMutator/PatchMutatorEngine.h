#pragma once

#include <functional>

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

#include "Core/Actions/ActionExecutionHooks.h"
#include "Core/Services/PatchMutator/MutationAlgorithm.h"
#include "Core/Services/PatchMutator/MutationHistoryStore.h"
#include "Core/Services/PatchMutator/PatchLoadContext.h"
#include "Core/Services/DeviceMemoryLimits.h"

class MidiManager;
class SysExEncoder;

class PatchMutatorEngineTestAccess;

namespace Core
{
    class ApvtsPatchMapper;
    class PatchFileService;
    class PatchModel;
    class PatchNameSyncer;

    struct MutatorActionResult
    {
        bool success = false;
        juce::String footerMessage;
        juce::String footerSeverity;
        bool defragModalRequested = false;
        // Export target session folder already exists — caller must resolve via modal
        // (Overwrite / Keep / Cancel) then call exportHistoryResolved().
        bool exportCollisionModalRequested = false;
    };

    enum class ExportCollisionResolution
    {
        kOverwrite,
        kKeep,
        kCancel
    };

    // User's choice in the pre-patch-context-change history gate (Export / Cancel / Discard).
    enum class MutatorHistoryGateChoice
    {
        kExport,
        kCancel,
        kDiscard
    };

    class PatchMutatorEnginePort
    {
    public:
        virtual ~PatchMutatorEnginePort() = default;

        virtual MutatorActionResult mutate() = 0;
        virtual MutatorActionResult retry() = 0;
        virtual MutatorActionResult toggleCompare() = 0;
        virtual MutatorActionResult deleteSelected() = 0;
        virtual MutatorActionResult clearHistory() = 0;
        virtual MutatorActionResult exportHistory(const juce::File& destinationFolder) = 0;
        virtual MutatorActionResult exportHistoryResolved(const juce::File& destinationFolder,
                                                          ExportCollisionResolution resolution) = 0;
        virtual MutatorActionResult defragHistory() = 0;
        virtual void auditionSelectedHistoryEntry() = 0;
        virtual void rebuildHistoryListMirrors() = 0;
        virtual void advanceHistorySelection(bool isNext) = 0;
    };

    class PatchMutatorEngine final : public PatchMutatorEnginePort
    {
    public:
        struct Dependencies
        {
            PatchModel* patchModel = nullptr;
            ApvtsPatchMapper* apvtsPatchMapper = nullptr;
            PatchNameSyncer* patchNameSyncer = nullptr;
            MidiManager* midiManager = nullptr;
            juce::AudioProcessorValueTreeState& apvts;
            std::function<int()> getCurrentPatchNumber;
            std::function<DeviceMemoryLimits()> getDeviceMemoryLimits;
            PatchFileService* patchFileService = nullptr;
            SysExEncoder* sysExEncoder = nullptr;
        };

        PatchMutatorEngine(Dependencies dependencies, ActionExecutionHooks hooks);

        MutatorActionResult mutate() override;
        MutatorActionResult retry() override;
        MutatorActionResult toggleCompare() override;
        MutatorActionResult deleteSelected() override;
        MutatorActionResult clearHistory() override;
        MutatorActionResult resetSessionForPatchLoad();
        MutatorActionResult exportHistory(const juce::File& destinationFolder) override;
        // Second phase of export after a collision modal chose Overwrite / Keep / Cancel.
        MutatorActionResult exportHistoryResolved(const juce::File& destinationFolder,
                                                  ExportCollisionResolution resolution) override;
        MutatorActionResult defragHistory() override;

        // Supplies the current PatchLoadContext (device vs computer file) so the engine can
        // freeze the Export folder basename on the first MUTATE. Owned by PluginProcessor.
        void setPatchLoadContextProvider(std::function<PatchLoadContext()> provider);

        // Recomputes the frozen Export folder basename with the new patch name after a rename.
        // No-op when no basename has been frozen yet (no MUTATE performed in this session).
        void refreshFrozenExportBasename(const juce::String& newPatchName);

        void auditionSelectedHistoryEntry() override;
        void rebuildHistoryListMirrors() override;
        void advanceHistorySelection(bool isNext) override;
        void syncHistoryUiProperties(juce::AudioProcessorValueTreeState& apvts); // Story 6.6
        void refreshActionEnabledMirrors(juce::AudioProcessorValueTreeState& apvts); // Story 6.12

        void setAuditionSelection(int rootIndex, int retryIndex = MutationHistoryStore::kRootOnly);

        int rootCount() const noexcept;
        std::optional<MutationEntry> getEntry(int rootIndex,
                                              int retryIndex = MutationHistoryStore::kRootOnly) const;

    private:
        friend class ::PatchMutatorEngineTestAccess;

        // Reserved flat-navigation slot for the session origin snapshot (HISTORY INITIAL row).
        // Never a store index — MutationHistoryStore roots start at 0.
        static constexpr int kInitialFlatRootIndex = -2;

        struct FlatHistoryEntry
        {
            int rootIndex = -1;
            int retryIndex = MutationHistoryStore::kRootOnly;

            bool isInitial() const noexcept { return rootIndex == kInitialFlatRootIndex; }
        };

        static MutatorActionResult makeWarningResult(const char* message);
        static MutatorActionResult makeHistoryLimitResult();
        static MutatorActionResult makeSuccessResult();

        MutationRecipe buildRecipeFromApvts() const;
        std::optional<MutatorActionResult> validateMutationRecipe(const MutationRecipe& recipe) const;
        std::optional<MutatorActionResult> validateRetryPreconditions(int& outRootIndex) const;
        bool applyRecipeMutation(PatchModel& working, const MutationRecipe& recipe);

        enum class DiverseMutationOutcome
        {
            kApplied,
            kNoUsableRoll,
            kTooSimilar
        };

        // Re-rolls a few times so a second try on the same parent is not a near-clone.
        DiverseMutationOutcome applyDiverseRecipeMutation(PatchModel& working,
                                                          const MutationRecipe& recipe,
                                                          const PatchModel& previousResult);
        static const char* footerForDiverseMutation(DiverseMutationOutcome outcome) noexcept;
        // resolveAuditionBuffer: empty history -> live editor; selectedRootIndex_ < 0 -> highest sorted root
        // (root-only); missing entry at selection -> root-only entry or live editor fallback.
        // MUTATE-only — returns selected entry result, not parentSnapshot (D-083).
        PatchModel resolveAuditionBuffer();
        // RETRY-only — resolves Mi from selection with same root fallback as resolveAuditionBuffer.
        std::optional<int> resolveSelectedRootIndex() const;
        // RETRY-only — returns selected entry for parentSnapshot input; differs from audition semantics (D-083).
        std::optional<MutationEntry> resolveSelectedEntryForRetry(int rootIndex) const;
        void pushResultToEditorAndSynth(const PatchModel& mutatedModel);
        // True when `candidate`, once stamped with the live model's name (the same stamp
        // pushResultToEditorAndSynth would apply), differs from the live model buffer.
        // Callers use this instead of a raw memcmp so name stamping cannot defeat the
        // duplicate-SysEx guard.
        bool candidateDiffersFromLive(const PatchModel& candidate) const;
        void freezeExportBasename(const PatchModel& snapshot);
        MutatorActionResult runSessionExport(const juce::File& sessionFolder, bool clearExisting);
        void applySelectionFromApvts();
        void forceExitCompare();
        MutatorActionResult exitCompareMode();
        MutatorActionResult enterCompareMode();
        std::pair<int, int> resolveSelectionAfterDelete(int rootIndex,
                                                        int retryIndex,
                                                        bool isRetryDelete);
        void auditionAfterHistoryMutation();
        juce::Array<FlatHistoryEntry> buildFlatHistoryEntries() const;
        static int indexOfFlatHistoryEntry(const juce::Array<FlatHistoryEntry>& flat,
                                           int rootIndex,
                                           int retryIndex);
        void commitHistorySelectionToApvts();
        // HISTORY INITIAL row is offered only while the origin snapshot can be auditioned
        // alongside at least one mutation.
        bool canOfferInitialSelection() const;
        void clearEmptyHistoryUi(juce::AudioProcessorValueTreeState& apvts);
        void writeHistoryListMirrors(juce::ValueTree& state);
        void writeSelectedHistoryUi(juce::ValueTree& state, const juce::Array<int>& roots);
        static bool readBoolProperty(const juce::ValueTree& state,
                                     const juce::Identifier& propertyId,
                                     bool defaultValue);

        PatchModel* patchModel_;
        ApvtsPatchMapper* apvtsPatchMapper_;
        PatchNameSyncer* patchNameSyncer_;
        MidiManager* midiManager_;
        juce::AudioProcessorValueTreeState& apvts_;
        ActionExecutionHooks hooks_;
        std::function<int()> getCurrentPatchNumber_;
        std::function<DeviceMemoryLimits()> getDeviceMemoryLimits_;
        PatchFileService* patchFileService_ = nullptr;
        SysExEncoder* sysExEncoder_ = nullptr;
        std::function<PatchLoadContext()> patchLoadContextProvider_;

        MutationHistoryStore historyStore_;
        MutationAlgorithm algorithm_;
        juce::Random rng_;
        int selectedRootIndex_ = -1;
        int selectedRetryIndex_ = MutationHistoryStore::kRootOnly;
        // HISTORY INITIAL selected: origin snapshot auditioned while selectedRootIndex_ /
        // selectedRetryIndex_ keep the mutation to come back to. Independent of kCompareActive.
        bool initialSelected_ = false;
        int compareSavedMutateRootIndex_ = -1;
        int compareSavedRetryIndex_ = MutationHistoryStore::kRootOnly;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchMutatorEngine)
    };

} // namespace Core
