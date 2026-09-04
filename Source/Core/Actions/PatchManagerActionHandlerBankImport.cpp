#include "Core/Actions/PatchManagerActionHandler.h"

#include "Core/MIDI/MidiManager.h"
#include "Core/MIDI/PatchSelectionMidiSync.h"
#include "Core/MIDI/Queue/MidiRequestTiming.h"
#include "Core/MIDI/SysEx/SysExConstants.h"
#include "Core/Models/PatchModel.h"
#include "Core/Services/BankImportPlanner.h"
#include "Core/Services/PatchFileNameSanitizer.h"
#include "Core/Services/PatchFileService.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"

#include <cstring>

namespace BankFooterMessages = PluginDisplayNames::PatchManagerSection::BankUtilityModule::FooterMessages;

namespace Core
{

    bool PatchManagerActionHandler::loadImportPatchesFromScan(const PatchFolderScanResult& scan,
                                                              int cappedValidCount)
    {
        for (int i = 0; i < cappedValidCount; ++i)
        {
            const auto file = scan.folder.getChildFile(scan.sortedValidFileNames[i]);
            PackedPatchBuffer packed {};
            const auto loadResult = patchFileService_->loadPatchSysExFile(file, packed.data());
            if (! loadResult.success)
                continue;

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
        return bankTransfer_.importValidCount > 0;
    }

    bool PatchManagerActionHandler::validateBankImportGate(const DeviceMemoryLimits& limits)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (isBankTransferBusy() || patchFileService_ == nullptr)
            return false;

        const bool hasBankConcept = limits.hasBankConcept();
        const int bank = getSelectedBankForTransfer(limits);

        if (hasBankConcept && limits.isRomBank(bank))
        {
            publishBankTransferFooter(kImportRomBlockedFooterMessage, "warning");
            return false;
        }

        if (! isDeviceDumpAvailable())
        {
            publishBankTransferFooter(kDeviceUnavailableFooterMessage, "warning");
            return false;
        }

        if (! bankImportFolderPicker_)
            return false;

        return confirmPatchContextChange();
    }

    std::optional<juce::File> PatchManagerActionHandler::pickBankImportFolder()
    {
        const juce::File folder = bankImportFolderPicker_();
        if (! folder.isDirectory())
            return std::nullopt;

        return folder;
    }

    std::optional<PatchManagerActionHandler::BankImportFolderSelection>
    PatchManagerActionHandler::scanBankImportSelection(const juce::File& folder)
    {
        const auto scan = patchFileService_->scanFolder(folder);
        if (! scan.folderUsable)
        {
            publishBankTransferFooter(scan.footerMessage, scan.footerSeverity);
            return std::nullopt;
        }

        const auto plan = BankImportPlanner::resolve(scan);
        if (plan.cappedFileCount <= 0)
        {
            publishBankTransferFooter(BankFooterMessages::formatImportNoValidFiles(plan.foundCount), "warning");
            return std::nullopt;
        }

        return BankImportFolderSelection { folder, plan.foundCount, plan.cappedFileCount };
    }

    std::optional<PatchManagerActionHandler::BankImportFolderSelection>
    PatchManagerActionHandler::resolveBankImportFolderSelection(const DeviceMemoryLimits& limits)
    {
        if (! validateBankImportGate(limits))
            return std::nullopt;

        const auto folder = pickBankImportFolder();
        if (! folder.has_value())
            return std::nullopt;

        if (! bankImportConfirmGate_ || ! bankImportConfirmGate_())
            return std::nullopt;

        return scanBankImportSelection(*folder);
    }

    void PatchManagerActionHandler::initializeBankImportState(const DeviceMemoryLimits& limits, int foundCount)
    {
        bankTransfer_ = BankTransferState {};
        bankTransfer_.kind = BankTransferState::Kind::kImport;
        bankTransfer_.generation = ++bankTransferGeneration_;
        bankTransfer_.limits = limits;
        bankTransfer_.bank = getSelectedBankForTransfer(limits);
        bankTransfer_.importFoundCount = foundCount;
    }

    void PatchManagerActionHandler::showBankImportProgress(std::uint64_t generation, const juce::File& folder)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;
        using namespace PluginDisplayNames::Dialogs::BankTransferProgress;

        if (! bankTransferProgress_.show)
            return;

        bankTransferProgress_.show(
            juce::String(kImportTitle),
            juce::String(kImportingReadingMessage),
            folder.getFullPathName(),
            bankTransfer_.totalSlots,
            [this, generation] { requestBankTransferCancel(generation); });
    }

    void PatchManagerActionHandler::scheduleBankImportAfterSetBank(std::uint64_t generation,
                                                                   const DeviceMemoryLimits& limits,
                                                                   int bank)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (midiManager_ == nullptr || ! midiManager_->isEditorOutboundAllowed())
        {
            finishBankImport(kDeviceUnavailableFooterMessage, "warning");
            return;
        }

        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::InternalPatchesModule::StandaloneWidgets::kCurrentBankNumber,
            bank,
            nullptr);

        if (patchSelectionMidiSync_ != nullptr)
            patchSelectionMidiSync_->sendSetBank(bank, limits);
        else
            midiManager_->sendSetBank(bank);

        const int settleMs = MidiRequestTiming::deviceSettleMs(midiManager_->getRequiredSysExDelayMs());
        juce::WeakReference<PatchManagerActionHandler> weakThis(this);
        juce::Timer::callAfterDelay(settleMs,
            [weakThis, generation]
            {
                if (auto* self = weakThis.get())
                    self->beginBankImportSnapshot(generation);
            });
    }

    void PatchManagerActionHandler::startBankImportAfterConfirm(const DeviceMemoryLimits& limits,
                                                                const juce::File& folder,
                                                                int foundCount,
                                                                int cappedValidCount)
    {
        initializeBankImportState(limits, foundCount);

        if (! loadImportPatchesFromScan(patchFileService_->getLastScanResult(), cappedValidCount))
        {
            const auto message = BankFooterMessages::formatImportNoValidFiles(foundCount);
            bankTransfer_ = BankTransferState {};
            publishBankTransferFooter(message, "warning");
            return;
        }

        bankTransfer_.totalSlots = bankTransfer_.importValidCount;
        const auto generation = bankTransfer_.generation;
        showBankImportProgress(generation, folder);

        if (limits.hasBankConcept())
        {
            scheduleBankImportAfterSetBank(generation, limits, bankTransfer_.bank);
            return;
        }

        beginBankImportSnapshot(generation);
    }

    void PatchManagerActionHandler::handleBankImport(const DeviceMemoryLimits& limits)
    {
        const auto selection = resolveBankImportFolderSelection(limits);
        if (! selection.has_value())
            return;

        startBankImportAfterConfirm(limits,
                                    selection->folder,
                                    selection->foundCount,
                                    selection->cappedValidCount);
    }

    void PatchManagerActionHandler::beginBankImportSnapshot(std::uint64_t generation)
    {
        // Stale settle timers must not clear a newer transfer's safety snapshot.
        if (! isImportFamilyKind(bankTransfer_.kind) || bankTransfer_.generation != generation)
            return;

        bankTransfer_.deviceSnapshot.clear();
        snapshotNextImportSlot(0, generation);
    }

    bool PatchManagerActionHandler::processImportSnapshotDump(int slot,
                                                              std::uint64_t generation,
                                                              std::vector<juce::uint8> dump)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (! isImportFamilyKind(bankTransfer_.kind) || bankTransfer_.generation != generation)
            return false;

        if (bankTransfer_.cancelRequested)
        {
            const auto cancelMessage = bankTransfer_.kind == BankTransferState::Kind::kPaste
                ? juce::String(kPasteCancelledFooterMessage)
                : juce::String(kImportCancelledFooterMessage);
            finishBankImport(cancelMessage, "warning");
            return false;
        }

        if (dump.size() != SysExConstants::kPatchPackedDataSize)
        {
            const auto snapshotFailed = bankTransfer_.kind == BankTransferState::Kind::kPaste
                ? juce::String(kPasteSnapshotFailedFooterMessage)
                : juce::String(kSnapshotFailedFooterMessage);
            finishBankImport(snapshotFailed, "warning");
            return false;
        }

        PackedPatchBuffer packed {};
        std::memcpy(packed.data(), dump.data(), dump.size());
        bankTransfer_.deviceSnapshot.push_back(packed);

        if (bankTransferProgress_.update)
            bankTransferProgress_.update(slot + 1);

        return true;
    }

    void PatchManagerActionHandler::snapshotNextImportSlot(int slot, std::uint64_t generation)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (! isImportFamilyKind(bankTransfer_.kind) || bankTransfer_.generation != generation)
            return;

        if (bankTransfer_.cancelRequested)
        {
            const auto cancelMessage = bankTransfer_.kind == BankTransferState::Kind::kPaste
                ? juce::String(kPasteCancelledFooterMessage)
                : juce::String(kImportCancelledFooterMessage);
            finishBankImport(cancelMessage, "warning");
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

                if (! self->processImportSnapshotDump(slot, generation, std::move(dump)))
                    return;

                self->snapshotNextImportSlot(slot + 1, generation);
            });
    }

} // namespace Core
