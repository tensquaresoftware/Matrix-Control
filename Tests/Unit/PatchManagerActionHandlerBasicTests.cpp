#include "PatchManagerActionHandlerTestSupport.h"

using namespace PatchManagerActionHandlerTestSupport;

class PatchManagerActionHandlerBasicTests : public juce::UnitTest
{
public:
    PatchManagerActionHandlerBasicTests() : juce::UnitTest("PatchManagerActionHandlerBasic") {}

    void runTest() override
    {
        testPasteRomBankBlocked();
        testPasteRamBankSuccess();
        testPasteMatrix6_sendsPatchSlot();
        testStoreRomBankBlocked();
        testStoreRamBankSuccess();
        testInitLoadsTemplateAndBufferToApvts();
        testInitMatrix1000_sendsEditBuffer();
        testInitMatrix6_sendsPatchToCurrentSlot();
        testInitRomBankBlocked();
        testInitCompareActiveBlocked();
        testBankSelectMatrix1000SetBank();
        testBankSelectMatrix6NoSetBank();
        testNavigationWithinBankNoSetBank();
        testAt99_fourNext_staysBank0();
        testAt99_next_wrapsToPatch0_sameBank();
        testUnlock_doesNotChangeApvtsCoordinates();
        testColdStart_setBank_setsLockIndicatorTrue();
        testNavigationStaleSyncedBank_sendsSetBankOnFirstClick();
        testUnlockBankSends0CHOnly();
        testBankSelectSetsBanksLockedTrue();
        testUnlockBankMatrix6NoOp();
    }

private:
    void testPasteRomBankBlocked()
    {
        beginTest("paste_romBank_blocked");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 3, 12, false);
        harness.mapper.apvtsToBuffer();
        harness.clipboard.copyFullPatch(harness.model, "BANK 0 / PATCH 0");

        harness.handler.handleAction(InternalPatches::kPastePatch, juce::var());

        // ROM gating is visual-only — no warning footer.
        expect(harness.proc.apvts.state.getProperty("uiMessageText").toString().isEmpty());
        expect(harness.queue.isEmpty());
    }

    void testPasteRamBankSuccess()
    {
        beginTest("paste_ramBank_success");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 0, 7, false);
        harness.mapper.apvtsToBuffer();
        harness.clipboard.copyFullPatch(harness.model, "BANK 0 / PATCH 0");

        harness.handler.handleAction(InternalPatches::kPastePatch, juce::var());

        expect(harness.clipboard.canPasteFullPatch());
        const auto queued = scanQueue(harness.queue);
        expect(!queued.patchData);
        expect(queued.editBufferPatch);
    }

    void testPasteMatrix6_sendsPatchSlot()
    {
        beginTest("paste_matrix6_sendsPatchSlot");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix6));
        initializePatchManagerState(harness.proc.apvts.state, 0, 7, false);
        harness.mapper.apvtsToBuffer();
        harness.clipboard.copyFullPatch(harness.model, "BANK 0 / PATCH 0");

        harness.handler.handleAction(InternalPatches::kPastePatch, juce::var());

        const auto queued = scanQueue(harness.queue);
        expect(queued.patchData);
        expect(!queued.editBufferPatch);
        expectEquals(queued.patchNumber, 7);
    }

    void testStoreRomBankBlocked()
    {
        beginTest("store_romBank_blocked");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 5, 1, false);

        harness.handler.handleAction(InternalPatches::kStorePatch, juce::var());

        expect(harness.proc.apvts.state.getProperty("uiMessageText").toString().isEmpty());
        expect(harness.queue.isEmpty());
    }

    void testStoreRamBankSuccess()
    {
        beginTest("store_ramBank_success");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 1, 42, false);

        harness.handler.handleAction(InternalPatches::kStorePatch, juce::var());

        const auto queued = scanQueue(harness.queue);
        expect(queued.setBank);
        expectEquals(queued.setBankValue, 1);
        expect(queued.patchData);
        expect(!queued.editBufferPatch);
    }

    void testInitLoadsTemplateAndBufferToApvts()
    {
        beginTest("initFullPatch_loadsTemplate");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        harness.handler.handleAction(InternalPatches::kInitPatch, juce::var());

        expect(!harness.suppressPatchSysEx);
        expect(!harness.suppressMatrixModSysEx);
        expect(harness.proc.apvts.state.getProperty("uiMessageText").toString().isNotEmpty());

        const auto queued = scanQueue(harness.queue);
        expect(!queued.patchData);
        expect(queued.editBufferPatch);
        expectEquals(queued.patchSysExCount, 1);
    }

    void testInitMatrix1000_sendsEditBuffer()
    {
        beginTest("init_matrix1000_sendsEditBuffer");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 1, 12, false);

        harness.handler.handleAction(InternalPatches::kInitPatch, juce::var());

        const auto queued = scanQueue(harness.queue);
        expect(!queued.patchData);
        expect(queued.editBufferPatch);
        expectEquals(queued.patchSysExCount, 1);
    }

    void testInitMatrix6_sendsPatchToCurrentSlot()
    {
        beginTest("init_matrix6_sendsPatchToCurrentSlot");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix6));
        initializePatchManagerState(harness.proc.apvts.state, 0, 42, false);

        harness.handler.handleAction(InternalPatches::kInitPatch, juce::var());

        const auto queued = scanQueue(harness.queue);
        expect(queued.patchData);
        expect(!queued.editBufferPatch);
        expectEquals(queued.patchNumber, 42);
        expectEquals(queued.patchSysExCount, 1);
    }

    void testInitRomBankBlocked()
    {
        beginTest("init_romBank_blocked");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 5, 1, false);

        harness.handler.handleAction(InternalPatches::kInitPatch, juce::var());

        expect(harness.proc.apvts.state.getProperty("uiMessageText").toString().isEmpty());
        expect(harness.proc.apvts.state.getProperty("uiMessageSeverity").toString().isEmpty());
        expect(harness.queue.isEmpty());
    }

    void testInitCompareActiveBlocked()
    {
        beginTest("init_compareActive_blocked");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 1, 12, false);
        harness.proc.apvts.state.setProperty(MutatorState::kCompareActive, true, nullptr);

        harness.handler.handleAction(InternalPatches::kInitPatch, juce::var());

        expect(harness.queue.isEmpty());
    }

    void testBankSelectMatrix1000SetBank()
    {
        beginTest("bankSelect_matrix1000_setBank");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 0, 4, false);
        harness.patchSelectionMidiSync.resetLastSyncedBank(0);

        harness.handler.handleAction(BankUtility::StandaloneWidgets::kSelectBank3, juce::var());

        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(InternalPatches::kCurrentBankNumber)), 3);
        const auto queued = scanQueue(harness.queue);
        expect(queued.setBank);
        expectEquals(queued.setBankValue, 3);
    }

    void testBankSelectMatrix6NoSetBank()
    {
        beginTest("bankSelect_matrix6_noSetBank");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix6));
        initializePatchManagerState(harness.proc.apvts.state, 0, 4, false);
        harness.patchSelectionMidiSync.resetLastSyncedBank(0);

        harness.handler.handleAction(BankUtility::StandaloneWidgets::kSelectBank3, juce::var());

        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(InternalPatches::kCurrentBankNumber)), 0);
        expect(!static_cast<bool>(harness.proc.apvts.state.getProperty(BankUtility::StateProperties::kBanksLocked)));
        expect(harness.queue.isEmpty());
    }

    void testNavigationWithinBankNoSetBank()
    {
        beginTest("navigation_withinBank_noSetBank");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 0, 5, true);
        harness.patchSelectionMidiSync.resetLastSyncedBank(0);

        harness.handler.handleAction(InternalPatches::kLoadNextPatch, juce::var());

        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(InternalPatches::kCurrentBankNumber)), 0);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(InternalPatches::kCurrentPatchNumber)), 6);
        expect(static_cast<bool>(harness.proc.apvts.state.getProperty(BankUtility::StateProperties::kBanksLocked)));
        const auto queued = scanQueue(harness.queue);
        expect(!queued.setBank);
    }

    void testAt99_fourNext_staysBank0()
    {
        beginTest("at99_fourNext_staysBank0");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state,
                                    0,
                                    Matrix1000Limits::kMaxPatchNumber,
                                    true);
        harness.patchSelectionMidiSync.resetLastSyncedBank(0);

        for (int step = 0; step < 4; ++step)
            harness.handler.handleAction(InternalPatches::kLoadNextPatch, juce::var());

        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(InternalPatches::kCurrentBankNumber)), 0);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(InternalPatches::kCurrentPatchNumber)), 3);
        expect(static_cast<bool>(harness.proc.apvts.state.getProperty(BankUtility::StateProperties::kBanksLocked)));
        const auto queued = scanQueue(harness.queue);
        expect(!queued.setBank);
    }

    void testAt99_next_wrapsToPatch0_sameBank()
    {
        beginTest("at99_next_wrapsToPatch0_sameBank");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state,
                                    0,
                                    Matrix1000Limits::kMaxPatchNumber,
                                    false);
        harness.patchSelectionMidiSync.resetLastSyncedBank(0);

        harness.handler.handleAction(InternalPatches::kLoadNextPatch, juce::var());

        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(InternalPatches::kCurrentBankNumber)), 0);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(InternalPatches::kCurrentPatchNumber)),
                     Matrix1000Limits::kMinPatchNumber);
        expect(!static_cast<bool>(harness.proc.apvts.state.getProperty(BankUtility::StateProperties::kBanksLocked)));
        const auto queued = scanQueue(harness.queue);
        expect(!queued.setBank);
    }

    void testUnlock_doesNotChangeApvtsCoordinates()
    {
        beginTest("unlock_doesNotChangeApvtsCoordinates");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 2, 5, true);

        harness.handler.handleAction(BankUtility::StandaloneWidgets::kUnlockBank, juce::var());

        expect(!static_cast<bool>(harness.proc.apvts.state.getProperty(BankUtility::StateProperties::kBanksLocked)));
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(InternalPatches::kCurrentBankNumber)), 2);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(InternalPatches::kCurrentPatchNumber)), 5);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(BankUtility::StateProperties::kSelectedBank)), 2);
    }

    void testColdStart_setBank_setsLockIndicatorTrue()
    {
        beginTest("coldStart_setBank_setsLockIndicatorTrue");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 0, 0, false);

        harness.handler.handleAction(InternalPatches::kLoadNextPatch, juce::var());

        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(InternalPatches::kCurrentBankNumber)), 0);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(InternalPatches::kCurrentPatchNumber)), 1);
        expect(static_cast<bool>(harness.proc.apvts.state.getProperty(BankUtility::StateProperties::kBanksLocked)));
        const auto queued = scanQueue(harness.queue);
        expect(queued.setBank);
        expectEquals(queued.setBankValue, 0);
    }

    void testNavigationStaleSyncedBank_sendsSetBankOnFirstClick()
    {
        beginTest("navigation_staleSyncedBank_sendsSetBankOnFirstClick");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 9, 93, false);
        harness.patchSelectionMidiSync.resetLastSyncedBank(0);

        harness.handler.handleAction(InternalPatches::kLoadNextPatch, juce::var());

        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(InternalPatches::kCurrentBankNumber)), 9);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(InternalPatches::kCurrentPatchNumber)), 94);
        const auto queued = scanQueue(harness.queue);
        expect(queued.setBank);
        expectEquals(queued.setBankValue, 9);
    }

    void testUnlockBankSends0CHOnly()
    {
        beginTest("unlockBank_sends0CHOnly");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 7, 12, true);

        harness.handler.handleAction(BankUtility::StandaloneWidgets::kUnlockBank, juce::var());

        const auto queued = scanQueue(harness.queue);
        expect(queued.unlockBank);
        expect(!queued.setBank);
        expect(!queued.patchData);
    }

    void testBankSelectSetsBanksLockedTrue()
    {
        beginTest("bankSelect_setsBanksLockedTrue");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 0, 4, false);

        harness.handler.handleAction(BankUtility::StandaloneWidgets::kSelectBank3, juce::var());

        expect(static_cast<bool>(harness.proc.apvts.state.getProperty(BankUtility::StateProperties::kBanksLocked)));
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(InternalPatches::kCurrentBankNumber)), 3);
        const auto queued = scanQueue(harness.queue);
        expect(queued.setBank);
        expectEquals(queued.setBankValue, 3);
    }

    void testUnlockBankMatrix6NoOp()
    {
        beginTest("unlockBank_matrix6_noOp");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix6));
        initializePatchManagerState(harness.proc.apvts.state, 0, 4, true);
        harness.patchSelectionMidiSync.resetLastSyncedBank(0);

        harness.handler.handleAction(BankUtility::StandaloneWidgets::kUnlockBank, juce::var());

        expect(static_cast<bool>(harness.proc.apvts.state.getProperty(BankUtility::StateProperties::kBanksLocked)));
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(InternalPatches::kCurrentBankNumber)), 0);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(InternalPatches::kCurrentPatchNumber)), 4);
        expect(harness.queue.isEmpty());
    }
};

static PatchManagerActionHandlerBasicTests patchManagerActionHandlerBasicTests;
