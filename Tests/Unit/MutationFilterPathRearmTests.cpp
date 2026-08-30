#include "MutationCalibrationTestSupport.h"

using namespace MutationCalibrationTestSupport;

// Frozen regressions for the post-MM filter re-arm (opener restore can revive MM→VCF
// after the first filter-path pass). Kept out of MutationAntiSilenceTests for file size.
class MutationFilterPathRearmTests : public juce::UnitTest
{
public:
    MutationFilterPathRearmTests() : juce::UnitTest("MutationFilterPathRearm") {}

    void runTest() override
    {
        guard_filterCoupleReappliedAfterMmOpenerRestore();
        guard_filterCoupleReappliedAfterOpenerRestore_deterministic();
    }

private:
    void guard_filterCoupleReappliedAfterMmOpenerRestore()
    {
        beginTest("guard_filterCoupleReappliedAfterMmOpenerRestore");

        auto seed = makeInitPatchModel();
        clearAllMatrixModBuses(seed);
        writeInt(seed, VcfVcaIds::kFrequency, 5);
        writeInt(seed, VcfVcaIds::kResonance, 1);
        writeMatrixModBus(seed, { 0, SourceNames::kTrack, DestinationNames::kVcfFrequency, 40 });

        auto recipe = makeRecipe(Core::MutationMode::kWild, Core::MutationPitchMode::kFree);
        recipe.enableVcfVca = true;
        recipe.enableMatrixMod = true;

        int observedMmVcfTrials = 0;
        for (int trial = 0; trial < kTrialCount; ++trial)
        {
            const auto rngSeed = static_cast<juce::uint32>(0xF17E + trial);
            const auto result = mutated(seed, recipe, rngSeed);

            if (! Core::matrixModDrivesVcfFrequency(result))
                continue;

            ++observedMmVcfTrials;
            const int cutoff = readInt(result, VcfVcaIds::kFrequency);
            const int resonance = readInt(result, VcfVcaIds::kResonance);
            expect(cutoff >= Core::MutationCalibration::kVcfFrequencyComfortableOpen,
                   "cutoff=" + juce::String(cutoff) + " rng=" + juce::String(static_cast<int>(rngSeed)));
            expect(resonance >= Core::MutationCalibration::kVcfResonanceNearZeroThreshold,
                   "resonance=" + juce::String(resonance) + " rng=" + juce::String(static_cast<int>(rngSeed)));
        }

        expect(observedMmVcfTrials > 0, "expected at least one trial to retain MM→VCF");
    }

    void guard_filterCoupleReappliedAfterOpenerRestore_deterministic()
    {
        beginTest("guard_filterCoupleReappliedAfterOpenerRestore_deterministic");

        auto seed = makeInitPatchModel();
        clearAllMatrixModBuses(seed);
        writeInt(seed, VcfVcaIds::kFrequency, 5);
        writeInt(seed, VcfVcaIds::kResonance, 1);
        writeMatrixModBus(seed, { 0, SourceNames::kTrack, DestinationNames::kVcfFrequency, 40 });

        auto recipe = makeRecipe(Core::MutationMode::kDrift, Core::MutationPitchMode::kFree);
        recipe.enableVcfVca = true;
        recipe.enableMatrixMod = true;

        auto working = seed;
        working.loadFrom(seed.data());
        clearAllMatrixModBuses(working);
        writeInt(working, VcfVcaIds::kFrequency, Core::MutationCalibration::kVcfFrequencyRescueValue);
        writeInt(working, VcfVcaIds::kResonance, 1);

        Core::ensureMatrixModFilterOpeners(working, seed, recipe);
        expect(Core::matrixModDrivesVcfFrequency(working));

        Core::applyFilterPathGuards(working, recipe);
        expect(readInt(working, VcfVcaIds::kFrequency)
               >= Core::MutationCalibration::kVcfFrequencyComfortableOpen);
        expect(readInt(working, VcfVcaIds::kResonance)
               >= Core::MutationCalibration::kVcfResonanceNearZeroThreshold);
    }
};

static MutationFilterPathRearmTests mutationFilterPathRearmTests;
