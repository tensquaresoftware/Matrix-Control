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

        if (bankTransfer_.kind != BankTransferState::Kind::kImport || bankTransfer_.generation != generation)
            return;

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
        using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;
        using namespace PluginDisplayNames::Dialogs::BankTransferProgress;

        if (bankTransfer_.kind != BankTransferState::Kind::kImport || bankTransfer_.generation != generation)
            return;

        if (bankTransfer_.deviceSnapshot.empty())
        {
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
        captureCleanSnapshot();

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

    void PatchManagerActionHandler::finishBankImport(const juce::String& footerMessage,
                                                     const juce::String& severity)
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

        const auto writtenCurrentSlot = takeWrittenCurrentImportSlot(importSucceeded, currentPatch);

        bankTransfer_ = BankTransferState {};
        publishBankTransferFooter(footerMessage, severity);

        if (writtenCurrentSlot.has_value())
            applyWrittenImportSlotToEditor(limits, importedBank, currentPatch, *writtenCurrentSlot);

        if (! deviceMayHaveChanged || ! isDeviceDumpAvailable())
            return;

        schedulePostImportDeviceReload(limits);
    }

} // namespace Core
