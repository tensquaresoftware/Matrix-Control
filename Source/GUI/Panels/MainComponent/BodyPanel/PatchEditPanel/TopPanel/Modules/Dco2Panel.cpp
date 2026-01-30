#include "Dco2Panel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/BaseModulePanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

ModulePanelConfig Dco2Panel::createConfig()
{
    ModulePanelConfig config;
    config.moduleId = PluginDescriptors::ModuleIds::kDco2;
    config.buttonSet = ModulePanelButtonSet::InitCopyPaste;
    config.moduleType = ModulePanelModuleType::PatchEdit;
    config.initWidgetId = PluginDescriptors::StandaloneWidgetIds::kDco2Init;
    config.copyWidgetId = PluginDescriptors::StandaloneWidgetIds::kDco2Copy;
    config.pasteWidgetId = PluginDescriptors::StandaloneWidgetIds::kDco2Paste;
    
    config.parameters = {
        {PluginDescriptors::ParameterIds::kDco2Frequency, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kDco2FrequencyModByLfo1, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kDco2Detune, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kDco2PulseWidth, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kDco2PulseWidthModByLfo2, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kDco2WaveShape, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kDco2WaveSelect, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kDco2Levers, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kDco2KeyboardPortamento, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kDco2KeyClick, ModulePanelParameterType::ComboBox}
    };
    
    return config;
}

Dco2Panel::Dco2Panel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : BaseModulePanel(theme, widgetFactory, apvts, createConfig(), getWidth(), getHeight())
{
}
