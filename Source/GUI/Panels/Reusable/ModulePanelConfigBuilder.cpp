#include "ModulePanelConfigBuilder.h"

#include "Shared/Definitions/PluginHelpers.h"
#include "Shared/Exceptions/WidgetFactoryExceptions.h"

namespace
{
    struct ModuleLayoutSpec
    {
        juce::String moduleGroupId;
        ModulePanelButtonSet buttonSet = ModulePanelButtonSet::InitCopyPaste;
        ModulePanelModuleType moduleType = ModulePanelModuleType::PatchEdit;
        ModulePanelActionWidgetIds actionWidgetIds;
    };

    ModulePanelLayout makeModuleLayout(
        const ModuleLayoutSpec& spec,
        std::initializer_list<const char*> orderedParameterIds)
    {
        ModulePanelLayout layout;
        layout.moduleId = spec.moduleGroupId;
        layout.buttonSet = spec.buttonSet;
        layout.moduleType = spec.moduleType;
        layout.initWidgetId = spec.actionWidgetIds.initWidgetId;
        layout.copyWidgetId = spec.actionWidgetIds.copyWidgetId;
        layout.pasteWidgetId = spec.actionWidgetIds.pasteWidgetId;
        layout.pasteEnabledPropertyId = spec.actionWidgetIds.pasteEnabledPropertyId;
        layout.orderedParameterIds.reserve(orderedParameterIds.size());

        for (const auto* parameterId : orderedParameterIds)
            layout.orderedParameterIds.emplace_back(parameterId);

        return layout;
    }
}

ModulePanelConfig buildModulePanelConfig(const ModulePanelLayout& layout)
{
    ModulePanelConfig config;
    config.moduleId = layout.moduleId;
    config.buttonSet = layout.buttonSet;
    config.moduleType = layout.moduleType;
    config.initWidgetId = layout.initWidgetId;
    config.copyWidgetId = layout.copyWidgetId;
    config.pasteWidgetId = layout.pasteWidgetId;
    config.pasteEnabledPropertyId = layout.pasteEnabledPropertyId;
    config.parameters.reserve(layout.orderedParameterIds.size());

    for (const auto& parameterId : layout.orderedParameterIds)
    {
        if (parameterId.isEmpty())
        {
            config.parameters.push_back({ juce::String(), ParameterCell::ParameterType::None });
            continue;
        }

        const auto widgetKind = PluginHelpers::resolveParameterWidgetKind(parameterId);
        if (! widgetKind.has_value())
        {
            jassertfalse;
            throw ParameterNotFoundException(parameterId);
        }

        const auto parameterType = (*widgetKind == PluginHelpers::ParameterWidgetKind::Slider)
            ? ParameterCell::ParameterType::Slider
            : ParameterCell::ParameterType::ComboBox;

        config.parameters.push_back({ parameterId, parameterType });
    }

    return config;
}

ModulePanelLayout makePatchEditModuleLayout(
    const juce::String& moduleGroupId,
    const ModulePanelActionWidgetIds& actionWidgetIds,
    std::initializer_list<const char*> orderedParameterIds)
{
    return makeModuleLayout(
        {
            .moduleGroupId = moduleGroupId,
            .buttonSet = ModulePanelButtonSet::InitCopyPaste,
            .moduleType = ModulePanelModuleType::PatchEdit,
            .actionWidgetIds = actionWidgetIds
        },
        orderedParameterIds);
}

ModulePanelLayout makePatchEditInitOnlyModuleLayout(
    const juce::String& moduleGroupId,
    const juce::String& initId,
    std::initializer_list<const char*> orderedParameterIds)
{
    return makeModuleLayout(
        {
            .moduleGroupId = moduleGroupId,
            .buttonSet = ModulePanelButtonSet::InitOnly,
            .moduleType = ModulePanelModuleType::PatchEdit,
            .actionWidgetIds = { .initWidgetId = initId }
        },
        orderedParameterIds);
}

ModulePanelLayout makeMasterEditModuleLayout(
    const juce::String& moduleGroupId,
    const juce::String& initId,
    std::initializer_list<const char*> orderedParameterIds)
{
    return makeModuleLayout(
        {
            .moduleGroupId = moduleGroupId,
            .buttonSet = ModulePanelButtonSet::InitOnly,
            .moduleType = ModulePanelModuleType::MasterEdit,
            .actionWidgetIds = { .initWidgetId = initId }
        },
        orderedParameterIds);
}
