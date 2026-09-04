#include "Core/Actions/PatchManagerActionHandler.h"

#include "Core/MIDI/MidiManager.h"
#include "Core/MIDI/PatchSelectionMidiSync.h"
#include "Core/MIDI/Queue/MidiRequestTiming.h"
#include "Core/MIDI/SysEx/SysExConstants.h"
#include "Core/Models/PatchModel.h"
#include "Core/Services/ClipboardService.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"

#include <cstring>

namespace BankFooterMessages = PluginDisplayNames::PatchManagerSection::BankUtilityModule::FooterMessages;

namespace Core
{

    bool PatchManagerActionHandler::isImportFamilyKind(BankTransferState::Kind kind) noexcept
    {
        return kind == BankTransferState::Kind::kImport
            || kind == BankTransferState::Kind::kPaste;
    }

    bool PatchManagerActionHandler::isExportFamilyKind(BankTransferState::Kind kind) noexcept
    {
        return kind == BankTransferState::Kind::kExport
            || kind == BankTransferState::Kind::kCopy;
    }

    void PatchManagerActionHandler::setBankPasteConfirmGate(BankPasteConfirmGate gate)
    {
        bankPasteConfirmGate_ = std::move(gate);
    }

    bool PatchManagerActionHandler::validateBankCopyPrerequisites(const DeviceMemoryLimits& limits)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (isBankTransferBusy())
        {
            publishBankTransferFooter(kBankTransferBusyFooterMessage, "warning");
            return false;
        }

        if (clipboardService_ == nullptr)
            return false;

        if (! limits.hasBankConcept())
            return false;

        if (! isDeviceDumpAvailable())
        {
            publishBankTransferFooter(kDeviceUnavailableFooterMessage, "warning");
            return false;
        }

        return true;
    }

    void PatchManagerActionHandler::initializeBankCopyState(const DeviceMemoryLimits& limits, int bank)
    {
        bankTransfer_ = BankTransferState {};
        bankTransfer_.kind = BankTransferState::Kind::kCopy;
        bankTransfer_.generation = ++bankTransferGeneration_;
        bankTransfer_.totalSlots = ClipboardService::kBankSlotCount;
        bankTransfer_.limits = limits;
        bankTransfer_.bank = bank;
        bankTransfer_.hasBankConcept = true;
        bankTransfer_.importPatches.clear();
        bankTransfer_.importPatches.reserve(static_cast<size_t>(ClipboardService::kBankSlotCount));
    }

    bool PatchManagerActionHandler::showBankCopyProgress(std::uint64_t generation, int bank)
    {
        using namespace PluginDisplayNames::Dialogs::BankTransferProgress;

        if (! bankTransferProgress_.show)
            return false;

        bankTransferProgress_.show(
            juce::String(kCopyTitle),
            formatCopyProgressMessage(bank),
            juce::String(kClipboardLabel),
            bankTransfer_.totalSlots,
            [this, generation] { requestBankTransferCancel(generation); });
        return true;
    }

    void PatchManagerActionHandler::beginBankCopyDumpLoop(std::uint64_t generation,
                                                          const DeviceMemoryLimits& limits,
                                                          int bank)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (midiManager_ == nullptr || ! midiManager_->isEditorOutboundAllowed())
        {
            finishBankCopy(false, kDeviceUnavailableFooterMessage, "warning");
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
                    self->copyNextSlot(0, generation);
            });
    }

    void PatchManagerActionHandler::restoreBankCopyFeedbackAfterConfirmCancel()
    {
        if (hooks_.clearBankCopyFeedbackPending)
            hooks_.clearBankCopyFeedbackPending();

        if (clipboardService_ != nullptr && clipboardService_->getMode() == ClipboardMode::Bank)
        {
            if (hooks_.armClipboardFeedback)
                hooks_.armClipboardFeedback();
        }
        else if (hooks_.disarmClipboardFeedback)
        {
            hooks_.disarmClipboardFeedback();
        }

        if (hooks_.refreshClipboardMirrors)
            hooks_.refreshClipboardMirrors();
    }

    void PatchManagerActionHandler::handleBankCopy(const DeviceMemoryLimits& limits)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (! validateBankCopyPrerequisites(limits))
            return;

        if (hooks_.armBankCopyFeedbackPending)
            hooks_.armBankCopyFeedbackPending();

        if (! confirmPatchContextChange())
        {
            restoreBankCopyFeedbackAfterConfirmCancel();
            return;
        }

        const int bank = getSelectedBankForTransfer(limits);
        initializeBankCopyState(limits, bank);
        const auto generation = bankTransfer_.generation;
        if (! showBankCopyProgress(generation, bank))
        {
            finishBankCopy(false, kCopyFailedFooterMessage, "warning");
            return;
        }

        beginBankCopyDumpLoop(generation, limits, bank);
    }

    void PatchManagerActionHandler::commitCopiedBankToClipboard()
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (bankTransfer_.importPatches.size() != static_cast<size_t>(ClipboardService::kBankSlotCount))
        {
            finishBankCopy(false, kCopyFailedFooterMessage, "warning");
            return;
        }

        ClipboardService::BankPatchArray patches {};
        for (int i = 0; i < ClipboardService::kBankSlotCount; ++i)
            patches[static_cast<size_t>(i)] = bankTransfer_.importPatches[static_cast<size_t>(i)];

        const int sourceBank = bankTransfer_.bank;
        clipboardService_->copyBank(patches, sourceBank);
        finishBankCopy(true, BankFooterMessages::formatCopySuccess(sourceBank), "info");
    }

    bool PatchManagerActionHandler::processCopyDumpSlot(int slot,
                                                        std::uint64_t generation,
                                                        std::vector<juce::uint8> dump)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (bankTransfer_.kind != BankTransferState::Kind::kCopy || bankTransfer_.generation != generation)
            return false;

        if (bankTransfer_.cancelRequested)
        {
            finishBankCopy(false, kCopyCancelledFooterMessage, "warning");
            return false;
        }

        if (dump.size() != SysExConstants::kPatchPackedDataSize)
        {
            finishBankCopy(false, kCopyFailedFooterMessage, "warning");
            return false;
        }

        PackedPatchBuffer packed {};
        std::memcpy(packed.data(), dump.data(), dump.size());
        bankTransfer_.importPatches.push_back(packed);
        bankTransfer_.completedSlots = slot + 1;

        if (bankTransferProgress_.update)
            bankTransferProgress_.update(bankTransfer_.completedSlots);

        return true;
    }

    void PatchManagerActionHandler::copyNextSlot(int slot, std::uint64_t generation)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (bankTransfer_.kind != BankTransferState::Kind::kCopy || bankTransfer_.generation != generation)
            return;

        if (bankTransfer_.cancelRequested)
        {
            finishBankCopy(false, kCopyCancelledFooterMessage, "warning");
            return;
        }

        if (slot >= bankTransfer_.totalSlots)
        {
            commitCopiedBankToClipboard();
            return;
        }

        if (midiManager_ == nullptr || ! midiManager_->isEditorOutboundAllowed())
        {
            finishBankCopy(false, kCopyFailedFooterMessage, "warning");
            return;
        }

        juce::WeakReference<PatchManagerActionHandler> weakThis(this);
        requestDeviceDump(static_cast<juce::uint8>(slot),
            [weakThis, slot, generation](std::vector<juce::uint8> dump)
            {
                auto* self = weakThis.get();
                if (self == nullptr)
                    return;

                if (self->processCopyDumpSlot(slot, generation, std::move(dump)))
                    self->copyNextSlot(slot + 1, generation);
            });
    }

    void PatchManagerActionHandler::refreshBankCopyFeedbackAfterFinish(bool success)
    {
        if (! success)
        {
            if (hooks_.clearBankCopyFeedbackPending)
                hooks_.clearBankCopyFeedbackPending();

            // Keep prior bank clipboard session blinking if the dump failed/cancelled.
            if (clipboardService_ != nullptr && clipboardService_->getMode() == ClipboardMode::Bank)
            {
                if (hooks_.armClipboardFeedback)
                    hooks_.armClipboardFeedback();
            }
            else if (hooks_.disarmClipboardFeedback)
            {
                hooks_.disarmClipboardFeedback();
            }
        }
        else if (hooks_.armClipboardFeedback)
        {
            hooks_.armClipboardFeedback();
        }

        if (hooks_.refreshClipboardMirrors)
            hooks_.refreshClipboardMirrors();
    }

    void PatchManagerActionHandler::finishBankCopy(bool success,
                                                   const juce::String& footerMessage,
                                                   const juce::String& severity)
    {
        if (bankTransfer_.kind != BankTransferState::Kind::kCopy)
            return;

        if (bankTransferProgress_.hide)
            bankTransferProgress_.hide();

        bankTransfer_ = BankTransferState {};
        publishBankTransferFooter(footerMessage, severity);
        refreshBankCopyFeedbackAfterFinish(success);
    }

    bool PatchManagerActionHandler::validateBankPastePrerequisites(const DeviceMemoryLimits& limits,
                                                                   int targetBank)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (isBankTransferBusy())
        {
            publishBankTransferFooter(kBankTransferBusyFooterMessage, "warning");
            return false;
        }

        if (clipboardService_ == nullptr)
            return false;

        if (! limits.hasBankConcept())
            return false;

        if (! limits.isPasteStoreAllowed(targetBank))
        {
            publishBankTransferFooter(kPasteRomBlockedFooterMessage, "warning");
            return false;
        }

        if (! clipboardService_->canPasteBank(targetBank))
            return false;

        if (! isDeviceDumpAvailable())
        {
            publishBankTransferFooter(kDeviceUnavailableFooterMessage, "warning");
            return false;
        }

        return true;
    }

    bool PatchManagerActionHandler::prepareBankPastePatches(int sourceBank,
                                                            int targetBank,
                                                            const DeviceMemoryLimits& limits)
    {
        ClipboardService::BankPatchArray patches {};
        if (! clipboardService_->pasteBank(patches))
            return false;

        bankTransfer_ = BankTransferState {};
        bankTransfer_.kind = BankTransferState::Kind::kPaste;
        bankTransfer_.generation = ++bankTransferGeneration_;
        bankTransfer_.limits = limits;
        bankTransfer_.bank = targetBank;
        bankTransfer_.pasteSourceBank = sourceBank;
        bankTransfer_.hasBankConcept = true;
        bankTransfer_.importFoundCount = ClipboardService::kBankSlotCount;
        bankTransfer_.importValidCount = ClipboardService::kBankSlotCount;
        bankTransfer_.totalSlots = ClipboardService::kBankSlotCount;
        bankTransfer_.importPatches.clear();
        bankTransfer_.importPatches.reserve(static_cast<size_t>(ClipboardService::kBankSlotCount));

        for (int i = 0; i < ClipboardService::kBankSlotCount; ++i)
            bankTransfer_.importPatches.push_back(patches[static_cast<size_t>(i)]);

        return true;
    }

    bool PatchManagerActionHandler::showBankPasteProgress(std::uint64_t generation, int targetBank)
    {
        using namespace PluginDisplayNames::Dialogs::BankTransferProgress;

        if (! bankTransferProgress_.show)
            return false;

        bankTransferProgress_.show(
            juce::String(kPasteTitle),
            formatPasteSafetyCopyMessage(targetBank),
            juce::String(kClipboardLabel),
            bankTransfer_.totalSlots,
            [this, generation] { requestBankTransferCancel(generation); });
        return true;
    }

    void PatchManagerActionHandler::beginBankPasteWriteLoop(std::uint64_t generation,
                                                            const DeviceMemoryLimits& limits,
                                                            int targetBank)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (midiManager_ == nullptr || ! midiManager_->isEditorOutboundAllowed())
        {
            finishBankImport(kDeviceUnavailableFooterMessage, "warning");
            return;
        }

        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::InternalPatchesModule::StandaloneWidgets::kCurrentBankNumber,
            targetBank,
            nullptr);

        if (patchSelectionMidiSync_ != nullptr)
            patchSelectionMidiSync_->sendSetBank(targetBank, limits);
        else
            midiManager_->sendSetBank(targetBank);

        const int settleMs = MidiRequestTiming::deviceSettleMs(midiManager_->getRequiredSysExDelayMs());
        juce::WeakReference<PatchManagerActionHandler> weakThis(this);
        juce::Timer::callAfterDelay(settleMs,
            [weakThis, generation]
            {
                if (auto* self = weakThis.get())
                    self->beginBankImportSnapshot(generation);
            });
    }

    void PatchManagerActionHandler::handleBankPaste(const DeviceMemoryLimits& limits)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        const int targetBank = getSelectedBankForTransfer(limits);
        if (! validateBankPastePrerequisites(limits, targetBank))
            return;

        const auto sourceBank = clipboardService_->getBankSource();
        if (! sourceBank.has_value())
            return;

        if (! bankPasteConfirmGate_ || ! bankPasteConfirmGate_(*sourceBank, targetBank))
            return;

        if (! prepareBankPastePatches(*sourceBank, targetBank, limits))
        {
            publishBankTransferFooter(kPasteClipboardFailedFooterMessage, "warning");
            return;
        }

        const auto generation = bankTransfer_.generation;
        if (! showBankPasteProgress(generation, targetBank))
        {
            bankTransfer_ = BankTransferState {};
            publishBankTransferFooter(kPasteClipboardFailedFooterMessage, "warning");
            return;
        }

        beginBankPasteWriteLoop(generation, limits, targetBank);
    }

} // namespace Core
