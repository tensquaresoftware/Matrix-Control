#include "Core/Services/PatchMutator/MutationAlgorithm.h"

#include <cmath>
#include <cstring>

#include "Core/Models/ApvtsPatchMapper.h"
#include "Shared/Definitions/PluginIDs.h"

namespace Core
{

namespace
{
    constexpr int kMatrixModOffsetMin = 104;
    constexpr int kMatrixModOffsetMax = 133;
    constexpr size_t kMutableRangeStart = 8;

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
} // namespace

bool MutationAlgorithm::apply(PatchModel& inOut, const MutationRecipe& recipe, IRandomSource& rng) const
{
    const int amountPercent = clampPercent(recipe.amountPercent);
    const int randomPercent = clampPercent(recipe.randomPercent);

    if (amountPercent == 0 || randomPercent == 0)
        return false;

    PatchModel before;
    before.loadFrom(inOut.data());

    const MutationPass pass {
        recipe,
        rng,
        static_cast<double>(amountPercent) / 100.0,
        static_cast<double>(randomPercent) / 100.0,
        recipe.enableMatrixMod
    };

    mutateIntDescriptors(inOut, pass);
    mutateChoiceDescriptors(inOut, pass);
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

int MutationAlgorithm::uniformRandomInt(IRandomSource& rng, int lo, int hi)
{
    jassert(lo <= hi);
    return lo + rng.nextInt(hi - lo + 1);
}

bool MutationAlgorithm::isMatrixModOffset(int sysExOffset) noexcept
{
    return sysExOffset >= kMatrixModOffsetMin && sysExOffset <= kMatrixModOffsetMax;
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

void MutationAlgorithm::mutateIntDescriptors(PatchModel& inOut, const MutationPass& pass)
{
    for (const auto& descriptor : ApvtsPatchMapper::buildIntDescriptors())
    {
        if (! isIntDescriptorEligible(descriptor, pass.recipe, pass.matrixModScopeActive))
            continue;

        const int minValue = descriptor.minValue;
        const int maxValue = descriptor.maxValue;
        const int oldValue = inOut.getValue(descriptor);
        const int range = maxValue - minValue;
        if (range <= 0)
            continue;

        const double halfSpan = (static_cast<double>(range) / 2.0) * pass.amount;
        const int spread = roundHalfUp(halfSpan * pass.random);

        if (spread == 0)
            continue;

        const int delta = uniformRandomInt(pass.rng, -spread, spread);
        const int newValue = juce::jlimit(minValue, maxValue, oldValue + delta);
        inOut.setValue(descriptor, newValue);
    }
}

void MutationAlgorithm::mutateChoiceDescriptors(PatchModel& inOut, const MutationPass& pass)
{
    for (const auto& descriptor : ApvtsPatchMapper::buildChoiceDescriptors())
    {
        if (! isChoiceDescriptorEligible(descriptor, pass.recipe, pass.matrixModScopeActive))
            continue;

        const int choiceCount = descriptor.choices.size();
        if (choiceCount <= 1)
            continue;

        const double pMutate = pass.amount * pass.random;
        if (pass.rng.nextFloat() >= static_cast<float>(pMutate))
            continue;

        const int newIndex = uniformRandomInt(pass.rng, 0, choiceCount - 1);
        inOut.setChoiceIndex(descriptor, newIndex);
    }
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
