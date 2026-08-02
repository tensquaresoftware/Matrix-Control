#pragma once

#include <unordered_map>
#include <vector>

#include <juce_core/juce_core.h>

#include "Core/Models/PatchModel.h"
#include "Core/Services/ClipboardService.h"
#include "Shared/Definitions/PluginDescriptors.h"

namespace Core::ClipboardModulePaste
{

struct ModuleDescriptorViews
{
    const std::vector<PluginDescriptors::IntParameterDescriptor>& intParams;
    const std::vector<PluginDescriptors::ChoiceParameterDescriptor>& choiceParams;
};

struct CrossModulePasteArgs
{
    PatchModuleKind source {};
    PatchModuleKind target {};
    const ModuleDescriptorViews& sourceViews;
    const ModuleDescriptorViews& targetViews;
    const std::unordered_map<juce::String, int>& intValues;
    const std::unordered_map<juce::String, int>& choiceIndices;
    PatchModel& model;
};

ModuleDescriptorViews descriptorsFor(PatchModuleKind kind);

void pasteMatchedByDisplayName(const CrossModulePasteArgs& args);
void pasteDcoByDisplayName(const CrossModulePasteArgs& args);
void pasteLfoByDisplayName(const CrossModulePasteArgs& args);

} // namespace Core::ClipboardModulePaste
