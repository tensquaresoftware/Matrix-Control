#include "Core/Actions/PatchManagerActionHandler.h"
#include "Core/Actions/PatchManagerActionHandlerInternal.h"

#include "Core/MIDI/MidiManager.h"
#include "Core/MIDI/Queue/MidiRequestTiming.h"
#include "Core/Models/ApvtsPatchMapper.h"
#include "Core/Models/PatchModel.h"
#include "Core/Services/PatchFileNameSanitizer.h"
#include "Shared/Definitions/PluginDisplayNames.h"

namespace BankFooterMessages = PluginDisplayNames::PatchManagerSection::BankUtilityModule::FooterMessages;

namespace Core
{

    void PatchManagerActionHandler::beginBankImportWrite(std::uint64_t generation)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;
        using namespace PluginDisplayNames::Dialogs::BankTransferProgress;

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

        bankTransfer_.completedSlots = 0;
        bankTransfer_.importWrittenCount = 0;

        if (bankTransferProgress_.beginSecondaryPhase)
        {
            const auto writingMessage = bankTransfer_.kind == BankTransferState::Kind::kPaste
                ? juce::String(kPastingWritingMessage)
                : juce::String(kImportingWritingMessage);
            bankTransferProgress_.beginSecondaryPhase(
                writingMessage,
                bankTransfer_.importValidCount);
        }
        else if (bankTransferProgress_.setMessage)
        {
            const auto writingMessage = bankTransfer_.kind == BankTransferState::Kind::kPaste
                ? juce::String(kPastingWritingMessage)
                : juce::String(kImportingWritingMessage);
            bankTransferProgress_.setMessage(writingMessage);
            if (bankTransferProgress_.update)
                bankTransferProgress_.update(0);
        }

        writeNextImportSlot(0, generation);
    }

    void PatchManagerActionHandler::rememberOverlayFromPackedSlot(int bank, int slot, const juce::uint8* packed)
    {
        PatchModel imported;
        imported.loadFrom(packed);
        imported.normalizeNameEncoding();
        rememberOverlayName(bank, slot, imported.getName());
    }

    void PatchManagerActionHandler::restoreOverlayFromPackedSlot(int bank, int slot, const juce::uint8* packed)
    {
        PatchModel snapped;
        snapped.loadFrom(packed);
        snapped.normalizeNameEncoding();
        const auto name = snapped.getName();
        if (PatchFileNameSanitizer::isUsablePatchName(name)
            && ! PatchFileNameSanitizer::isOberheimBankPlaceholderName(name))
        {
            rememberOverlayName(bank, slot, name);
            return;
        }

        loadPatchNameOverlayFromApvts();
        patchNameOverlay_.forget(bank, slot);
        persistPatchNameOverlayToApvts();
    }

    juce::String PatchManagerActionHandler::bankImportFamilyCancelMessage() const
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        return bankTransfer_.kind == BankTransferState::Kind::kPaste
            ? juce::String(kPasteCancelledFooterMessage)
            : juce::String(kImportCancelledFooterMessage);
    }

    juce::String PatchManagerActionHandler::bankImportFamilyRestoreFailedMessage() const
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        return bankTransfer_.kind == BankTransferState::Kind::kPaste
            ? juce::String(kPasteRestoreFailedFooterMessage)
            : juce::String(kImportRestoreFailedFooterMessage);
    }

    juce::String PatchManagerActionHandler::bankImportFamilyRestoringProgressMessage() const
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        return bankTransfer_.kind == BankTransferState::Kind::kPaste
            ? juce::String(kPastingRestoringMessage)
            : juce::String(kImportingRestoringMessage);
    }

    void PatchManagerActionHandler::showBankImportRestoreProgress()
    {
        const auto restoringMessage = bankImportFamilyRestoringProgressMessage();
        const int totalSteps = juce::jmax(1, static_cast<int>(bankTransfer_.deviceSnapshot.size()));

        if (bankTransferProgress_.beginSecondaryPhase)
        {
            bankTransferProgress_.beginSecondaryPhase(restoringMessage, totalSteps);
            return;
        }

        if (bankTransferProgress_.setMessage)
            bankTransferProgress_.setMessage(restoringMessage);
        if (bankTransferProgress_.update)
            bankTransferProgress_.update(0);
    }

    void PatchManagerActionHandler::finishImportFamilyWriteSuccess()
    {
        if (bankTransfer_.kind == BankTransferState::Kind::kPaste)
        {
            finishBankImport(
                BankFooterMessages::formatPasteSuccess(
                    bankTransfer_.pasteSourceBank,
                    bankTransfer_.bank),
                "info");
            return;
        }

        finishBankImport(
            BankFooterMessages::formatImportSuccess(
                bankTransfer_.importFoundCount,
                bankTransfer_.importValidCount,
                bankTransfer_.importWrittenCount),
            "info");
    }

    void PatchManagerActionHandler::writeNextImportSlot(int slot, std::uint64_t generation)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (! isImportFamilyKind(bankTransfer_.kind) || bankTransfer_.generation != generation)
            return;

        if (bankTransfer_.cancelRequested)
        {
            beginBankImportRestore(generation, bankImportFamilyCancelMessage(), "warning");
            return;
        }

        if (midiManager_ == nullptr || ! midiManager_->isEditorOutboundAllowed())
        {
            beginBankImportRestore(generation, kDeviceUnavailableFooterMessage, "warning");
            return;
        }

        if (slot >= bankTransfer_.importValidCount)
        {
            finishImportFamilyWriteSuccess();
            return;
        }

        const auto* packed = bankTransfer_.importPatches[static_cast<size_t>(slot)].data();
        midiManager_->sendPatch(static_cast<juce::uint8>(slot), packed);
        rememberOverlayFromPackedSlot(bankTransfer_.bank, slot, packed);

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
        if (! isImportFamilyKind(bankTransfer_.kind) || bankTransfer_.generation != generation)
            return;

        if (bankTransfer_.deviceSnapshot.empty())
        {
            finishBankImport(footerMessage, severity);
            return;
        }

        if (midiManager_ == nullptr || ! midiManager_->isEditorOutboundAllowed())
        {
            finishBankImport(bankImportFamilyRestoreFailedMessage(), "warning");
            return;
        }

        bankTransfer_.isRestoring = true;
        bankTransfer_.pendingFooterMessage = footerMessage;
        bankTransfer_.pendingFooterSeverity = severity;
        bankTransfer_.completedSlots = 0;

        if (bankTransferProgress_.setCancelEnabled)
            bankTransferProgress_.setCancelEnabled(false);

        showBankImportRestoreProgress();
        restoreNextSnapshotSlot(0, generation);
    }

    void PatchManagerActionHandler::restoreNextSnapshotSlot(int slot, std::uint64_t generation)
    {
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

        if (! isImportFamilyKind(bankTransfer_.kind) || bankTransfer_.generation != generation)
            return;

        if (slot >= static_cast<int>(bankTransfer_.deviceSnapshot.size()))
        {
            finishBankImport(bankTransfer_.pendingFooterMessage, bankTransfer_.pendingFooterSeverity);
            return;
        }

        if (midiManager_ == nullptr || ! midiManager_->isEditorOutboundAllowed())
        {
            finishBankImport(bankImportFamilyRestoreFailedMessage(), "warning");
            return;
        }

        const auto* packed = bankTransfer_.deviceSnapshot[static_cast<size_t>(slot)].data();
        midiManager_->sendPatch(static_cast<juce::uint8>(slot), packed);
        restoreOverlayFromPackedSlot(bankTransfer_.bank, slot, packed);

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

    void PatchManagerActionHandler::applyWrittenImportSlotToEditor(const DeviceMemoryLimits& limits,
                                                                   int importedBank,
                                                                   int currentPatch,
                                                                   const PackedPatchBuffer& writtenCurrentSlot)
    {
        if (patchModel_ == nullptr || apvtsPatchMapper_ == nullptr)
            return;

        abandonPendingDeviceLoad();
        clearLastDeviceDumpRawName();
        patchModel_->loadFrom(writtenCurrentSlot.data());
        patchModel_->normalizeNameEncoding();
        applyResolvedPatchName(*patchModel_,
                               PatchCoordinates { importedBank, currentPatch },
                               limits,
                               PatchNameResolvePurpose::kDisplay);

        using namespace PatchManagerActionHandlerInternal;
        pushPatchModelToApvtsWithSuppress(apvts_, hooks_, *apvtsPatchMapper_, patchNameSyncer_);
        establishEditorialCheckpoint();

        noteDevicePatchOrigin(importedBank, currentPatch);

        if (hooks_.onPatchLoaded)
            hooks_.onPatchLoaded();
    }

    void PatchManagerActionHandler::schedulePostImportDeviceReload(const DeviceMemoryLimits& limits)
    {
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

    std::optional<PatchManagerActionHandler::PackedPatchBuffer>
    PatchManagerActionHandler::takeWrittenCurrentImportSlot(bool importSucceeded, int currentPatch) const
    {
        if (! importSucceeded || currentPatch < 0)
            return std::nullopt;

        if (currentPatch >= bankTransfer_.importWrittenCount)
            return std::nullopt;

        if (currentPatch >= static_cast<int>(bankTransfer_.importPatches.size()))
            return std::nullopt;

        return bankTransfer_.importPatches[static_cast<size_t>(currentPatch)];
    }

    void PatchManagerActionHandler::applyBankImportFinishSideEffects(const BankImportFinishContext& context)
    {
        if (context.wasPaste && context.importSucceeded && hooks_.disarmClipboardFeedback)
            hooks_.disarmClipboardFeedback();
        if (hooks_.refreshClipboardMirrors)
            hooks_.refreshClipboardMirrors();
        if (context.writtenCurrentSlot.has_value())
        {
            applyWrittenImportSlotToEditor(context.limits,
                                           context.importedBank,
                                           context.currentPatch,
                                           *context.writtenCurrentSlot);
        }
        if (context.deviceMayHaveChanged && isDeviceDumpAvailable())
            schedulePostImportDeviceReload(context.limits);
    }

    void PatchManagerActionHandler::finishBankImport(const juce::String& footerMessage,
                                                     const juce::String& severity)
    {
        if (bankTransferProgress_.hide)
            bankTransferProgress_.hide();

        BankImportFinishContext context;
        context.limits = bankTransfer_.limits;
        context.importedBank = bankTransfer_.bank;
        context.currentPatch = getCurrentPatch(context.limits);
        context.importSucceeded = severity == "info";
        context.wasPaste = bankTransfer_.kind == BankTransferState::Kind::kPaste;
        context.deviceMayHaveChanged = context.importSucceeded
            || bankTransfer_.isRestoring
            || bankTransfer_.importWrittenCount > 0;
        context.writtenCurrentSlot = takeWrittenCurrentImportSlot(
            context.importSucceeded, context.currentPatch);

        bankTransfer_ = BankTransferState {};
        publishBankTransferFooter(footerMessage, severity);
        applyBankImportFinishSideEffects(context);
    }

} // namespace Core
