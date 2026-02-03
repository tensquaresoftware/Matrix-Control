#include "Lfo1Panel.h"

#include "GUI/Themes/Skin.h"
#include "GUI/Panels/Reusable/BaseModulePanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"


ModulePanelConfig Lfo1Panel::createConfig()
{
    ModulePanelConfig config;
    config.moduleId = PluginDescriptors::ModuleIds::kLfo1;
    config.buttonSet = ModulePanelButtonSet::InitCopyPaste;
    config.moduleType = ModulePanelModuleType::PatchEdit;
    config.initWidgetId = PluginDescriptors::StandaloneWidgetIds::kLfo1Init;
    config.copyWidgetId = PluginDescriptors::StandaloneWidgetIds::kLfo1Copy;
    config.pasteWidgetId = PluginDescriptors::StandaloneWidgetIds::kLfo1Paste;
    
    config.parameters = {
        {PluginDescriptors::ParameterIds::kLfo1Speed, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kLfo1SpeedModByPressure, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kLfo1RetriggerPoint, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kLfo1Amplitude, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kLfo1AmplitudeModByRamp1, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kLfo1Waveform, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kLfo1TriggerMode, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kLfo1Lag, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kLfo1SampleInput, ModulePanelParameterType::ComboBox},
        {"", ModulePanelParameterType::None}
    };
    
    return config;
}

Lfo1Panel::Lfo1Panel(tss::Skin& skin, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : BaseModulePanel(skin, widgetFactory, apvts, createConfig(), getWidth(), getHeight())
{
}
