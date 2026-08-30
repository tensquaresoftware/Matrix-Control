#include "Core/Services/PatchMutator/MutationMatrixModPolicy.h"

#include <cstdlib>

#include "Core/Services/PatchMutator/MutationPitchPolicy.h"
#include "Shared/Definitions/Matrix1000Limits.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"

namespace Core
{

namespace
{
    namespace Source = PluginDisplayNames::ChoiceLists::ModulationBus::Source;
    namespace Destination = PluginDisplayNames::ChoiceLists::ModulationBus::Destination;

    constexpr int kBusCount = ::Matrix1000Limits::kModulationBusCount;

    // Sources that make Matrix Modulation audible as a gesture rather than a static offset.
    constexpr const char* kExpressiveSourceNames[] = {
        Source::kEnvelope1, Source::kEnvelope2, Source::kEnvelope3,
        Source::kLfo1, Source::kLfo2, Source::kVibrato,
        Source::kRamp1, Source::kRamp2, Source::kTrack, Source::kPortamento
    };

    // Timbre and motion destinations that cannot mute the patch on their own.
    constexpr const char* kTimbreDestinationNames[] = {
        Destination::kDco1PulseWidth, Destination::kDco1WaveShape,
        Destination::kDco2PulseWidth, Destination::kDco2WaveShape,
        Destination::kVcfFmByDco1, Destination::kVcfResonance,
        Destination::kLfo1Speed, Destination::kLfo1Amplitude,
        Destination::kLfo2Speed, Destination::kLfo2Amplitude,
        Destination::kEnv1Attack, Destination::kEnv1Decay, Destination::kEnv1Release,
        Destination::kPortamentoRate
    };

    // Destinations that can smother or silence the patch when driven too hard.
    constexpr const char* kRiskDestinationNames[] = {
        Destination::kVca1Volume, Destination::kVca2Volume, Destination::kEnv2Amplitude,
        Destination::kVcfFrequency, Destination::kDco1Dco2Mix
    };

    constexpr const char* kAmplitudeDestinationNames[] = {
        Destination::kVca1Volume, Destination::kVca2Volume, Destination::kEnv2Amplitude
    };

    constexpr const char* kPitchDestinationNames[] = {
        Destination::kDco1Frequency, Destination::kDco2Frequency
    };

    const PluginDescriptors::ChoiceParameterDescriptor& busSourceDescriptor(int busIndex)
    {
        return PluginDescriptors::MatrixModulationSection::kModulationBusChoiceParameters[
            static_cast<size_t>(busIndex)][0];
    }

    const PluginDescriptors::ChoiceParameterDescriptor& busDestinationDescriptor(int busIndex)
    {
        return PluginDescriptors::MatrixModulationSection::kModulationBusChoiceParameters[
            static_cast<size_t>(busIndex)][1];
    }

    const PluginDescriptors::IntParameterDescriptor& busAmountDescriptor(int busIndex)
    {
        return PluginDescriptors::MatrixModulationSection::kModulationBusIntParameters[
            static_cast<size_t>(busIndex)][0];
    }

    template <size_t Count>
    juce::Array<int> resolveIndices(const juce::StringArray& choices, const char* const (&names)[Count])
    {
        juce::Array<int> indices;

        for (const auto* name : names)
        {
            const int index = choices.indexOf(juce::String(name));
            if (index >= 0)
                indices.addIfNotAlreadyThere(index);
        }

        return indices;
    }

    int pickFromArrayOrFullRange(const juce::Array<int>& preferred,
                                 bool usePreferred,
                                 int choiceCount,
                                 IRandomSource& rng)
    {
        if (usePreferred && ! preferred.isEmpty())
            return preferred[uniformRandomInt(rng, 0, preferred.size() - 1)];

        return uniformRandomInt(rng, 0, juce::jmax(1, choiceCount) - 1);
    }

    bool reviveSilentLiveMatrixModBuses(PatchModel& inOut, MutationPitchMode pitchMode)
    {
        const auto& catalog = MatrixModChoiceCatalog::shared();
        bool anyMovingBus = false;

        for (int busIndex = 0; busIndex < kBusCount; ++busIndex)
        {
            const auto bus = readMatrixModBus(inOut, busIndex);
            if (! bus.isLive)
                continue;

            if (pitchPreserveBlocksMutation(pitchMode) && catalog.isPitchDestination(bus.destinationIndex))
                continue;

            if (bus.amount == 0)
                inOut.setValue(busAmountDescriptor(busIndex), MutationCalibration::kMatrixModMotionAmount);

            anyMovingBus = true;
        }

        return anyMovingBus;
    }

    void inventMatrixModMotionBus(PatchModel& inOut, IRandomSource& rng)
    {
        const int growthBusIndex = firstDeadMatrixModBus(inOut);
        if (growthBusIndex < 0)
            return;

        const auto& catalog = MatrixModChoiceCatalog::shared();
        if (catalog.expressiveSources.isEmpty() || catalog.timbreDestinations.isEmpty())
            return;

        const int sourceIndex = catalog.expressiveSources[
            uniformRandomInt(rng, 0, catalog.expressiveSources.size() - 1)];
        const int destinationIndex = catalog.timbreDestinations[
            uniformRandomInt(rng, 0, catalog.timbreDestinations.size() - 1)];

        inOut.setChoiceIndex(busSourceDescriptor(growthBusIndex), sourceIndex);
        inOut.setChoiceIndex(busDestinationDescriptor(growthBusIndex), destinationIndex);
        inOut.setValue(busAmountDescriptor(growthBusIndex), MutationCalibration::kMatrixModMotionAmount);
    }
} // namespace

bool isMatrixModOffset(int sysExOffset) noexcept
{
    return sysExOffset >= kMatrixModOffsetMin && sysExOffset <= kMatrixModOffsetMax;
}

int matrixModBusForOffset(int sysExOffset) noexcept
{
    return (sysExOffset - kMatrixModOffsetMin) / kMatrixModBytesPerBus;
}

MatrixModField matrixModFieldForOffset(int sysExOffset) noexcept
{
    return static_cast<MatrixModField>((sysExOffset - kMatrixModOffsetMin) % kMatrixModBytesPerBus);
}

const MatrixModChoiceCatalog& MatrixModChoiceCatalog::shared()
{
    static const MatrixModChoiceCatalog catalog = []
    {
        const auto& sources = busSourceDescriptor(0).choices;
        const auto& destinations = busDestinationDescriptor(0).choices;

        MatrixModChoiceCatalog built;
        built.noneSourceIndex = juce::jmax(0, sources.indexOf(juce::String(Source::kNone)));
        built.noneDestinationIndex = juce::jmax(0, destinations.indexOf(juce::String(Destination::kNone)));
        built.expressiveSources = resolveIndices(sources, kExpressiveSourceNames);
        built.timbreDestinations = resolveIndices(destinations, kTimbreDestinationNames);
        built.riskDestinations = resolveIndices(destinations, kRiskDestinationNames);
        built.pitchDestinations = resolveIndices(destinations, kPitchDestinationNames);
        built.amplitudeDestinations = resolveIndices(destinations, kAmplitudeDestinationNames);
        return built;
    }();

    return catalog;
}

bool MatrixModChoiceCatalog::isRiskDestination(int destinationIndex) const
{
    return riskDestinations.contains(destinationIndex);
}

bool MatrixModChoiceCatalog::isPitchDestination(int destinationIndex) const
{
    return pitchDestinations.contains(destinationIndex);
}

bool MatrixModChoiceCatalog::isAmplitudeDestination(int destinationIndex) const
{
    return amplitudeDestinations.contains(destinationIndex);
}

int MatrixModChoiceCatalog::sourceIndexFor(const char* displayName) const
{
    return busSourceDescriptor(0).choices.indexOf(juce::String(displayName));
}

MatrixModBusView readMatrixModBus(const PatchModel& patch, int busIndex)
{
    const auto& catalog = MatrixModChoiceCatalog::shared();

    MatrixModBusView bus;
    bus.sourceIndex = patch.getChoiceIndex(busSourceDescriptor(busIndex));
    bus.amount = patch.getValue(busAmountDescriptor(busIndex));
    bus.destinationIndex = patch.getChoiceIndex(busDestinationDescriptor(busIndex));
    bus.isLive = bus.sourceIndex != catalog.noneSourceIndex
                 && bus.destinationIndex != catalog.noneDestinationIndex;
    return bus;
}

MatrixModLadderDecision matrixModLadderFor(MutationMode mode,
                                           const MatrixModBusView& seedBus,
                                           bool isGrowthBus)
{
    MatrixModLadderDecision decision;

    switch (mode)
    {
        case MutationMode::kKindred:
            decision.allowAmount = seedBus.isLive;
            decision.allowRouting = false;
            break;

        case MutationMode::kDrift:
            decision.allowAmount = seedBus.isLive;
            decision.allowRouting = isGrowthBus;
            break;

        case MutationMode::kWarp:
            decision.allowAmount = true;
            decision.allowRouting = true;
            break;

        case MutationMode::kWild:
            decision.allowAmount = true;
            decision.allowRouting = true;
            decision.preferExpressivePairs = false;
            break;
    }

    return decision;
}

int firstDeadMatrixModBus(const PatchModel& seed)
{
    for (int busIndex = 0; busIndex < kBusCount; ++busIndex)
    {
        if (! readMatrixModBus(seed, busIndex).isLive)
            return busIndex;
    }

    return -1;
}

int pickMatrixModSource(const MatrixModLadderDecision& decision, int choiceCount, IRandomSource& rng)
{
    const auto& catalog = MatrixModChoiceCatalog::shared();
    return pickFromArrayOrFullRange(catalog.expressiveSources,
                                    decision.preferExpressivePairs,
                                    choiceCount,
                                    rng);
}

int pickMatrixModDestination(const MatrixModLadderDecision& decision,
                             MutationPitchMode pitchMode,
                             int choiceCount,
                             IRandomSource& rng)
{
    const auto& catalog = MatrixModChoiceCatalog::shared();
    const int picked = pickFromArrayOrFullRange(catalog.timbreDestinations,
                                                decision.preferExpressivePairs,
                                                choiceCount,
                                                rng);

    // Preserve owns every pitch destination, so an unrestricted pick that landed on one
    // is redirected to timbre. With no timbre fallback, fail closed to None.
    if (pitchPreserveBlocksMutation(pitchMode) && catalog.isPitchDestination(picked))
    {
        if (! catalog.timbreDestinations.isEmpty())
        {
            return catalog.timbreDestinations[
                uniformRandomInt(rng, 0, catalog.timbreDestinations.size() - 1)];
        }

        return catalog.noneDestinationIndex;
    }

    return picked;
}

void capMatrixModRiskAmounts(PatchModel& inOut)
{
    const auto& catalog = MatrixModChoiceCatalog::shared();
    const int lfo1 = catalog.sourceIndexFor(Source::kLfo1);
    const int lfo2 = catalog.sourceIndexFor(Source::kLfo2);
    const int vibrato = catalog.sourceIndexFor(Source::kVibrato);

    for (int busIndex = 0; busIndex < kBusCount; ++busIndex)
    {
        const auto bus = readMatrixModBus(inOut, busIndex);
        if (! bus.isLive)
            continue;

        if (catalog.isRiskDestination(bus.destinationIndex))
        {
            const int capped = juce::jlimit(MutationCalibration::kMatrixModRiskAmountFloor,
                                            MutationCalibration::kMatrixModRiskAmountCeiling,
                                            bus.amount);
            if (capped != bus.amount)
                inOut.setValue(busAmountDescriptor(busIndex), capped);
        }

        // Periodic sources into amplitude dig silence holes (Cas 5: LFO 1 → VCA 1 VOLUME).
        if (catalog.isAmplitudeDestination(bus.destinationIndex)
            && (bus.sourceIndex == lfo1 || bus.sourceIndex == lfo2 || bus.sourceIndex == vibrato))
        {
            const int capped = juce::jlimit(-MutationCalibration::kMatrixModTremoloAmountCeiling,
                                            MutationCalibration::kMatrixModTremoloAmountCeiling,
                                            bus.amount);
            if (capped != bus.amount)
                inOut.setValue(busAmountDescriptor(busIndex), capped);
        }
    }
}

void ensureMatrixModMotion(PatchModel& inOut,
                           MutationMode mode,
                           MutationPitchMode pitchMode,
                           IRandomSource& rng)
{
    // Revive every live silent bus that Preserve does not own. Drift's optional growth
    // bus often lands here while another bus is already moving.
    if (reviveSilentLiveMatrixModBuses(inOut, pitchMode))
    {
        capMatrixModRiskAmounts(inOut);
        return;
    }

    // Kindred must not invent a route when nothing honest can be revived.
    if (mode == MutationMode::kKindred)
        return;

    inventMatrixModMotionBus(inOut, rng);
    capMatrixModRiskAmounts(inOut);
}

namespace
{
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

namespace
{
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
