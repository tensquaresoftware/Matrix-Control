#include "Core/Services/PatchMutator/MutationMatrixModPolicy.h"

#include <cstdlib>

#include "Core/Services/PatchMutator/MutationMatrixModPolicyInternal.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"

namespace Core
{

namespace
{
    using MatrixModPolicyDetail::busAmountDescriptor;
    using MatrixModPolicyDetail::busDestinationDescriptor;
    using MatrixModPolicyDetail::busSourceDescriptor;
    using MatrixModPolicyDetail::kBusCount;

    namespace Destination = PluginDisplayNames::ChoiceLists::ModulationBus::Destination;
    namespace Source = PluginDisplayNames::ChoiceLists::ModulationBus::Source;

    // Sources that open amplitude and hold it — not LFO/vibrato tremolo.
    constexpr const char* kStableAmplitudeOpenerSourceNames[] = {
        Source::kEnvelope1, Source::kEnvelope2, Source::kEnvelope3,
        Source::kRamp1, Source::kRamp2, Source::kTrack,
        Source::kVelocity, Source::kReleaseVelocity, Source::kPressure,
        Source::kPedal1, Source::kPedal2, Source::kKeyboardGate
    };

    bool isStableAmplitudeOpenerSource(int sourceIndex)
    {
        const auto& catalog = MatrixModChoiceCatalog::shared();

        for (const auto* name : kStableAmplitudeOpenerSourceNames)
        {
            if (sourceIndex == catalog.sourceIndexFor(name))
                return true;
        }

        return false;
    }

    bool busIsStableAmplitudeOpener(const MatrixModBusView& bus)
    {
        const auto& catalog = MatrixModChoiceCatalog::shared();
        return bus.isLive
               && catalog.isAmplitudeDestination(bus.destinationIndex)
               && bus.amount >= MutationCalibration::kMatrixModAmplitudeOpenerMinAmount
               && isStableAmplitudeOpenerSource(bus.sourceIndex);
    }

    int findStableAmplitudeOpenerBus(const PatchModel& patch)
    {
        for (int busIndex = 0; busIndex < kBusCount; ++busIndex)
        {
            if (busIsStableAmplitudeOpener(readMatrixModBus(patch, busIndex)))
                return busIndex;
        }

        return -1;
    }

    int vcfFrequencyDestinationIndex()
    {
        return busDestinationDescriptor(0).choices.indexOf(juce::String(Destination::kVcfFrequency));
    }

    bool busIsFilterFrequencyOpener(const MatrixModBusView& bus, int minAmount)
    {
        const int vcfFrequencyIndex = vcfFrequencyDestinationIndex();
        return vcfFrequencyIndex >= 0
               && bus.isLive
               && bus.destinationIndex == vcfFrequencyIndex
               && bus.amount >= minAmount;
    }

    int findFilterFrequencyOpenerBus(const PatchModel& patch, int minAmount)
    {
        for (int busIndex = 0; busIndex < kBusCount; ++busIndex)
        {
            if (busIsFilterFrequencyOpener(readMatrixModBus(patch, busIndex), minAmount))
                return busIndex;
        }

        return -1;
    }

    int readVcfFrequency(const PatchModel& patch)
    {
        namespace VcfVca = PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets;
        const auto* descriptor = findMutationIntDescriptor(VcfVca::kFrequency);
        return descriptor != nullptr ? patch.getValue(*descriptor) : 0;
    }
} // namespace

void ensureMatrixModAmplitudeOpeners(PatchModel& inOut,
                                     const PatchModel& seed,
                                     const MutationRecipe& recipe)
{
    if (! recipe.enableMatrixMod)
        return;

    const int seedOpenerBus = findStableAmplitudeOpenerBus(seed);
    if (seedOpenerBus < 0)
        return;

    if (findStableAmplitudeOpenerBus(inOut) >= 0)
        return;

    // Restore the seed's first stable amplitude opener onto the same bus slot.
    const auto seedBus = readMatrixModBus(seed, seedOpenerBus);
    inOut.setChoiceIndex(busSourceDescriptor(seedOpenerBus), seedBus.sourceIndex);
    inOut.setChoiceIndex(busDestinationDescriptor(seedOpenerBus), seedBus.destinationIndex);
    inOut.setValue(busAmountDescriptor(seedOpenerBus), seedBus.amount);
    capMatrixModRiskAmounts(inOut);
}

void ensureMatrixModFilterOpeners(PatchModel& inOut,
                                  const PatchModel& seed,
                                  const MutationRecipe& recipe)
{
    if (! recipe.enableMatrixMod)
        return;

    // Only Banjo-style seeds: static cutoff closed, opened by Matrix Modulation.
    if (readVcfFrequency(seed) >= MutationCalibration::kVcfFrequencyLowThreshold)
        return;

    const int seedOpenerBus = findFilterFrequencyOpenerBus(
        seed, MutationCalibration::kMatrixModFilterOpenerMinAmount);
    if (seedOpenerBus < 0)
        return;

    const auto seedBus = readMatrixModBus(seed, seedOpenerBus);
    const int requiredAmount = juce::jmin(seedBus.amount,
                                          MutationCalibration::kMatrixModRiskAmountCeiling);

    if (findFilterFrequencyOpenerBus(inOut, requiredAmount) >= 0)
        return;

    inOut.setChoiceIndex(busSourceDescriptor(seedOpenerBus), seedBus.sourceIndex);
    inOut.setChoiceIndex(busDestinationDescriptor(seedOpenerBus), seedBus.destinationIndex);
    inOut.setValue(busAmountDescriptor(seedOpenerBus),
                   juce::jmax(seedBus.amount, MutationCalibration::kMatrixModFilterOpenerMinAmount));
    capMatrixModRiskAmounts(inOut);
}

bool matrixModDrivesVca2Volume(const PatchModel& seed)
{
    const int vca2Index = busDestinationDescriptor(0).choices.indexOf(juce::String(Destination::kVca2Volume));
    if (vca2Index < 0)
        return false;

    for (int busIndex = 0; busIndex < kBusCount; ++busIndex)
    {
        const auto bus = readMatrixModBus(seed, busIndex);
        if (bus.isLive && bus.amount != 0 && bus.destinationIndex == vca2Index)
            return true;
    }

    return false;
}

bool matrixModDrivesVcfFrequency(const PatchModel& patch)
{
    const int vcfFrequencyIndex = busDestinationDescriptor(0).choices.indexOf(
        juce::String(Destination::kVcfFrequency));
    if (vcfFrequencyIndex < 0)
        return false;

    for (int busIndex = 0; busIndex < kBusCount; ++busIndex)
    {
        const auto bus = readMatrixModBus(patch, busIndex);
        if (bus.isLive
            && bus.destinationIndex == vcfFrequencyIndex
            && std::abs(bus.amount) >= MutationCalibration::kMatrixModAmplitudeOpenerMinAmount)
        {
            return true;
        }
    }

    return false;
}

bool matrixModSourceDrivesRiskDestination(const PatchModel& seed, const char* sourceDisplayName)
{
    const auto& catalog = MatrixModChoiceCatalog::shared();
    const int sourceIndex = catalog.sourceIndexFor(sourceDisplayName);
    if (sourceIndex < 0)
        return false;

    for (int busIndex = 0; busIndex < kBusCount; ++busIndex)
    {
        const auto bus = readMatrixModBus(seed, busIndex);
        if (bus.isLive && bus.amount != 0
            && bus.sourceIndex == sourceIndex
            && catalog.isRiskDestination(bus.destinationIndex))
        {
            return true;
        }
    }

    return false;
}

} // namespace Core
