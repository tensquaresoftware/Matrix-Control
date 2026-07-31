#include "Core/Actions/PatchManagerActionHandler.h"

#include "Core/Init/InitTemplateFooter.h"
#include "Core/Init/PatchInitService.h"
#include "Core/MIDI/MidiManager.h"
#include "Core/MIDI/PatchSelectionMidiSync.h"
#include "Core/MIDI/Queue/MidiRequestTiming.h"
#include "Core/MIDI/SysEx/SysExConstants.h"
#include "Core/Models/ApvtsPatchMapper.h"
#include "Core/Models/PatchModel.h"
#include "Core/Models/PatchNameSyncer.h"
#include "Core/Services/BankImportPlanner.h"
#include "Core/Services/ClipboardService.h"
#include "Core/Services/DirtyPatchTracker.h"
#include "Core/Services/PatchFileService.h"
#include "Core/Services/Matrix1000FactoryPatchNames.h"
#include "Core/Services/PatchFileNameSanitizer.h"
#include "Core/Services/PatchFileNameReconciler.h"
#include "Core/Services/PatchFileServiceFooter.h"
#include "Core/Services/PatchNameDisplayMode.h"
#include "Core/Services/PatchNameOverlayStore.h"
#include "Core/Services/PatchNameResolver.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"

#include <cstring>
#include <optional>
#include <vector>

namespace FooterMessages = PluginDisplayNames::PatchManagerSection::ComputerPatchesModule::FooterMessages;
namespace MutatorMessages = PluginDisplayNames::PatchManagerSection::PatchMutatorModule::Messages;
namespace BankFooterMessages = PluginDisplayNames::PatchManagerSection::BankUtilityModule::FooterMessages;

namespace
{
    void setPatchLoadSuppressHooks(Core::ActionExecutionHooks& hooks, bool suppress)
    {
        if (hooks.setSuppressPatchSysEx)
            hooks.setSuppressPatchSysEx(suppress);

        if (hooks.setSuppressMatrixModSysEx)
            hooks.setSuppressMatrixModSysEx(suppress);
    }

    void flushDeferredApvtsParameterSync(juce::AudioProcessorValueTreeState& apvts)
    {
        // setValueNotifyingHost queues ValueTree sync on APVTS's internal timer; copyState() forces flush (public API).
        (void) apvts.copyState();
    }

    void pushPatchModelToApvtsWithSuppress(juce::AudioProcessorValueTreeState& apvts,
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
}

namespace Core
{

    namespace
    {
        int indexOfFileNameIgnoreCase(const juce::StringArray& names, const juce::String& fileName)
        {
            for (int i = 0; i < names.size(); ++i)
            {
                if (names[i].equalsIgnoreCase(fileName))
                    return i;
            }

            return -1;
        }

        juce::String savedSyxFileName(const juce::File& targetFile)
        {
            return targetFile.hasFileExtension(PatchFileService::kSyxExtension)
                ? targetFile.getFileName()
                : targetFile.withFileExtension(PatchFileService::kSyxExtension).getFileName();
        }
    }

    PatchManagerActionHandler::PatchManagerActionHandler(
        juce::AudioProcessorValueTreeState& apvts,
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
        ActionExecutionHooks hooks)
        : apvts_(apvts)
        , deviceMemoryLimits_(std::move(deviceMemoryLimits))
        , patchModel_(patchModel)
        , apvtsPatchMapper_(apvtsPatchMapper)
        , clipboardService_(clipboardService)
        , patchInitService_(patchInitService)
        , patchSelectionMidiSync_(patchSelectionMidiSync)
        , midiManager_(midiManager)
        , patchFileService_(patchFileService)
        , patchNameSyncer_(patchNameSyncer)
        , dirtyPatchTracker_(dirtyPatchTracker)
        , sysExEncoder_(sysExEncoder)
        , pickFolder_(std::move(pickFolder))
        , pickSaveFile_(std::move(pickSaveFile))
        , pickNameReconciliation_(std::move(pickNameReconciliation))
        , hooks_(std::move(hooks))
    {
        loadPatchNameOverlayFromApvts();
    }

    void PatchManagerActionHandler::loadPatchNameOverlayFromApvts()
    {
        if (patchNameOverlayLoaded_)
            return;

        reloadPatchNameOverlayFromApvts();
    }

    void PatchManagerActionHandler::reloadPatchNameOverlayFromApvts()
    {
        using namespace PluginIDs::PatchManagerSection::BankUtilityModule::StateProperties;

        patchNameOverlay_.replaceFromValueTree(apvts_.state.getChildWithName(kPatchNameOverlay));
        patchNameOverlayLoaded_ = true;
    }

    void PatchManagerActionHandler::persistPatchNameOverlayToApvts()
    {
        using namespace PluginIDs::PatchManagerSection::BankUtilityModule::StateProperties;

        const auto existing = apvts_.state.getChildWithName(kPatchNameOverlay);
        if (existing.isValid())
            apvts_.state.removeChild(existing, nullptr);

        juce::ValueTree named { kPatchNameOverlay };
        const auto entries = patchNameOverlay_.toValueTree();
        for (int i = 0; i < entries.getNumChildren(); ++i)
            named.appendChild(entries.getChild(i).createCopy(), nullptr);

        apvts_.state.appendChild(named, nullptr);
    }

    void PatchManagerActionHandler::rememberOverlayName(int bank, int patch, const juce::String& name)
    {
        if (! PatchFileNameSanitizer::isUsablePatchName(name)
            || PatchFileNameSanitizer::isOberheimBankPlaceholderName(name))
            return;

        loadPatchNameOverlayFromApvts();
        patchNameOverlay_.remember(bank, patch, name);
        persistPatchNameOverlayToApvts();
    }

    void PatchManagerActionHandler::clearLastDeviceDumpRawName()
    {
        hasLastDeviceDumpRawName_ = false;
        lastDeviceDumpRawName_.clear();
        lastDeviceDumpBank_ = -1;
        lastDeviceDumpPatch_ = -1;
    }

    void PatchManagerActionHandler::rememberCurrentOverlayFromModel()
    {
        if (patchModel_ == nullptr)
            return;

        const auto limits = deviceMemoryLimits_();
        rememberOverlayName(getCurrentBank(limits), getCurrentPatch(limits), patchModel_->getName());
    }

    void PatchManagerActionHandler::applyResolvedPatchName(PatchModel& model,
                                                           int bank,
                                                           int patch,
                                                           const DeviceMemoryLimits& limits,
                                                           PatchNameResolvePurpose purpose)
    {
        loadPatchNameOverlayFromApvts();

        const bool isRom = limits.isRomBank(bank);
        const auto factoryName = isRom ? Matrix1000FactoryPatchNames::nameFor(bank, patch)
                                       : juce::String();
        const auto overlayName = patchNameOverlay_.lookup(bank, patch);

        const auto mode = [&]()
        {
            if (purpose == PatchNameResolvePurpose::kExportMusical)
                return PatchNameResolver::Mode::kMusical;

            const int modeId = PatchNameDisplay::normalize(static_cast<int>(
                apvts_.state.getProperty(PluginIDs::Settings::kPatchNameDisplayMode,
                                         PluginIDs::Settings::PatchNameDisplayMode::kDefault)));

            return PatchNameDisplay::isHardwareNames(modeId) ? PatchNameResolver::Mode::kHardware
                                                             : PatchNameResolver::Mode::kMusical;
        }();

        model.setName(PatchNameResolver::resolve(model.getName(),
                                                 bank,
                                                 patch,
                                                 isRom,
                                                 factoryName,
                                                 overlayName,
                                                 mode));
    }

    void PatchManagerActionHandler::reapplyDisplayedPatchName()
    {
        if (patchModel_ == nullptr || apvtsPatchMapper_ == nullptr)
            return;

        if (pendingDeviceLoad_.has_value())
            return;

        if (! hasLastDeviceDumpRawName_)
            return;

        const auto limits = deviceMemoryLimits_();
        const int bank = getCurrentBank(limits);
        const int patch = getCurrentPatch(limits);

        if (bank != lastDeviceDumpBank_ || patch != lastDeviceDumpPatch_)
            return;

        const bool wasDirty = dirtyPatchTracker_ != nullptr
            && dirtyPatchTracker_->isDirty(*patchModel_);

        patchModel_->setName(lastDeviceDumpRawName_);
        applyResolvedPatchName(*patchModel_,
                               bank,
                               patch,
                               limits,
                               PatchNameResolvePurpose::kDisplay);
        pushPatchModelToApvtsWithSuppress(apvts_, hooks_, *apvtsPatchMapper_, patchNameSyncer_);

        // Display-only re-resolve must not invent unsaved edits.
        if (dirtyPatchTracker_ != nullptr && ! wasDirty)
            dirtyPatchTracker_->captureSnapshot(*patchModel_);
    }

    void PatchManagerActionHandler::handleAction(const juce::String& propertyId, const juce::var&)
    {
        using namespace PluginIDs::PatchManagerSection;

        const auto limits = deviceMemoryLimits_();

        if (propertyId == InternalPatchesModule::StandaloneWidgets::kLoadPreviousPatch
            || propertyId == InternalPatchesModule::StandaloneWidgets::kLoadNextPatch)
        {
            if (! confirmPatchContextChange())
                return;

            const bool isNext = propertyId == InternalPatchesModule::StandaloneWidgets::kLoadNextPatch;
            const int direction = isNext ? 1 : -1;

            PatchCoordinates current;
            current.bank = getCurrentBank(limits);
            current.patch = getCurrentPatch(limits);

            const auto priorCoordinates = captureInternalCoordinates(limits);

            // Display-only lock indicator (D-023a-R3); navigation must not read kBanksLocked.
            applyPatchCoordinates(limits.advancePatch(current, direction), limits);
            beginPendingDeviceLoad(priorCoordinates);
            loadCurrentPatchFromDevice(limits);
            return;
        }

        if (propertyId == InternalPatchesModule::StandaloneWidgets::kInitPatch)
        {
            handleInternalPatchInit();
            return;
        }

        if (propertyId == InternalPatchesModule::StandaloneWidgets::kPastePatch)
        {
            handleInternalPatchPaste(limits);
            return;
        }

        if (propertyId == InternalPatchesModule::StandaloneWidgets::kStorePatch)
        {
            handleInternalPatchStore(limits);
            return;
        }

        if (propertyId == ComputerPatchesModule::StandaloneWidgets::kOpenPatchFolder)
        {
            handleOpenPatchFolder(limits);
            return;
        }

        if (propertyId == ComputerPatchesModule::StandaloneWidgets::kSavePatchAs)
        {
            handleSavePatchAs();
            return;
        }

        if (propertyId == ComputerPatchesModule::StandaloneWidgets::kSavePatchFile)
        {
            handleSavePatchFile();
            return;
        }

        if (propertyId == ComputerPatchesModule::StandaloneWidgets::kSelectPatchFile)
        {
            handleLoadSelectedPatchFile(limits);
            return;
        }

        if (propertyId == ComputerPatchesModule::StandaloneWidgets::kLoadPreviousPatchFile
            || propertyId == ComputerPatchesModule::StandaloneWidgets::kLoadNextPatchFile)
        {
            const int beforeId = readComputerPatchesSelectedId();
            const bool isNext = propertyId == ComputerPatchesModule::StandaloneWidgets::kLoadNextPatchFile;
            const auto nextId = advanceComputerPatchesSelection(isNext);

            // Single-file wrap: JUCE skips notification when the id is unchanged — force one load.
            // Do NOT key off post-load selectedId: Cancel revert restores beforeId and would
            // otherwise re-enter the load (second FR-51 prompt / silent discard of kept edits).
            if (nextId.has_value() && *nextId == beforeId)
                handleLoadSelectedPatchFile(limits);

            return;
        }

        if (propertyId == BankUtilityModule::StandaloneWidgets::kExportBank)
        {
            handleBankExport(limits);
            return;
        }

        if (propertyId == BankUtilityModule::StandaloneWidgets::kImportBank)
        {
            handleBankImport(limits);
            return;
        }

        if (!limits.hasBankConcept())
        {
            if (parseBankButtonIndex(propertyId) >= 0
                || propertyId == BankUtilityModule::StandaloneWidgets::kUnlockBank)
            {
                return;
            }
        }

        const int bankIndex = parseBankButtonIndex(propertyId);
        if (bankIndex >= 0)
        {
            if (! confirmPatchContextChange())
                return;

            const auto priorCoordinates = captureInternalCoordinates(limits);
            const int clampedBank = juce::jlimit(limits.minBankNumber(), limits.maxBankNumber(), bankIndex);
            apvts_.state.setProperty(BankUtilityModule::StateProperties::kSelectedBank, clampedBank, nullptr);
            apvts_.state.setProperty(InternalPatchesModule::StandaloneWidgets::kCurrentBankNumber, clampedBank, nullptr);

            if (patchSelectionMidiSync_ != nullptr)
                patchSelectionMidiSync_->syncSelection(clampedBank, getCurrentPatch(limits), limits, true);

            markBanksLockedInApvts();
            beginPendingDeviceLoad(priorCoordinates);
            loadCurrentPatchFromDevice(limits);
            return;
        }

        if (propertyId == BankUtilityModule::StandaloneWidgets::kUnlockBank)
            handleUnlockBank(limits);
    }

    void PatchManagerActionHandler::handleUnlockBank(const DeviceMemoryLimits& limits)
    {
        if (!limits.hasBankConcept())
            return;

        if (patchSelectionMidiSync_ != nullptr)
            patchSelectionMidiSync_->sendUnlockBank(limits);

        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::BankUtilityModule::StateProperties::kBanksLocked,
            false,
            nullptr);
    }

    void PatchManagerActionHandler::markBanksLockedInApvts()
    {
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::BankUtilityModule::StateProperties::kBanksLocked,
            true,
            nullptr);
    }

    void PatchManagerActionHandler::handleInternalPatchInit()
    {
        if (patchInitService_ == nullptr || apvtsPatchMapper_ == nullptr)
            return;

        const auto limits = deviceMemoryLimits_();

        if (!limits.isPasteStoreAllowed(getCurrentBank(limits)))
        {
            propagateRomBlockedFooter();
            return;
        }

        if (static_cast<bool>(apvts_.state.getProperty(
                PluginIDs::PatchManagerSection::PatchMutatorModule::StateProperties::kCompareActive,
                false)))
            return;

        if (! confirmPatchContextChange(false))
            return;

        abandonPendingDeviceLoad();
        clearLastDeviceDumpRawName();

        const auto result = patchInitService_->initFullPatch();

        if (hooks_.setPatchLoadContext)
            hooks_.setPatchLoadContext(
                PatchLoadContext::deviceMemory(getCurrentBank(limits), getCurrentPatch(limits)));

        pushPatchModelToApvtsWithSuppress(apvts_, hooks_, *apvtsPatchMapper_, patchNameSyncer_);
        captureCleanSnapshot();

        if (hooks_.onPatchLoaded)
            hooks_.onPatchLoaded();

        InitTemplateFooter::propagateMessage(apvts_, result);

        if (patchModel_ == nullptr || midiManager_ == nullptr)
            return;

        apvtsPatchMapper_->apvtsToBuffer();
        midiManager_->sendFullPatchForAudition(patchModel_->data(),
                                               static_cast<juce::uint8>(getCurrentPatch(limits)),
                                               limits.hasBankConcept());
    }

    void PatchManagerActionHandler::handleInternalPatchPaste(const DeviceMemoryLimits& limits)
    {
        const int currentBank = getCurrentBank(limits);

        if (!limits.isPasteStoreAllowed(currentBank))
        {
            propagateRomBlockedFooter();
            return;
        }

        if (clipboardService_ == nullptr || !clipboardService_->canPasteFullPatch())
        {
            apvts_.state.setProperty(
                "uiMessageText",
                juce::String(PluginDisplayNames::ShortLabels::kNothingToPasteFooter),
                nullptr);
            apvts_.state.setProperty("uiMessageSeverity", juce::String("warning"), nullptr);
            return;
        }

        if (patchModel_ == nullptr || apvtsPatchMapper_ == nullptr)
            return;

        if (! confirmPatchContextChange(false))
            return;

        abandonPendingDeviceLoad();
        clearLastDeviceDumpRawName();

        const auto sourceLabel = clipboardService_->getFullPatchSourceLabel().isNotEmpty()
            ? clipboardService_->getFullPatchSourceLabel()
            : juce::String("Patch");
        const auto targetLabel = PluginDisplayNames::ClipboardMessages::formatInternalPatchLocation(
            limits.hasBankConcept(), currentBank, getCurrentPatch(limits));

        apvtsPatchMapper_->apvtsToBuffer();
        if (!clipboardService_->pasteFullPatch(*patchModel_))
        {
            apvts_.state.setProperty(
                "uiMessageText",
                PluginDisplayNames::ClipboardMessages::formatPasteFailed(sourceLabel, targetLabel),
                nullptr);
            apvts_.state.setProperty("uiMessageSeverity", juce::String("error"), nullptr);
            return;
        }

        if (hooks_.disarmClipboardFeedback)
            hooks_.disarmClipboardFeedback();

        if (hooks_.setPatchLoadContext)
            hooks_.setPatchLoadContext(
                PatchLoadContext::deviceMemory(currentBank, getCurrentPatch(limits)));

        pushPatchModelToApvtsWithSuppress(apvts_, hooks_, *apvtsPatchMapper_, patchNameSyncer_);

        if (hooks_.onPatchLoaded)
            hooks_.onPatchLoaded();

        if (midiManager_ != nullptr)
            midiManager_->sendFullPatchForAudition(patchModel_->data(),
                                                   static_cast<juce::uint8>(getCurrentPatch(limits)),
                                                   limits.hasBankConcept());

        apvts_.state.setProperty(
            "uiMessageText",
            PluginDisplayNames::ClipboardMessages::formatPatchPasted(sourceLabel, targetLabel),
            nullptr);
        apvts_.state.setProperty("uiMessageSeverity", juce::String("info"), nullptr);
    }

    void PatchManagerActionHandler::handleInternalPatchStore(const DeviceMemoryLimits& limits)
    {
        const int currentBank = getCurrentBank(limits);

        if (!limits.isPasteStoreAllowed(currentBank))
        {
            propagateRomBlockedFooter();
            return;
        }

        if (patchModel_ == nullptr || apvtsPatchMapper_ == nullptr || midiManager_ == nullptr)
            return;

        if (patchSelectionMidiSync_ != nullptr)
            patchSelectionMidiSync_->syncSelection(currentBank, getCurrentPatch(limits), limits, true);

        if (limits.hasBankConcept())
            markBanksLockedInApvts();

        apvtsPatchMapper_->apvtsToBuffer();
        if (patchNameSyncer_ != nullptr)
            patchNameSyncer_->apvtsToBuffer();

        midiManager_->sendPatch(static_cast<juce::uint8>(getCurrentPatch(limits)), patchModel_->data());

        // Matrix-1000 rewrites name bytes to BNK on store — keep the editor name in the overlay.
        rememberOverlayName(currentBank, getCurrentPatch(limits), patchModel_->getName());

        // sendPatch is void and may no-op when outbound is blocked — only clear dirty on a real send.
        if (midiManager_->isEditorOutboundAllowed())
            captureCleanSnapshot();
    }

    void PatchManagerActionHandler::handleOpenPatchFolder(const DeviceMemoryLimits& limits)
    {
        if (patchFileService_ == nullptr || ! pickFolder_)
            return;

        const juce::File folder = pickFolder_();

        if (! folder.isDirectory())
            return;

        seedCommittedComputerPatchesSelectionIfNeeded();

        const juce::String previousFolderPath = apvts_.state.getProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kFolderPath,
            juce::String()).toString();
        const int previousSelectedId = lastCommittedComputerPatchesSelectedId_;

        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kFolderPath,
            folder.getFullPathName(),
            nullptr);
        scanAndPublishFolder(folder);

        const auto& scan = patchFileService_->getLastScanResult();
        if (! scan.folderUsable || scan.validCount < 1)
        {
            apvts_.state.setProperty(
                PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSelectPatchFile,
                0,
                nullptr);
            rememberComputerPatchesSelection(0);
            return;
        }

        // Auto-load may Cancel — restore the prior browser (folder + scan + selection).
        pendingBrowserRestoreOnCancel_ = ComputerPatchesBrowserSnapshot {
            previousFolderPath,
            previousSelectedId
        };

        constexpr int kFirstPatchFileId = 1;
        const int beforeId = readComputerPatchesSelectedId();
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSelectPatchFile,
            kFirstPatchFileId,
            nullptr);

        // JUCE skips notification when the value is unchanged — same as prev/next with N==1.
        if (readComputerPatchesSelectedId() == beforeId && beforeId == kFirstPatchFileId)
            handleLoadSelectedPatchFile(limits);
    }

    void PatchManagerActionHandler::resetComputerPatchesBrowserAfterSessionLoad()
    {
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSelectPatchFile,
            0,
            nullptr);
        rememberComputerPatchesSelection(0);
        lastStableComputerPatchesSelectedId_ = 0;
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kSelectPatchCancelBaseline,
            0,
            nullptr);

        if (patchFileService_ != nullptr && patchFileService_->hasCachedScanResult())
            clearPublishedScanCache();
        else
            bumpScanRevision();

        pendingBrowserRestoreOnCancel_.reset();
        reloadPatchNameOverlayFromApvts();
    }

    void PatchManagerActionHandler::discardComputerPatchesScanCacheQuietly()
    {
        if (patchFileService_ != nullptr && patchFileService_->hasCachedScanResult())
            patchFileService_->clearLastScan();
    }

    void PatchManagerActionHandler::handleSavePatchAs()
    {
        if (! pickSaveFile_)
            return;

        const auto target = pickSaveFile_(resolveDefaultSaveFolder(), resolveSuggestedSaveStem());

        if (target.getFullPathName().isEmpty())
            return;

        saveCurrentPatchToFile(target);
    }

    void PatchManagerActionHandler::handleSavePatchFile()
    {
        if (patchFileService_ == nullptr)
            return;

        const int selectedId = static_cast<int>(apvts_.state.getProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSelectPatchFile,
            0));

        if (selectedId < 1)
            return;

        const auto& scan = patchFileService_->getLastScanResult();
        const auto expectedFolder = resolveRescanFolder();

        if (! scan.folderUsable || ! scan.folder.isDirectory() || ! expectedFolder.isDirectory()
            || scan.folder.getFullPathName() != expectedFolder.getFullPathName())
            return;

        const int index = selectedId - 1;
        if (index < 0 || index >= scan.sortedValidFileNames.size())
            return;

        saveCurrentPatchToFile(scan.folder.getChildFile(scan.sortedValidFileNames[index]));
    }

    void PatchManagerActionHandler::handleLoadSelectedPatchFile(const DeviceMemoryLimits& limits)
    {
        if (suppressComputerPatchesSelectLoad_)
            return;

        const auto resolution = resolveSelectedPatchFileForLoad();

        if (resolution.kind == SelectedPatchFileResolution::Kind::kSilentNoOp)
            return;

        if (resolution.kind == SelectedPatchFileResolution::Kind::kFailed)
        {
            publishLoadFailureFooter(resolution.failureMessage);
            abortComputerPatchesNavigation();
            return;
        }

        const int requestedId = readComputerPatchesSelectedId();

        // Combo writes the new id before this runs — adopt UI cancel baseline when needed.
        const int cancelBaseline = static_cast<int>(apvts_.state.getProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kSelectPatchCancelBaseline,
            0));
        if (lastCommittedComputerPatchesSelectedId_ < 1 && cancelBaseline >= 1)
            noteStableComputerPatchesSelection(cancelBaseline);

        if (! confirmPatchContextChange(true))
        {
            abortComputerPatchesNavigation();
            return;
        }

        const auto reconciliation = decodeAndReconcilePatchFile(resolution.file);
        if (! reconciliation.has_value())
        {
            abortComputerPatchesNavigation();
            return;
        }

        pendingBrowserRestoreOnCancel_.reset();
        abandonPendingDeviceLoad();
        clearLastDeviceDumpRawName();

        if (hooks_.setPatchLoadContext)
            hooks_.setPatchLoadContext(
                PatchLoadContext::computerFile(resolution.file.getFileNameWithoutExtension()));

        applyLoadedPatchToApvtsAndSynth(limits);
        rememberComputerPatchesSelection(requestedId);
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kSelectPatchCancelBaseline,
            0,
            nullptr);
        publishLoadFooters(resolution.file.getFileName(), *reconciliation);
    }

    std::optional<int> PatchManagerActionHandler::advanceComputerPatchesSelection(bool isNext)
    {
        const int currentId = readComputerPatchesSelectedId();
        if (currentId < 1 || ! isComputerPatchesScanCurrent())
            return std::nullopt;

        const int count = patchFileService_->getLastScanResult().sortedValidFileNames.size();
        if (count < 1 || currentId > count)
            return std::nullopt;

        // Bootstrap: lock the pre-navigation selection as the Cancel revert target when no
        // successful load has committed an id yet.
        seedCommittedComputerPatchesSelectionIfNeeded();

        const int nextId = isNext
            ? (currentId >= count ? 1 : currentId + 1)
            : (currentId <= 1 ? count : currentId - 1);

        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSelectPatchFile,
            nextId,
            nullptr);
        return nextId;
    }

    int PatchManagerActionHandler::readComputerPatchesSelectedId() const
    {
        return static_cast<int>(apvts_.state.getProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSelectPatchFile,
            0));
    }

    bool PatchManagerActionHandler::isComputerPatchesScanCurrent() const
    {
        if (patchFileService_ == nullptr)
            return false;

        const auto& scan = patchFileService_->getLastScanResult();
        const auto expectedFolder = resolveRescanFolder();

        return scan.folderUsable
            && scan.folder.isDirectory()
            && expectedFolder.isDirectory()
            && scan.folder.getFullPathName() == expectedFolder.getFullPathName();
    }

    juce::File PatchManagerActionHandler::fileAtComputerPatchesIndex(int index) const
    {
        const auto& scan = patchFileService_->getLastScanResult();

        if (index < 0 || index >= scan.sortedValidFileNames.size())
            return {};

        return scan.folder.getChildFile(scan.sortedValidFileNames[index]);
    }

    PatchManagerActionHandler::SelectedPatchFileResolution
    PatchManagerActionHandler::makeLoadFailedResolution(const juce::String& message) const
    {
        SelectedPatchFileResolution resolution;
        resolution.kind = SelectedPatchFileResolution::Kind::kFailed;
        resolution.failureMessage = message;
        return resolution;
    }

    PatchManagerActionHandler::SelectedPatchFileResolution
    PatchManagerActionHandler::resolveSelectedPatchFileForLoad() const
    {
        const int selectedId = readComputerPatchesSelectedId();
        if (selectedId < 1)
            return {};

        if (! isComputerPatchesScanCurrent())
            return makeLoadFailedResolution(FooterMessages::kLoadSelectionStale);

        const auto file = fileAtComputerPatchesIndex(selectedId - 1);
        if (file.getFullPathName().isEmpty())
            return makeLoadFailedResolution(FooterMessages::kLoadSelectionStale);

        if (! file.existsAsFile())
            return makeLoadFailedResolution(FooterMessages::kPatchFileNotFound);

        SelectedPatchFileResolution resolution;
        resolution.kind = SelectedPatchFileResolution::Kind::kOk;
        resolution.file = file;
        return resolution;
    }

    bool PatchManagerActionHandler::canExecutePatchLoad() const
    {
        return patchModel_ != nullptr
            && apvtsPatchMapper_ != nullptr
            && patchFileService_ != nullptr
            && patchNameSyncer_ != nullptr;
    }

    bool PatchManagerActionHandler::loadPackedPatchFromFile(const juce::File& file, juce::uint8* packedOut)
    {
        const auto loadResult = patchFileService_->loadPatchSysExFile(file, packedOut);
        if (loadResult.success)
            return true;

        publishLoadFailureFooter(loadResult.errorMessage);
        return false;
    }

    PatchNameReconciliationResult PatchManagerActionHandler::reconcileLoadedPatchName(const juce::File& file)
    {
        const auto policy = static_cast<int>(apvts_.state.getProperty(
            PluginIDs::Settings::kComputerPatchesNameReconciliationPolicy,
            PluginIDs::Settings::NameReconciliationPolicy::kDefault));

        return PatchFileNameReconciler::reconcile(
            *patchModel_,
            file.getFileNameWithoutExtension(),
            policy,
            pickNameReconciliation_);
    }

    std::optional<PatchNameReconciliationResult>
    PatchManagerActionHandler::decodeAndReconcilePatchFile(const juce::File& file)
    {
        if (! canExecutePatchLoad())
            return std::nullopt;

        juce::uint8 previousPacked[SysExConstants::kPatchPackedDataSize] = {};
        std::memcpy(previousPacked, patchModel_->data(), sizeof(previousPacked));

        juce::uint8 packed[SysExConstants::kPatchPackedDataSize] = {};
        if (! loadPackedPatchFromFile(file, packed))
            return std::nullopt;

        patchModel_->loadFrom(packed);
        patchModel_->normalizeNameEncoding();

        const auto reconciliation = reconcileLoadedPatchName(file);
        if (reconciliation.cancelled)
        {
            patchModel_->loadFrom(previousPacked);
            return std::nullopt;
        }

        return reconciliation;
    }

    void PatchManagerActionHandler::syncLoadedPatchToApvts()
    {
        pushPatchModelToApvtsWithSuppress(apvts_, hooks_, *apvtsPatchMapper_, patchNameSyncer_);
    }

    void PatchManagerActionHandler::applyLoadedPatchToApvtsAndSynth(const DeviceMemoryLimits& limits)
    {
        // Caller may already have abandoned a pending device dump; keep this idempotent.
        abandonPendingDeviceLoad();

        syncLoadedPatchToApvts();
        captureCleanSnapshot();

        if (hooks_.onPatchLoaded)
            hooks_.onPatchLoaded();

        if (midiManager_ != nullptr)
            midiManager_->sendFullPatchForAudition(patchModel_->data(),
                                                   static_cast<juce::uint8>(getCurrentPatch(limits)),
                                                   limits.hasBankConcept());
    }

    void PatchManagerActionHandler::publishLoadFooters(const juce::String& fileName,
                                                       const PatchNameReconciliationResult& reconciliation)
    {
        const auto message = reconciliation.hadMismatch
            ? PluginDisplayNames::PatchManagerSection::ComputerPatchesModule::FooterMessages::formatReconciliationNotice(
                  reconciliation.resolvedName,
                  reconciliation.usedFilename)
            : PluginDisplayNames::PatchManagerSection::ComputerPatchesModule::FooterMessages::formatLoadSuccess(
                  fileName);

        apvts_.state.setProperty("uiMessageText", message, nullptr);
        apvts_.state.setProperty("uiMessageSeverity", juce::String("info"), nullptr);
    }

    void PatchManagerActionHandler::publishLoadFailureFooter(const juce::String& message)
    {
        apvts_.state.setProperty("uiMessageText", message, nullptr);
        apvts_.state.setProperty("uiMessageSeverity", juce::String("warning"), nullptr);
    }

    bool PatchManagerActionHandler::confirmPatchContextChange(bool includeUnsavedEditWarning)
    {
        return ! hooks_.confirmPatchContextChange
            || hooks_.confirmPatchContextChange(includeUnsavedEditWarning);
    }

    void PatchManagerActionHandler::captureCleanSnapshot()
    {
        if (dirtyPatchTracker_ == nullptr || patchModel_ == nullptr || apvtsPatchMapper_ == nullptr)
            return;

        // Re-sync APVTS → model before capture so name encode/decode asymmetry after
        // bufferToApvts cannot leave a false-dirty baseline (deferred 9.1 residual risk).
        apvtsPatchMapper_->apvtsToBuffer();
        if (patchNameSyncer_ != nullptr)
            patchNameSyncer_->apvtsToBuffer();

        patchModel_->normalizeNameEncoding();
        dirtyPatchTracker_->captureSnapshot(*patchModel_);
    }

    PatchManagerActionHandler::InternalCoordinatesSnapshot
    PatchManagerActionHandler::captureInternalCoordinates(const DeviceMemoryLimits& limits) const
    {
        InternalCoordinatesSnapshot snapshot;
        snapshot.bank = getCurrentBank(limits);
        snapshot.patch = getCurrentPatch(limits);
        snapshot.selectedBank = static_cast<int>(apvts_.state.getProperty(
            PluginIDs::PatchManagerSection::BankUtilityModule::StateProperties::kSelectedBank,
            snapshot.bank));
        snapshot.banksLocked = static_cast<bool>(apvts_.state.getProperty(
            PluginIDs::PatchManagerSection::BankUtilityModule::StateProperties::kBanksLocked,
            false));
        return snapshot;
    }

    void PatchManagerActionHandler::restoreInternalCoordinates(const InternalCoordinatesSnapshot& snapshot,
                                                               const DeviceMemoryLimits& limits)
    {
        applyPatchCoordinates({ snapshot.bank, snapshot.patch }, limits);

        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::BankUtilityModule::StateProperties::kSelectedBank,
            snapshot.selectedBank,
            nullptr);
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::BankUtilityModule::StateProperties::kBanksLocked,
            snapshot.banksLocked,
            nullptr);
    }

    void PatchManagerActionHandler::beginPendingDeviceLoad(const InternalCoordinatesSnapshot& priorCoordinates)
    {
        if (midiManager_ != nullptr)
            midiManager_->cancelPendingSysExRequest();

        PendingDeviceLoad pending;
        pending.generation = ++deviceLoadGeneration_;
        pending.priorCoordinates = priorCoordinates;

        if (patchModel_ != nullptr)
        {
            if (apvtsPatchMapper_ != nullptr)
                apvtsPatchMapper_->apvtsToBuffer();
            if (patchNameSyncer_ != nullptr)
                patchNameSyncer_->apvtsToBuffer();

            std::memcpy(pending.bufferAtRequest.data(),
                        patchModel_->data(),
                        PatchModel::kBufferSize);
        }

        pendingDeviceLoad_ = pending;
    }

    void PatchManagerActionHandler::clearPendingDeviceLoad()
    {
        pendingDeviceLoad_.reset();
    }

    void PatchManagerActionHandler::abandonPendingDeviceLoad()
    {
        if (midiManager_ != nullptr)
            midiManager_->cancelPendingSysExRequest();

        clearPendingDeviceLoad();
    }

    void PatchManagerActionHandler::failPendingDeviceLoad(const DeviceMemoryLimits& limits,
                                                          const juce::String& footerMessage)
    {
        if (pendingDeviceLoad_.has_value())
            restoreInternalCoordinates(pendingDeviceLoad_->priorCoordinates, limits);

        clearPendingDeviceLoad();
        apvts_.state.setProperty("uiMessageText", footerMessage, nullptr);
        apvts_.state.setProperty("uiMessageSeverity", juce::String("warning"), nullptr);
    }

    bool PatchManagerActionHandler::isDeviceDumpAvailable() const
    {
        if (hooks_.isDeviceDumpAvailable)
            return hooks_.isDeviceDumpAvailable();

        return midiManager_ != nullptr && midiManager_->isDeviceDumpAvailable();
    }

    void PatchManagerActionHandler::requestDeviceDump(juce::uint8 patchNumber,
                                                      ActionExecutionHooks::DeviceDumpCallback onResult)
    {
        if (hooks_.requestDeviceDump)
        {
            hooks_.requestDeviceDump(patchNumber, std::move(onResult));
            return;
        }

        if (midiManager_ == nullptr)
        {
            if (onResult)
                onResult({});
            return;
        }

        const int profileDelayMs = midiManager_->getRequiredSysExDelayMs();
        midiManager_->requestSinglePatchAsync(
            patchNumber,
            std::move(onResult),
            Core::MidiRequestTiming::deviceSettleMs(profileDelayMs),
            Core::MidiRequestTiming::outboundIdleTimeoutMs(profileDelayMs));
    }

    void PatchManagerActionHandler::noteStableComputerPatchesSelection(int selectedId)
    {
        if (selectedId >= 1)
            lastStableComputerPatchesSelectedId_ = selectedId;
    }

    int PatchManagerActionHandler::resolveComputerPatchesCancelRevertId() const
    {
        if (lastCommittedComputerPatchesSelectedId_ >= 1)
            return lastCommittedComputerPatchesSelectedId_;

        if (lastStableComputerPatchesSelectedId_ >= 1)
            return lastStableComputerPatchesSelectedId_;

        return static_cast<int>(apvts_.state.getProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kSelectPatchCancelBaseline,
            0));
    }

    void PatchManagerActionHandler::revertComputerPatchesSelectionIfNeeded(int previousSelectedId)
    {
        if (readComputerPatchesSelectedId() == previousSelectedId)
            return;

        suppressComputerPatchesSelectLoad_ = true;
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSelectPatchFile,
            previousSelectedId,
            nullptr);
        suppressComputerPatchesSelectLoad_ = false;
        noteStableComputerPatchesSelection(previousSelectedId);
    }

    void PatchManagerActionHandler::rememberComputerPatchesSelection(int selectedId)
    {
        lastCommittedComputerPatchesSelectedId_ = selectedId;
        noteStableComputerPatchesSelection(selectedId);
    }

    void PatchManagerActionHandler::seedCommittedComputerPatchesSelectionIfNeeded()
    {
        if (lastCommittedComputerPatchesSelectedId_ >= 1)
            return;

        const int currentId = readComputerPatchesSelectedId();
        if (currentId >= 1)
        {
            lastCommittedComputerPatchesSelectedId_ = currentId;
            noteStableComputerPatchesSelection(currentId);
        }
    }

    void PatchManagerActionHandler::restoreComputerPatchesBrowser(const juce::String& folderPath,
                                                                  int selectedId)
    {
        suppressComputerPatchesSelectLoad_ = true;

        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kFolderPath,
            folderPath,
            nullptr);

        if (folderPath.isEmpty())
            clearPublishedScanCache();
        else
            scanAndPublishFolder(juce::File(folderPath));

        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSelectPatchFile,
            selectedId,
            nullptr);

        suppressComputerPatchesSelectLoad_ = false;
        noteStableComputerPatchesSelection(selectedId);
    }

    void PatchManagerActionHandler::abortComputerPatchesNavigation()
    {
        if (pendingBrowserRestoreOnCancel_.has_value())
        {
            const auto snapshot = *pendingBrowserRestoreOnCancel_;
            pendingBrowserRestoreOnCancel_.reset();
            restoreComputerPatchesBrowser(snapshot.folderPath, snapshot.selectedId);
            noteStableComputerPatchesSelection(snapshot.selectedId);
            apvts_.state.setProperty(
                PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kSelectPatchCancelBaseline,
                0,
                nullptr);
            return;
        }

        revertComputerPatchesSelectionIfNeeded(resolveComputerPatchesCancelRevertId());
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kSelectPatchCancelBaseline,
            0,
            nullptr);
    }

    void PatchManagerActionHandler::loadCurrentPatchFromDevice(const DeviceMemoryLimits& limits,
                                                               int priorBank,
                                                               int priorPatch,
                                                               int priorSelectedBank,
                                                               bool priorBanksLocked)
    {
        InternalCoordinatesSnapshot prior;
        prior.bank = priorBank;
        prior.patch = priorPatch;
        prior.selectedBank = priorSelectedBank;
        prior.banksLocked = priorBanksLocked;
        beginPendingDeviceLoad(prior);
        loadCurrentPatchFromDevice(limits);
    }

    void PatchManagerActionHandler::loadCurrentPatchFromDevice(const DeviceMemoryLimits& limits)
    {
        if (patchModel_ == nullptr || apvtsPatchMapper_ == nullptr)
            return;

        // Callers that already advanced coordinates (NumberBox) must use the prior overload.
        if (! pendingDeviceLoad_.has_value())
            beginPendingDeviceLoad(captureInternalCoordinates(limits));

        const auto generation = pendingDeviceLoad_->generation;
        const auto failFooter = juce::String(MutatorMessages::kDeviceDumpFailedFooter);

        // No synth connected: roll back UI coordinates, keep the editor buffer / dirty / history.
        // History clears only after a successful dump (real patch load), not on dump failure.
        if (! isDeviceDumpAvailable())
        {
            failPendingDeviceLoad(limits, failFooter);
            return;
        }

        const int bank = getCurrentBank(limits);
        const int patch = getCurrentPatch(limits);

        // Fully async: never block the message thread (button release + NumberBox paint depend on it).
        requestDeviceDump(
            static_cast<juce::uint8>(patch),
            [this, bank, patch, generation, limits, failFooter](std::vector<juce::uint8> dump)
            {
                if (! pendingDeviceLoad_.has_value()
                    || pendingDeviceLoad_->generation != generation)
                {
                    return;
                }

                if (dump.size() != SysExConstants::kPatchPackedDataSize)
                {
                    failPendingDeviceLoad(limits, failFooter);
                    return;
                }

                if (patchModel_ == nullptr || apvtsPatchMapper_ == nullptr)
                {
                    failPendingDeviceLoad(limits, failFooter);
                    return;
                }

                // Mid-window edits after Continue: abort apply, restore coords, keep edits/dirty.
                if (apvtsPatchMapper_ != nullptr)
                    apvtsPatchMapper_->apvtsToBuffer();
                if (patchNameSyncer_ != nullptr)
                    patchNameSyncer_->apvtsToBuffer();

                if (std::memcmp(patchModel_->data(),
                                pendingDeviceLoad_->bufferAtRequest.data(),
                                PatchModel::kBufferSize)
                    != 0)
                {
                    failPendingDeviceLoad(
                        limits,
                        juce::String(MutatorMessages::kDeviceDumpAbortedEditedFooter));
                    return;
                }

                patchModel_->loadFrom(dump.data());
                patchModel_->normalizeNameEncoding();
                lastDeviceDumpRawName_ = patchModel_->getName();
                lastDeviceDumpBank_ = bank;
                lastDeviceDumpPatch_ = patch;
                hasLastDeviceDumpRawName_ = true;
                applyResolvedPatchName(*patchModel_,
                                       bank,
                                       patch,
                                       limits,
                                       PatchNameResolvePurpose::kDisplay);

                pushPatchModelToApvtsWithSuppress(apvts_, hooks_, *apvtsPatchMapper_, patchNameSyncer_);
                captureCleanSnapshot();
                clearPendingDeviceLoad();

                if (hooks_.setPatchLoadContext)
                    hooks_.setPatchLoadContext(PatchLoadContext::deviceMemory(bank, patch));

                // Treat a successful dump like a patch load: clears Mutator history/Compare.
                // Do NOT sendPatch back.
                if (hooks_.onPatchLoaded)
                    hooks_.onPatchLoaded();
            });
    }

    void PatchManagerActionHandler::saveCurrentPatchToFile(const juce::File& targetFile)
    {
        if (patchModel_ == nullptr || apvtsPatchMapper_ == nullptr || patchFileService_ == nullptr
            || patchNameSyncer_ == nullptr || sysExEncoder_ == nullptr)
            return;

        apvtsPatchMapper_->apvtsToBuffer();
        if (patchNameSyncer_ != nullptr)
            patchNameSyncer_->apvtsToBuffer();

        // Preserve Matrix name bytes 0–7 — do not inject the sanitized filename into the payload.
        const auto result = patchFileService_->savePatchSysExFile(
            targetFile.withFileExtension(PatchFileService::kSyxExtension),
            patchModel_->data(),
            *sysExEncoder_);

        if (! result.success)
        {
            publishSaveFailureFooter(result.errorMessage);
            return;
        }

        completeSuccessfulSave(savedSyxFileName(targetFile));
    }

    void PatchManagerActionHandler::completeSuccessfulSave(const juce::String& savedFileName)
    {
        patchNameSyncer_->bufferToApvts();
        captureCleanSnapshot();
        publishSaveSuccessFooter(savedFileName);
        rescanAndSelectSavedFile(savedFileName);
    }

    void PatchManagerActionHandler::rescanAndSelectSavedFile(const juce::String& savedFileName)
    {
        if (patchFileService_ == nullptr)
            return;

        const auto folder = resolveRescanFolder();
        if (! folder.isDirectory())
            return;

        patchFileService_->scanFolder(folder);

        const auto& names = patchFileService_->getLastScanResult().sortedValidFileNames;
        const int index = indexOfFileNameIgnoreCase(names, savedFileName);

        suppressComputerPatchesSelectLoad_ = true;
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSelectPatchFile,
            index >= 0 ? index + 1 : 0,
            nullptr);
        suppressComputerPatchesSelectLoad_ = false;
        rememberComputerPatchesSelection(index >= 0 ? index + 1 : 0);
        bumpScanRevision();
    }

    juce::File PatchManagerActionHandler::resolveRescanFolder() const
    {
        const auto path = apvts_.state.getProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kFolderPath,
            juce::String()).toString();

        if (path.isNotEmpty())
        {
            const juce::File persisted(path);
            if (persisted.isDirectory())
                return persisted;
        }

        if (patchFileService_ != nullptr)
            return patchFileService_->getLastScanResult().folder;

        return {};
    }

    juce::File PatchManagerActionHandler::resolveDefaultSaveFolder() const
    {
        const auto folder = resolveRescanFolder();
        return folder.isDirectory() ? folder : juce::File();
    }

    juce::String PatchManagerActionHandler::resolveSuggestedSaveStem() const
    {
        const auto raw = apvts_.state.getProperty(
            PluginIDs::PatchEditSection::PatchNameModule::kPatchName,
            juce::String()).toString();

        return PatchFileNameSanitizer::sanitizeOsFileStem(raw);
    }

    void PatchManagerActionHandler::publishSaveSuccessFooter(const juce::String& fileName)
    {
        apvts_.state.setProperty(
            "uiMessageText",
            PluginDisplayNames::PatchManagerSection::ComputerPatchesModule::FooterMessages::formatSaveSuccess(
                fileName),
            nullptr);
        apvts_.state.setProperty("uiMessageSeverity", juce::String("info"), nullptr);
    }

    void PatchManagerActionHandler::publishSaveFailureFooter(const juce::String& message)
    {
        apvts_.state.setProperty("uiMessageText", message, nullptr);
        apvts_.state.setProperty("uiMessageSeverity", juce::String("warning"), nullptr);
    }

    void PatchManagerActionHandler::rescanPersistedComputerPatchesFolder()
    {
        const auto path = apvts_.state.getProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kFolderPath,
            juce::String()).toString();

        if (path.isEmpty())
        {
            clearPublishedScanCache();
            return;
        }

        scanAndPublishFolder(juce::File(path));
    }

    void PatchManagerActionHandler::clearPublishedScanCache()
    {
        if (patchFileService_ == nullptr || ! patchFileService_->hasCachedScanResult())
            return;

        const auto footerMessage = patchFileService_->getLastScanResult().footerMessage;
        patchFileService_->clearLastScan();

        if (footerMessage.isNotEmpty()
            && apvts_.state.getProperty("uiMessageText").toString() == footerMessage)
        {
            apvts_.state.setProperty("uiMessageText", juce::String(), nullptr);
            apvts_.state.setProperty("uiMessageSeverity", juce::String(), nullptr);
        }

        bumpScanRevision();
    }

    void PatchManagerActionHandler::scanAndPublishFolder(const juce::File& folder)
    {
        if (patchFileService_ == nullptr)
            return;

        const auto result = patchFileService_->scanFolder(folder);
        PatchFileServiceFooter::propagateScanResult(apvts_, result);
        bumpScanRevision();
    }

    void PatchManagerActionHandler::bumpScanRevision()
    {
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kScanRevision,
            juce::Time::getMillisecondCounterHiRes(),
            nullptr);
    }

    void PatchManagerActionHandler::propagateRomBlockedFooter()
    {
        // ROM Init/Paste/Store are grayed in the UI; no warning footer (avoids false alarms).
    }

    int PatchManagerActionHandler::getCurrentBank(const DeviceMemoryLimits& limits) const
    {
        return static_cast<int>(apvts_.state.getProperty(
            PluginIDs::PatchManagerSection::InternalPatchesModule::StandaloneWidgets::kCurrentBankNumber,
            limits.minBankNumber()));
    }

    int PatchManagerActionHandler::getCurrentPatch(const DeviceMemoryLimits& limits) const
    {
        return static_cast<int>(apvts_.state.getProperty(
            PluginIDs::PatchManagerSection::InternalPatchesModule::StandaloneWidgets::kCurrentPatchNumber,
            limits.minPatchNumber()));
    }

    void PatchManagerActionHandler::applyPatchCoordinates(const PatchCoordinates& coordinates,
                                                          const DeviceMemoryLimits& limits)
    {
        if (hooks_.setSuppressPatchSelectionMidiSync)
            hooks_.setSuppressPatchSelectionMidiSync(true);

        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::InternalPatchesModule::StandaloneWidgets::kCurrentBankNumber,
            coordinates.bank,
            nullptr);
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::InternalPatchesModule::StandaloneWidgets::kCurrentPatchNumber,
            coordinates.patch,
            nullptr);

        if (hooks_.setSuppressPatchSelectionMidiSync)
            hooks_.setSuppressPatchSelectionMidiSync(false);

        bool setBankSent = false;
        if (patchSelectionMidiSync_ != nullptr)
            setBankSent = patchSelectionMidiSync_->syncSelection(coordinates.bank, coordinates.patch, limits, false);

        if (setBankSent)
            markBanksLockedInApvts();
    }

    int PatchManagerActionHandler::parseBankButtonIndex(const juce::String& propertyId) const
    {
        using namespace PluginIDs::PatchManagerSection::BankUtilityModule::StandaloneWidgets;

        if (propertyId == kSelectBank0) return 0;
        if (propertyId == kSelectBank1) return 1;
        if (propertyId == kSelectBank2) return 2;
        if (propertyId == kSelectBank3) return 3;
        if (propertyId == kSelectBank4) return 4;
        if (propertyId == kSelectBank5) return 5;
        if (propertyId == kSelectBank6) return 6;
        if (propertyId == kSelectBank7) return 7;
        if (propertyId == kSelectBank8) return 8;
        if (propertyId == kSelectBank9) return 9;

        return -1;
    }

    void PatchManagerActionHandler::setBankExportFolderPicker(PatchFolderPicker picker)
    {
        bankExportFolderPicker_ = std::move(picker);
    }

    void PatchManagerActionHandler::setBankImportFolderPicker(PatchFolderPicker picker)
    {
        bankImportFolderPicker_ = std::move(picker);
    }

    void PatchManagerActionHandler::setBankImportConfirmGate(BankImportConfirmGate gate)
    {
        bankImportConfirmGate_ = std::move(gate);
    }

    void PatchManagerActionHandler::setBankExportOverwriteConfirmGate(BankImportConfirmGate gate)
    {
        bankExportOverwriteConfirmGate_ = std::move(gate);
    }

    void PatchManagerActionHandler::setBankTransferProgressPresenter(BankTransferProgressPresenter presenter)
    {
        bankTransferProgress_ = std::move(presenter);
    }

    bool PatchManagerActionHandler::isBankTransferBusy() const noexcept
    {
        return bankTransfer_.kind != BankTransferState::Kind::kNone;
    }

    void PatchManagerActionHandler::publishBankTransferFooter(const juce::String& message, const juce::String& severity)
    {
        apvts_.state.setProperty("uiMessageText", message, nullptr);
        apvts_.state.setProperty("uiMessageSeverity", severity, nullptr);
    }

    int PatchManagerActionHandler::bankTransferWriteDelayMs() const
    {
        // MidiOutput::sendMessageNow returns before the UART finishes. A 275-byte patch SysEx
        // needs ~90ms on the wire @ 31.25 kbaud; Oberheim also wants ≥10ms between patches after
        // reception. Spacing only by the inter-SysEx profile (10–20ms) therefore delivers
        // back-to-back stores that the Matrix can silently drop — leaving RAM names as BNK0:xx
        // while the UI reports a successful import.
        constexpr int kPatchSysExWireMs = 100;
        constexpr int kMatrixStoreGapMs = 20;
        const int profileDelayMs = midiManager_ != nullptr ? midiManager_->getRequiredSysExDelayMs() : 0;
        return juce::jmax(kPatchSysExWireMs + kMatrixStoreGapMs,
                          profileDelayMs + kPatchSysExWireMs);
    }

    void PatchManagerActionHandler::requestBankTransferCancel(std::uint64_t generation)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (bankTransfer_.kind == BankTransferState::Kind::kNone || bankTransfer_.generation != generation)
            return;

        // Restore-in-progress cannot be cancelled (Cancel is already disabled in the UI).
        if (bankTransfer_.isRestoring)
            return;

        bankTransfer_.cancelRequested = true;

        // Drops any pending dump callback without invoking it — we must finish the UI ourselves
        // when the transfer was waiting on requestDeviceDump.
        if (midiManager_ != nullptr)
            midiManager_->cancelPendingSysExRequest();

        if (bankTransfer_.kind == BankTransferState::Kind::kExport)
        {
            finishBankExport(false, kExportCancelledFooterMessage, "warning");
            return;
        }

        // Import write already started: a delayed write step will observe cancelRequested and restore.
        if (bankTransfer_.importWrittenCount > 0)
            return;

        // Snapshot / pre-write: dump callback will never arrive — close cleanly with no device restore.
        finishBankImport(kImportCancelledFooterMessage, "warning");
    }

    int PatchManagerActionHandler::getSelectedBankForTransfer(const DeviceMemoryLimits& limits) const
    {
        if (! limits.hasBankConcept())
            return 0;

        const int selected = static_cast<int>(apvts_.state.getProperty(
            PluginIDs::PatchManagerSection::BankUtilityModule::StateProperties::kSelectedBank,
            limits.minBankNumber()));
        return juce::jlimit(limits.minBankNumber(), limits.maxBankNumber(), selected);
    }

    void PatchManagerActionHandler::handleBankExport(const DeviceMemoryLimits& limits)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;
        using namespace PluginDisplayNames::Dialogs::BankTransferProgress;

        if (isBankTransferBusy() || patchFileService_ == nullptr || sysExEncoder_ == nullptr)
            return;

        if (! isDeviceDumpAvailable())
        {
            publishBankTransferFooter(kDeviceUnavailableFooterMessage, "warning");
            return;
        }

        if (! bankExportFolderPicker_)
            return;

        // Match Bank Utility / Internal navigation gates before touching the device bank.
        if (! confirmPatchContextChange())
            return;

        const juce::File parentFolder = bankExportFolderPicker_();
        if (! parentFolder.isDirectory())
            return; // OS picker cancelled

        const bool hasBankConcept = limits.hasBankConcept();
        const int bank = getSelectedBankForTransfer(limits);
        const juce::String childName = hasBankConcept
            ? ("BANK " + juce::String(bank))
            : juce::String(kMatrix6ExportFolderName);

        const juce::File folder = parentFolder.getChildFile(childName);
        const bool folderExisted = folder.isDirectory();

        // Second export to the same destination — replace the folder entirely for a clean export.
        if (folderExisted)
        {
            if (! bankExportOverwriteConfirmGate_ || ! bankExportOverwriteConfirmGate_())
                return;

            if (! folder.deleteRecursively())
            {
                publishBankTransferFooter(kFolderNotWritableFooterMessage, "warning");
                return;
            }
        }

        if (! folder.createDirectory())
        {
            publishBankTransferFooter(kFolderNotWritableFooterMessage, "warning");
            return;
        }

        bankTransfer_ = BankTransferState {};
        bankTransfer_.kind = BankTransferState::Kind::kExport;
        bankTransfer_.generation = ++bankTransferGeneration_;
        bankTransfer_.totalSlots = 100;
        bankTransfer_.limits = limits;
        bankTransfer_.bank = bank;
        bankTransfer_.targetFolder = folder;
        bankTransfer_.childFolderDisplayName = childName;
        bankTransfer_.createdTargetFolderThisRun = true;
        bankTransfer_.hasBankConcept = hasBankConcept;

        const auto generation = bankTransfer_.generation;

        if (bankTransferProgress_.show)
        {
            using namespace PluginDisplayNames::Dialogs::BankTransferProgress;

            const auto progressMessage = hasBankConcept
                ? formatExportProgressMessage(bank)
                : formatExportProgressMessageNoBank();

            bankTransferProgress_.show(
                juce::String(kExportTitle),
                progressMessage,
                folder.getFullPathName(),
                bankTransfer_.totalSlots,
                [this, generation] { requestBankTransferCancel(generation); });
        }

        if (hasBankConcept)
        {
            if (midiManager_ == nullptr || ! midiManager_->isEditorOutboundAllowed())
            {
                finishBankExport(false, kDeviceUnavailableFooterMessage, "warning");
                return;
            }

            if (patchSelectionMidiSync_ != nullptr)
                patchSelectionMidiSync_->sendSetBank(bank, limits);
            else
                midiManager_->sendSetBank(bank);

            // Brief settle so the device finishes bank change before the first dump request.
            const int settleMs = MidiRequestTiming::deviceSettleMs(
                midiManager_->getRequiredSysExDelayMs());
            juce::WeakReference<PatchManagerActionHandler> weakThis(this);
            juce::Timer::callAfterDelay(settleMs,
                [weakThis, generation]
                {
                    if (auto* self = weakThis.get())
                        self->exportNextSlot(0, generation);
                });
            return;
        }

        exportNextSlot(0, generation);
    }

    void PatchManagerActionHandler::exportNextSlot(int slot, std::uint64_t generation)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (bankTransfer_.kind != BankTransferState::Kind::kExport || bankTransfer_.generation != generation)
            return;

        if (bankTransfer_.cancelRequested)
        {
            finishBankExport(false, kExportCancelledFooterMessage, "warning");
            return;
        }

        if (slot >= bankTransfer_.totalSlots)
        {
            finishBankExport(true,
                             BankFooterMessages::formatExportSuccess(
                                 bankTransfer_.hasBankConcept,
                                 bankTransfer_.bank,
                                 bankTransfer_.targetFolder.getFullPathName()),
                             "info");
            return;
        }

        juce::WeakReference<PatchManagerActionHandler> weakThis(this);
        requestDeviceDump(static_cast<juce::uint8>(slot),
            [weakThis, slot, generation](std::vector<juce::uint8> dump)
            {
                auto* self = weakThis.get();
                if (self == nullptr)
                    return;

                using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

                if (self->bankTransfer_.kind != BankTransferState::Kind::kExport
                    || self->bankTransfer_.generation != generation)
                {
                    return;
                }

                if (self->bankTransfer_.cancelRequested)
                {
                    self->finishBankExport(false, kExportCancelledFooterMessage, "warning");
                    return;
                }

                if (dump.size() != SysExConstants::kPatchPackedDataSize)
                {
                    self->finishBankExport(false, kDeviceUnavailableFooterMessage, "warning");
                    return;
                }

                PatchModel dumpedPatch;
                dumpedPatch.loadFrom(dump.data());
                dumpedPatch.normalizeNameEncoding();
                self->applyResolvedPatchName(dumpedPatch,
                                             self->bankTransfer_.bank,
                                             slot,
                                             self->bankTransfer_.limits,
                                             PatchNameResolvePurpose::kExportMusical);

                const auto nameForFile = dumpedPatch.getName();
                const auto stem = PatchFileNameSanitizer::bankExportFileStem(slot, nameForFile);
                const auto file = self->bankTransfer_.targetFolder.getChildFile(
                    PatchFileNameSanitizer::ensureSyxExtension(stem));

                const bool existedBefore = file.existsAsFile();
                const auto saveResult = self->patchFileService_->savePatchSysExFile(
                    file, dumpedPatch.data(), *self->sysExEncoder_, slot);
                if (! saveResult.success)
                {
                    self->finishBankExport(false, saveResult.errorMessage, "warning");
                    return;
                }

                if (! existedBefore)
                    self->bankTransfer_.filesCreatedThisRun.add(file.getFullPathName());

                self->bankTransfer_.completedSlots = slot + 1;

                if (self->bankTransferProgress_.update)
                    self->bankTransferProgress_.update(self->bankTransfer_.completedSlots);

                self->exportNextSlot(slot + 1, generation);
            });
    }

    void PatchManagerActionHandler::finishBankExport(bool success,
                                                     const juce::String& footerMessage,
                                                     const juce::String& severity)
    {
        if (bankTransferProgress_.hide)
            bankTransferProgress_.hide();

        if (! success)
        {
            for (const auto& path : bankTransfer_.filesCreatedThisRun)
                juce::File(path).deleteFile();

            if (bankTransfer_.createdTargetFolderThisRun
                && bankTransfer_.targetFolder.isDirectory()
                && bankTransfer_.targetFolder.getNumberOfChildFiles(juce::File::findFilesAndDirectories) == 0)
            {
                bankTransfer_.targetFolder.deleteRecursively();
            }
        }

        bankTransfer_ = BankTransferState {};
        publishBankTransferFooter(footerMessage, severity);
    }

    void PatchManagerActionHandler::handleBankImport(const DeviceMemoryLimits& limits)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;
        using namespace PluginDisplayNames::Dialogs::BankTransferProgress;

        if (isBankTransferBusy() || patchFileService_ == nullptr)
            return;

        const bool hasBankConcept = limits.hasBankConcept();
        const int bank = getSelectedBankForTransfer(limits);

        if (hasBankConcept && limits.isRomBank(bank))
        {
            publishBankTransferFooter(kImportRomBlockedFooterMessage, "warning");
            return;
        }

        if (! isDeviceDumpAvailable())
        {
            publishBankTransferFooter(kDeviceUnavailableFooterMessage, "warning");
            return;
        }

        if (! bankImportFolderPicker_)
            return;

        if (! confirmPatchContextChange())
            return;

        const juce::File folder = bankImportFolderPicker_();
        if (! folder.isDirectory())
            return; // OS picker cancelled

        // Cancel/Continue confirm — Cancel leaves the device untouched.
        if (! bankImportConfirmGate_ || ! bankImportConfirmGate_())
            return;

        const auto scan = patchFileService_->scanFolder(folder);
        if (! scan.folderUsable)
        {
            publishBankTransferFooter(scan.footerMessage, scan.footerSeverity);
            return;
        }

        const auto plan = BankImportPlanner::resolve(scan);
        const int found = plan.foundCount;
        const int cappedValidCount = plan.cappedFileCount;

        if (cappedValidCount <= 0)
        {
            publishBankTransferFooter(BankFooterMessages::formatImportNoValidFiles(found), "warning");
            return;
        }

        bankTransfer_ = BankTransferState {};
        bankTransfer_.kind = BankTransferState::Kind::kImport;
        bankTransfer_.generation = ++bankTransferGeneration_;
        bankTransfer_.limits = limits;
        bankTransfer_.bank = bank;
        bankTransfer_.importFoundCount = found;

        for (int i = 0; i < cappedValidCount; ++i)
        {
            const auto file = scan.folder.getChildFile(scan.sortedValidFileNames[i]);
            PackedPatchBuffer packed {};
            const auto loadResult = patchFileService_->loadPatchSysExFile(file, packed.data());
            if (! loadResult.success)
                continue;

            // Prefer payload name; if the file still carries Oberheim "BNKx:yy" placeholders
            // (or a blank name), recover the musical name from the Bank Utility export filename.
            PatchModel importedPatch;
            importedPatch.loadFrom(packed.data());
            importedPatch.normalizeNameEncoding();

            const auto payloadName = importedPatch.getName();
            if (! PatchFileNameSanitizer::isUsablePatchName(payloadName)
                || PatchFileNameSanitizer::isOberheimBankPlaceholderName(payloadName))
            {
                const auto fromFile = PatchFileNameSanitizer::nameFromBankExportStem(
                    file.getFileNameWithoutExtension());
                if (fromFile.isNotEmpty())
                    importedPatch.setName(fromFile);
            }

            std::memcpy(packed.data(), importedPatch.data(), packed.size());
            bankTransfer_.importPatches.push_back(packed);
        }

        bankTransfer_.importValidCount = static_cast<int>(bankTransfer_.importPatches.size());

        if (bankTransfer_.importValidCount <= 0)
        {
            const auto message = BankFooterMessages::formatImportNoValidFiles(found);
            bankTransfer_ = BankTransferState {};
            publishBankTransferFooter(message, "warning");
            return;
        }

        bankTransfer_.totalSlots = bankTransfer_.importValidCount;

        const auto generation = bankTransfer_.generation;

        if (bankTransferProgress_.show)
        {
            bankTransferProgress_.show(
                juce::String(kImportTitle),
                juce::String(kImportingReadingMessage),
                folder.getFullPathName(),
                bankTransfer_.totalSlots,
                [this, generation] { requestBankTransferCancel(generation); });
        }

        if (hasBankConcept)
        {
            if (midiManager_ == nullptr || ! midiManager_->isEditorOutboundAllowed())
            {
                finishBankImport(kDeviceUnavailableFooterMessage, "warning");
                return;
            }

            // Keep Internal Patches / Bank Utility bank aligned with the import target, and
            // update PatchSelectionMidiSync so later patch navigations do not send a stale Set Bank.
            apvts_.state.setProperty(
                PluginIDs::PatchManagerSection::InternalPatchesModule::StandaloneWidgets::kCurrentBankNumber,
                bank,
                nullptr);

            if (patchSelectionMidiSync_ != nullptr)
                patchSelectionMidiSync_->sendSetBank(bank, limits);
            else
                midiManager_->sendSetBank(bank);

            const int settleMs = MidiRequestTiming::deviceSettleMs(
                midiManager_->getRequiredSysExDelayMs());
            juce::WeakReference<PatchManagerActionHandler> weakThis(this);
            juce::Timer::callAfterDelay(settleMs,
                [weakThis, generation]
                {
                    if (auto* self = weakThis.get())
                        self->beginBankImportSnapshot(generation);
                });
            return;
        }

        beginBankImportSnapshot(generation);
    }

    void PatchManagerActionHandler::beginBankImportSnapshot(std::uint64_t generation)
    {
        bankTransfer_.deviceSnapshot.clear();
        snapshotNextImportSlot(0, generation);
    }

    void PatchManagerActionHandler::snapshotNextImportSlot(int slot, std::uint64_t generation)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (bankTransfer_.kind != BankTransferState::Kind::kImport || bankTransfer_.generation != generation)
            return;

        // Snapshot only reads the device; cancel here aborts with no restore needed.
        // Note: sendSetBank may already have run before the snapshot loop.
        if (bankTransfer_.cancelRequested)
        {
            finishBankImport(kImportCancelledFooterMessage, "warning");
            return;
        }

        if (midiManager_ == nullptr || ! midiManager_->isEditorOutboundAllowed())
        {
            finishBankImport(kDeviceUnavailableFooterMessage, "warning");
            return;
        }

        if (slot >= bankTransfer_.totalSlots)
        {
            beginBankImportWrite(generation);
            return;
        }

        juce::WeakReference<PatchManagerActionHandler> weakThis(this);
        requestDeviceDump(static_cast<juce::uint8>(slot),
            [weakThis, slot, generation](std::vector<juce::uint8> dump)
            {
                auto* self = weakThis.get();
                if (self == nullptr)
                    return;

                using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

                if (self->bankTransfer_.kind != BankTransferState::Kind::kImport
                    || self->bankTransfer_.generation != generation)
                {
                    return;
                }

                if (self->bankTransfer_.cancelRequested)
                {
                    self->finishBankImport(kImportCancelledFooterMessage, "warning");
                    return;
                }

                if (dump.size() != SysExConstants::kPatchPackedDataSize)
                {
                    self->finishBankImport(kSnapshotFailedFooterMessage, "warning");
                    return;
                }

                PackedPatchBuffer packed {};
                std::memcpy(packed.data(), dump.data(), packed.size());
                self->bankTransfer_.deviceSnapshot.push_back(packed);

                if (self->bankTransferProgress_.update)
                    self->bankTransferProgress_.update(slot + 1);

                self->snapshotNextImportSlot(slot + 1, generation);
            });
    }

    void PatchManagerActionHandler::beginBankImportWrite(std::uint64_t generation)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (bankTransfer_.kind != BankTransferState::Kind::kImport || bankTransfer_.generation != generation)
            return;

        // Snapshot is complete but nothing has been written yet — safe to abort without restoring.
        if (bankTransfer_.cancelRequested)
        {
            finishBankImport(kImportCancelledFooterMessage, "warning");
            return;
        }

        bankTransfer_.completedSlots = 0;
        bankTransfer_.importWrittenCount = 0;

        if (bankTransferProgress_.beginSecondaryPhase)
        {
            bankTransferProgress_.beginSecondaryPhase(
                juce::String(kImportingWritingMessage),
                bankTransfer_.importValidCount);
        }
        else if (bankTransferProgress_.setMessage)
        {
            bankTransferProgress_.setMessage(juce::String(kImportingWritingMessage));
            if (bankTransferProgress_.update)
                bankTransferProgress_.update(0);
        }

        writeNextImportSlot(0, generation);
    }

    void PatchManagerActionHandler::writeNextImportSlot(int slot, std::uint64_t generation)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (bankTransfer_.kind != BankTransferState::Kind::kImport || bankTransfer_.generation != generation)
            return;

        if (bankTransfer_.cancelRequested)
        {
            beginBankImportRestore(generation, kImportCancelledFooterMessage, "warning");
            return;
        }

        if (midiManager_ == nullptr || ! midiManager_->isEditorOutboundAllowed())
        {
            beginBankImportRestore(generation, kDeviceUnavailableFooterMessage, "warning");
            return;
        }

        if (slot >= bankTransfer_.importValidCount)
        {
            finishBankImport(
                BankFooterMessages::formatImportSuccess(
                    bankTransfer_.importFoundCount,
                    bankTransfer_.importValidCount,
                    bankTransfer_.importWrittenCount),
                "info");
            return;
        }

        const auto* packed = bankTransfer_.importPatches[static_cast<size_t>(slot)].data();
        midiManager_->sendPatch(static_cast<juce::uint8>(slot), packed);

        // Firmware keeps params but rewrites names to BNK — remember the musical name for the UI.
        {
            PatchModel imported;
            imported.loadFrom(packed);
            imported.normalizeNameEncoding();
            rememberOverlayName(bankTransfer_.bank, slot, imported.getName());
        }

        ++bankTransfer_.importWrittenCount;
        bankTransfer_.completedSlots = slot + 1;
        if (bankTransferProgress_.update)
            bankTransferProgress_.update(bankTransfer_.completedSlots);

        juce::WeakReference<PatchManagerActionHandler> weakThis(this);
        juce::Timer::callAfterDelay(bankTransferWriteDelayMs(),
            [weakThis, slot, generation]
            {
                if (auto* self = weakThis.get())
                    self->writeNextImportSlot(slot + 1, generation);
            });
    }

    void PatchManagerActionHandler::beginBankImportRestore(std::uint64_t generation,
                                                           const juce::String& footerMessage,
                                                           const juce::String& severity)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (bankTransfer_.kind != BankTransferState::Kind::kImport || bankTransfer_.generation != generation)
            return;

        if (bankTransfer_.deviceSnapshot.empty())
        {
            // Cancel arrived before the first write reached the device — nothing to restore.
            finishBankImport(footerMessage, severity);
            return;
        }

        if (midiManager_ == nullptr || ! midiManager_->isEditorOutboundAllowed())
        {
            finishBankImport(kImportRestoreFailedFooterMessage, "warning");
            return;
        }

        bankTransfer_.isRestoring = true;
        bankTransfer_.pendingFooterMessage = footerMessage;
        bankTransfer_.pendingFooterSeverity = severity;
        bankTransfer_.completedSlots = 0;

        if (bankTransferProgress_.setCancelEnabled)
            bankTransferProgress_.setCancelEnabled(false);

        if (bankTransferProgress_.beginSecondaryPhase)
        {
            bankTransferProgress_.beginSecondaryPhase(
                juce::String(kImportingRestoringMessage),
                juce::jmax(1, static_cast<int>(bankTransfer_.deviceSnapshot.size())));
        }
        else
        {
            if (bankTransferProgress_.setMessage)
                bankTransferProgress_.setMessage(juce::String(kImportingRestoringMessage));
            if (bankTransferProgress_.update)
                bankTransferProgress_.update(0);
        }

        restoreNextSnapshotSlot(0, generation);
    }

    void PatchManagerActionHandler::restoreNextSnapshotSlot(int slot, std::uint64_t generation)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (bankTransfer_.kind != BankTransferState::Kind::kImport || bankTransfer_.generation != generation)
            return;

        if (slot >= static_cast<int>(bankTransfer_.deviceSnapshot.size()))
        {
            finishBankImport(bankTransfer_.pendingFooterMessage, bankTransfer_.pendingFooterSeverity);
            return;
        }

        if (midiManager_ == nullptr || ! midiManager_->isEditorOutboundAllowed())
        {
            finishBankImport(kImportRestoreFailedFooterMessage, "warning");
            return;
        }

        const auto* packed = bankTransfer_.deviceSnapshot[static_cast<size_t>(slot)].data();
        midiManager_->sendPatch(static_cast<juce::uint8>(slot), packed);

        // Restore overlay to the pre-import name when it was musical; drop BNK placeholders.
        {
            PatchModel snapped;
            snapped.loadFrom(packed);
            snapped.normalizeNameEncoding();
            const auto name = snapped.getName();
            if (PatchFileNameSanitizer::isUsablePatchName(name)
                && ! PatchFileNameSanitizer::isOberheimBankPlaceholderName(name))
            {
                rememberOverlayName(bankTransfer_.bank, slot, name);
            }
            else
            {
                loadPatchNameOverlayFromApvts();
                patchNameOverlay_.forget(bankTransfer_.bank, slot);
                persistPatchNameOverlayToApvts();
            }
        }

        bankTransfer_.completedSlots = slot + 1;
        if (bankTransferProgress_.update)
            bankTransferProgress_.update(bankTransfer_.completedSlots);

        juce::WeakReference<PatchManagerActionHandler> weakThis(this);
        juce::Timer::callAfterDelay(bankTransferWriteDelayMs(),
            [weakThis, slot, generation]
            {
                if (auto* self = weakThis.get())
                    self->restoreNextSnapshotSlot(slot + 1, generation);
            });
    }

    void PatchManagerActionHandler::finishBankImport(const juce::String& footerMessage, const juce::String& severity)
    {
        if (bankTransferProgress_.hide)
            bankTransferProgress_.hide();

        const auto limits = bankTransfer_.limits;
        const int importedBank = bankTransfer_.bank;
        const int currentPatch = getCurrentPatch(limits);
        const bool importSucceeded = severity == "info";
        const bool deviceMayHaveChanged = importSucceeded
            || bankTransfer_.isRestoring
            || bankTransfer_.importWrittenCount > 0;

        PackedPatchBuffer writtenCurrentSlot {};
        const bool haveWrittenCurrentSlot = importSucceeded
            && currentPatch >= 0
            && currentPatch < bankTransfer_.importWrittenCount
            && currentPatch < static_cast<int>(bankTransfer_.importPatches.size());

        if (haveWrittenCurrentSlot)
            writtenCurrentSlot = bankTransfer_.importPatches[static_cast<size_t>(currentPatch)];

        bankTransfer_ = BankTransferState {};
        publishBankTransferFooter(footerMessage, severity);

        // Show what we just wrote right away (names included), then verify with a settled dump.
        if (haveWrittenCurrentSlot && patchModel_ != nullptr && apvtsPatchMapper_ != nullptr)
        {
            abandonPendingDeviceLoad();
            clearLastDeviceDumpRawName();
            patchModel_->loadFrom(writtenCurrentSlot.data());
            patchModel_->normalizeNameEncoding();
            applyResolvedPatchName(*patchModel_,
                                   importedBank,
                                   currentPatch,
                                   limits,
                                   PatchNameResolvePurpose::kDisplay);
            pushPatchModelToApvtsWithSuppress(apvts_, hooks_, *apvtsPatchMapper_, patchNameSyncer_);
            captureCleanSnapshot();

            if (hooks_.setPatchLoadContext)
                hooks_.setPatchLoadContext(PatchLoadContext::deviceMemory(importedBank, currentPatch));

            if (hooks_.onPatchLoaded)
                hooks_.onPatchLoaded();
        }

        if (! deviceMayHaveChanged || ! isDeviceDumpAvailable())
            return;

        // After 100 stores the Matrix may ignore dump requests for a short while; also let the
        // outbound queue finish so we do not race the last write.
        const int profileDelayMs = midiManager_ != nullptr ? midiManager_->getRequiredSysExDelayMs() : 0;
        const int settleMs = juce::jmax(250, MidiRequestTiming::deviceSettleMs(profileDelayMs));
        juce::WeakReference<PatchManagerActionHandler> weakThis(this);
        juce::Timer::callAfterDelay(settleMs,
            [weakThis, limits]
            {
                if (auto* self = weakThis.get())
                {
                    if (self->isDeviceDumpAvailable())
                        self->loadCurrentPatchFromDevice(limits);
                }
            });
    }

} // namespace Core
