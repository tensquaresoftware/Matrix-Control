#include "Core/Services/PatchMutator/MutationPostApply.h"

#include <cmath>

#include "Core/Services/PatchMutator/MutationMatrixModPolicy.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"

namespace Core
{

namespace
{
    namespace Dco1 = PluginIDs::PatchEditSection::Dco1Module::ParameterWidgets;
    namespace Dco2 = PluginIDs::PatchEditSection::Dco2Module::ParameterWidgets;
    namespace VcfVca = PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets;
    namespace FmTrack = PluginIDs::PatchEditSection::FmTrackModule::ParameterWidgets;
    namespace Env1 = PluginIDs::PatchEditSection::Envelope1Module::ParameterWidgets;
    namespace Env2 = PluginIDs::PatchEditSection::Envelope2Module::ParameterWidgets;
    namespace Env3 = PluginIDs::PatchEditSection::Envelope3Module::ParameterWidgets;
    namespace WaveSelectNames = PluginDisplayNames::ChoiceLists::WaveSelect;
    namespace SourceNames = PluginDisplayNames::ChoiceLists::ModulationBus::Source;
    namespace TriggerModeNames = PluginDisplayNames::ChoiceLists::TriggerMode;
    namespace EnvelopeModeNames = PluginDisplayNames::ChoiceLists::EnvelopeMode;

    int readInt(const PatchModel& patch, const char* parameterId)
    {
        const auto* descriptor = findMutationIntDescriptor(parameterId);
        return descriptor != nullptr ? patch.getValue(*descriptor) : 0;
    }

    void raiseIntToFloor(PatchModel& inOut, const char* parameterId, int floorValue)
    {
        const auto* descriptor = findMutationIntDescriptor(parameterId);
        if (descriptor == nullptr)
            return;

        const int current = inOut.getValue(*descriptor);
        const int raised = juce::jlimit(descriptor->minValue, descriptor->maxValue,
                                        juce::jmax(current, floorValue));
        if (raised != current)
            inOut.setValue(*descriptor, raised);
    }

    void setInt(PatchModel& inOut, const char* parameterId, int value)
    {
        const auto* descriptor = findMutationIntDescriptor(parameterId);
        if (descriptor == nullptr)
            return;

        const int clamped = juce::jlimit(descriptor->minValue, descriptor->maxValue, value);
        if (clamped != inOut.getValue(*descriptor))
            inOut.setValue(*descriptor, clamped);
    }

    int waveSelectChoiceIndex(const PatchModel& patch, const char* parameterId)
    {
        const auto* descriptor = findMutationChoiceDescriptor(parameterId);
        return descriptor != nullptr ? patch.getChoiceIndex(*descriptor) : 0;
    }

    void setWaveSelectChoiceIndex(PatchModel& inOut, const char* parameterId, int index)
    {
        const auto* descriptor = findMutationChoiceDescriptor(parameterId);
        if (descriptor != nullptr)
            inOut.setChoiceIndex(*descriptor, index);
    }

    int waveSelectIndexOf(const char* parameterId, const char* choiceName)
    {
        const auto* descriptor = findMutationChoiceDescriptor(parameterId);
        return descriptor != nullptr ? descriptor->choices.indexOf(juce::String(choiceName)) : -1;
    }

    bool isWaveSelectActive(const PatchModel& patch, const char* parameterId)
    {
        const int offIndex = waveSelectIndexOf(parameterId, WaveSelectNames::kOff);
        return waveSelectChoiceIndex(patch, parameterId) != offIndex;
    }

    int countActiveDcos(const PatchModel& patch)
    {
        return (isWaveSelectActive(patch, Dco1::kWaveSelect) ? 1 : 0)
               + (isWaveSelectActive(patch, Dco2::kWaveSelect) ? 1 : 0);
    }

    int choiceIndexOf(const char* parameterId, const char* choiceName)
    {
        const auto* descriptor = findMutationChoiceDescriptor(parameterId);
        return descriptor != nullptr ? descriptor->choices.indexOf(juce::String(choiceName)) : -1;
    }

    void setChoiceByName(PatchModel& inOut, const char* parameterId, const char* choiceName)
    {
        const int index = choiceIndexOf(parameterId, choiceName);
        if (index >= 0)
            setWaveSelectChoiceIndex(inOut, parameterId, index);
    }

    juce::String readChoiceName(const PatchModel& patch, const char* parameterId)
    {
        const auto* descriptor = findMutationChoiceDescriptor(parameterId);
        if (descriptor == nullptr)
            return {};

        const int index = patch.getChoiceIndex(*descriptor);
        if (! juce::isPositiveAndBelow(index, descriptor->choices.size()))
            return {};

        return descriptor->choices[index];
    }

    bool isExternalEnvelopeTrigger(const juce::String& triggerName)
    {
        return triggerName == TriggerModeNames::kXtrig
               || triggerName == TriggerModeNames::kXmtrig
               || triggerName == TriggerModeNames::kXreset
               || triggerName == TriggerModeNames::kXmrst;
    }

    bool isNonKeyboardVolumeEnvelopeMode(const juce::String& modeName)
    {
        return modeName == EnvelopeModeNames::kDadr
               || modeName == EnvelopeModeNames::kFree
               || modeName == EnvelopeModeNames::kBoth;
    }

    // ENV 2 opens VCA 2: it must answer the keyboard, not an external pedal (Cas 7).
    void guardEnv2KeyboardVolumePath(PatchModel& inOut, const MutationRecipe& recipe)
    {
        if (! recipe.enableEnvelope2)
            return;

        if (readInt(inOut, VcfVca::kVca2ModByEnv2) <= 0)
            return;

        if (isExternalEnvelopeTrigger(readChoiceName(inOut, Env2::kTriggerMode)))
            setChoiceByName(inOut, Env2::kTriggerMode, TriggerModeNames::kStrig);

        if (isNonKeyboardVolumeEnvelopeMode(readChoiceName(inOut, Env2::kEnvelopeMode)))
            setChoiceByName(inOut, Env2::kEnvelopeMode, EnvelopeModeNames::kNormal);

        const int delay = readInt(inOut, Env2::kDelay);
        if (delay > MutationCalibration::kEnv2DelayCeilingWhenVolumePath)
            setInt(inOut, Env2::kDelay, MutationCalibration::kEnv2DelayCeilingWhenVolumePath);
    }

    void constrainNegativeDepth(PatchModel& inOut, const char* parameterId, int floorValue)
    {
        const int current = readInt(inOut, parameterId);
        if (current >= floorValue)
            return;

        setInt(inOut, parameterId, floorValue);
    }

    void constrainNegativeDepth(PatchModel& inOut, const char* parameterId)
    {
        constrainNegativeDepth(inOut, parameterId, MutationCalibration::kNegativeDepthFloorWhenBaseLow);
    }

    // A6.1 — the hardwired amplitude path must never collapse.
    void guardAmplitudePath(PatchModel& inOut,
                            const MutationRecipe& recipe,
                            const MutationMatrixModRoles& roles)
    {
        if (recipe.enableVcfVca)
        {
            raiseIntToFloor(inOut, VcfVca::kVca1Volume, MutationCalibration::kVca1VolumeFloor);

            // A Matrix Modulation bus still opening VCA 2 after mutation is a valid relay,
            // so the hardwired ENV 2 depth may stay where the jitter left it. If Warp or
            // Wild rerouted that bus away, the floor comes back.
            if (! roles.drivesVca2Volume)
                raiseIntToFloor(inOut, VcfVca::kVca2ModByEnv2, MutationCalibration::kVca2ModByEnv2Floor);

            // Velocity soft-kill even when Volume is healthy (Cas 5: -18 with Volume ~62).
            constrainNegativeDepth(inOut, VcfVca::kVca1ModByVelocity,
                                   MutationCalibration::kVelocityNegativeFloor);
        }

        if (! recipe.enableEnvelope2)
            return;

        raiseIntToFloor(inOut, Env2::kAmplitude, MutationCalibration::kEnv2AmplitudeFloor);

        constrainNegativeDepth(inOut, Env2::kAmplitudeModByVelocity,
                               MutationCalibration::kVelocityNegativeFloor);

        // ENV 2 is the hardwired volume envelope: sustain 0 + short release dies under the finger.
        if (readInt(inOut, VcfVca::kVca2ModByEnv2) > 0
            && readInt(inOut, Env2::kSustain) <= 0
            && readInt(inOut, Env2::kRelease) < MutationCalibration::kEnv2ReleaseShortThreshold)
        {
            raiseIntToFloor(inOut, Env2::kSustain,
                            MutationCalibration::kEnv2SustainFloorWhenReleaseShort);
        }

        guardEnv2KeyboardVolumePath(inOut, recipe);
    }

    // A6.5 — a filter or FM envelope that also drives a risk destination becomes critical.
    void guardEscalatedEnvelopeRoles(PatchModel& inOut,
                                     const MutationRecipe& recipe,
                                     const MutationMatrixModRoles& roles)
    {
        if (recipe.enableEnvelope1 && roles.envelope1DrivesRiskDestination)
            raiseIntToFloor(inOut, Env1::kAmplitude, MutationCalibration::kEscalatedEnvelopeAmplitudeFloor);

        if (recipe.enableEnvelope3 && roles.envelope3DrivesRiskDestination)
            raiseIntToFloor(inOut, Env3::kAmplitude, MutationCalibration::kEscalatedEnvelopeAmplitudeFloor);
    }

    // A6.2 — a closed filter with no resonance to rescue it reads as silence.
    // FREQ < ENV 1 / PRESSURE can also smother while the static cutoff still looks open.
    void guardFilterSmother(PatchModel& inOut, const MutationRecipe& recipe)
    {
        if (! recipe.enableVcfVca)
            return;

        const int cutoff = readInt(inOut, VcfVca::kFrequency);
        const int resonance = readInt(inOut, VcfVca::kResonance);

        if (cutoff < MutationCalibration::kVcfFrequencyLowThreshold
            && resonance < MutationCalibration::kVcfResonanceRescueThreshold)
        {
            setInt(inOut, VcfVca::kFrequency, MutationCalibration::kVcfFrequencyRescueValue);
        }

        // Always — not only when cutoff is already low (Cas 4 smoke: -63 with cutoff ~119).
        constrainNegativeDepth(inOut, VcfVca::kFrequencyModByEnv1);
        constrainNegativeDepth(inOut, VcfVca::kFrequencyModByPressure);

        // Near-zero resonance + Matrix Mod wiggling cutoff → raise a little resonance (Cas 3).
        if (readInt(inOut, VcfVca::kResonance) < MutationCalibration::kVcfResonanceNearZeroThreshold
            && matrixModDrivesVcfFrequency(inOut))
        {
            raiseIntToFloor(inOut, VcfVca::kResonance,
                            MutationCalibration::kVcfResonanceWhenFrequencyModulatedFloor);
        }

        // Banjo-style: MM opens the filter — keep static cutoff out of the mud (Cas 6).
        if (readInt(inOut, VcfVca::kFrequency) < MutationCalibration::kVcfFrequencyComfortableOpen
            && matrixModDrivesVcfFrequency(inOut))
        {
            raiseIntToFloor(inOut, VcfVca::kFrequency,
                            MutationCalibration::kVcfFrequencyComfortableOpen);
        }
    }

    // FREQ < ENV 1 is how a closed filter still plucks — ENV 1 amp must not collapse (Cas 6).
    void guardFilterOpenEnvelope(PatchModel& inOut, const MutationRecipe& recipe)
    {
        if (! recipe.enableVcfVca || ! recipe.enableEnvelope1)
            return;

        if (readInt(inOut, VcfVca::kFrequencyModByEnv1)
            < MutationCalibration::kFilterEnvOpenModThreshold)
        {
            return;
        }

        raiseIntToFloor(inOut, Env1::kAmplitude, MutationCalibration::kEscalatedEnvelopeAmplitudeFloor);
    }

    // A noise seed stays a noise seed: no other DCO 2 shape reproduces it.
    void restoreDco2NoiseFromSeed(PatchModel& inOut, const MutationRecipe& recipe, const MutationSeedFacts& facts)
    {
        if (! recipe.enableDco2 || ! facts.dco2WasNoise)
            return;

        const int noiseIndex = waveSelectIndexOf(Dco2::kWaveSelect, WaveSelectNames::kNoise);
        if (noiseIndex >= 0 && waveSelectChoiceIndex(inOut, Dco2::kWaveSelect) != noiseIndex)
            setWaveSelectChoiceIndex(inOut, Dco2::kWaveSelect, noiseIndex);
    }

    // Too few voices: put a writable oscillator back to the shape the seed used.
    void restoreMissingOscillator(PatchModel& inOut,
                                  const PatchModel& seed,
                                  const char* waveSelectId,
                                  bool moduleWritable)
    {
        if (! moduleWritable || isWaveSelectActive(inOut, waveSelectId))
            return;

        setWaveSelectChoiceIndex(inOut, waveSelectId, waveSelectChoiceIndex(seed, waveSelectId));
    }

    // Too many voices: the roles may swap, but a one-oscillator seed stays a
    // one-oscillator patch, so silence the oscillator the seed had switched off.
    void silenceOscillatorTheSeedHadOff(PatchModel& inOut,
                                        const PatchModel& seed,
                                        const char* waveSelectId,
                                        bool moduleWritable)
    {
        if (! moduleWritable || isWaveSelectActive(seed, waveSelectId))
            return;

        const int offIndex = waveSelectIndexOf(waveSelectId, WaveSelectNames::kOff);
        if (offIndex >= 0)
            setWaveSelectChoiceIndex(inOut, waveSelectId, offIndex);
    }

    // A6.3 — dual WaveSelect OFF is forbidden and the seed's active-DCO count is kept.
    void guardWaveSelectCardinality(PatchModel& inOut,
                                    const PatchModel& seed,
                                    const MutationRecipe& recipe,
                                    const MutationSeedFacts& facts)
    {
        // A dual-OFF seed cannot stay dual-OFF when a DCO toggle is writable: restore a
        // playable WAVE shape rather than copying the silent seed choice.
        if (facts.activeDcoCount == 0)
        {
            const char* targetId = nullptr;
            if (recipe.enableDco1)
                targetId = Dco1::kWaveSelect;
            else if (recipe.enableDco2)
                targetId = Dco2::kWaveSelect;

            if (targetId == nullptr)
                return;

            const int waveIndex = waveSelectIndexOf(targetId, WaveSelectNames::kWave);
            if (waveIndex >= 0)
                setWaveSelectChoiceIndex(inOut, targetId, waveIndex);
            return;
        }

        restoreDco2NoiseFromSeed(inOut, recipe, facts);

        if (countActiveDcos(inOut) < facts.activeDcoCount)
            restoreMissingOscillator(inOut, seed, Dco1::kWaveSelect, recipe.enableDco1);

        if (countActiveDcos(inOut) < facts.activeDcoCount)
            restoreMissingOscillator(inOut, seed, Dco2::kWaveSelect, recipe.enableDco2);

        if (countActiveDcos(inOut) <= facts.activeDcoCount)
            return;

        silenceOscillatorTheSeedHadOff(inOut, seed, Dco2::kWaveSelect, recipe.enableDco2);

        if (countActiveDcos(inOut) > facts.activeDcoCount)
            silenceOscillatorTheSeedHadOff(inOut, seed, Dco1::kWaveSelect, recipe.enableDco1);
    }

    // A6.3 — Mix hard against an oscillator that is switched off is a silent patch.
    // Mix polarity is 0 = DCO 2 only, 63 = DCO 1 only.
    void guardMixAgainstSilentOscillator(PatchModel& inOut, const MutationRecipe& recipe)
    {
        if (! recipe.enableVcfVca)
            return;

        const auto* descriptor = findMutationIntDescriptor(VcfVca::kBalance);
        if (descriptor == nullptr)
            return;

        const int mix = inOut.getValue(*descriptor);
        const bool dco1Silent = ! isWaveSelectActive(inOut, Dco1::kWaveSelect);
        const bool dco2Silent = ! isWaveSelectActive(inOut, Dco2::kWaveSelect);

        const bool mixFavoursDco1 = mix > descriptor->maxValue - MutationCalibration::kMixExtremeMargin;
        const bool mixFavoursDco2 = mix < descriptor->minValue + MutationCalibration::kMixExtremeMargin;

        if ((dco1Silent && mixFavoursDco1) || (dco2Silent && mixFavoursDco2))
            setInt(inOut, VcfVca::kBalance, MutationCalibration::kMixRescueValue);
    }

    // A6.6 — FM is about interest, not silence: keep it audible in the explorer modes.
    void nudgeFmAmountForInterest(PatchModel& inOut,
                                 const MutationRecipe& recipe,
                                 const MutationSeedFacts& facts)
    {
        if (! recipe.enableFmTrack || ! facts.hasFmModulation)
            return;

        if (recipe.mode != MutationMode::kWarp && recipe.mode != MutationMode::kWild)
            return;

        raiseIntToFloor(inOut, FmTrack::kFmAmount, MutationCalibration::kFmAmountAudibleFloor);
    }
} // namespace

MutationMatrixModRoles MutationMatrixModRoles::read(const PatchModel& patch)
{
    MutationMatrixModRoles roles;
    roles.drivesVca2Volume = matrixModDrivesVca2Volume(patch);
    roles.envelope1DrivesRiskDestination =
        matrixModSourceDrivesRiskDestination(patch, SourceNames::kEnvelope1);
    roles.envelope3DrivesRiskDestination =
        matrixModSourceDrivesRiskDestination(patch, SourceNames::kEnvelope3);
    return roles;
}

MutationSeedFacts MutationSeedFacts::read(const PatchModel& seed)
{
    MutationSeedFacts facts;
    facts.hasFmModulation = readInt(seed, FmTrack::kFmModByEnv3) != 0
                            || readInt(seed, FmTrack::kFmModByPressure) != 0;
    facts.dco2WasNoise = waveSelectChoiceIndex(seed, Dco2::kWaveSelect)
                         == waveSelectIndexOf(Dco2::kWaveSelect, WaveSelectNames::kNoise);
    facts.activeDcoCount = countActiveDcos(seed);
    return facts;
}

void applyPostMutationGuards(PatchModel& inOut,
                             const PatchModel& seed,
                             const MutationRecipe& recipe,
                             const MutationSeedFacts& facts)
{
    // Matrix Modulation has already been jittered at this point, so its risk roles are
    // read from the working patch rather than from the seed.
    applyMatrixModRoleGuards(inOut, recipe);
    guardFilterSmother(inOut, recipe);
    guardFilterOpenEnvelope(inOut, recipe);
    guardWaveSelectCardinality(inOut, seed, recipe, facts);
    guardMixAgainstSilentOscillator(inOut, recipe);
    nudgeFmAmountForInterest(inOut, recipe, facts);
}

void applyMatrixModRoleGuards(PatchModel& inOut, const MutationRecipe& recipe)
{
    const auto roles = MutationMatrixModRoles::read(inOut);
    guardAmplitudePath(inOut, recipe, roles);
    guardEscalatedEnvelopeRoles(inOut, recipe, roles);
}

namespace
{
    void couplePartnerTowardLeaderDelta(PatchModel& inOut,
                                         const PatchModel& seed,
                                         const char* leaderId,
                                         const char* partnerId)
    {
        const auto* leader = findMutationIntDescriptor(leaderId);
        const auto* partner = findMutationIntDescriptor(partnerId);
        if (leader == nullptr || partner == nullptr)
            return;

        const int leaderDelta = inOut.getValue(*leader) - seed.getValue(*leader);
        if (leaderDelta == 0)
            return;

        const int coupledDelta = static_cast<int>(std::floor(
            static_cast<double>(leaderDelta)
                * static_cast<double>(MutationCalibration::kFamilyCoupleScale)
            + 0.5));
        const int target = juce::jlimit(partner->minValue, partner->maxValue,
                                        seed.getValue(*partner) + coupledDelta);
        const int current = inOut.getValue(*partner);
        inOut.setValue(*partner, (current + target) / 2);
    }
} // namespace

void applyFamilyGestures(PatchModel& inOut, const PatchModel& seed, const MutationRecipe& recipe)
{
    namespace Vcf = PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets;
    namespace E1 = PluginIDs::PatchEditSection::Envelope1Module::ParameterWidgets;
    namespace E2 = PluginIDs::PatchEditSection::Envelope2Module::ParameterWidgets;
    namespace E3 = PluginIDs::PatchEditSection::Envelope3Module::ParameterWidgets;

    if (recipe.enableVcfVca)
        couplePartnerTowardLeaderDelta(inOut, seed, Vcf::kFrequency, Vcf::kResonance);

    if (recipe.enableEnvelope1)
    {
        couplePartnerTowardLeaderDelta(inOut, seed, E1::kAttack, E1::kDecay);
        couplePartnerTowardLeaderDelta(inOut, seed, E1::kAttack, E1::kRelease);
    }

    if (recipe.enableEnvelope2)
    {
        couplePartnerTowardLeaderDelta(inOut, seed, E2::kAttack, E2::kDecay);
        couplePartnerTowardLeaderDelta(inOut, seed, E2::kAttack, E2::kRelease);
    }

    if (recipe.enableEnvelope3)
    {
        couplePartnerTowardLeaderDelta(inOut, seed, E3::kAttack, E3::kDecay);
        couplePartnerTowardLeaderDelta(inOut, seed, E3::kAttack, E3::kRelease);
    }
}

} // namespace Core
