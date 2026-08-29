#pragma once

#include <juce_core/juce_core.h>

#include "Core/Models/PatchModel.h"
#include "Core/Services/PatchMutator/MutationPitchPolicy.h"
#include "Core/Services/PatchMutator/MutationPostApply.h"
#include "Core/Services/PatchMutator/MutationRandomSource.h"
#include "Core/Services/PatchMutator/MutationRecipe.h"
#include "Shared/Definitions/PluginDescriptors.h"

namespace Core
{

    class MutationAlgorithm
    {
    public:
        bool apply(PatchModel& inOut, const MutationRecipe& recipe, IRandomSource& rng) const;

    private:
        struct MutationPass
        {
            const MutationRecipe& recipe;
            const PatchModel& seed;
            IRandomSource& rng;
            double amount = 0.0;
            double random = 0.0;
            bool matrixModScopeActive = false;
            // Drift may bring exactly one dead Matrix Modulation bus to life.
            int matrixModGrowthBusIndex = -1;
            // A usable seed relay into VCA 2 Level lifts the VCA 2 <- ENV 2 lock.
            bool seedHasMatrixModVca2Relay = false;
            MutationPitchPlan pitchPlan {};
        };

        static int clampPercent(int value) noexcept;
        static int roundHalfUp(double value) noexcept;
        static bool isModuleEnabled(const MutationRecipe& recipe, const juce::String& parentGroupId);
        static bool isIntDescriptorEligible(const PluginDescriptors::IntParameterDescriptor& descriptor,
                                            const MutationRecipe& recipe,
                                            bool matrixModScopeActive);
        static bool isChoiceDescriptorEligible(const PluginDescriptors::ChoiceParameterDescriptor& descriptor,
                                               const MutationRecipe& recipe,
                                               bool matrixModScopeActive);
        static int neighborhoodSpread(const MutationPass& pass, int range);
        static bool passesSparseHitGate(const MutationPass& pass);
        static MutationPitchPlan planPitchForPass(const MutationPass& pass);
        static bool applyPitchGatedValue(PatchModel& inOut,
                                         const PluginDescriptors::IntParameterDescriptor& descriptor,
                                         const MutationPass& pass);
        static void jitterIntDescriptor(PatchModel& inOut,
                                        const PluginDescriptors::IntParameterDescriptor& descriptor,
                                        const MutationPass& pass);
        static void mutateIntDescriptors(PatchModel& inOut, const MutationPass& pass);
        static void mutateChoiceDescriptors(PatchModel& inOut, const MutationPass& pass);
        static void mutateMatrixModAmount(PatchModel& inOut,
                                          const PluginDescriptors::IntParameterDescriptor& descriptor,
                                          const MutationPass& pass);
        static void mutateMatrixModRouting(PatchModel& inOut,
                                           const PluginDescriptors::ChoiceParameterDescriptor& descriptor,
                                           const MutationPass& pass);
        static void restoreProtectedNameBytes(const PatchModel& before, PatchModel& inOut);
        static bool anyByteChangedInRange(const PatchModel& before,
                                          const PatchModel& after,
                                          size_t startIndex,
                                          size_t endExclusive);
    };

} // namespace Core
