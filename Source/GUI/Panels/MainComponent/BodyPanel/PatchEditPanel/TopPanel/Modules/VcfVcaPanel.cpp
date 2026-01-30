#include "VcfVcaPanel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/BaseModulePanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

ModulePanelConfig VcfVcaPanel::createConfig()
{
    ModulePanelConfig config;
    config.moduleId = PluginDescriptors::ModuleIds::kVcfVca;
    config.buttonSet = ModulePanelButtonSet::InitOnly;
    config.moduleType = ModulePanelModuleType::PatchEdit;
    config.initWidgetId = PluginDescriptors::StandaloneWidgetIds::kVcfVcaInit;
    
    config.parameters = {
        {PluginDescriptors::ParameterIds::kVcfBalance, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kVcfFequency, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kVcfFrequencyModByEnv1, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kVcfFrequencyModByPressure, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kVcfResonance, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kVca1Volume, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kVca1ModByVelocity, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kVca2ModByEnv2, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kVcfLevers, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kVcfKeyboardPortamento, ModulePanelParameterType::ComboBox}
    };
    
    return config;
}

VcfVcaPanel::VcfVcaPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : BaseModulePanel(theme, widgetFactory, apvts, createConfig(), getWidth(), getHeight())
{
}
