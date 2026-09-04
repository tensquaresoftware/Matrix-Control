#include "PatchManagerActionHandlerTestSupport.h"

#include "Core/Actions/BankTransferProgressPresenter.h"
#include "Core/MIDI/Queue/MidiRequestTiming.h"
#include "Core/Services/ClipboardService.h"

using namespace PatchManagerActionHandlerTestSupport;

namespace
{
    void installNoopProgressPresenter(HandlerHarness& harness)
    {
        Core::BankTransferProgressPresenter presenter;
        presenter.show = [](const juce::String&,
                            const juce::String&,
                            const juce::String&,
                            int,
                            std::function<void()>) {};
        presenter.hide = [] {};
        presenter.update = [](int) {};
        harness.handler.setBankTransferProgressPresenter(std::move(presenter));
    }

    void pumpMessageLoop(int milliseconds)
    {
        if (auto* mm = juce::MessageManager::getInstanceWithoutCreating())
            mm->runDispatchLoopUntil(milliseconds);
    }

    Core::ClipboardService::BankPatchArray makeBankPatches(juce::uint8 marker)
    {
        Core::ClipboardService::BankPatchArray patches {};
        for (auto& slot : patches)
            slot[0] = marker;
        return patches;
    }
}

class PatchManagerActionHandlerBankCopyPasteTests : public juce::UnitTest
{
public:
    PatchManagerActionHandlerBankCopyPasteTests()
        : juce::UnitTest("PatchManagerActionHandlerBankCopyPaste")
    {
    }

    void runTest() override
    {
        testCopyBankCommitsClipboardAfterDump();
        testCopyBusyRefusesBankSelectWithFooter();
        testPasteConfirmCancelLeavesClipboardAndSkipsWrite();
        testPasteRomTargetPublishesFooter();
    }

private:
    void testCopyBankCommitsClipboardAfterDump()
    {
        beginTest("copyBank_dumpCompletes_commitsBankClipboard");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        installNoopProgressPresenter(harness);
        harness.useSuccessfulDeviceDump();
        initializePatchManagerState(harness.proc.apvts.state, 2, 0, false);

        harness.handler.handleAction(BankUtility::StandaloneWidgets::kCopyBank, juce::var());

        const int settleMs = Core::MidiRequestTiming::deviceSettleMs(
            harness.midiManager.getRequiredSysExDelayMs());
        pumpMessageLoop(settleMs + 50);

        expect(harness.clipboard.getMode() == Core::ClipboardMode::Bank);
        expect(harness.clipboard.getBankSource().has_value());
        expectEquals(*harness.clipboard.getBankSource(), 2);
        expect(harness.proc.apvts.state.getProperty("uiMessageSeverity").toString() == "info");
    }

    void testCopyBusyRefusesBankSelectWithFooter()
    {
        beginTest("copyBank_busy_bankSelectPublishesBusyFooter");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        installNoopProgressPresenter(harness);
        harness.useSuccessfulDeviceDump();
        harness.dumpFakeState->deferCallback = true;
        initializePatchManagerState(harness.proc.apvts.state, 0, 0, false);

        // COPY marks the transfer busy before the settle timer fires.
        harness.handler.handleAction(BankUtility::StandaloneWidgets::kCopyBank, juce::var());

        harness.proc.apvts.state.setProperty("uiMessageText", "", nullptr);
        harness.handler.handleAction(BankUtility::StandaloneWidgets::kSelectBank1, juce::var());

        expectEquals(
            harness.proc.apvts.state.getProperty("uiMessageText").toString(),
            juce::String(PluginDisplayNames::PatchManagerSection::BankUtilityModule::kBankTransferBusyFooterMessage));
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(
                         BankUtility::StateProperties::kSelectedBank)),
                     0);
    }

    void testPasteConfirmCancelLeavesClipboardAndSkipsWrite()
    {
        beginTest("pasteBank_confirmCancel_skipsWriteKeepsClipboard");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        installNoopProgressPresenter(harness);
        initializePatchManagerState(harness.proc.apvts.state, 1, 0, false);
        harness.clipboard.copyBank(makeBankPatches(0x11), 0);

        harness.handler.setBankPasteConfirmGate([](int, int) { return false; });
        harness.handler.handleAction(BankUtility::StandaloneWidgets::kPasteBank, juce::var());

        expect(harness.clipboard.getMode() == Core::ClipboardMode::Bank);
        expectEquals(*harness.clipboard.getBankSource(), 0);
        expect(harness.queue.isEmpty());
        expect(harness.proc.apvts.state.getProperty("uiMessageText").toString().isEmpty());
    }

    void testPasteRomTargetPublishesFooter()
    {
        beginTest("pasteBank_romTarget_publishesBlockedFooter");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        installNoopProgressPresenter(harness);
        initializePatchManagerState(harness.proc.apvts.state, 5, 0, false);
        harness.clipboard.copyBank(makeBankPatches(0x22), 0);

        harness.handler.setBankPasteConfirmGate([](int, int) { return true; });
        harness.handler.handleAction(BankUtility::StandaloneWidgets::kPasteBank, juce::var());

        expectEquals(
            harness.proc.apvts.state.getProperty("uiMessageText").toString(),
            juce::String(PluginDisplayNames::PatchManagerSection::BankUtilityModule::kPasteRomBlockedFooterMessage));
        expect(harness.queue.isEmpty());
    }
};

static PatchManagerActionHandlerBankCopyPasteTests patchManagerActionHandlerBankCopyPasteTests;
