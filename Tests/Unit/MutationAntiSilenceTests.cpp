#include "MutationCalibrationTestSupport.h"

#include <cstring>

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
        guard_openCutoffStillConstrainsNegativeFreqEnvMod();
        guard_nearZeroResonanceWithVcfFreqMmIsRaised();
        guard_dualWaveSelectOffIsRepaired();
        guard_dualWaveSelectOffSeedIsRepaired();
        guard_mixMovesOffASilentOscillator();
        guard_disabledModuleIsNeverRepaired();
        guard_env2SustainZeroWithShortReleaseIsRaised();
        guard_seedAmplitudeOpenerIsRestored();
        guard_seedAmplitudeOpenerSurvivesApply();
        guard_velocitySoftKillIsClampedEvenWithHealthyVolume();
        guard_lfoIntoVcaAmountIsCapped();
        guard_banjoStyleFilterOpenKeepsEnv1Amplitude();
        guard_seedFilterOpenerAmountIsRestored();
        guard_env2ExternalTriggerIsForcedToStrig();
        guard_env2DadrAndLongDelayAreTamedForVolumePath();
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

    void guard_openCutoffStillConstrainsNegativeFreqEnvMod()
    {
        beginTest("guard_openCutoffStillConstrainsNegativeFreqEnvMod");

        const auto seed = makeInitPatchModel();

        auto ducked = seed;
        writeInt(ducked, VcfVcaIds::kFrequency, 119);
        writeInt(ducked, VcfVcaIds::kResonance, 53);
        writeInt(ducked, VcfVcaIds::kFrequencyModByEnv1, -63);
        writeInt(ducked, VcfVcaIds::kFrequencyModByPressure, -40);

        auto recipe = makeRecipe(Core::MutationMode::kDrift, Core::MutationPitchMode::kFree);
        recipe.enableVcfVca = true;

        Core::applyPostMutationGuards(ducked, seed, recipe, Core::MutationSeedFacts::read(seed));

        expectEquals(readInt(ducked, VcfVcaIds::kFrequencyModByEnv1),
                     Core::MutationCalibration::kNegativeDepthFloorWhenBaseLow);
        expectEquals(readInt(ducked, VcfVcaIds::kFrequencyModByPressure),
                     Core::MutationCalibration::kNegativeDepthFloorWhenBaseLow);
    }

    void guard_nearZeroResonanceWithVcfFreqMmIsRaised()
    {
        beginTest("guard_nearZeroResonanceWithVcfFreqMmIsRaised");

        auto seed = makeInitPatchModel();
        clearAllMatrixModBuses(seed);
        writeInt(seed, VcfVcaIds::kFrequency, 61);
        writeInt(seed, VcfVcaIds::kResonance, 1);
        writeMatrixModBus(seed, { 0, SourceNames::kLfo2, DestinationNames::kVcfFrequency, 32 });

        auto working = seed;
        auto recipe = makeRecipe(Core::MutationMode::kDrift, Core::MutationPitchMode::kFree);
        recipe.enableVcfVca = true;

        Core::applyPostMutationGuards(working, seed, recipe, Core::MutationSeedFacts::read(seed));

        expectEquals(readInt(working, VcfVcaIds::kResonance),
                     Core::MutationCalibration::kVcfResonanceWhenFrequencyModulatedFloor);
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

    void guard_env2SustainZeroWithShortReleaseIsRaised()
    {
        beginTest("guard_env2SustainZeroWithShortReleaseIsRaised");

        auto seed = makeInitPatchModel();
        writeInt(seed, VcfVcaIds::kVca2ModByEnv2, 63);
        writeInt(seed, Env2Ids::kSustain, 0);
        writeInt(seed, Env2Ids::kRelease, 3);

        auto working = seed;
        auto recipe = makeRecipe(Core::MutationMode::kWarp, Core::MutationPitchMode::kFree);
        recipe.enableEnvelope2 = true;

        Core::applyPostMutationGuards(working, seed, recipe, Core::MutationSeedFacts::read(seed));

        expectEquals(readInt(working, Env2Ids::kSustain),
                     Core::MutationCalibration::kEnv2SustainFloorWhenReleaseShort);
    }

    void guard_seedAmplitudeOpenerIsRestored()
    {
        beginTest("guard_seedAmplitudeOpenerIsRestored");

        auto seed = makeInitPatchModel();
        clearAllMatrixModBuses(seed);
        writeInt(seed, VcfVcaIds::kVca1Volume, 0);
        writeMatrixModBus(seed, { 0, SourceNames::kEnvelope2, DestinationNames::kVca1Volume, 50 });

        auto working = seed;
        clearAllMatrixModBuses(working);
        writeMatrixModBus(working, { 0, SourceNames::kLfo1, DestinationNames::kVca1Volume, 40 });

        auto recipe = makeRecipe(Core::MutationMode::kWarp, Core::MutationPitchMode::kFree);
        recipe.enableMatrixMod = true;

        Core::ensureMatrixModAmplitudeOpeners(working, seed, recipe);

        expectEquals(working.getChoiceIndex(busSource(0)), seed.getChoiceIndex(busSource(0)));
        expectEquals(working.getChoiceIndex(busDestination(0)), seed.getChoiceIndex(busDestination(0)));
        expectEquals(working.getValue(busAmount(0)),
                     juce::jmin(seed.getValue(busAmount(0)),
                                Core::MutationCalibration::kMatrixModRiskAmountCeiling));
    }

    void guard_seedAmplitudeOpenerSurvivesApply()
    {
        beginTest("guard_seedAmplitudeOpenerSurvivesApply");

        auto seed = makeInitPatchModel();
        clearAllMatrixModBuses(seed);
        writeInt(seed, VcfVcaIds::kVca1Volume, 0);
        writeMatrixModBus(seed, { 0, SourceNames::kEnvelope2, DestinationNames::kVca1Volume, 50 });

        auto recipe = makeRecipe(Core::MutationMode::kWarp, Core::MutationPitchMode::kFree);
        recipe.enableMatrixMod = true;
        recipe.enableVcfVca = true;

        const auto result = mutated(seed, recipe, 0xA11D0001U);
        auto afterGuard = result;
        Core::ensureMatrixModAmplitudeOpeners(afterGuard, seed, recipe);

        // Apply already kept a stable opener, so a second ensure is a no-op.
        expect(std::memcmp(result.data(), afterGuard.data(), Core::PatchModel::kBufferSize) == 0);
    }

    void guard_velocitySoftKillIsClampedEvenWithHealthyVolume()
    {
        beginTest("guard_velocitySoftKillIsClampedEvenWithHealthyVolume");

        auto seed = makeInitPatchModel();
        writeInt(seed, VcfVcaIds::kVca1Volume, 62);
        writeInt(seed, VcfVcaIds::kVca1ModByVelocity, -18);

        auto working = seed;
        auto recipe = makeRecipe(Core::MutationMode::kWarp, Core::MutationPitchMode::kFree);
        recipe.enableVcfVca = true;

        Core::applyPostMutationGuards(working, seed, recipe, Core::MutationSeedFacts::read(seed));

        expect(readInt(working, VcfVcaIds::kVca1ModByVelocity)
               >= Core::MutationCalibration::kVelocityNegativeFloor);
    }

    void guard_lfoIntoVcaAmountIsCapped()
    {
        beginTest("guard_lfoIntoVcaAmountIsCapped");

        auto working = makeInitPatchModel();
        clearAllMatrixModBuses(working);
        writeMatrixModBus(working, { 0, SourceNames::kLfo1, DestinationNames::kVca1Volume, 38 });

        Core::capMatrixModRiskAmounts(working);

        expect(std::abs(working.getValue(busAmount(0)))
               <= Core::MutationCalibration::kMatrixModTremoloAmountCeiling);
    }

    void guard_banjoStyleFilterOpenKeepsEnv1Amplitude()
    {
        beginTest("guard_banjoStyleFilterOpenKeepsEnv1Amplitude");

        auto seed = makeInitPatchModel();
        clearAllMatrixModBuses(seed);
        writeInt(seed, VcfVcaIds::kFrequency, 0);
        writeInt(seed, VcfVcaIds::kResonance, 0);
        writeInt(seed, VcfVcaIds::kFrequencyModByEnv1, 63);
        writeInt(seed, Env1Ids::kAmplitude, 50);
        writeMatrixModBus(seed, { 0, SourceNames::kTrack, DestinationNames::kVcfFrequency, 59 });

        auto working = seed;
        writeInt(working, VcfVcaIds::kFrequency, 40);
        writeInt(working, Env1Ids::kAmplitude, 4);
        writeMatrixModBus(working, { 0, SourceNames::kTrack, DestinationNames::kVcfFrequency, 40 });

        auto recipe = makeRecipe(Core::MutationMode::kWarp, Core::MutationPitchMode::kFree);
        recipe.enableVcfVca = true;
        recipe.enableEnvelope1 = true;
        recipe.enableMatrixMod = true;

        Core::applyPostMutationGuards(working, seed, recipe, Core::MutationSeedFacts::read(seed));

        expect(readInt(working, Env1Ids::kAmplitude)
               >= Core::MutationCalibration::kEscalatedEnvelopeAmplitudeFloor);
        expect(readInt(working, VcfVcaIds::kFrequency)
               >= Core::MutationCalibration::kVcfFrequencyComfortableOpen);
    }

    void guard_seedFilterOpenerAmountIsRestored()
    {
        beginTest("guard_seedFilterOpenerAmountIsRestored");

        auto seed = makeInitPatchModel();
        clearAllMatrixModBuses(seed);
        writeInt(seed, VcfVcaIds::kFrequency, 0);
        writeMatrixModBus(seed, { 0, SourceNames::kTrack, DestinationNames::kVcfFrequency, 59 });

        auto working = seed;
        clearAllMatrixModBuses(working);
        writeMatrixModBus(working, { 0, SourceNames::kLfo1, DestinationNames::kVcfResonance, 20 });

        auto recipe = makeRecipe(Core::MutationMode::kWarp, Core::MutationPitchMode::kFree);
        recipe.enableMatrixMod = true;

        Core::ensureMatrixModFilterOpeners(working, seed, recipe);

        expectEquals(working.getChoiceIndex(busSource(0)), seed.getChoiceIndex(busSource(0)));
        expectEquals(working.getChoiceIndex(busDestination(0)), seed.getChoiceIndex(busDestination(0)));
        expectEquals(working.getValue(busAmount(0)),
                     Core::MutationCalibration::kMatrixModRiskAmountCeiling);
    }

    void guard_env2ExternalTriggerIsForcedToStrig()
    {
        beginTest("guard_env2ExternalTriggerIsForcedToStrig");

        auto seed = makeInitPatchModel();
        writeInt(seed, VcfVcaIds::kVca2ModByEnv2, 63);
        writeChoiceByName(seed, Env2Ids::kTriggerMode, TriggerModeNames::kXmtrig);

        auto working = seed;
        auto recipe = makeRecipe(Core::MutationMode::kWild, Core::MutationPitchMode::kPreserve);
        recipe.enableEnvelope2 = true;

        Core::applyPostMutationGuards(working, seed, recipe, Core::MutationSeedFacts::read(seed));

        const auto* trigger = Core::findMutationChoiceDescriptor(Env2Ids::kTriggerMode);
        expect(trigger != nullptr);
        expectEquals(working.getChoiceIndex(*trigger),
                     trigger->choices.indexOf(juce::String(TriggerModeNames::kStrig)));
    }

    void guard_env2DadrAndLongDelayAreTamedForVolumePath()
    {
        beginTest("guard_env2DadrAndLongDelayAreTamedForVolumePath");

        auto seed = makeInitPatchModel();
        writeInt(seed, VcfVcaIds::kVca2ModByEnv2, 63);
        writeInt(seed, Env2Ids::kDelay, 23);
        writeChoiceByName(seed, Env2Ids::kEnvelopeMode, EnvelopeModeNames::kDadr);
        writeChoiceByName(seed, Env2Ids::kTriggerMode, TriggerModeNames::kStrig);

        auto working = seed;
        auto recipe = makeRecipe(Core::MutationMode::kWild, Core::MutationPitchMode::kPreserve);
        recipe.enableEnvelope2 = true;

        Core::applyPostMutationGuards(working, seed, recipe, Core::MutationSeedFacts::read(seed));

        expect(readInt(working, Env2Ids::kDelay)
               <= Core::MutationCalibration::kEnv2DelayCeilingWhenVolumePath);

        const auto* mode = Core::findMutationChoiceDescriptor(Env2Ids::kEnvelopeMode);
        expect(mode != nullptr);
        expectEquals(working.getChoiceIndex(*mode),
                     mode->choices.indexOf(juce::String(EnvelopeModeNames::kNormal)));
    }
};

static MutationAntiSilenceTests mutationAntiSilenceTests;
