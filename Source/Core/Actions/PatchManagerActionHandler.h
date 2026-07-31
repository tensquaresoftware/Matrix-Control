#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>

#include "Core/Actions/ActionExecutionHooks.h"
#include "Core/Actions/BankTransferProgressPresenter.h"
#include "Core/Actions/IActionHandler.h"
#include "Core/Models/PatchModel.h"
#include "Core/Services/DeviceMemoryLimits.h"
#include "Core/Services/PatchFileNameReconciler.h"
#include "Core/Services/PatchNameOverlayStore.h"
#include "Shared/Definitions/MatrixDeviceTypes.h"

class MidiManager;
class SysExEncoder;

namespace Core
{

    class ApvtsPatchMapper;
    class ClipboardService;
    class DirtyPatchTracker;
    class PatchFileService;
    class PatchInitService;
    class PatchNameSyncer;
    class PatchSelectionMidiSync;

    class PatchManagerActionHandler final : public IActionHandler
    {
    public:
        using DeviceMemoryLimitsSupplier = std::function<DeviceMemoryLimits()>;
        using PatchFolderPicker = std::function<juce::File()>;
        using PatchSaveFilePicker = std::function<juce::File(juce::File suggestedFolder,
                                                             juce::String suggestedStem)>;
        using PatchNameReconciliationPicker = PatchFileNameReconciler::Picker;

        // Bank Utility IMPORT/EXPORT: OS folder pickers (reuse the PatchFolderPicker shape) and
        // an ordered Cancel/Continue confirm gate for IMPORT. See BankTransferProgressPresenter.h
        // for the progress-modal presenter type shared with PluginProcessor.
        using BankImportConfirmGate = std::function<bool()>;

        PatchManagerActionHandler(juce::AudioProcessorValueTreeState& apvts,
                                  DeviceMemoryLimitsSupplier deviceMemoryLimits,
                                  PatchModel* patchModel,
                                  ApvtsPatchMapper* apvtsPatchMapper,
                                  ClipboardService* clipboardService,
                                  PatchInitService* patchInitService,
                                  PatchSelectionMidiSync* patchSelectionMidiSync,
                                  MidiManager* midiManager,
                                  PatchFileService* patchFileService,
                                  PatchNameSyncer* patchNameSyncer,
                                  DirtyPatchTracker* dirtyPatchTracker,
                                  SysExEncoder* sysExEncoder,
                                  PatchFolderPicker pickFolder,
                                  PatchSaveFilePicker pickSaveFile,
                                  PatchNameReconciliationPicker pickNameReconciliation,
                                  ActionExecutionHooks hooks);

        void handleAction(const juce::String& propertyId, const juce::var& newValue) override;

        // Bank Utility IMPORT/EXPORT wiring — optional; empty pickers/gates make the buttons no-op.
        void setBankExportFolderPicker(PatchFolderPicker picker);
        void setBankImportFolderPicker(PatchFolderPicker picker);
        void setBankImportConfirmGate(BankImportConfirmGate gate);
        void setBankExportOverwriteConfirmGate(BankImportConfirmGate gate);
        void setBankTransferProgressPresenter(BankTransferProgressPresenter presenter);

        void rescanPersistedComputerPatchesFolder();
        void resetComputerPatchesBrowserAfterSessionLoad();
        // Drops in-memory scan before APVTS replaceState so valueTreeRedirected cannot flash a stale list.
        void discardComputerPatchesScanCacheQuietly();

        // Re-resolve the editor Patch Name from the last device dump (or a synthesized BNK
        // placeholder on banked devices) using the current Patch Name Display Settings mode.
        void reapplyDisplayedPatchName();

        // Persist a musical overlay for the current Internal bank/patch when the name is usable
        // and not an Oberheim BNK placeholder (inline rename / STORE paths).
        void rememberCurrentOverlayFromModel();

        // Requests the current patch from the synth (async dump) and mirrors it into the editor
        // (PatchModel + APVTS) as a patch load. Rolls back Internal coordinates on failure.
        // Prefer the prior-coordinates overload when APVTS / lock state were already advanced
        // (NumberBox) so failure can restore the true pre-navigation values.
        void loadCurrentPatchFromDevice(const DeviceMemoryLimits& limits);
        void loadCurrentPatchFromDevice(const DeviceMemoryLimits& limits,
                                        int priorBank,
                                        int priorPatch,
                                        int priorSelectedBank,
                                        bool priorBanksLocked);

    private:
        enum class PatchNameResolvePurpose
        {
            kDisplay,
            kExportMusical
        };
        struct InternalCoordinatesSnapshot
        {
            int bank = 0;
            int patch = 0;
            int selectedBank = 0;
            bool banksLocked = false;
        };

        struct PendingDeviceLoad
        {
            std::uint64_t generation = 0;
            InternalCoordinatesSnapshot priorCoordinates;
            std::array<juce::uint8, PatchModel::kBufferSize> bufferAtRequest {};
        };

        // Returns true when the pending patch-context change may proceed.
        // `includeUnsavedEditWarning` selects FR-51 + history (navigation) vs history-only (INIT/PASTE).
        bool confirmPatchContextChange(bool includeUnsavedEditWarning = true);
        void captureCleanSnapshot();
        void revertComputerPatchesSelectionIfNeeded(int previousSelectedId);
        void rememberComputerPatchesSelection(int selectedId);
        void seedCommittedComputerPatchesSelectionIfNeeded();
        void restoreComputerPatchesBrowser(const juce::String& folderPath, int selectedId);
        void abortComputerPatchesNavigation();
        // Returns the id written to APVTS, or nullopt when navigation was a no-op.
        std::optional<int> advanceComputerPatchesSelection(bool isNext);
        void applyPatchCoordinates(const PatchCoordinates& coordinates, const DeviceMemoryLimits& limits);
        InternalCoordinatesSnapshot captureInternalCoordinates(const DeviceMemoryLimits& limits) const;
        void restoreInternalCoordinates(const InternalCoordinatesSnapshot& snapshot,
                                        const DeviceMemoryLimits& limits);
        void beginPendingDeviceLoad(const InternalCoordinatesSnapshot& priorCoordinates);
        void clearPendingDeviceLoad();
        void abandonPendingDeviceLoad();
        void failPendingDeviceLoad(const DeviceMemoryLimits& limits, const juce::String& footerMessage);
        int resolveComputerPatchesCancelRevertId() const;
        void noteStableComputerPatchesSelection(int selectedId);
        bool isDeviceDumpAvailable() const;
        void requestDeviceDump(juce::uint8 patchNumber, ActionExecutionHooks::DeviceDumpCallback onResult);
        void handleUnlockBank(const DeviceMemoryLimits& limits);
        void markBanksLockedInApvts();
        void handleInternalPatchInit();
        void handleInternalPatchPaste(const DeviceMemoryLimits& limits);
        void handleInternalPatchStore(const DeviceMemoryLimits& limits);
        void handleOpenPatchFolder(const DeviceMemoryLimits& limits);
        void handleSavePatchAs();
        void handleSavePatchFile();
        void handleLoadSelectedPatchFile(const DeviceMemoryLimits& limits);
        struct SelectedPatchFileResolution
        {
            enum class Kind
            {
                kSilentNoOp,
                kFailed,
                kOk
            };

            Kind kind = Kind::kSilentNoOp;
            juce::File file;
            juce::String failureMessage;
        };

        int readComputerPatchesSelectedId() const;
        bool isComputerPatchesScanCurrent() const;
        juce::File fileAtComputerPatchesIndex(int index) const;
        SelectedPatchFileResolution resolveSelectedPatchFileForLoad() const;
        SelectedPatchFileResolution makeLoadFailedResolution(const juce::String& message) const;
        bool canExecutePatchLoad() const;
        bool loadPackedPatchFromFile(const juce::File& file, juce::uint8* packedOut);
        PatchNameReconciliationResult reconcileLoadedPatchName(const juce::File& file);
        std::optional<PatchNameReconciliationResult> decodeAndReconcilePatchFile(const juce::File& file);
        void syncLoadedPatchToApvts();
        void applyLoadedPatchToApvtsAndSynth(const DeviceMemoryLimits& limits);
        void publishLoadFooters(const juce::String& fileName,
                                  const PatchNameReconciliationResult& reconciliation);
        void publishLoadFailureFooter(const juce::String& message);
        void saveCurrentPatchToFile(const juce::File& targetFile);
        void completeSuccessfulSave(const juce::String& savedFileName);
        void rescanAndSelectSavedFile(const juce::String& savedFileName);
        juce::File resolveRescanFolder() const;
        juce::File resolveDefaultSaveFolder() const;
        juce::String resolveSuggestedSaveStem() const;
        void scanAndPublishFolder(const juce::File& folder);
        void clearPublishedScanCache();
        void bumpScanRevision();
        void publishSaveSuccessFooter(const juce::String& fileName);
        void publishSaveFailureFooter(const juce::String& message);
        void propagateRomBlockedFooter();
        int getCurrentBank(const DeviceMemoryLimits& limits) const;
        int getCurrentPatch(const DeviceMemoryLimits& limits) const;
        int parseBankButtonIndex(const juce::String& propertyId) const;

        // Bank Utility EXPORT/IMPORT orchestration (live MIDI dump/write, cancellable).
        using PackedPatchBuffer = std::array<juce::uint8, PatchModel::kBufferSize>;

        struct BankTransferState
        {
            enum class Kind
            {
                kNone,
                kExport,
                kImport
            };

            Kind kind = Kind::kNone;
            std::uint64_t generation = 0;
            bool cancelRequested = false;
            int totalSlots = 0;
            int completedSlots = 0;
            DeviceMemoryLimits limits { DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kUnknown) };
            int bank = 0;
            bool hasBankConcept = false;

            // EXPORT-only.
            juce::File targetFolder;
            juce::String childFolderDisplayName;
            juce::StringArray filesCreatedThisRun; // only paths that did not exist before this run
            bool createdTargetFolderThisRun = false;

            // IMPORT-only.
            int importFoundCount = 0;
            int importValidCount = 0;
            int importWrittenCount = 0;
            std::vector<PackedPatchBuffer> importPatches;
            std::vector<PackedPatchBuffer> deviceSnapshot;
            bool isRestoring = false;
            juce::String pendingFooterMessage;
            juce::String pendingFooterSeverity;
        };

        bool isBankTransferBusy() const noexcept;
        int getSelectedBankForTransfer(const DeviceMemoryLimits& limits) const;
        void handleBankExport(const DeviceMemoryLimits& limits);
        void handleBankImport(const DeviceMemoryLimits& limits);
        void exportNextSlot(int slot, std::uint64_t generation);
        void finishBankExport(bool success, const juce::String& footerMessage, const juce::String& severity);
        void beginBankImportSnapshot(std::uint64_t generation);
        void snapshotNextImportSlot(int slot, std::uint64_t generation);
        void beginBankImportWrite(std::uint64_t generation);
        void writeNextImportSlot(int slot, std::uint64_t generation);
        void beginBankImportRestore(std::uint64_t generation, const juce::String& footerMessage, const juce::String& severity);
        void restoreNextSnapshotSlot(int slot, std::uint64_t generation);
        void finishBankImport(const juce::String& footerMessage, const juce::String& severity);
        void requestBankTransferCancel(std::uint64_t generation);
        void publishBankTransferFooter(const juce::String& message, const juce::String& severity);
        int bankTransferWriteDelayMs() const;
        void loadPatchNameOverlayFromApvts();
        void reloadPatchNameOverlayFromApvts();
        void persistPatchNameOverlayToApvts();
        void rememberOverlayName(int bank, int patch, const juce::String& name);
        void applyResolvedPatchName(PatchModel& model,
                                    int bank,
                                    int patch,
                                    const DeviceMemoryLimits& limits,
                                    PatchNameResolvePurpose purpose);

        BankTransferState bankTransfer_;
        std::uint64_t bankTransferGeneration_ = 0;
        PatchFolderPicker bankExportFolderPicker_;
        PatchFolderPicker bankImportFolderPicker_;
        BankImportConfirmGate bankImportConfirmGate_;
        BankImportConfirmGate bankExportOverwriteConfirmGate_;
        BankTransferProgressPresenter bankTransferProgress_;
        PatchNameOverlayStore patchNameOverlay_;
        bool patchNameOverlayLoaded_ = false;
        juce::String lastDeviceDumpRawName_;
        int lastDeviceDumpBank_ = -1;
        int lastDeviceDumpPatch_ = -1;
        bool hasLastDeviceDumpRawName_ = false;

        void clearLastDeviceDumpRawName();

        juce::AudioProcessorValueTreeState& apvts_;
        DeviceMemoryLimitsSupplier deviceMemoryLimits_;
        PatchModel* patchModel_;
        ApvtsPatchMapper* apvtsPatchMapper_;
        ClipboardService* clipboardService_;
        PatchInitService* patchInitService_;
        PatchSelectionMidiSync* patchSelectionMidiSync_;
        MidiManager* midiManager_;
        PatchFileService* patchFileService_;
        PatchNameSyncer* patchNameSyncer_;
        DirtyPatchTracker* dirtyPatchTracker_;
        SysExEncoder* sysExEncoder_;
        PatchFolderPicker pickFolder_;
        PatchSaveFilePicker pickSaveFile_;
        PatchNameReconciliationPicker pickNameReconciliation_;
        ActionExecutionHooks hooks_;
        int lastCommittedComputerPatchesSelectedId_ = 0;
        int lastStableComputerPatchesSelectedId_ = 0;
        bool suppressComputerPatchesSelectLoad_ = false;
        std::uint64_t deviceLoadGeneration_ = 0;
        std::optional<PendingDeviceLoad> pendingDeviceLoad_;

        // When OPEN replaces the browser then Cancel/fails the auto-load, restore this snapshot.
        struct ComputerPatchesBrowserSnapshot
        {
            juce::String folderPath;
            int selectedId = 0;
        };

        std::optional<ComputerPatchesBrowserSnapshot> pendingBrowserRestoreOnCancel_;

        JUCE_DECLARE_WEAK_REFERENCEABLE(PatchManagerActionHandler)
    };

} // namespace Core
