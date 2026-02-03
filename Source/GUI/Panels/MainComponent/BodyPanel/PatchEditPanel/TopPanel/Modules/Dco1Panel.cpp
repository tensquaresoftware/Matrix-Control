#include "Dco1Panel.h"

#include "GUI/Themes/Skin.h"
#include "GUI/Panels/Reusable/BaseModulePanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"


ModulePanelConfig Dco1Panel::createConfig()
{
    ModulePanelConfig config;
    config.moduleId = PluginDescriptors::ModuleIds::kDco1;
    config.buttonSet = ModulePanelButtonSet::InitCopyPaste;
    config.moduleType = ModulePanelModuleType::PatchEdit;
    config.initWidgetId = PluginDescriptors::StandaloneWidgetIds::kDco1Init;
    config.copyWidgetId = PluginDescriptors::StandaloneWidgetIds::kDco1Copy;
    config.pasteWidgetId = PluginDescriptors::StandaloneWidgetIds::kDco1Paste;
    
    config.parameters = {
        {PluginDescriptors::ParameterIds::kDco1Frequency, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kDco1FrequencyModByLfo1, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kDco1Sync, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kDco1PulseWidth, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kDco1PulseWidthModByLfo2, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kDco1WaveShape, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kDco1WaveSelect, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kDco1Levers, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kDco1KeyboardPortamento, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kDco1KeyClick, ModulePanelParameterType::ComboBox}
    };
    
    return config;
}

Dco1Panel::Dco1Panel(tss::Skin& skin, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : BaseModulePanel(skin, widgetFactory, apvts, createConfig(), getWidth(), getHeight())
{
}

