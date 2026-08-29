#pragma once

#include "Core/Models/PatchModel.h"
#include "Core/Services/PatchMutator/MutationRecipe.h"

namespace Core
{

    // Which risk roles Matrix Modulation plays in a given patch. Read from the working
    // patch at guard time, because Warp and Wild may have rerouted a bus away from the
    // role the seed relied on.
    struct MutationMatrixModRoles
    {
        bool drivesVca2Volume = false;
        bool envelope1DrivesRiskDestination = false;
        bool envelope3DrivesRiskDestination = false;

        static MutationMatrixModRoles read(const PatchModel& patch);
    };

    // Read from the full seed patch. Reading a disabled module is allowed; writing is not.
    struct MutationSeedFacts
    {
        bool hasFmModulation = false;
        bool dco2WasNoise = false;
        int activeDcoCount = 0;

        static MutationSeedFacts read(const PatchModel& seed);
    };

    // Coupled anti-silence corrections that need more than one parameter to decide.
    // Writes only parameters whose owning Mutator toggle is on.
    void applyPostMutationGuards(PatchModel& inOut,
                                 const PatchModel& seed,
                                 const MutationRecipe& recipe,
                                 const MutationSeedFacts& facts);

    // Amplitude path + escalated envelope floors from the working patch's Matrix Mod roles.
    // Call again after MM motion so a revived risk bus still lifts ENV floors.
    void applyMatrixModRoleGuards(PatchModel& inOut, const MutationRecipe& recipe);

    // A4 #2 — couple cutoff↔resonance and envelope Attack→Decay/Release after int jitter.
    void applyFamilyGestures(PatchModel& inOut, const PatchModel& seed, const MutationRecipe& recipe);

} // namespace Core
