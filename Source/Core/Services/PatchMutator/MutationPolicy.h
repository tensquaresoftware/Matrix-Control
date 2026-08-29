#pragma once

#include <limits>

#include <juce_core/juce_core.h>

#include "Shared/Definitions/PluginDescriptors.h"

namespace Core
{

    // Artistic distance from the seed patch. Anti-silence guards stay armed in every mode.
    enum class MutationMode
    {
        kKindred = 0,
        kDrift,
        kWarp,
        kWild
    };

    // Creative permission for DCO pitch and the Matrix Modulation pitch destinations.
    enum class MutationPitchMode
    {
        kPreserve = 0,
        kConsonant,
        kDissonant,
        kFree
    };

    constexpr int kMutationModeCount = 4;
    constexpr int kMutationPitchModeCount = 4;

    MutationMode mutationModeFromIndex(int index) noexcept;
    MutationPitchMode mutationPitchModeFromIndex(int index) noexcept;

    // Only the two interval-based pitch modes need an octave window.
    constexpr bool pitchModeUsesOctaveWindow(MutationPitchMode pitchMode) noexcept
    {
        return pitchMode == MutationPitchMode::kConsonant || pitchMode == MutationPitchMode::kDissonant;
    }

    // Playable calibration lives here so it can be retuned without touching the
    // Amount x Random curve math in MutationAlgorithm.
    namespace MutationCalibration
    {
        constexpr int kKindredAmountPercent = 20;
        constexpr int kKindredRandomPercent = 15;
        constexpr int kDriftAmountPercent = 45;
        constexpr int kDriftRandomPercent = 30;
        constexpr int kWarpAmountPercent = 70;
        constexpr int kWarpRandomPercent = 55;
        constexpr int kWildAmountPercent = 95;
        constexpr int kWildRandomPercent = 85;

        // Kindred touches fewer parameters so each gesture stays readable.
        constexpr float kKindredHitProbability = 0.40f;
        constexpr float kDriftHitProbability = 0.75f;
        constexpr float kEveryParameterHitProbability = 1.0f;

        // How far a discrete choice may travel from its seed index.
        constexpr int kUnlimitedChoiceStep = -1;
        constexpr int kKindredChoiceStep = 1;
        constexpr int kDriftChoiceStep = 2;

        // Pitch window sizes offered by the PITCH combo.
        constexpr int kMinPitchOctaves = 1;
        constexpr int kMaxPitchOctaves = 5;
        constexpr int kDefaultPitchOctaves = 2;

        // Anti-silence floors and thresholds. Placeholders for hardware tuning.
        constexpr int kVca1VolumeFloor = 34;
        constexpr int kVca2ModByEnv2Floor = 40;
        constexpr int kEnv2AmplitudeFloor = 28;
        constexpr int kEscalatedEnvelopeAmplitudeFloor = 24;
        constexpr int kVcfFrequencyLowThreshold = 20;
        constexpr int kVcfResonanceRescueThreshold = 32;
        constexpr int kVcfFrequencyRescueValue = 40;
        // How deep a negative modulation may still go once its base value is already low.
        constexpr int kNegativeDepthFloorWhenBaseLow = -20;
        constexpr int kLowBaseMargin = 8;
        constexpr int kMixExtremeMargin = 6;
        constexpr int kMixRescueValue = 32;
        constexpr int kFmAmountAudibleFloor = 20;
        constexpr int kMatrixModRiskAmountFloor = -20;
        constexpr int kMatrixModRiskAmountCeiling = 40;
        constexpr int kMatrixModMotionAmount = 24;

        // Sacred amplitude parameters keep a very narrow jitter span even in Wild.
        constexpr float kSacredNeighborhoodScale = 0.15f;
        // Velocity depths are a soft kill: narrowed, not locked.
        constexpr float kVelocityNeighborhoodScale = 0.6f;
        // Family gestures: how hard a leader parameter (e.g. cutoff) drags its partner.
        constexpr float kFamilyCoupleScale = 0.5f;
    }

    int modeAmountPercent(MutationMode mode) noexcept;
    int modeRandomPercent(MutationMode mode) noexcept;
    float modeHitProbability(MutationMode mode) noexcept;
    int modeChoiceStepLimit(MutationMode mode) noexcept;

    enum class MutationPolicyKind
    {
        kFree,       // Plain mode-scaled neighborhood around the seed value
        kLock,       // Keep the seed value up to and including lockThroughMode
        kBounded,    // Narrowed neighborhood and / or an absolute floor
        kPitchGated  // Owned by the PITCH control instead of the neighborhood jitter
    };

    constexpr int kNoMutationFloor = std::numeric_limits<int>::min();
    constexpr int kNoMutationCeiling = std::numeric_limits<int>::max();

    struct MutationParameterPolicy
    {
        MutationPolicyKind kind = MutationPolicyKind::kFree;
        MutationMode lockThroughMode = MutationMode::kWild;
        float neighborhoodScale = 1.0f;
        int absoluteFloor = kNoMutationFloor;
        int absoluteCeiling = kNoMutationCeiling;
    };

    MutationParameterPolicy policyForIntParameter(const juce::String& parameterId);

    bool isLockedForMode(const MutationParameterPolicy& policy, MutationMode mode) noexcept;

    // Addendum exception: the hardwired VCA 2 <- ENV 2 depth is locked below Wild only
    // because it is usually the sole way to open VCA 2. A usable Matrix Modulation relay
    // already present in the seed removes that reason, so the lock lifts.
    bool matrixModRelayLiftsLock(const juce::String& parameterId, bool seedHasVca2Relay) noexcept;
    int scaleNeighborhood(const MutationParameterPolicy& policy, int spread) noexcept;
    int applyPolicyBounds(const MutationParameterPolicy& policy, int value, int minValue, int maxValue) noexcept;

    // Descriptor lookups by parameter id. PluginDescriptors stays the single source of truth.
    const PluginDescriptors::IntParameterDescriptor* findMutationIntDescriptor(const juce::String& parameterId);
    const PluginDescriptors::ChoiceParameterDescriptor* findMutationChoiceDescriptor(const juce::String& parameterId);

} // namespace Core
