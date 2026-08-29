#include "Core/Services/PatchMutator/MutationAlgorithm.h"

#include <cmath>

#include "Core/Models/ApvtsPatchMapper.h"
#include "Core/Services/PatchMutator/MutationMatrixModPolicy.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"

namespace Core
{

namespace
{
    constexpr size_t kMutableRangeStart = 8;

    namespace Dco1 = PluginIDs::PatchEditSection::Dco1Module::ParameterWidgets;
    namespace Dco2 = PluginIDs::PatchEditSection::Dco2Module::ParameterWidgets;
    namespace WaveSelectNames = PluginDisplayNames::ChoiceLists::WaveSelect;

    struct ModuleEnableFlag
    {
        const char* groupId = nullptr;
        bool MutationRecipe::* flag = nullptr;
    };

    constexpr ModuleEnableFlag kModuleEnableFlags[] = {
        { PluginIDs::PatchEditSection::Dco1Module::kGroupId, &MutationRecipe::enableDco1 },
        { PluginIDs::PatchEditSection::Dco2Module::kGroupId, &MutationRecipe::enableDco2 },
        { PluginIDs::PatchEditSection::VcfVcaModule::kGroupId, &MutationRecipe::enableVcfVca },
        { PluginIDs::PatchEditSection::FmTrackModule::kGroupId, &MutationRecipe::enableFmTrack },
        { PluginIDs::PatchEditSection::RampPortamentoModule::kGroupId, &MutationRecipe::enableRampPortamento },
        { PluginIDs::PatchEditSection::Envelope1Module::kGroupId, &MutationRecipe::enableEnvelope1 },
        { PluginIDs::PatchEditSection::Envelope2Module::kGroupId, &MutationRecipe::enableEnvelope2 },
        { PluginIDs::PatchEditSection::Envelope3Module::kGroupId, &MutationRecipe::enableEnvelope3 },
        { PluginIDs::PatchEditSection::Lfo1Module::kGroupId, &MutationRecipe::enableLfo1 },
        { PluginIDs::PatchEditSection::Lfo2Module::kGroupId, &MutationRecipe::enableLfo2 },
    };

    bool isDcoFrequencyParameter(const juce::String& parameterId, bool& isDco1)
    {
        if (parameterId == Dco1::kFrequency)
        {
            isDco1 = true;
            return true;
        }

        if (parameterId == Dco2::kFrequency)
        {
            isDco1 = false;
            return true;
        }

        return false;
    }

    bool isWaveSelectParameter(const juce::String& parameterId) noexcept
    {
        return parameterId == Dco1::kWaveSelect || parameterId == Dco2::kWaveSelect;
    }

    int waveSelectOffIndex(const PluginDescriptors::ChoiceParameterDescriptor& descriptor)
    {
        return descriptor.choices.indexOf(juce::String(WaveSelectNames::kOff));
    }

    bool waveSelectIsOff(const PatchModel& patch,
                         const PluginDescriptors::ChoiceParameterDescriptor& descriptor)
    {
        const int offIndex = waveSelectOffIndex(descriptor);
        return offIndex >= 0 && patch.getChoiceIndex(descriptor) == offIndex;
    }

    // Preserve owns the pitch destinations, so a bus already pointing at DCO pitch is
    // left exactly as the seed had it, amount included.
    bool pitchPreserveBlocksBus(const MutationRecipe& recipe, const MatrixModBusView& seedBus)
    {
        return pitchPreserveBlocksMutation(recipe.pitchMode)
               && MatrixModChoiceCatalog::shared().isPitchDestination(seedBus.destinationIndex);
    }

    int pickNearbyOrAnyChoiceIndex(const PluginDescriptors::ChoiceParameterDescriptor& descriptor,
                                   MutationMode mode,
                                   IRandomSource& rng,
                                   int currentIndex)
    {
        const int choiceCount = descriptor.choices.size();
        const int stepLimit = modeChoiceStepLimit(mode);

        if (stepLimit == MutationCalibration::kUnlimitedChoiceStep)
            return uniformRandomInt(rng, 0, choiceCount - 1);

        // Closer modes keep discrete jumps near the seed so a shape stays recognizable.
        const int seedIndex = juce::jlimit(0, choiceCount - 1, currentIndex);
        const int lowest = juce::jmax(0, seedIndex - stepLimit);
        const int highest = juce::jmin(choiceCount - 1, seedIndex + stepLimit);
        return uniformRandomInt(rng, lowest, highest);
    }

    // CHOIX: never leave both oscillators OFF in the middle of the pass.
    int avoidDualWaveSelectOff(const PatchModel& patch,
                               const PluginDescriptors::ChoiceParameterDescriptor& descriptor,
                               int proposedIndex)
    {
        if (! isWaveSelectParameter(descriptor.parameterId))
            return proposedIndex;

        const int offIndex = waveSelectOffIndex(descriptor);
        if (offIndex < 0 || proposedIndex != offIndex)
            return proposedIndex;

        const auto* otherWave = findMutationChoiceDescriptor(
            descriptor.parameterId == Dco1::kWaveSelect ? Dco2::kWaveSelect : Dco1::kWaveSelect);
        if (otherWave == nullptr || ! waveSelectIsOff(patch, *otherWave))
            return proposedIndex;

        const int waveIndex = descriptor.choices.indexOf(juce::String(WaveSelectNames::kWave));
        if (waveIndex >= 0)
            return waveIndex;

        return offIndex == 0 && descriptor.choices.size() > 1 ? 1 : 0;
    }
} // namespace

bool MutationAlgorithm::apply(PatchModel& inOut, const MutationRecipe& recipe, IRandomSource& rng) const
{
    const int amountPercent = clampPercent(recipe.amountPercent);
    const int randomPercent = clampPercent(recipe.randomPercent);

    if (amountPercent == 0 || randomPercent == 0)
        return false;

    PatchModel before;
    before.loadFrom(inOut.data());

    const auto seedFacts = MutationSeedFacts::read(before);
    const double amount = static_cast<double>(amountPercent) / 100.0;
    const double random = static_cast<double>(randomPercent) / 100.0;

    MutationPass pass {
        recipe,
        before,
        rng,
        amount,
        random,
        recipe.enableMatrixMod,
        recipe.enableMatrixMod && recipe.mode == MutationMode::kDrift
            ? firstDeadMatrixModBus(before)
            : -1,
        matrixModDrivesVca2Volume(before),
        {}
    };
    pass.pitchPlan = planPitchForPass(pass);

    mutateIntDescriptors(inOut, pass);
    applyFamilyGestures(inOut, before, recipe);
    mutateChoiceDescriptors(inOut, pass);
    applyPostMutationGuards(inOut, before, recipe, seedFacts);

    if (recipe.enableMatrixMod)
    {
        capMatrixModRiskAmounts(inOut);
        ensureMatrixModMotion(inOut, recipe.mode, recipe.pitchMode, rng);
        // Motion may revive a risk bus after the first guard pass — re-arm amplitude floors.
        applyMatrixModRoleGuards(inOut, recipe);
    }

    restoreProtectedNameBytes(before, inOut);

    return anyByteChangedInRange(before, inOut, kMutableRangeStart, PatchModel::kBufferSize);
}

int MutationAlgorithm::clampPercent(int value) noexcept
{
    return juce::jlimit(0, 100, value);
}

int MutationAlgorithm::roundHalfUp(double value) noexcept
{
    return static_cast<int>(std::floor(value + 0.5));
}

bool MutationAlgorithm::isModuleEnabled(const MutationRecipe& recipe, const juce::String& parentGroupId)
{
    for (const auto& entry : kModuleEnableFlags)
    {
        if (parentGroupId == entry.groupId)
            return recipe.*(entry.flag);
    }

    return false;
}

bool MutationAlgorithm::isIntDescriptorEligible(const PluginDescriptors::IntParameterDescriptor& descriptor,
                                                const MutationRecipe& recipe,
                                                bool matrixModScopeActive)
{
    if (isMatrixModOffset(descriptor.sysExOffset))
        return matrixModScopeActive;

    return isModuleEnabled(recipe, descriptor.parentGroupId);
}

bool MutationAlgorithm::isChoiceDescriptorEligible(const PluginDescriptors::ChoiceParameterDescriptor& descriptor,
                                                   const MutationRecipe& recipe,
                                                   bool matrixModScopeActive)
{
    if (isMatrixModOffset(descriptor.sysExOffset))
        return matrixModScopeActive;

    return isModuleEnabled(recipe, descriptor.parentGroupId);
}

int MutationAlgorithm::neighborhoodSpread(const MutationPass& pass, int range)
{
    const double halfSpan = (static_cast<double>(range) / 2.0) * pass.amount;
    return roundHalfUp(halfSpan * pass.random);
}

bool MutationAlgorithm::passesSparseHitGate(const MutationPass& pass)
{
    const float hitProbability = modeHitProbability(pass.recipe.mode);

    // Only the sparse modes spend a draw here, so the fuller modes keep the plain
    // parent draw sequence.
    if (hitProbability >= MutationCalibration::kEveryParameterHitProbability)
        return true;

    return pass.rng.nextFloat() < hitProbability;
}

MutationPitchPlan MutationAlgorithm::planPitchForPass(const MutationPass& pass)
{
    const auto* dco1Frequency = findMutationIntDescriptor(Dco1::kFrequency);
    const auto* dco2Frequency = findMutationIntDescriptor(Dco2::kFrequency);

    if (dco1Frequency == nullptr || dco2Frequency == nullptr)
        return {};

    MutationPitchRequest request;
    request.pitchMode = pass.recipe.pitchMode;
    request.mode = pass.recipe.mode;
    request.octaveWindow = pass.recipe.pitchOctaveWindow;
    request.minValue = dco1Frequency->minValue;
    request.maxValue = dco1Frequency->maxValue;
    request.dco1Seed = pass.seed.getValue(*dco1Frequency);
    request.dco2Seed = pass.seed.getValue(*dco2Frequency);
    request.dco1Writable = pass.recipe.enableDco1;
    request.dco2Writable = pass.recipe.enableDco2;
    request.freeSpread = neighborhoodSpread(pass, dco1Frequency->maxValue - dco1Frequency->minValue);

    return planPitchJumps(request, pass.rng);
}

// Returns true when PITCH owns this parameter, so the neighborhood jitter must stand down.
bool MutationAlgorithm::applyPitchGatedValue(PatchModel& inOut,
                                            const PluginDescriptors::IntParameterDescriptor& descriptor,
                                            const MutationPass& pass)
{
    bool isDco1 = false;
    if (! isDcoFrequencyParameter(descriptor.parameterId, isDco1))
        return false;

    const bool hasPlannedValue = isDco1 ? pass.pitchPlan.hasDco1Value : pass.pitchPlan.hasDco2Value;
    if (hasPlannedValue)
        inOut.setValue(descriptor, isDco1 ? pass.pitchPlan.dco1Value : pass.pitchPlan.dco2Value);

    return true;
}

void MutationAlgorithm::jitterIntDescriptor(PatchModel& inOut,
                                            const PluginDescriptors::IntParameterDescriptor& descriptor,
                                            const MutationPass& pass)
{
    const auto policy = policyForIntParameter(descriptor.parameterId);

    if (policy.kind == MutationPolicyKind::kPitchGated
        && applyPitchGatedValue(inOut, descriptor, pass))
    {
        return;
    }

    if (isLockedForMode(policy, pass.recipe.mode)
        && ! matrixModRelayLiftsLock(descriptor.parameterId, pass.seedHasMatrixModVca2Relay))
    {
        return;
    }

    if (! passesSparseHitGate(pass))
        return;

    const int range = descriptor.maxValue - descriptor.minValue;
    const int spread = scaleNeighborhood(policy, neighborhoodSpread(pass, range));
    if (spread == 0)
        return;

    const int oldValue = inOut.getValue(descriptor);
    const int delta = uniformRandomInt(pass.rng, -spread, spread);
    inOut.setValue(descriptor,
                   applyPolicyBounds(policy, oldValue + delta, descriptor.minValue, descriptor.maxValue));
}

void MutationAlgorithm::mutateIntDescriptors(PatchModel& inOut, const MutationPass& pass)
{
    for (const auto& descriptor : ApvtsPatchMapper::buildIntDescriptors())
    {
        if (! isIntDescriptorEligible(descriptor, pass.recipe, pass.matrixModScopeActive))
            continue;

        if (isMatrixModOffset(descriptor.sysExOffset))
        {
            mutateMatrixModAmount(inOut, descriptor, pass);
            continue;
        }

        if (descriptor.maxValue <= descriptor.minValue)
            continue;

        jitterIntDescriptor(inOut, descriptor, pass);
    }
}

void MutationAlgorithm::mutateChoiceDescriptors(PatchModel& inOut, const MutationPass& pass)
{
    for (const auto& descriptor : ApvtsPatchMapper::buildChoiceDescriptors())
    {
        if (! isChoiceDescriptorEligible(descriptor, pass.recipe, pass.matrixModScopeActive))
            continue;

        if (isMatrixModOffset(descriptor.sysExOffset))
        {
            mutateMatrixModRouting(inOut, descriptor, pass);
            continue;
        }

        if (descriptor.choices.size() <= 1)
            continue;

        if (pass.rng.nextFloat() >= static_cast<float>(pass.amount * pass.random))
            continue;

        const int proposed = pickNearbyOrAnyChoiceIndex(descriptor,
                                                        pass.recipe.mode,
                                                        pass.rng,
                                                        inOut.getChoiceIndex(descriptor));
        inOut.setChoiceIndex(descriptor, avoidDualWaveSelectOff(inOut, descriptor, proposed));
    }
}

void MutationAlgorithm::mutateMatrixModAmount(PatchModel& inOut,
                                              const PluginDescriptors::IntParameterDescriptor& descriptor,
                                              const MutationPass& pass)
{
    if (matrixModFieldForOffset(descriptor.sysExOffset) != MatrixModField::kAmount)
        return;

    const int busIndex = matrixModBusForOffset(descriptor.sysExOffset);
    const auto seedBus = readMatrixModBus(pass.seed, busIndex);

    if (pitchPreserveBlocksBus(pass.recipe, seedBus))
        return;

    const auto decision = matrixModLadderFor(pass.recipe.mode,
                                             seedBus,
                                             busIndex == pass.matrixModGrowthBusIndex);
    // A routing grant alone is not an amount grant: a dead Drift growth bus gets its
    // starting amount from the motion guarantee, not from blind jitter.
    if (! decision.allowAmount)
        return;

    const int range = descriptor.maxValue - descriptor.minValue;
    if (range <= 0)
        return;

    const int spread = neighborhoodSpread(pass, range);
    if (spread == 0)
        return;

    const int oldValue = inOut.getValue(descriptor);
    const int delta = uniformRandomInt(pass.rng, -spread, spread);
    inOut.setValue(descriptor,
                   juce::jlimit(descriptor.minValue, descriptor.maxValue, oldValue + delta));
}

void MutationAlgorithm::mutateMatrixModRouting(PatchModel& inOut,
                                               const PluginDescriptors::ChoiceParameterDescriptor& descriptor,
                                               const MutationPass& pass)
{
    const auto field = matrixModFieldForOffset(descriptor.sysExOffset);
    const int busIndex = matrixModBusForOffset(descriptor.sysExOffset);
    const auto seedBus = readMatrixModBus(pass.seed, busIndex);

    if (pitchPreserveBlocksBus(pass.recipe, seedBus))
        return;

    const auto decision = matrixModLadderFor(pass.recipe.mode,
                                             seedBus,
                                             busIndex == pass.matrixModGrowthBusIndex);
    if (! decision.allowRouting)
        return;

    const int choiceCount = descriptor.choices.size();
    if (choiceCount <= 1)
        return;

    const double pMutate = pass.amount * pass.random;
    if (pass.rng.nextFloat() >= static_cast<float>(pMutate))
        return;

    const int newIndex = field == MatrixModField::kSource
        ? pickMatrixModSource(decision, choiceCount, pass.rng)
        : pickMatrixModDestination(decision, pass.recipe.pitchMode, choiceCount, pass.rng);

    inOut.setChoiceIndex(descriptor, newIndex);
}

void MutationAlgorithm::restoreProtectedNameBytes(const PatchModel& before, PatchModel& inOut)
{
    for (size_t i = 0; i < PatchModel::kNameLength; ++i)
    {
        if (inOut.data()[i] != before.data()[i])
        {
            inOut.data()[i] = before.data()[i];
            jassertfalse;
        }
    }
}

bool MutationAlgorithm::anyByteChangedInRange(const PatchModel& before,
                                              const PatchModel& after,
                                              size_t startIndex,
                                              size_t endExclusive)
{
    for (size_t i = startIndex; i < endExclusive; ++i)
    {
        if (before.data()[i] != after.data()[i])
            return true;
    }

    return false;
}

} // namespace Core
