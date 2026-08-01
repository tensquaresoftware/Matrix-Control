#include "Core/Actions/PatchManagerActionHandler.h"

#include "Core/MIDI/MidiManager.h"
#include "Core/MIDI/PatchSelectionMidiSync.h"
#include "Core/MIDI/Queue/MidiRequestTiming.h"
#include "Core/MIDI/SysEx/SysExConstants.h"
#include "Core/Models/PatchModel.h"
#include "Core/Services/PatchFileNameSanitizer.h"
#include "Core/Services/PatchFileService.h"
#include "Shared/Definitions/PluginDisplayNames.h"

namespace BankFooterMessages = PluginDisplayNames::PatchManagerSection::BankUtilityModule::FooterMessages;

namespace Core
{

    bool PatchManagerActionHandler::validateBankExportPrerequisites(const DeviceMemoryLimits& limits)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (isBankTransferBusy() || patchFileService_ == nullptr || sysExEncoder_ == nullptr)
            return false;

        if (! isDeviceDumpAvailable())
        {
            publishBankTransferFooter(kDeviceUnavailableFooterMessage, "warning");
            return false;
        }

        if (! bankExportFolderPicker_)
            return false;

        (void) limits;
        return confirmPatchContextChange();
    }

    juce::String PatchManagerActionHandler::resolveExportChildFolderName(const DeviceMemoryLimits& limits) const
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (limits.hasBankConcept())
            return "BANK " + juce::String(getSelectedBankForTransfer(limits));

        return juce::String(kMatrix6ExportFolderName);
    }

    bool PatchManagerActionHandler::ensureExportChildFolder(const juce::File& parentFolder,
                                                            const juce::String& childName,
                                                            juce::File& outFolder)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        const juce::File folder = parentFolder.getChildFile(childName);
        if (folder.isDirectory())
        {
            if (! bankExportOverwriteConfirmGate_ || ! bankExportOverwriteConfirmGate_())
                return false;

            if (! folder.deleteRecursively())
            {
                publishBankTransferFooter(kFolderNotWritableFooterMessage, "warning");
                return false;
            }
        }

        if (! folder.createDirectory())
        {
            publishBankTransferFooter(kFolderNotWritableFooterMessage, "warning");
            return false;
        }

        outFolder = folder;
        return true;
    }

    bool PatchManagerActionHandler::prepareBankExportDestination(const DeviceMemoryLimits& limits,
                                                                 juce::File& outFolder,
                                                                 juce::String& outChildName,
                                                                 bool& outCreatedThisRun)
    {
        if (! validateBankExportPrerequisites(limits))
            return false;

        const juce::File parentFolder = bankExportFolderPicker_();
        if (! parentFolder.isDirectory())
            return false;

        outChildName = resolveExportChildFolderName(limits);
        if (! ensureExportChildFolder(parentFolder, outChildName, outFolder))
            return false;

        outCreatedThisRun = true;
        return true;
    }

    void PatchManagerActionHandler::initializeBankExportState(const DeviceMemoryLimits& limits,
                                                              const juce::File& folder,
                                                              const juce::String& childName,
                                                              bool createdTargetFolderThisRun)
    {
        const bool hasBankConcept = limits.hasBankConcept();
        const int bank = getSelectedBankForTransfer(limits);

        bankTransfer_ = BankTransferState {};
        bankTransfer_.kind = BankTransferState::Kind::kExport;
        bankTransfer_.generation = ++bankTransferGeneration_;
        bankTransfer_.totalSlots = 100;
        bankTransfer_.limits = limits;
        bankTransfer_.bank = bank;
        bankTransfer_.targetFolder = folder;
        bankTransfer_.childFolderDisplayName = childName;
        bankTransfer_.createdTargetFolderThisRun = createdTargetFolderThisRun;
        bankTransfer_.hasBankConcept = hasBankConcept;
    }

    void PatchManagerActionHandler::showBankExportProgress(std::uint64_t generation,
                                                           const juce::File& folder,
                                                           bool hasBankConcept,
                                                           int bank)
    {
        using namespace PluginDisplayNames::Dialogs::BankTransferProgress;

        if (! bankTransferProgress_.show)
            return;

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

    void PatchManagerActionHandler::beginBankExportDumpLoop(std::uint64_t generation,
                                                            const DeviceMemoryLimits& limits,
                                                            bool hasBankConcept,
                                                            int bank)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (! hasBankConcept)
        {
            exportNextSlot(0, generation);
            return;
        }

        if (midiManager_ == nullptr || ! midiManager_->isEditorOutboundAllowed())
        {
            finishBankExport(false, kDeviceUnavailableFooterMessage, "warning");
            return;
        }

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
                    self->exportNextSlot(0, generation);
            });
    }

    void PatchManagerActionHandler::startBankExportTransfer(const DeviceMemoryLimits& limits,
                                                            const juce::File& folder,
                                                            const juce::String& childName,
                                                            bool createdTargetFolderThisRun)
    {
        initializeBankExportState(limits, folder, childName, createdTargetFolderThisRun);

        const auto generation = bankTransfer_.generation;
        showBankExportProgress(generation, folder, bankTransfer_.hasBankConcept, bankTransfer_.bank);
        beginBankExportDumpLoop(generation, limits, bankTransfer_.hasBankConcept, bankTransfer_.bank);
    }

    void PatchManagerActionHandler::handleBankExport(const DeviceMemoryLimits& limits)
    {
        juce::File folder;
        juce::String childName;
        bool createdThisRun = false;

        if (! prepareBankExportDestination(limits, folder, childName, createdThisRun))
            return;

        startBankExportTransfer(limits, folder, childName, createdThisRun);
    }

    bool PatchManagerActionHandler::validateExportDumpSlot(int slot,
                                                           std::uint64_t generation,
                                                           const std::vector<juce::uint8>& dump)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (bankTransfer_.kind != BankTransferState::Kind::kExport || bankTransfer_.generation != generation)
            return false;

        if (bankTransfer_.cancelRequested)
        {
            finishBankExport(false, kExportCancelledFooterMessage, "warning");
            return false;
        }

        if (dump.size() != SysExConstants::kPatchPackedDataSize)
        {
            finishBankExport(false, kDeviceUnavailableFooterMessage, "warning");
            return false;
        }

        (void) slot;
        return true;
    }

    bool PatchManagerActionHandler::saveExportedDumpToFile(int slot, PatchModel& dumpedPatch)
    {
        const auto stem = PatchFileNameSanitizer::bankExportFileStem(slot, dumpedPatch.getName());
        const auto file = bankTransfer_.targetFolder.getChildFile(
            PatchFileNameSanitizer::ensureSyxExtension(stem));

        const bool existedBefore = file.existsAsFile();
        const auto saveResult = patchFileService_->savePatchSysExFile(
            file, dumpedPatch.data(), *sysExEncoder_, slot);
        if (! saveResult.success)
        {
            finishBankExport(false, saveResult.errorMessage, "warning");
            return false;
        }

        if (! existedBefore)
            bankTransfer_.filesCreatedThisRun.add(file.getFullPathName());

        return true;
    }

    bool PatchManagerActionHandler::processExportDumpSlot(int slot,
                                                          std::uint64_t generation,
                                                          std::vector<juce::uint8> dump)
    {
        if (! validateExportDumpSlot(slot, generation, dump))
            return false;

        PatchModel dumpedPatch;
        dumpedPatch.loadFrom(dump.data());
        dumpedPatch.normalizeNameEncoding();
        applyResolvedPatchName(dumpedPatch,
                               PatchCoordinates { bankTransfer_.bank, slot },
                               bankTransfer_.limits,
                               PatchNameResolvePurpose::kExportMusical);

        if (! saveExportedDumpToFile(slot, dumpedPatch))
            return false;

        bankTransfer_.completedSlots = slot + 1;

        if (bankTransferProgress_.update)
            bankTransferProgress_.update(bankTransfer_.completedSlots);

        return true;
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

                if (self->processExportDumpSlot(slot, generation, std::move(dump)))
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

} // namespace Core
