#include "Lfo2Panel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/BaseModulePanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

ModulePanelConfig Lfo2Panel::createConfig()
{
    ModulePanelConfig config;
    config.moduleId = PluginDescriptors::ModuleIds::kLfo2;
    config.buttonSet = ModulePanelButtonSet::InitCopyPaste;
    config.moduleType = ModulePanelModuleType::PatchEdit;
    config.initWidgetId = PluginDescriptors::StandaloneWidgetIds::kLfo2Init;
    config.copyWidgetId = PluginDescriptors::StandaloneWidgetIds::kLfo2Copy;
    config.pasteWidgetId = PluginDescriptors::StandaloneWidgetIds::kLfo2Paste;
    
    config.parameters = {
        {PluginDescriptors::ParameterIds::kLfo2Speed, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kLfo2SpeedModByKeyboard, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kLfo2RetriggerPoint, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kLfo2Amplitude, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kLfo2AmplitudeModByRamp2, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kLfo2Waveform, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kLfo2TriggerMode, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kLfo2Lag, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kLfo2SampleInput, ModulePanelParameterType::ComboBox},
        {"", ModulePanelParameterType::None}
    };
    
    return config;
}

Lfo2Panel::Lfo2Panel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : BaseModulePanel(theme, widgetFactory, apvts, createConfig(), getWidth(), getHeight())
{
}
