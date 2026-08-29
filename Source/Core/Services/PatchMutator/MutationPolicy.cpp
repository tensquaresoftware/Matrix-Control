#include "Core/Services/PatchMutator/MutationPolicy.h"

#include <cmath>

#include "Core/Models/ApvtsPatchMapper.h"
#include "Shared/Definitions/PluginIDs.h"

namespace Core
{

namespace
{
    namespace Dco1 = PluginIDs::PatchEditSection::Dco1Module::ParameterWidgets;
    namespace Dco2 = PluginIDs::PatchEditSection::Dco2Module::ParameterWidgets;
    namespace VcfVca = PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets;
    namespace Env2 = PluginIDs::PatchEditSection::Envelope2Module::ParameterWidgets;

    struct ModeProfile
    {
        int amountPercent = 0;
        int randomPercent = 0;
        float hitProbability = 1.0f;
        int choiceStepLimit = MutationCalibration::kUnlimitedChoiceStep;
    };

    constexpr ModeProfile kModeProfiles[kMutationModeCount] = {
        { MutationCalibration::kKindredAmountPercent,
          MutationCalibration::kKindredRandomPercent,
          MutationCalibration::kKindredHitProbability,
          MutationCalibration::kKindredChoiceStep },
        { MutationCalibration::kDriftAmountPercent,
          MutationCalibration::kDriftRandomPercent,
          MutationCalibration::kDriftHitProbability,
          MutationCalibration::kDriftChoiceStep },
        { MutationCalibration::kWarpAmountPercent,
          MutationCalibration::kWarpRandomPercent,
          MutationCalibration::kEveryParameterHitProbability,
          MutationCalibration::kUnlimitedChoiceStep },
        { MutationCalibration::kWildAmountPercent,
          MutationCalibration::kWildRandomPercent,
          MutationCalibration::kEveryParameterHitProbability,
          MutationCalibration::kUnlimitedChoiceStep }
    };

    const ModeProfile& profileFor(MutationMode mode) noexcept
    {
        return kModeProfiles[static_cast<size_t>(mode)];
    }

    struct IntPolicyRow
    {
        const char* parameterId = nullptr;
        MutationParameterPolicy policy {};
    };

    // Only the parameters that need more than the default neighborhood appear here.
    // Everything else falls back to MutationPolicyKind::kFree.
    const IntPolicyRow kIntPolicyRows[] = {
        { VcfVca::kVca1Volume,
          { MutationPolicyKind::kLock,
            MutationMode::kWarp,
            MutationCalibration::kSacredNeighborhoodScale,
            MutationCalibration::kVca1VolumeFloor,
            kNoMutationCeiling } },
        { VcfVca::kVca2ModByEnv2,
          { MutationPolicyKind::kLock,
            MutationMode::kWarp,
            MutationCalibration::kSacredNeighborhoodScale,
            MutationCalibration::kVca2ModByEnv2Floor,
            kNoMutationCeiling } },
        { Env2::kAmplitude,
          { MutationPolicyKind::kBounded,
            MutationMode::kWild,
            1.0f,
            MutationCalibration::kEnv2AmplitudeFloor,
            kNoMutationCeiling } },
        { VcfVca::kVca1ModByVelocity,
          { MutationPolicyKind::kBounded,
            MutationMode::kWild,
            MutationCalibration::kVelocityNeighborhoodScale,
            kNoMutationFloor,
            kNoMutationCeiling } },
        { Env2::kAmplitudeModByVelocity,
          { MutationPolicyKind::kBounded,
            MutationMode::kWild,
            MutationCalibration::kVelocityNeighborhoodScale,
            kNoMutationFloor,
            kNoMutationCeiling } },
        { Dco1::kFrequency, { MutationPolicyKind::kPitchGated } },
        { Dco2::kFrequency, { MutationPolicyKind::kPitchGated } },
    };

    template <typename DescriptorT>
    const DescriptorT* findDescriptorIn(const std::vector<DescriptorT>& descriptors,
                                       const juce::String& parameterId)
    {
        for (const auto& descriptor : descriptors)
        {
            if (parameterId == descriptor.parameterId)
                return &descriptor;
        }

        return nullptr;
    }
} // namespace

MutationMode mutationModeFromIndex(int index) noexcept
{
    return static_cast<MutationMode>(juce::jlimit(0, kMutationModeCount - 1, index));
}

MutationPitchMode mutationPitchModeFromIndex(int index) noexcept
{
    return static_cast<MutationPitchMode>(juce::jlimit(0, kMutationPitchModeCount - 1, index));
}

int modeAmountPercent(MutationMode mode) noexcept
{
    return profileFor(mode).amountPercent;
}

int modeRandomPercent(MutationMode mode) noexcept
{
    return profileFor(mode).randomPercent;
}

float modeHitProbability(MutationMode mode) noexcept
{
    return profileFor(mode).hitProbability;
}

int modeChoiceStepLimit(MutationMode mode) noexcept
{
    return profileFor(mode).choiceStepLimit;
}

MutationParameterPolicy policyForIntParameter(const juce::String& parameterId)
{
    for (const auto& row : kIntPolicyRows)
    {
        if (parameterId == row.parameterId)
            return row.policy;
    }

    return {};
}

bool isLockedForMode(const MutationParameterPolicy& policy, MutationMode mode) noexcept
{
    return policy.kind == MutationPolicyKind::kLock && mode <= policy.lockThroughMode;
}

bool matrixModRelayLiftsLock(const juce::String& parameterId, bool seedHasVca2Relay) noexcept
{
    return seedHasVca2Relay && parameterId == VcfVca::kVca2ModByEnv2;
}

int scaleNeighborhood(const MutationParameterPolicy& policy, int spread) noexcept
{
    if (juce::approximatelyEqual(policy.neighborhoodScale, 1.0f) || spread <= 0)
        return spread;

    const auto scaled = static_cast<double>(spread) * static_cast<double>(policy.neighborhoodScale);

    // Never collapse a live neighborhood to zero: a sacred parameter still breathes.
    return juce::jmax(1, static_cast<int>(std::floor(scaled + 0.5)));
}

int applyPolicyBounds(const MutationParameterPolicy& policy, int value, int minValue, int maxValue) noexcept
{
    int bounded = value;

    if (policy.absoluteFloor != kNoMutationFloor)
        bounded = juce::jmax(bounded, policy.absoluteFloor);

    if (policy.absoluteCeiling != kNoMutationCeiling)
        bounded = juce::jmin(bounded, policy.absoluteCeiling);

    return juce::jlimit(minValue, maxValue, bounded);
}

const PluginDescriptors::IntParameterDescriptor* findMutationIntDescriptor(const juce::String& parameterId)
{
    static const auto descriptors = ApvtsPatchMapper::buildIntDescriptors();
    return findDescriptorIn(descriptors, parameterId);
}

const PluginDescriptors::ChoiceParameterDescriptor* findMutationChoiceDescriptor(const juce::String& parameterId)
{
    static const auto descriptors = ApvtsPatchMapper::buildChoiceDescriptors();
    return findDescriptorIn(descriptors, parameterId);
}

} // namespace Core
