#include "PatchMutatorEngineTestSupport.h"

using namespace PatchMutatorEngineTestSupport;

class PatchMutatorEngineSyncAuditionTests : public juce::UnitTest
{
public:
    PatchMutatorEngineSyncAuditionTests() : juce::UnitTest("PatchMutatorEngineSyncAudition") {}

    void runTest() override
    {
        sync_emptyHistory_emptySentinel();
        sync_afterInsertRoot_listsAndSelectsNewRoot();
        sync_sortedRoots_numericOrder();
        sync_retryListForSelectedRoot();
        sync_changingSelectedMutateRoot_rebuildsRetryList();
        sync_consecutiveMutates_listsAllRoots();
        applySelectionFromApvts_drivesAudition();
        audition_emptyHistory_noSysEx();
        audition_selectedRoot_sendsSysExOnce();
        audition_selectedRetry_sendsSysExOnce();
        audition_idempotent_skipsDuplicateSysEx();
        audition_compareActive_noSysEx();
    }

private:
    void sync_emptyHistory_emptySentinel()
    {
        beginTest("sync_emptyHistory_emptySentinel");

        EngineHarness harness;
        harness.engine.syncHistoryUiProperties(harness.proc.apvts);

        expect(harness.proc.apvts.state.getProperty(MutatorState::kHistoryMutateList).toString().isEmpty());
        expect(harness.proc.apvts.state.getProperty(MutatorState::kHistoryRetryList).toString().isEmpty());
        expect(harness.proc.apvts.state.getProperty(MutatorState::kHistoryRetryListsByRoot).toString().isEmpty());
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedMutateRootIndex)), -1);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedRetryIndex)),
                     Core::MutationHistoryStore::kRootOnly);
    }

    void sync_afterInsertRoot_listsAndSelectsNewRoot()
    {
        beginTest("sync_afterInsertRoot_listsAndSelectsNewRoot");

        EngineHarness harness;
        harness.setRecipe(100, 100, true);

        const auto result = harness.engine.mutate();
        expect(result.success);

        const auto mutateLabelList = harness.proc.apvts.state.getProperty(MutatorState::kHistoryMutateList).toString();
        expect(mutateLabelList.contains("M00"));
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedMutateRootIndex)), 0);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedRetryIndex)),
                     Core::MutationHistoryStore::kRootOnly);
    }

    void sync_sortedRoots_numericOrder()
    {
        beginTest("sync_sortedRoots_numericOrder");

        EngineHarness harness;

        const auto parent = makeDistinctBuffer(201);
        expect(harness.store().insertRoot(5, makeDistinctBuffer(205), parent));
        expect(harness.store().insertRoot(0, makeDistinctBuffer(200), parent));
        expect(harness.store().insertRoot(2, makeDistinctBuffer(202), parent));

        harness.engine.setAuditionSelection(2, Core::MutationHistoryStore::kRootOnly);
        harness.engine.syncHistoryUiProperties(harness.proc.apvts);

        expectEquals(harness.proc.apvts.state.getProperty(MutatorState::kHistoryMutateList).toString(),
                     juce::String("M00|M02|M05"));
    }

    void sync_retryListForSelectedRoot()
    {
        beginTest("sync_retryListForSelectedRoot");

        EngineHarness harness;

        const auto parent = makeDistinctBuffer(301);
        auto m05 = makeDistinctBuffer(305);
        Core::MutationNaming::applyPatchName(m05, 5);
        expect(harness.store().insertRoot(5, m05, parent));

        auto r00 = makeDistinctBuffer(310);
        Core::MutationNaming::applyPatchName(r00, 5, 0);
        expect(harness.store().insertRetry(5, 0, r00, parent));

        auto r02 = makeDistinctBuffer(312);
        Core::MutationNaming::applyPatchName(r02, 5, 2);
        expect(harness.store().insertRetry(5, 2, r02, parent));

        harness.engine.setAuditionSelection(5, Core::MutationHistoryStore::kRootOnly);
        harness.engine.syncHistoryUiProperties(harness.proc.apvts);

        const auto retryLabelList = harness.proc.apvts.state.getProperty(MutatorState::kHistoryRetryList).toString();
        const auto expectedRetryLabelList = MutatorDisplayNames::kHistoryRootSentinel + "|R00|R02";
        expectEquals(retryLabelList, expectedRetryLabelList);
    }

    void sync_changingSelectedMutateRoot_rebuildsRetryList()
    {
        beginTest("sync_changingSelectedMutateRoot_rebuildsRetryList");

        EngineHarness harness;

        const auto parent = makeDistinctBuffer(401);
        expect(harness.store().insertRoot(2, makeDistinctBuffer(402), parent));
        expect(harness.store().insertRoot(5, makeDistinctBuffer(405), parent));

        auto rOnM02 = makeDistinctBuffer(412);
        Core::MutationNaming::applyPatchName(rOnM02, 2, 1);
        expect(harness.store().insertRetry(2, 1, rOnM02, parent));

        auto rOnM05 = makeDistinctBuffer(415);
        Core::MutationNaming::applyPatchName(rOnM05, 5, 3);
        expect(harness.store().insertRetry(5, 3, rOnM05, parent));

        harness.proc.apvts.state.setProperty(MutatorState::kSelectedMutateRootIndex, 2, nullptr);
        harness.proc.apvts.state.setProperty(MutatorState::kSelectedRetryIndex,
                                             MutatorState::kSelectedRetryRootOnly,
                                             nullptr);
        harness.engine.rebuildHistoryListMirrors();
        const auto retryListForMutateRoot02 = harness.proc.apvts.state.getProperty(MutatorState::kHistoryRetryList).toString();

        harness.proc.apvts.state.setProperty(MutatorState::kSelectedMutateRootIndex, 5, nullptr);
        harness.proc.apvts.state.setProperty(MutatorState::kSelectedRetryIndex,
                                             MutatorState::kSelectedRetryRootOnly,
                                             nullptr);
        harness.engine.rebuildHistoryListMirrors();
        const auto retryListForMutateRoot05 = harness.proc.apvts.state.getProperty(MutatorState::kHistoryRetryList).toString();

        expect(retryListForMutateRoot02.contains("R01"));
        expect(retryListForMutateRoot05.contains("R03"));
        expect(retryListForMutateRoot02 != retryListForMutateRoot05);

        const auto retryListsByRoot = harness.proc.apvts.state.getProperty(MutatorState::kHistoryRetryListsByRoot).toString();
        expect(retryListsByRoot.contains("2=" + MutatorDisplayNames::kHistoryRootSentinel + "|R01"));
        expect(retryListsByRoot.contains("5=" + MutatorDisplayNames::kHistoryRootSentinel + "|R03"));
    }

    void sync_consecutiveMutates_listsAllRoots()
    {
        beginTest("sync_consecutiveMutates_listsAllRoots");

        EngineHarness harness;
        harness.setRecipe(100, 100, true);

        expect(harness.engine.mutate().success);
        expect(harness.engine.mutate().success);
        expect(harness.engine.mutate().success);

        expectEquals(harness.proc.apvts.state.getProperty(MutatorState::kHistoryMutateList).toString(),
                     juce::String("M00|M01|M02"));
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedMutateRootIndex)), 2);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedRetryIndex)),
                     Core::MutationHistoryStore::kRootOnly);
    }

    void applySelectionFromApvts_drivesAudition()
    {
        beginTest("applySelectionFromApvts_drivesAudition");

        EngineHarness harness;

        const auto parent = makeDistinctBuffer(501);
        auto m02 = makeDistinctBuffer(502);
        Core::MutationNaming::applyPatchName(m02, 2);
        expect(harness.store().insertRoot(2, m02, parent));

        harness.proc.apvts.state.setProperty(MutatorState::kSelectedMutateRootIndex, 2, nullptr);
        harness.proc.apvts.state.setProperty(MutatorState::kSelectedRetryIndex,
                                             Core::MutationHistoryStore::kRootOnly,
                                             nullptr);
        harness.applySelectionFromApvts();

        const auto audition = harness.resolveAuditionBuffer();
        expect(std::memcmp(audition.data(), m02.data(), Core::PatchModel::kBufferSize) == 0);
    }

    void audition_emptyHistory_noSysEx()
    {
        beginTest("audition_emptyHistory_noSysEx");

        EngineHarness harness;
        harness.engine.auditionSelectedHistoryEntry();
        expectEquals(countPatchSysExMessages(harness.queue), 0);
    }

    void audition_selectedRoot_sendsSysExOnce()
    {
        beginTest("audition_selectedRoot_sendsSysExOnce");

        EngineHarness harness;

        auto m00 = makeDistinctBuffer(701);
        auto m00Parent = makeDistinctBuffer(702);
        Core::MutationNaming::applyPatchName(m00, 0);
        expect(harness.store().insertRoot(0, m00, m00Parent));

        harness.proc.apvts.state.setProperty(MutatorState::kSelectedMutateRootIndex, 0, nullptr);
        harness.proc.apvts.state.setProperty(MutatorState::kSelectedRetryIndex,
                                             Core::MutationHistoryStore::kRootOnly,
                                             nullptr);

        harness.engine.auditionSelectedHistoryEntry();

        expectEquals(countPatchSysExMessages(harness.queue), 1);
        // Audition stamps the live model's name onto the pushed buffer (name SSOT stays
        // the editor, not the historical entry) — the params should still match m00.
        Core::PatchModel expected;
        expected.loadFrom(m00.data());
        expected.setName(juce::String());
        expect(std::memcmp(harness.model.data(), expected.data(), Core::PatchModel::kBufferSize) == 0);
    }

    void audition_selectedRetry_sendsSysExOnce()
    {
        beginTest("audition_selectedRetry_sendsSysExOnce");

        EngineHarness harness;

        auto m00 = makeDistinctBuffer(711);
        auto m00Parent = makeDistinctBuffer(712);
        Core::MutationNaming::applyPatchName(m00, 0);
        expect(harness.store().insertRoot(0, m00, m00Parent));

        auto r00 = makeDistinctBuffer(713);
        Core::MutationNaming::applyPatchName(r00, 0, 0);
        expect(harness.store().insertRetry(0, 0, r00, m00Parent));

        harness.proc.apvts.state.setProperty(MutatorState::kSelectedMutateRootIndex, 0, nullptr);
        harness.proc.apvts.state.setProperty(MutatorState::kSelectedRetryIndex, 0, nullptr);

        harness.engine.auditionSelectedHistoryEntry();

        expectEquals(countPatchSysExMessages(harness.queue), 1);
        // Audition stamps the live model's name onto the pushed buffer — compare params only.
        Core::PatchModel expected;
        expected.loadFrom(r00.data());
        expected.setName(juce::String());
        expect(std::memcmp(harness.model.data(), expected.data(), Core::PatchModel::kBufferSize) == 0);
    }

    void audition_idempotent_skipsDuplicateSysEx()
    {
        beginTest("audition_idempotent_skipsDuplicateSysEx");

        EngineHarness harness;

        auto m00 = makeDistinctBuffer(721);
        auto m00Parent = makeDistinctBuffer(722);
        Core::MutationNaming::applyPatchName(m00, 0);
        expect(harness.store().insertRoot(0, m00, m00Parent));

        harness.proc.apvts.state.setProperty(MutatorState::kSelectedMutateRootIndex, 0, nullptr);
        harness.proc.apvts.state.setProperty(MutatorState::kSelectedRetryIndex,
                                             Core::MutationHistoryStore::kRootOnly,
                                             nullptr);

        harness.engine.auditionSelectedHistoryEntry();
        expectEquals(countPatchSysExMessages(harness.queue), 1);

        harness.engine.auditionSelectedHistoryEntry();
        expectEquals(countPatchSysExMessages(harness.queue), 0);
    }

    void audition_compareActive_noSysEx()
    {
        beginTest("audition_compareActive_noSysEx");

        EngineHarness harness;

        auto m00 = makeDistinctBuffer(731);
        auto m00Parent = makeDistinctBuffer(732);
        Core::MutationNaming::applyPatchName(m00, 0);
        expect(harness.store().insertRoot(0, m00, m00Parent));

        harness.proc.apvts.state.setProperty(MutatorState::kSelectedMutateRootIndex, 0, nullptr);
        harness.proc.apvts.state.setProperty(MutatorState::kSelectedRetryIndex,
                                             Core::MutationHistoryStore::kRootOnly,
                                             nullptr);
        harness.proc.apvts.state.setProperty(MutatorState::kCompareActive, true, nullptr);

        harness.engine.auditionSelectedHistoryEntry();
        expectEquals(countPatchSysExMessages(harness.queue), 0);
    }

};

static PatchMutatorEngineSyncAuditionTests patchMutatorEngineSyncAuditionTests;
