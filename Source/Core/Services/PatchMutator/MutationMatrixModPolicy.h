#pragma once

#include <juce_core/juce_core.h>

#include "Core/Models/PatchModel.h"
#include "Core/Services/PatchMutator/MutationPolicy.h"
#include "Core/Services/PatchMutator/MutationRandomSource.h"
#include "Core/Services/PatchMutator/MutationRecipe.h"

namespace Core
{

    // Matrix Modulation occupies three bytes per bus: source, amount, destination.
    constexpr int kMatrixModOffsetMin = 104;
    constexpr int kMatrixModOffsetMax = 133;
    constexpr int kMatrixModBytesPerBus = 3;

    enum class MatrixModField
    {
        kSource = 0,
        kAmount,
        kDestination
    };

    bool isMatrixModOffset(int sysExOffset) noexcept;
    int matrixModBusForOffset(int sysExOffset) noexcept;
    MatrixModField matrixModFieldForOffset(int sysExOffset) noexcept;

    // Choice indices resolved from the shared descriptor lists, so PluginDescriptors stays SSOT.
    struct MatrixModChoiceCatalog
    {
        int noneSourceIndex = 0;
        int noneDestinationIndex = 0;
        juce::Array<int> expressiveSources;
        juce::Array<int> timbreDestinations;
        juce::Array<int> riskDestinations;
        juce::Array<int> pitchDestinations;
        juce::Array<int> amplitudeDestinations;

        static const MatrixModChoiceCatalog& shared();

        bool isRiskDestination(int destinationIndex) const;
        bool isPitchDestination(int destinationIndex) const;
        bool isAmplitudeDestination(int destinationIndex) const;
        int sourceIndexFor(const char* displayName) const;
    };

    struct MatrixModBusView
    {
        int sourceIndex = 0;
        int amount = 0;
        int destinationIndex = 0;
        bool isLive = false;
    };

    MatrixModBusView readMatrixModBus(const PatchModel& patch, int busIndex);

    struct MatrixModLadderDecision
    {
        bool allowAmount = false;
        bool allowRouting = false;
        bool preferExpressivePairs = true;
    };

    // Kindred stays honest: amounts on already-live buses only. Drift may grow one bus,
    // Warp and Wild reroute more freely while the anti-silence amount caps stay on.
    MatrixModLadderDecision matrixModLadderFor(MutationMode mode,
                                               const MatrixModBusView& seedBus,
                                               bool isGrowthBus);

    int firstDeadMatrixModBus(const PatchModel& seed);

    int pickMatrixModSource(const MatrixModLadderDecision& decision, int choiceCount, IRandomSource& rng);
    int pickMatrixModDestination(const MatrixModLadderDecision& decision,
                                 MutationPitchMode pitchMode,
                                 int choiceCount,
                                 IRandomSource& rng);

    void capMatrixModRiskAmounts(PatchModel& inOut);

    // After mutation at least one bus must actually move the sound, otherwise Matrix
    // Modulation was enabled for nothing. Pitch Preserve never revives a pitch destination.
    void ensureMatrixModMotion(PatchModel& inOut,
                               MutationMode mode,
                               MutationPitchMode pitchMode,
                               IRandomSource& rng);

    // If the seed opened amplitude through Matrix Modulation (ENV/Track/Velocity → VCA…),
    // Warp/Wild must not leave the patch with only LFO tremolo (or nothing) on those paths.
    void ensureMatrixModAmplitudeOpeners(PatchModel& inOut,
                                         const PatchModel& seed,
                                         const MutationRecipe& recipe);

    // Banjo-style seeds: closed filter opened by TRACK (etc.) → VCF FREQUENCY must keep a
    // usable Amount after mutation.
    void ensureMatrixModFilterOpeners(PatchModel& inOut,
                                      const PatchModel& seed,
                                      const MutationRecipe& recipe);

    bool matrixModDrivesVca2Volume(const PatchModel& seed);
    bool matrixModDrivesVcfFrequency(const PatchModel& patch);
    bool matrixModSourceDrivesRiskDestination(const PatchModel& seed, const char* sourceDisplayName);

} // namespace Core
