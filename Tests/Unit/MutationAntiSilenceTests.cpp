#include "MutationCalibrationTestSupport.h"

using namespace MutationCalibrationTestSupport;

// A mutation may be strange, but it must still make a sound: sacred amplitude parameters,
// the coupled anti-silence corrections, and the promise that a disabled module is left
// exactly as the user set it.
class MutationAntiSilenceTests : public juce::UnitTest
{
public:
    MutationAntiSilenceTests() : juce::UnitTest("MutationAntiSilence") {}

    void runTest() override
    {
        apply_guardsRunInsideThePipeline();
        closerModes_keepVca1VolumeAtSeed();
        seedRelay_freesVca2EnvelopeDepthFromItsLock();
        guard_vca1VolumeStaysAboveFloor();
        guard_closedFilterWithoutResonanceIsReopened();
        guard_dualWaveSelectOffIsRepaired();
        guard_dualWaveSelectOffSeedIsRepaired();
        guard_mixMovesOffASilentOscillator();
        guard_disabledModuleIsNeverRepaired();
    }

private:
    // The guards must be wired into the pipeline, not merely callable on their own.
    void apply_guardsRunInsideThePipeline()
    {
        beginTest("apply_guardsRunInsideThePipeline");

        auto seed = makeInitPatchModel();
        clearAllMatrixModBuses(seed);
        writeInt(seed, VcfVcaIds::kVca1Volume, 0);
        writeChoiceByName(seed, Dco1Ids::kWaveSelect, WaveSelectNames::kWave);
        writeChoiceByName(seed, Dco2Ids::kWaveSelect, WaveSelectNames::kOff);

        // Warp keeps VCA 1 Volume locked, so only the post-apply guard can lift it off zero.
        auto recipe = makeRecipe(Core::MutationMode::kWarp, Core::MutationPitchMode::kFree);
        recipe.enableDco1 = true;
        recipe.enableDco2 = true;
        recipe.enableVcfVca = true;

        for (int trial = 0; trial < kTrialCount; ++trial)
        {
            const auto result = mutated(seed, recipe, static_cast<juce::uint32>(0x6A11 + trial));

            expect(readInt(result, VcfVcaIds::kVca1Volume)
                   >= Core::MutationCalibration::kVca1VolumeFloor);
            expect(! isWaveSelectOff(result, Dco1Ids::kWaveSelect)
                   || ! isWaveSelectOff(result, Dco2Ids::kWaveSelect));
        }
    }

    // VCA 1 Volume is locked through Warp, and the guard only ever raises it, so a healthy
    // seed value must survive untouched.
    void closerModes_keepVca1VolumeAtSeed()
    {
        beginTest("closerModes_keepVca1VolumeAtSeed");

        constexpr int kHealthySeedVolume = 50;

        auto seed = makeInitPatchModel();
        clearAllMatrixModBuses(seed);
        writeInt(seed, VcfVcaIds::kVca1Volume, kHealthySeedVolume);

        for (const auto mode : { Core::MutationMode::kKindred,
                                 Core::MutationMode::kDrift,
                                 Core::MutationMode::kWarp })
        {
            auto recipe = makeRecipe(mode, Core::MutationPitchMode::kPreserve);
            recipe.enableVcfVca = true;
            recipe.enableEnvelope2 = true;

            for (int trial = 0; trial < kTrialCount; ++trial)
            {
                const auto result = mutated(seed, recipe, static_cast<juce::uint32>(0x1CA1 + trial));
                expectEquals(readInt(result, VcfVcaIds::kVca1Volume), kHealthySeedVolume);
            }
        }
    }

    // The VCA 2 <- ENV 2 depth is only sacred while it is the sole way to open VCA 2. A
    // Matrix Modulation bus already doing that job in the seed releases it.
    void seedRelay_freesVca2EnvelopeDepthFromItsLock()
    {
        beginTest("seedRelay_freesVca2EnvelopeDepthFromItsLock");

        constexpr int kSeedDepth = 50;

        auto withoutRelay = makeInitPatchModel();
        clearAllMatrixModBuses(withoutRelay);
        writeInt(withoutRelay, VcfVcaIds::kVca2ModByEnv2, kSeedDepth);

        auto withRelay = withoutRelay;
        writeMatrixModBus(withRelay, { 0, SourceNames::kEnvelope2, DestinationNames::kVca2Volume, 30 });

        auto recipe = makeRecipe(Core::MutationMode::kWarp, Core::MutationPitchMode::kPreserve);
        recipe.enableVcfVca = true;

        bool depthMovedWithRelay = false;

        for (int trial = 0; trial < kTrialCount; ++trial)
        {
            const auto rngSeed = static_cast<juce::uint32>(0x2E12 + trial);

            // No relay: the depth is the only amplitude path left, so it stays put.
            expectEquals(readInt(mutated(withoutRelay, recipe, rngSeed), VcfVcaIds::kVca2ModByEnv2),
                         kSeedDepth);

            if (readInt(mutated(withRelay, recipe, rngSeed), VcfVcaIds::kVca2ModByEnv2) != kSeedDepth)
                depthMovedWithRelay = true;
        }

        expect(depthMovedWithRelay);
    }

    void guard_vca1VolumeStaysAboveFloor()
    {
        beginTest("guard_vca1VolumeStaysAboveFloor");

        auto seed = makeInitPatchModel();
        clearAllMatrixModBuses(seed);

        auto collapsed = seed;
        writeInt(collapsed, VcfVcaIds::kVca1Volume, 0);
        writeInt(collapsed, VcfVcaIds::kVca2ModByEnv2, 0);

        auto recipe = makeRecipe(Core::MutationMode::kKindred, Core::MutationPitchMode::kPreserve);
        recipe.enableVcfVca = true;

        Core::applyPostMutationGuards(collapsed, seed, recipe, Core::MutationSeedFacts::read(seed));

        expect(readInt(collapsed, VcfVcaIds::kVca1Volume)
               >= Core::MutationCalibration::kVca1VolumeFloor);
        expect(readInt(collapsed, VcfVcaIds::kVca2ModByEnv2)
               >= Core::MutationCalibration::kVca2ModByEnv2Floor);
    }

    void guard_closedFilterWithoutResonanceIsReopened()
    {
        beginTest("guard_closedFilterWithoutResonanceIsReopened");

        const auto seed = makeInitPatchModel();

        auto smothered = seed;
        writeInt(smothered, VcfVcaIds::kFrequency, 0);
        writeInt(smothered, VcfVcaIds::kResonance, 0);

        auto recipe = makeRecipe(Core::MutationMode::kWarp, Core::MutationPitchMode::kFree);
        recipe.enableVcfVca = true;

        Core::applyPostMutationGuards(smothered, seed, recipe, Core::MutationSeedFacts::read(seed));

        expectEquals(readInt(smothered, VcfVcaIds::kFrequency),
                     Core::MutationCalibration::kVcfFrequencyRescueValue);
    }

    void guard_dualWaveSelectOffIsRepaired()
    {
        beginTest("guard_dualWaveSelectOffIsRepaired");

        auto seed = makeInitPatchModel();
        writeChoiceByName(seed, Dco1Ids::kWaveSelect, WaveSelectNames::kBoth);
        writeChoiceByName(seed, Dco2Ids::kWaveSelect, WaveSelectNames::kWave);

        auto silenced = seed;
        writeChoiceByName(silenced, Dco1Ids::kWaveSelect, WaveSelectNames::kOff);
        writeChoiceByName(silenced, Dco2Ids::kWaveSelect, WaveSelectNames::kOff);

        auto recipe = makeRecipe(Core::MutationMode::kWild, Core::MutationPitchMode::kFree);
        recipe.enableDco1 = true;
        recipe.enableDco2 = true;

        Core::applyPostMutationGuards(silenced, seed, recipe, Core::MutationSeedFacts::read(seed));

        expect(! isWaveSelectOff(silenced, Dco1Ids::kWaveSelect)
               || ! isWaveSelectOff(silenced, Dco2Ids::kWaveSelect));
    }

    void guard_dualWaveSelectOffSeedIsRepaired()
    {
        beginTest("guard_dualWaveSelectOffSeedIsRepaired");

        auto seed = makeInitPatchModel();
        writeChoiceByName(seed, Dco1Ids::kWaveSelect, WaveSelectNames::kOff);
        writeChoiceByName(seed, Dco2Ids::kWaveSelect, WaveSelectNames::kOff);

        auto silenced = seed;

        auto recipe = makeRecipe(Core::MutationMode::kWild, Core::MutationPitchMode::kFree);
        recipe.enableDco1 = true;
        recipe.enableDco2 = true;

        Core::applyPostMutationGuards(silenced, seed, recipe, Core::MutationSeedFacts::read(seed));

        expect(! isWaveSelectOff(silenced, Dco1Ids::kWaveSelect)
               || ! isWaveSelectOff(silenced, Dco2Ids::kWaveSelect));
    }

    void guard_mixMovesOffASilentOscillator()
    {
        beginTest("guard_mixMovesOffASilentOscillator");

        auto seed = makeInitPatchModel();
        writeChoiceByName(seed, Dco1Ids::kWaveSelect, WaveSelectNames::kOff);
        writeChoiceByName(seed, Dco2Ids::kWaveSelect, WaveSelectNames::kWave);

        const auto* balance = Core::findMutationIntDescriptor(VcfVcaIds::kBalance);
        expect(balance != nullptr);

        auto starved = seed;
        // Mix pinned to DCO 1 while DCO 1 is switched off — a silent patch.
        writeInt(starved, VcfVcaIds::kBalance, balance->maxValue);

        auto recipe = makeRecipe(Core::MutationMode::kDrift, Core::MutationPitchMode::kFree);
        recipe.enableVcfVca = true;

        Core::applyPostMutationGuards(starved, seed, recipe, Core::MutationSeedFacts::read(seed));

        expectEquals(readInt(starved, VcfVcaIds::kBalance),
                     Core::MutationCalibration::kMixRescueValue);
    }

    void guard_disabledModuleIsNeverRepaired()
    {
        beginTest("guard_disabledModuleIsNeverRepaired");

        const auto seed = makeInitPatchModel();

        auto collapsed = seed;
        writeInt(collapsed, VcfVcaIds::kVca1Volume, 0);
        const int volumeBefore = readInt(collapsed, VcfVcaIds::kVca1Volume);

        // VCF / VCA is off, so silence in that module is the user's business, not ours.
        auto recipe = makeRecipe(Core::MutationMode::kWild, Core::MutationPitchMode::kFree);
        recipe.enableDco1 = true;

        Core::applyPostMutationGuards(collapsed, seed, recipe, Core::MutationSeedFacts::read(seed));

        expectEquals(readInt(collapsed, VcfVcaIds::kVca1Volume), volumeBefore);
    }
};

static MutationAntiSilenceTests mutationAntiSilenceTests;
