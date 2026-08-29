#include "MutationCalibrationTestSupport.h"

#include <cmath>

using namespace MutationCalibrationTestSupport;

// Playable calibration: what MODE and PITCH promise, plus the Matrix Modulation ladder.
// Anti-silence guards live in MutationAntiSilenceTests, and the golden vectors for the
// parent Amount x Random curve stay in MutationAlgorithmTests.
class MutationCalibrationTests : public juce::UnitTest
{
public:
    MutationCalibrationTests() : juce::UnitTest("MutationCalibration") {}

    void runTest() override
    {
        modeConstants_driveAmountAndRandomAboveZero();
        pitchPreserve_keepsDcoFrequency();
        pitchPreserve_keepsMatrixModPitchBus();
        kindredAndDrift_preserveDcoInterval();
        kindredMatrixMod_touchesLiveBusAmountsOnly();
        matrixModRiskDestination_amountIsCapped();
        motion_deadLiveBusAmountIsRevived();
        motion_kindredNeverInventsARoute();
        motion_widerModesGrowOneBusWhenAllAreDead();
        motion_preserveDoesNotRevivePitchBusAmount();
        motion_silentLiveBusRevivedEvenWhenAnotherMoves();
        motion_deadLiveBusAmountIsRevivedThroughApply();
        family_cutoffDeltaDragsResonance();
        family_cutoffDeltaDragsResonanceThroughApply();
    }

private:
    void modeConstants_driveAmountAndRandomAboveZero()
    {
        beginTest("modeConstants_driveAmountAndRandomAboveZero");

        using namespace Core;
        namespace Cal = Core::MutationCalibration;

        const struct
        {
            MutationMode mode;
            int amount;
            int random;
        } expected[] = {
            { MutationMode::kKindred, Cal::kKindredAmountPercent, Cal::kKindredRandomPercent },
            { MutationMode::kDrift, Cal::kDriftAmountPercent, Cal::kDriftRandomPercent },
            { MutationMode::kWarp, Cal::kWarpAmountPercent, Cal::kWarpRandomPercent },
            { MutationMode::kWild, Cal::kWildAmountPercent, Cal::kWildRandomPercent },
        };

        for (const auto& row : expected)
        {
            MutationRecipe recipe;
            recipe.mode = row.mode;
            recipe.applyModeCalibration();

            expectEquals(recipe.amountPercent, row.amount);
            expectEquals(recipe.randomPercent, row.random);
            // A MODE must never early-exit on its own.
            expectGreaterThan(recipe.amountPercent, 0);
            expectGreaterThan(recipe.randomPercent, 0);
        }
    }

    void pitchPreserve_keepsDcoFrequency()
    {
        beginTest("pitchPreserve_keepsDcoFrequency");

        const auto seed = makeInitPatchModel();

        auto recipe = makeRecipe(Core::MutationMode::kWild, Core::MutationPitchMode::kPreserve);
        recipe.enableDco1 = true;
        recipe.enableDco2 = true;

        for (int trial = 0; trial < kTrialCount; ++trial)
        {
            const auto result = mutated(seed, recipe, static_cast<juce::uint32>(0x7000 + trial));
            expectEquals(readInt(result, Dco1Ids::kFrequency), readInt(seed, Dco1Ids::kFrequency));
            expectEquals(readInt(result, Dco2Ids::kFrequency), readInt(seed, Dco2Ids::kFrequency));
        }
    }

    void pitchPreserve_keepsMatrixModPitchBus()
    {
        beginTest("pitchPreserve_keepsMatrixModPitchBus");

        auto seed = makeInitPatchModel();
        clearAllMatrixModBuses(seed);
        writeMatrixModBus(seed, { 0, SourceNames::kLfo1, DestinationNames::kDco1Frequency, 24 });
        writeMatrixModBus(seed, { 1, SourceNames::kEnvelope1, DestinationNames::kVcfResonance, 20 });

        auto recipe = makeRecipe(Core::MutationMode::kWild, Core::MutationPitchMode::kPreserve);
        recipe.enableMatrixMod = true;

        for (int trial = 0; trial < kTrialCount; ++trial)
        {
            const auto result = mutated(seed, recipe, static_cast<juce::uint32>(0x9100 + trial));

            expectEquals(result.getChoiceIndex(busSource(0)), seed.getChoiceIndex(busSource(0)));
            expectEquals(result.getChoiceIndex(busDestination(0)),
                         seed.getChoiceIndex(busDestination(0)));
            expectEquals(result.getValue(busAmount(0)), seed.getValue(busAmount(0)));
        }
    }

    void kindredAndDrift_preserveDcoInterval()
    {
        beginTest("kindredAndDrift_preserveDcoInterval");

        auto seed = makeInitPatchModel();
        writeInt(seed, Dco1Ids::kFrequency, 24);
        writeInt(seed, Dco2Ids::kFrequency, 31);
        const int seedInterval = readInt(seed, Dco1Ids::kFrequency) - readInt(seed, Dco2Ids::kFrequency);

        for (const auto mode : { Core::MutationMode::kKindred, Core::MutationMode::kDrift })
        {
            for (const auto pitchMode : { Core::MutationPitchMode::kConsonant,
                                          Core::MutationPitchMode::kDissonant,
                                          Core::MutationPitchMode::kFree })
            {
                auto recipe = makeRecipe(mode, pitchMode);
                recipe.enableDco1 = true;
                recipe.enableDco2 = true;

                for (int trial = 0; trial < kTrialCount; ++trial)
                {
                    const auto result = mutated(seed, recipe, static_cast<juce::uint32>(0x1A70 + trial));
                    expectEquals(readInt(result, Dco1Ids::kFrequency)
                                     - readInt(result, Dco2Ids::kFrequency),
                                 seedInterval);
                }
            }
        }
    }

    void kindredMatrixMod_touchesLiveBusAmountsOnly()
    {
        beginTest("kindredMatrixMod_touchesLiveBusAmountsOnly");

        auto seed = makeInitPatchModel();
        clearAllMatrixModBuses(seed);
        writeMatrixModBus(seed, { 0, SourceNames::kLfo1, DestinationNames::kDco1PulseWidth, 18 });

        auto recipe = makeRecipe(Core::MutationMode::kKindred, Core::MutationPitchMode::kFree);
        recipe.enableMatrixMod = true;

        bool liveAmountEverMoved = false;

        for (int trial = 0; trial < kTrialCount; ++trial)
        {
            const auto result = mutated(seed, recipe, static_cast<juce::uint32>(0x4B4D + trial));

            // Kindred never invents a route, so every source and destination byte holds.
            for (int busIndex = 0; busIndex < kBusCount; ++busIndex)
            {
                expectEquals(result.getChoiceIndex(busSource(busIndex)),
                             seed.getChoiceIndex(busSource(busIndex)));
                expectEquals(result.getChoiceIndex(busDestination(busIndex)),
                             seed.getChoiceIndex(busDestination(busIndex)));
            }

            // Dead buses keep their amount too — only the live bus may breathe.
            for (int busIndex = 1; busIndex < kBusCount; ++busIndex)
                expectEquals(result.getValue(busAmount(busIndex)), seed.getValue(busAmount(busIndex)));

            if (result.getValue(busAmount(0)) != seed.getValue(busAmount(0)))
                liveAmountEverMoved = true;
        }

        // Freezing the routing must not freeze Matrix Modulation altogether.
        expect(liveAmountEverMoved);
    }

    void matrixModRiskDestination_amountIsCapped()
    {
        beginTest("matrixModRiskDestination_amountIsCapped");

        auto seed = makeInitPatchModel();
        clearAllMatrixModBuses(seed);
        writeMatrixModBus(seed, { 0, SourceNames::kEnvelope1, DestinationNames::kVca1Volume, 60 });

        auto recipe = makeRecipe(Core::MutationMode::kWild, Core::MutationPitchMode::kFree);
        recipe.enableMatrixMod = true;

        const auto& catalog = Core::MatrixModChoiceCatalog::shared();

        for (int trial = 0; trial < kTrialCount; ++trial)
        {
            const auto result = mutated(seed, recipe, static_cast<juce::uint32>(0x5253 + trial));

            for (int busIndex = 0; busIndex < kBusCount; ++busIndex)
            {
                const auto bus = Core::readMatrixModBus(result, busIndex);
                if (! bus.isLive || ! catalog.isRiskDestination(bus.destinationIndex))
                    continue;

                expect(bus.amount >= Core::MutationCalibration::kMatrixModRiskAmountFloor);
                expect(bus.amount <= Core::MutationCalibration::kMatrixModRiskAmountCeiling);
            }
        }
    }

    void motion_deadLiveBusAmountIsRevived()
    {
        beginTest("motion_deadLiveBusAmountIsRevived");

        auto patch = makeInitPatchModel();
        clearAllMatrixModBuses(patch);
        // Fully routed but silent: the one thing Matrix Modulation must never hand back.
        writeMatrixModBus(patch, { 0, SourceNames::kLfo1, DestinationNames::kDco1PulseWidth, 0 });

        Core::SeededRandom rng(0x4D4F5401U);
        Core::ensureMatrixModMotion(patch, Core::MutationMode::kKindred,
                                    Core::MutationPitchMode::kFree, rng);

        expectEquals(patch.getValue(busAmount(0)), Core::MutationCalibration::kMatrixModMotionAmount);
    }

    void motion_kindredNeverInventsARoute()
    {
        beginTest("motion_kindredNeverInventsARoute");

        auto patch = makeInitPatchModel();
        clearAllMatrixModBuses(patch);
        const auto before = patch;

        Core::SeededRandom rng(0x4D4F5402U);
        Core::ensureMatrixModMotion(patch, Core::MutationMode::kKindred,
                                    Core::MutationPitchMode::kFree, rng);

        for (int busIndex = 0; busIndex < kBusCount; ++busIndex)
        {
            expectEquals(patch.getChoiceIndex(busSource(busIndex)),
                         before.getChoiceIndex(busSource(busIndex)));
            expectEquals(patch.getChoiceIndex(busDestination(busIndex)),
                         before.getChoiceIndex(busDestination(busIndex)));
            expectEquals(patch.getValue(busAmount(busIndex)), before.getValue(busAmount(busIndex)));
        }
    }

    void motion_widerModesGrowOneBusWhenAllAreDead()
    {
        beginTest("motion_widerModesGrowOneBusWhenAllAreDead");

        for (const auto mode : { Core::MutationMode::kDrift,
                                 Core::MutationMode::kWarp,
                                 Core::MutationMode::kWild })
        {
            auto patch = makeInitPatchModel();
            clearAllMatrixModBuses(patch);

            Core::SeededRandom rng(0x4D4F5403U);
            Core::ensureMatrixModMotion(patch, mode, Core::MutationPitchMode::kFree, rng);

            int movingBusCount = 0;
            for (int busIndex = 0; busIndex < kBusCount; ++busIndex)
            {
                const auto bus = Core::readMatrixModBus(patch, busIndex);
                if (bus.isLive && bus.amount != 0)
                    ++movingBusCount;
            }

            // Exactly one new gesture, not a full matrix of them.
            expectEquals(movingBusCount, 1);
        }
    }

    void motion_preserveDoesNotRevivePitchBusAmount()
    {
        beginTest("motion_preserveDoesNotRevivePitchBusAmount");

        auto patch = makeInitPatchModel();
        clearAllMatrixModBuses(patch);
        writeMatrixModBus(patch, { 0, SourceNames::kLfo1, DestinationNames::kDco1Frequency, 0 });

        Core::SeededRandom rng(0x4D4F5404U);
        Core::ensureMatrixModMotion(patch, Core::MutationMode::kKindred,
                                    Core::MutationPitchMode::kPreserve, rng);

        expectEquals(patch.getValue(busAmount(0)), 0);
    }

    void motion_silentLiveBusRevivedEvenWhenAnotherMoves()
    {
        beginTest("motion_silentLiveBusRevivedEvenWhenAnotherMoves");

        auto patch = makeInitPatchModel();
        clearAllMatrixModBuses(patch);
        writeMatrixModBus(patch, { 0, SourceNames::kLfo1, DestinationNames::kDco1PulseWidth, 18 });
        writeMatrixModBus(patch, { 1, SourceNames::kLfo2, DestinationNames::kVcfResonance, 0 });

        Core::SeededRandom rng(0x4D4F5405U);
        Core::ensureMatrixModMotion(patch, Core::MutationMode::kDrift,
                                    Core::MutationPitchMode::kFree, rng);

        expectEquals(patch.getValue(busAmount(1)), Core::MutationCalibration::kMatrixModMotionAmount);
    }

    void family_cutoffDeltaDragsResonance()
    {
        beginTest("family_cutoffDeltaDragsResonance");

        auto seed = makeInitPatchModel();
        writeInt(seed, VcfVcaIds::kFrequency, 30);
        writeInt(seed, VcfVcaIds::kResonance, 20);

        auto working = seed;
        writeInt(working, VcfVcaIds::kFrequency, 50);

        auto recipe = makeRecipe(Core::MutationMode::kWarp, Core::MutationPitchMode::kFree);
        recipe.enableVcfVca = true;

        Core::applyFamilyGestures(working, seed, recipe);

        // Leader +20 → partner target 20+10; halfway from current 20 → 25.
        expectEquals(readInt(working, VcfVcaIds::kResonance), 25);
    }

    void motion_deadLiveBusAmountIsRevivedThroughApply()
    {
        beginTest("motion_deadLiveBusAmountIsRevivedThroughApply");

        auto seed = makeInitPatchModel();
        clearAllMatrixModBuses(seed);
        writeMatrixModBus(seed, { 0, SourceNames::kLfo1, DestinationNames::kDco1PulseWidth, 0 });

        auto recipe = makeRecipe(Core::MutationMode::kKindred, Core::MutationPitchMode::kFree);
        recipe.enableMatrixMod = true;

        for (int trial = 0; trial < kTrialCount; ++trial)
        {
            const auto result = mutated(seed, recipe, static_cast<juce::uint32>(0x4D415001U + static_cast<juce::uint32>(trial)));
            // Full apply must keep a live bus audible — silent Amount 0 is never returned.
            expect(result.getValue(busAmount(0)) != 0);
        }
    }

    void family_cutoffDeltaDragsResonanceThroughApply()
    {
        beginTest("family_cutoffDeltaDragsResonanceThroughApply");

        auto seed = makeInitPatchModel();
        writeInt(seed, VcfVcaIds::kFrequency, 30);
        writeInt(seed, VcfVcaIds::kResonance, 20);

        auto recipe = makeRecipe(Core::MutationMode::kWarp, Core::MutationPitchMode::kFree);
        recipe.enableVcfVca = true;

        const auto* resonance = Core::findMutationIntDescriptor(VcfVcaIds::kResonance);
        expect(resonance != nullptr);
        if (resonance == nullptr)
            return;

        bool reconstructedFamilyOutcome = false;

        for (int trial = 0; trial < kTrialCount; ++trial)
        {
            const auto result = mutated(seed, recipe, static_cast<juce::uint32>(0x46414D01U + static_cast<juce::uint32>(trial)));
            const int freqDelta = readInt(result, VcfVcaIds::kFrequency) - 30;
            if (freqDelta == 0)
                continue;

            const int coupledDelta = static_cast<int>(std::floor(
                static_cast<double>(freqDelta)
                    * static_cast<double>(Core::MutationCalibration::kFamilyCoupleScale)
                + 0.5));
            const int target = juce::jlimit(resonance->minValue, resonance->maxValue, 20 + coupledDelta);
            const int finalResonance = readInt(result, VcfVcaIds::kResonance);

            // Invert (jitteredRes + target) / 2 and re-run family from that mid-state.
            // If apply skipped family coupling, this reconstruction rarely matches.
            for (const int jitteredResonance : { 2 * finalResonance - target,
                                                 2 * finalResonance + 1 - target })
            {
                if (jitteredResonance < resonance->minValue || jitteredResonance > resonance->maxValue)
                    continue;

                auto mid = seed;
                writeInt(mid, VcfVcaIds::kFrequency, readInt(result, VcfVcaIds::kFrequency));
                writeInt(mid, VcfVcaIds::kResonance, jitteredResonance);
                Core::applyFamilyGestures(mid, seed, recipe);

                if (readInt(mid, VcfVcaIds::kResonance) == finalResonance)
                    reconstructedFamilyOutcome = true;
            }
        }

        expect(reconstructedFamilyOutcome);
    }
};

static MutationCalibrationTests mutationCalibrationTests;
