#include "MutationPlayabilityOracle.h"

#include <cmath>

#include "Core/Services/PatchMutator/MutationMatrixModPolicy.h"
#include "Core/Services/PatchMutator/MutationPolicy.h"
#include "Shared/Definitions/Matrix1000Limits.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"

namespace MutationPlayabilityOracle
{
    namespace
    {
        namespace Cal = Core::MutationCalibration;
        namespace Dco1 = PluginIDs::PatchEditSection::Dco1Module::ParameterWidgets;
        namespace Dco2 = PluginIDs::PatchEditSection::Dco2Module::ParameterWidgets;
        namespace VcfVca = PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets;
        namespace Env2 = PluginIDs::PatchEditSection::Envelope2Module::ParameterWidgets;
        namespace WaveSelectNames = PluginDisplayNames::ChoiceLists::WaveSelect;
        namespace DestinationNames = PluginDisplayNames::ChoiceLists::ModulationBus::Destination;
        namespace SourceNames = PluginDisplayNames::ChoiceLists::ModulationBus::Source;

        int readInt(const Core::PatchModel& patch, const char* parameterId)
        {
            const auto* descriptor = Core::findMutationIntDescriptor(parameterId);
            return descriptor != nullptr ? patch.getValue(*descriptor) : 0;
        }

        bool isWaveSelectOff(const Core::PatchModel& patch, const char* parameterId)
        {
            const auto* descriptor = Core::findMutationChoiceDescriptor(parameterId);
            if (descriptor == nullptr)
                return true;

            return patch.getChoiceIndex(*descriptor)
                   == descriptor->choices.indexOf(juce::String(WaveSelectNames::kOff));
        }

        Result fail(const char* ruleId)
        {
            return { false, ruleId };
        }

        Result checkAmplitudePath(const Core::PatchModel& patch)
        {
            if (readInt(patch, VcfVca::kVca1Volume) < Cal::kVca1VolumeFloor)
                return fail("A.vca1Volume");

            if (! Core::matrixModDrivesVca2Volume(patch)
                && readInt(patch, VcfVca::kVca2ModByEnv2) < Cal::kVca2ModByEnv2Floor)
            {
                return fail("A.vca2Env2");
            }

            if (readInt(patch, Env2::kAmplitude) < Cal::kEnv2AmplitudeFloor)
                return fail("A.env2Amplitude");

            if (readInt(patch, VcfVca::kVca1ModByVelocity) < Cal::kVelocityNegativeFloor)
                return fail("A.vca1Velocity");

            if (readInt(patch, Env2::kAmplitudeModByVelocity) < Cal::kVelocityNegativeFloor)
                return fail("A.env2AmpVelocity");

            return {};
        }

        Result checkFilterPath(const Core::PatchModel& patch)
        {
            const int cutoff = readInt(patch, VcfVca::kFrequency);
            const int resonance = readInt(patch, VcfVca::kResonance);

            if (cutoff < Cal::kVcfFrequencyLowThreshold
                && resonance < Cal::kVcfResonanceRescueThreshold)
            {
                return fail("B.filterCouple");
            }

            if (readInt(patch, VcfVca::kFrequencyModByEnv1) < Cal::kNegativeDepthFloorWhenBaseLow)
                return fail("B.freqEnv1");

            if (readInt(patch, VcfVca::kFrequencyModByPressure) < Cal::kNegativeDepthFloorWhenBaseLow)
                return fail("B.freqPressure");

            if (Core::matrixModDrivesVcfFrequency(patch))
            {
                if (resonance < Cal::kVcfResonanceNearZeroThreshold)
                    return fail("B.mmResonance");

                if (cutoff < Cal::kVcfFrequencyComfortableOpen)
                    return fail("B.mmCutoff");
            }

            return {};
        }

        Result checkSourcePath(const Core::PatchModel& patch)
        {
            const bool dco1Off = isWaveSelectOff(patch, Dco1::kWaveSelect);
            const bool dco2Off = isWaveSelectOff(patch, Dco2::kWaveSelect);

            if (dco1Off && dco2Off)
                return fail("C.dualWaveOff");

            const auto* mixDescriptor = Core::findMutationIntDescriptor(VcfVca::kBalance);
            if (mixDescriptor == nullptr)
                return {};

            const int mix = patch.getValue(*mixDescriptor);
            const bool favoursDco1 = mix > mixDescriptor->maxValue - Cal::kMixExtremeMargin;
            const bool favoursDco2 = mix < mixDescriptor->minValue + Cal::kMixExtremeMargin;

            if ((favoursDco1 && dco1Off) || (favoursDco2 && dco2Off))
                return fail("C.mixVsOffDco");

            return {};
        }

        bool isTremoloSource(int sourceIndex)
        {
            const auto& catalog = Core::MatrixModChoiceCatalog::shared();
            return sourceIndex == catalog.sourceIndexFor(SourceNames::kLfo1)
                   || sourceIndex == catalog.sourceIndexFor(SourceNames::kLfo2)
                   || sourceIndex == catalog.sourceIndexFor(SourceNames::kVibrato);
        }

        Result checkMatrixModPath(const Core::PatchModel& patch)
        {
            const auto& catalog = Core::MatrixModChoiceCatalog::shared();

            for (int busIndex = 0; busIndex < ::Matrix1000Limits::kModulationBusCount; ++busIndex)
            {
                const auto bus = Core::readMatrixModBus(patch, busIndex);
                if (! bus.isLive)
                    continue;

                if (catalog.isRiskDestination(bus.destinationIndex)
                    && (bus.amount < Cal::kMatrixModRiskAmountFloor
                        || bus.amount > Cal::kMatrixModRiskAmountCeiling))
                {
                    return fail("E.riskAmount");
                }

                if (catalog.isAmplitudeDestination(bus.destinationIndex)
                    && isTremoloSource(bus.sourceIndex)
                    && std::abs(bus.amount) > Cal::kMatrixModTremoloAmountCeiling)
                {
                    return fail("E.tremoloAmount");
                }
            }

            return {};
        }
    } // namespace

    Result check(const Core::PatchModel& patch)
    {
        if (const auto amplitude = checkAmplitudePath(patch); ! amplitude.passed)
            return amplitude;

        if (const auto filter = checkFilterPath(patch); ! filter.passed)
            return filter;

        if (const auto source = checkSourcePath(patch); ! source.passed)
            return source;

        return checkMatrixModPath(patch);
    }
} // namespace MutationPlayabilityOracle
