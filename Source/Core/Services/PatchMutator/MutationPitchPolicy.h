#pragma once

#include <juce_core/juce_core.h>

#include "Core/Services/PatchMutator/MutationPolicy.h"
#include "Core/Services/PatchMutator/MutationRandomSource.h"

namespace Core
{

    // DCO FREQUENCY is semitone-calibrated on the Matrix, so interval jumps are plain integers.
    constexpr int kSemitonesPerOctave = 12;

    struct MutationPitchRequest
    {
        MutationPitchMode pitchMode = MutationPitchMode::kPreserve;
        MutationMode mode = MutationMode::kDrift;
        int octaveWindow = MutationCalibration::kDefaultPitchOctaves;
        int minValue = 0;
        int maxValue = 0;
        int dco1Seed = 0;
        int dco2Seed = 0;
        bool dco1Writable = false;
        bool dco2Writable = false;
        // Neighborhood half-span already derived from the mode Amount x Random curve.
        int freeSpread = 0;
    };

    struct MutationPitchPlan
    {
        bool hasDco1Value = false;
        int dco1Value = 0;
        bool hasDco2Value = false;
        int dco2Value = 0;
    };

    // Preserve blocks DCO FREQUENCY and the Matrix Modulation pitch destinations everywhere.
    bool pitchPreserveBlocksMutation(MutationPitchMode pitchMode) noexcept;

    // Consumes random draws only for the DCOs the caller marked writable.
    MutationPitchPlan planPitchJumps(const MutationPitchRequest& request, IRandomSource& rng);

} // namespace Core
