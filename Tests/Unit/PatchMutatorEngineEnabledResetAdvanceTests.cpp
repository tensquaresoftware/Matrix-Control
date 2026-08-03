#include <array>

#include "PatchMutatorEngineTestSupport.h"

using namespace PatchMutatorEngineTestSupport;

class PatchMutatorEngineEnabledResetAdvanceTests : public juce::UnitTest
{
public:
    PatchMutatorEngineEnabledResetAdvanceTests() : juce::UnitTest("PatchMutatorEngineEnabledResetAdvance") {}

    void runTest() override
    {
        enabled_emptyHistory();
        enabled_emptyHistory_noModuleToggle_disabled();
        enabled_afterFirstMutate();
        enabled_afterFirstMutate_clearLastToggle_disablesMutate();
        enabled_rootLimit();
        enabled_retryLimit();
        enabled_afterDeleteLast();
        enabled_afterDefrag();
        reset_afterMutate_clearsHistoryAndSnapshot();
        reset_afterCompareActive();
        reset_preservesRecipe();
        reset_noAuditionSysEx();
        reset_enabledMirrorsEmptyHistory();
        advance_emptyHistory_noOp();
        advance_singleEntry_noOp();
        advance_next_flatOrderMultiRootRetry();
        advance_previous_wrapsFromFirstToLast();
        advance_compareActive_noOp();
        advance_unknownSelection_noOp();
        advance_withInitialSnapshot_walksInitialFirst();
        advance_singleMutationWithInitial_togglesInitial();
    }

private:
    void enabled_emptyHistory()
    {
        beginTest("enabled_emptyHistory");

        EngineHarness harness;
        harness.setRecipe(100, 100, true);
        harness.engine.refreshActionEnabledMirrors(harness.proc.apvts);
        expectActionEnabledMirrors(*this, harness, ActionEnabledExpectations {
            .mutate = true,
            .retry = false,
            .exportEnabled = false,
            .deleteEnabled = false,
            .clear = false });
    }

    void enabled_emptyHistory_noModuleToggle_disabled()
    {
        beginTest("enabled_emptyHistory_noModuleToggle_disabled");

        EngineHarness harness;
        harness.setRecipe(100, 100, false);
        harness.engine.refreshActionEnabledMirrors(harness.proc.apvts);
        expectActionEnabledMirrors(*this, harness, ActionEnabledExpectations {
            .mutate = false,
            .retry = false,
            .exportEnabled = false,
            .deleteEnabled = false,
            .clear = false });
    }

    void enabled_afterFirstMutate()
    {
        beginTest("enabled_afterFirstMutate");

        EngineHarness harness;
        harness.setRecipe(100, 100, true);
        expect(harness.engine.mutate().success);
        expectActionEnabledMirrors(*this, harness, ActionEnabledExpectations {
            .mutate = true,
            .retry = true,
            .exportEnabled = true,
            .deleteEnabled = true,
            .clear = true });
    }

    void enabled_afterFirstMutate_clearLastToggle_disablesMutate()
    {
        beginTest("enabled_afterFirstMutate_clearLastToggle_disablesMutate");

        EngineHarness harness;
        harness.setRecipe(100, 100, true);
        expect(harness.engine.mutate().success);
        expectActionEnabledMirrors(*this, harness, ActionEnabledExpectations {
            .mutate = true,
            .retry = true,
            .exportEnabled = true,
            .deleteEnabled = true,
            .clear = true });

        harness.proc.apvts.state.setProperty(PatchMutator::kEnableDco1, false, nullptr);
        harness.engine.refreshActionEnabledMirrors(harness.proc.apvts);
        expectActionEnabledMirrors(*this, harness, ActionEnabledExpectations {
            .mutate = false,
            .retry = true,
            .exportEnabled = true,
            .deleteEnabled = true,
            .clear = true });
    }

    void enabled_rootLimit()
    {
        beginTest("enabled_rootLimit");

        EngineHarness harness;
        harness.setRecipe(100, 100, true);

        for (int i = 0; i < Core::MutationHistoryStore::kMaxRoots; ++i)
        {
            const auto resultPatch = makeDistinctBuffer(i + 10);
            const auto parentPatch = makeDistinctBuffer(i + 110);
            expect(harness.store().insertRoot(i, resultPatch, parentPatch));
        }

        harness.engine.syncHistoryUiProperties(harness.proc.apvts);
        expectActionEnabledMirrors(*this, harness, ActionEnabledExpectations {
            .mutate = false,
            .retry = true,
            .exportEnabled = true,
            .deleteEnabled = true,
            .clear = true });
    }

    void enabled_retryLimit()
    {
        beginTest("enabled_retryLimit");

        EngineHarness harness;
        harness.setRecipe(100, 100, true);

        auto m00 = makeDistinctBuffer(1801);
        auto m00Parent = makeDistinctBuffer(1802);
        Core::MutationNaming::applyPatchName(m00, 0);
        expect(harness.store().insertRoot(0, m00, m00Parent));

        for (int i = 0; i < Core::MutationHistoryStore::kMaxRetriesPerRoot; ++i)
        {
            auto retryPatch = makeDistinctBuffer(i + 1900);
            Core::MutationNaming::applyPatchName(retryPatch, 0, i);
            expect(harness.store().insertRetry(0, i, retryPatch, m00Parent));
        }

        harness.proc.apvts.state.setProperty(MutatorState::kSelectedMutateRootIndex, 0, nullptr);
        harness.engine.syncHistoryUiProperties(harness.proc.apvts);
        expectActionEnabledMirrors(*this, harness, ActionEnabledExpectations {
            .mutate = true,
            .retry = false,
            .exportEnabled = true,
            .deleteEnabled = true,
            .clear = true });
    }

    void enabled_afterDeleteLast()
    {
        beginTest("enabled_afterDeleteLast");

        EngineHarness harness;
        harness.setRecipe(100, 100, true);
        expect(harness.engine.mutate().success);
        expect(harness.engine.deleteSelected().success);
        expectActionEnabledMirrors(*this, harness, ActionEnabledExpectations {
            .mutate = true,
            .retry = false,
            .exportEnabled = false,
            .deleteEnabled = false,
            .clear = false });
    }

    void enabled_afterDefrag()
    {
        beginTest("enabled_afterDefrag");

        EngineHarness harness;
        harness.setRecipe(100, 100, true);

        auto m99 = makeDistinctBuffer(2001);
        auto m99Parent = makeDistinctBuffer(2002);
        Core::MutationNaming::applyPatchName(m99, 99);
        expect(harness.store().insertRoot(99, m99, m99Parent));

        harness.engine.syncHistoryUiProperties(harness.proc.apvts);
        expectActionEnabledMirrors(*this, harness, ActionEnabledExpectations {
            .mutate = false,
            .retry = true,
            .exportEnabled = true,
            .deleteEnabled = true,
            .clear = true });

        expect(harness.engine.defragHistory().success);
        expectActionEnabledMirrors(*this, harness, ActionEnabledExpectations {
            .mutate = true,
            .retry = true,
            .exportEnabled = true,
            .deleteEnabled = true,
            .clear = true });
    }

    void reset_afterMutate_clearsHistoryAndSnapshot()
    {
        beginTest("reset_afterMutate_clearsHistoryAndSnapshot");

        EngineHarness harness;
        harness.setRecipe(100, 100, true);
        expect(harness.engine.mutate().success);
        expect(harness.store().hasInitialSnapshot());
        expectEquals(harness.engine.rootCount(), 1);

        expect(harness.engine.resetSessionForPatchLoad().success);
        expectEquals(harness.store().rootCount(), 0);
        expect(! harness.store().hasInitialSnapshot());
        expect(! static_cast<bool>(harness.proc.apvts.state.getProperty(MutatorState::kCompareActive, false)));
        expect(harness.proc.apvts.state.getProperty(MutatorState::kHistoryMutateList).toString().isEmpty());
        expect(harness.proc.apvts.state.getProperty(MutatorState::kHistoryRetryList).toString().isEmpty());
        expect(harness.proc.apvts.state.getProperty(MutatorState::kHistoryRetryListsByRoot).toString().isEmpty());
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedMutateRootIndex)), -1);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedRetryIndex)),
                     Core::MutationHistoryStore::kRootOnly);
    }

    void reset_afterCompareActive()
    {
        beginTest("reset_afterCompareActive");

        EngineHarness harness;
        harness.setRecipe(100, 100, true);
        expect(harness.engine.mutate().success);
        expect(harness.engine.toggleCompare().success);
        expect(static_cast<bool>(harness.proc.apvts.state.getProperty(MutatorState::kCompareActive, false)));

        expect(harness.engine.resetSessionForPatchLoad().success);
        expect(! static_cast<bool>(harness.proc.apvts.state.getProperty(MutatorState::kCompareActive, false)));
        expectEquals(harness.store().rootCount(), 0);
    }

    void reset_preservesRecipe()
    {
        beginTest("reset_preservesRecipe");

        EngineHarness harness;
        harness.setRecipe(75, 50, true);
        harness.proc.apvts.state.setProperty(PatchMutator::kEnableDco2, true, nullptr);
        harness.proc.apvts.state.setProperty(PatchMutator::kEnableVcfVca, true, nullptr);
        expect(harness.engine.mutate().success);

        const auto amountBefore = static_cast<int>(harness.proc.apvts.state.getProperty(PatchMutator::kAmount, 0));
        const auto randomBefore = static_cast<int>(harness.proc.apvts.state.getProperty(PatchMutator::kRandom, 0));
        const auto dco1Before = static_cast<bool>(harness.proc.apvts.state.getProperty(PatchMutator::kEnableDco1, false));
        const auto dco2Before = static_cast<bool>(harness.proc.apvts.state.getProperty(PatchMutator::kEnableDco2, false));
        const auto vcfBefore = static_cast<bool>(harness.proc.apvts.state.getProperty(PatchMutator::kEnableVcfVca, false));

        expect(harness.engine.resetSessionForPatchLoad().success);

        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(PatchMutator::kAmount, 0)), amountBefore);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(PatchMutator::kRandom, 0)), randomBefore);
        expect(static_cast<bool>(harness.proc.apvts.state.getProperty(PatchMutator::kEnableDco1, false)) == dco1Before);
        expect(static_cast<bool>(harness.proc.apvts.state.getProperty(PatchMutator::kEnableDco2, false)) == dco2Before);
        expect(static_cast<bool>(harness.proc.apvts.state.getProperty(PatchMutator::kEnableVcfVca, false)) == vcfBefore);
    }

    void reset_noAuditionSysEx()
    {
        beginTest("reset_noAuditionSysEx");

        EngineHarness harness;
        harness.setRecipe(100, 100, true);
        expect(harness.engine.mutate().success);
        (void) countPatchSysExMessages(harness.queue);

        expect(harness.engine.resetSessionForPatchLoad().success);
        expectEquals(countPatchSysExMessages(harness.queue), 0);
    }

    void reset_enabledMirrorsEmptyHistory()
    {
        beginTest("reset_enabledMirrorsEmptyHistory");

        EngineHarness harness;
        harness.setRecipe(100, 100, true);
        expect(harness.engine.mutate().success);
        expect(harness.engine.resetSessionForPatchLoad().success);
        expectActionEnabledMirrors(*this, harness, ActionEnabledExpectations {
            .mutate = true,
            .retry = false,
            .exportEnabled = false,
            .deleteEnabled = false,
            .clear = false });
    }

    void advance_emptyHistory_noOp()
    {
        beginTest("advance_emptyHistory_noOp");

        EngineHarness harness;
        harness.engine.advanceHistorySelection(true);
        harness.engine.advanceHistorySelection(false);

        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedMutateRootIndex, -1)),
                     -1);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedRetryIndex,
                                                                         Core::MutationHistoryStore::kRootOnly)),
                     Core::MutationHistoryStore::kRootOnly);
    }

    void advance_singleEntry_noOp()
    {
        beginTest("advance_singleEntry_noOp");

        EngineHarness harness;
        const auto parent = makeDistinctBuffer(1);
        const auto m00 = makeDistinctBuffer(100);
        expect(harness.store().insertRoot(0, m00, parent));
        harness.engine.setAuditionSelection(0, Core::MutationHistoryStore::kRootOnly);
        harness.engine.syncHistoryUiProperties(harness.proc.apvts);

        harness.engine.advanceHistorySelection(true);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedMutateRootIndex)), 0);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedRetryIndex)),
                     Core::MutationHistoryStore::kRootOnly);

        harness.engine.advanceHistorySelection(false);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedMutateRootIndex)), 0);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedRetryIndex)),
                     Core::MutationHistoryStore::kRootOnly);
    }

    void advance_next_flatOrderMultiRootRetry()
    {
        beginTest("advance_next_flatOrderMultiRootRetry");

        EngineHarness harness;
        const auto parent = makeDistinctBuffer(1);
        expect(harness.store().insertRoot(0, makeDistinctBuffer(100), parent));
        expect(harness.store().insertRetry(0, 0, makeDistinctBuffer(110), parent));
        expect(harness.store().insertRetry(0, 1, makeDistinctBuffer(111), parent));
        expect(harness.store().insertRoot(1, makeDistinctBuffer(200), parent));
        expect(harness.store().insertRetry(1, 0, makeDistinctBuffer(210), parent));

        harness.engine.setAuditionSelection(0, Core::MutationHistoryStore::kRootOnly);
        harness.engine.syncHistoryUiProperties(harness.proc.apvts);

        // M00 → M00-R00 → M00-R01 → M01 → M01-R00 → wrap M00
        const std::array<std::pair<int, int>, 5> expected {
            std::pair { 0, 0 },
            std::pair { 0, 1 },
            std::pair { 1, Core::MutationHistoryStore::kRootOnly },
            std::pair { 1, 0 },
            std::pair { 0, Core::MutationHistoryStore::kRootOnly },
        };

        for (const auto& [root, retry] : expected)
        {
            harness.engine.advanceHistorySelection(true);
            expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedMutateRootIndex)),
                         root);
            expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedRetryIndex)),
                         retry);
        }
    }

    void advance_previous_wrapsFromFirstToLast()
    {
        beginTest("advance_previous_wrapsFromFirstToLast");

        EngineHarness harness;
        const auto parent = makeDistinctBuffer(1);
        expect(harness.store().insertRoot(0, makeDistinctBuffer(100), parent));
        expect(harness.store().insertRetry(0, 0, makeDistinctBuffer(110), parent));
        expect(harness.store().insertRoot(1, makeDistinctBuffer(200), parent));

        harness.engine.setAuditionSelection(0, Core::MutationHistoryStore::kRootOnly);
        harness.engine.syncHistoryUiProperties(harness.proc.apvts);

        harness.engine.advanceHistorySelection(false);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedMutateRootIndex)), 1);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedRetryIndex)),
                     Core::MutationHistoryStore::kRootOnly);
    }

    void advance_compareActive_noOp()
    {
        beginTest("advance_compareActive_noOp");

        EngineHarness harness;
        const auto parent = makeDistinctBuffer(1);
        expect(harness.store().insertRoot(0, makeDistinctBuffer(100), parent));
        expect(harness.store().insertRoot(1, makeDistinctBuffer(200), parent));
        harness.engine.setAuditionSelection(0, Core::MutationHistoryStore::kRootOnly);
        harness.engine.syncHistoryUiProperties(harness.proc.apvts);
        harness.proc.apvts.state.setProperty(MutatorState::kCompareActive, true, nullptr);

        harness.engine.advanceHistorySelection(true);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedMutateRootIndex)), 0);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedRetryIndex)),
                     Core::MutationHistoryStore::kRootOnly);
    }

    void advance_unknownSelection_noOp()
    {
        beginTest("advance_unknownSelection_noOp");

        EngineHarness harness;
        const auto parent = makeDistinctBuffer(1);
        expect(harness.store().insertRoot(0, makeDistinctBuffer(100), parent));
        expect(harness.store().insertRoot(1, makeDistinctBuffer(200), parent));
        harness.engine.setAuditionSelection(0, Core::MutationHistoryStore::kRootOnly);
        harness.engine.syncHistoryUiProperties(harness.proc.apvts);

        // Stale APVTS root not in the store — applySelection clears engine members; advance must not jump.
        harness.proc.apvts.state.setProperty(MutatorState::kSelectedMutateRootIndex, 5, nullptr);

        harness.engine.advanceHistorySelection(true);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedMutateRootIndex)), 5);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedRetryIndex)),
                     Core::MutationHistoryStore::kRootOnly);

        harness.engine.advanceHistorySelection(false);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedMutateRootIndex)), 5);
    }

    void advance_withInitialSnapshot_walksInitialFirst()
    {
        beginTest("advance_withInitialSnapshot_walksInitialFirst");

        EngineHarness harness;
        const auto parent = makeDistinctBuffer(1);
        expect(harness.store().insertRoot(0, makeDistinctBuffer(100), parent));
        expect(harness.store().insertRoot(1, makeDistinctBuffer(200), parent));
        harness.store().setInitialSnapshot(parent);

        harness.engine.setAuditionSelection(0, Core::MutationHistoryStore::kRootOnly);
        harness.engine.syncHistoryUiProperties(harness.proc.apvts);

        const auto expectInitialSelected = [this, &harness](bool selected)
        {
            expect(static_cast<bool>(
                       harness.proc.apvts.state.getProperty(MutatorState::kInitialSelected, false))
                   == selected);
        };

        // INITIAL leads the flat walk: M00 -> M01 -> INITIAL -> M00.
        harness.engine.advanceHistorySelection(true);
        expectInitialSelected(false);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedMutateRootIndex)), 1);

        harness.engine.advanceHistorySelection(true);
        expectInitialSelected(true);
        // M / R stay put so leaving INITIAL lands back on a real entry.
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedMutateRootIndex)), 1);

        harness.engine.advanceHistorySelection(true);
        expectInitialSelected(false);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedMutateRootIndex)), 0);

        // Backwards from the first mutation lands on INITIAL.
        harness.engine.advanceHistorySelection(false);
        expectInitialSelected(true);
    }

    void advance_singleMutationWithInitial_togglesInitial()
    {
        beginTest("advance_singleMutationWithInitial_togglesInitial");

        EngineHarness harness;
        const auto parent = makeDistinctBuffer(1);
        expect(harness.store().insertRoot(0, makeDistinctBuffer(100), parent));
        harness.store().setInitialSnapshot(parent);

        harness.engine.setAuditionSelection(0, Core::MutationHistoryStore::kRootOnly);
        harness.engine.syncHistoryUiProperties(harness.proc.apvts);

        // Two flat slots now (INITIAL + M00) — nav is no longer a no-op.
        harness.engine.advanceHistorySelection(true);
        expect(static_cast<bool>(harness.proc.apvts.state.getProperty(MutatorState::kInitialSelected, false)));

        harness.engine.advanceHistorySelection(true);
        expect(! static_cast<bool>(harness.proc.apvts.state.getProperty(MutatorState::kInitialSelected, false)));
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(MutatorState::kSelectedMutateRootIndex)), 0);
    }

};

static PatchMutatorEngineEnabledResetAdvanceTests patchMutatorEngineEnabledResetAdvanceTests;
