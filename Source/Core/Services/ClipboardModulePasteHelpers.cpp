#include "ClipboardModulePasteHelpers.h"

#include <optional>

#include "Shared/Definitions/PluginIDs.h"

namespace Core::ClipboardModulePaste
{

namespace
{

using ParamSkipFn = bool (*)(PatchModuleKind, PatchModuleKind, const juce::String&);

const PluginDescriptors::IntParameterDescriptor* findIntByDisplayName(
    const std::vector<PluginDescriptors::IntParameterDescriptor>& params,
    const juce::String& displayName)
{
    for (const auto& descriptor : params)
    {
        if (descriptor.displayName == displayName)
            return &descriptor;
    }

    return nullptr;
}

const PluginDescriptors::ChoiceParameterDescriptor* findChoiceByDisplayName(
    const std::vector<PluginDescriptors::ChoiceParameterDescriptor>& params,
    const juce::String& displayName)
{
    for (const auto& descriptor : params)
    {
        if (descriptor.displayName == displayName)
            return &descriptor;
    }

    return nullptr;
}

bool shouldSkipDcoIntParam(PatchModuleKind source, PatchModuleKind target, const juce::String& paramId)
{
    using namespace PluginIDs::PatchEditSection;

    if (source == target)
        return false;

    if (paramId == Dco2Module::ParameterWidgets::kDetune)
        return true;

    juce::ignoreUnused(source, target);
    return false;
}

bool shouldSkipDcoChoiceParam(PatchModuleKind source, PatchModuleKind target, const juce::String& paramId)
{
    using namespace PluginIDs::PatchEditSection;

    if (source == target)
        return false;

    if (paramId == Dco1Module::ParameterWidgets::kSync)
        return true;

    juce::ignoreUnused(source, target);
    return false;
}

std::optional<int> mapDcoWaveSelectIndex(PatchModuleKind source,
                                         PatchModuleKind target,
                                         int sourceIndex)
{
    if (source == target)
        return sourceIndex;

    if (source == PatchModuleKind::Dco2 && target == PatchModuleKind::Dco1 && sourceIndex == 4)
        return std::nullopt;

    if (sourceIndex >= 0 && sourceIndex <= 3)
        return sourceIndex;

    return std::nullopt;
}

std::optional<int> mapDco2ToDco1KeyboardPortamento(int sourceIndex)
{
    if (sourceIndex == 0)
        return std::nullopt;

    if (sourceIndex == 1)
        return 1;

    if (sourceIndex == 2)
        return 0;

    return std::nullopt;
}

std::optional<int> mapDco1ToDco2KeyboardPortamento(int sourceIndex)
{
    if (sourceIndex == 0)
        return 2;

    if (sourceIndex == 1)
        return 1;

    return std::nullopt;
}

std::optional<int> mapDcoKeyboardPortamentoIndex(PatchModuleKind source,
                                                 PatchModuleKind target,
                                                 int sourceIndex)
{
    if (source == target)
        return sourceIndex;

    if (source == PatchModuleKind::Dco2 && target == PatchModuleKind::Dco1)
        return mapDco2ToDco1KeyboardPortamento(sourceIndex);

    if (source == PatchModuleKind::Dco1 && target == PatchModuleKind::Dco2)
        return mapDco1ToDco2KeyboardPortamento(sourceIndex);

    return std::nullopt;
}

bool shouldSkipLfoIntParam(PatchModuleKind source, PatchModuleKind target, const juce::String& paramId)
{
    using namespace PluginIDs::PatchEditSection;

    if (source == target)
        return false;

    // Cross-LFO: leave module-specific mod sources untouched (no Pressure↔Keyboard
    // or Ramp1↔Ramp2 remapping — same spirit as DCO Sync/Detune skip).
    // Defense in depth: displayNames already differ, so name matching alone would
    // also skip; explicit IDs pin the product rule if labels ever change.
    if (target == PatchModuleKind::Lfo1)
    {
        return paramId == Lfo1Module::ParameterWidgets::kSpeedModByPressure
            || paramId == Lfo1Module::ParameterWidgets::kAmplitudeModByRamp1;
    }

    if (target == PatchModuleKind::Lfo2)
    {
        return paramId == Lfo2Module::ParameterWidgets::kSpeedModByKeyboard
            || paramId == Lfo2Module::ParameterWidgets::kAmplitudeModByRamp2;
    }

    return false;
}

void pasteMatchedInts(const CrossModulePasteArgs& args, ParamSkipFn shouldSkip = nullptr)
{
    for (const auto& targetDescriptor : args.targetViews.intParams)
    {
        if (shouldSkip != nullptr
            && shouldSkip(args.source, args.target, targetDescriptor.parameterId))
            continue;

        const auto* sourceDescriptor = findIntByDisplayName(
            args.sourceViews.intParams,
            targetDescriptor.displayName);

        if (sourceDescriptor == nullptr)
            continue;

        const auto it = args.intValues.find(sourceDescriptor->parameterId);
        if (it != args.intValues.end())
            args.model.setValue(targetDescriptor, it->second);
    }
}

void pasteMatchedChoices(const CrossModulePasteArgs& args)
{
    for (const auto& targetDescriptor : args.targetViews.choiceParams)
    {
        const auto* sourceDescriptor = findChoiceByDisplayName(
            args.sourceViews.choiceParams,
            targetDescriptor.displayName);

        if (sourceDescriptor == nullptr)
            continue;

        const auto it = args.choiceIndices.find(sourceDescriptor->parameterId);
        if (it != args.choiceIndices.end())
            args.model.setChoiceIndex(targetDescriptor, it->second);
    }
}

std::optional<int> mapDcoChoiceIndexIfNeeded(PatchModuleKind source,
                                             PatchModuleKind target,
                                             const juce::String& targetParamId,
                                             int sourceIndex)
{
    using namespace PluginIDs::PatchEditSection;

    if (targetParamId == Dco1Module::ParameterWidgets::kWaveSelect
        || targetParamId == Dco2Module::ParameterWidgets::kWaveSelect)
        return mapDcoWaveSelectIndex(source, target, sourceIndex);

    if (targetParamId == Dco1Module::ParameterWidgets::kKeyboardPortamento
        || targetParamId == Dco2Module::ParameterWidgets::kKeyboardPortamento)
        return mapDcoKeyboardPortamentoIndex(source, target, sourceIndex);

    return sourceIndex;
}

void pasteDcoChoices(const CrossModulePasteArgs& args)
{
    for (const auto& targetDescriptor : args.targetViews.choiceParams)
    {
        if (shouldSkipDcoChoiceParam(args.source, args.target, targetDescriptor.parameterId))
            continue;

        const auto* sourceDescriptor = findChoiceByDisplayName(
            args.sourceViews.choiceParams,
            targetDescriptor.displayName);

        if (sourceDescriptor == nullptr)
            continue;

        const auto it = args.choiceIndices.find(sourceDescriptor->parameterId);
        if (it == args.choiceIndices.end())
            continue;

        const auto mapped = mapDcoChoiceIndexIfNeeded(
            args.source, args.target, targetDescriptor.parameterId, it->second);

        if (! mapped.has_value())
            continue;

        args.model.setChoiceIndex(targetDescriptor, *mapped);
    }
}

} // namespace

ModuleDescriptorViews descriptorsFor(PatchModuleKind kind)
{
    using namespace PluginDescriptors::PatchEditSection;

    switch (kind)
    {
        case PatchModuleKind::Dco1: return { Dco1Module::kIntParameters, Dco1Module::kChoiceParameters };
        case PatchModuleKind::Dco2: return { Dco2Module::kIntParameters, Dco2Module::kChoiceParameters };
        case PatchModuleKind::Env1: return { Envelope1Module::kIntParameters, Envelope1Module::kChoiceParameters };
        case PatchModuleKind::Env2: return { Envelope2Module::kIntParameters, Envelope2Module::kChoiceParameters };
        case PatchModuleKind::Env3: return { Envelope3Module::kIntParameters, Envelope3Module::kChoiceParameters };
        case PatchModuleKind::Lfo1: return { Lfo1Module::kIntParameters, Lfo1Module::kChoiceParameters };
        case PatchModuleKind::Lfo2: return { Lfo2Module::kIntParameters, Lfo2Module::kChoiceParameters };
    }

    return { Dco1Module::kIntParameters, Dco1Module::kChoiceParameters };
}

void pasteMatchedByDisplayName(const CrossModulePasteArgs& args)
{
    pasteMatchedInts(args);
    pasteMatchedChoices(args);
}

void pasteDcoByDisplayName(const CrossModulePasteArgs& args)
{
    pasteMatchedInts(args, shouldSkipDcoIntParam);
    pasteDcoChoices(args);
}

void pasteLfoByDisplayName(const CrossModulePasteArgs& args)
{
    pasteMatchedInts(args, shouldSkipLfoIntParam);
    pasteMatchedChoices(args);
}

} // namespace Core::ClipboardModulePaste
