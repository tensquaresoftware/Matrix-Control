#include "RampPortamentoPanel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/BaseModulePanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

ModulePanelConfig RampPortamentoPanel::createConfig()
{
    ModulePanelConfig config;
    config.moduleId = PluginDescriptors::ModuleIds::kRampPortamento;
    config.buttonSet = ModulePanelButtonSet::InitOnly;
    config.moduleType = ModulePanelModuleType::PatchEdit;
    config.initWidgetId = PluginDescriptors::StandaloneWidgetIds::kRampPortamentoInit;
    
    config.parameters = {
        {PluginDescriptors::ParameterIds::kRamp1Rate, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kRamp1Trigger, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kRamp2Rate, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kRamp2Trigger, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kPortamentoRate, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kPortamentoModByVelocity, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kPortamentoMode, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kPortamentoLegato, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kPortamentoKeyboardMode, ModulePanelParameterType::ComboBox},
        {"", ModulePanelParameterType::None}
    };
    
    return config;
}

RampPortamentoPanel::RampPortamentoPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : BaseModulePanel(theme, widgetFactory, apvts, createConfig(), getWidth(), getHeight())
{
}
