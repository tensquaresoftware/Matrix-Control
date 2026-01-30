#include "Env2Panel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/BaseModulePanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

ModulePanelConfig Env2Panel::createConfig()
{
    ModulePanelConfig config;
    config.moduleId = PluginDescriptors::ModuleIds::kEnvelope2;
    config.buttonSet = ModulePanelButtonSet::InitCopyPaste;
    config.moduleType = ModulePanelModuleType::PatchEdit;
    config.initWidgetId = PluginDescriptors::StandaloneWidgetIds::kEnv2Init;
    config.copyWidgetId = PluginDescriptors::StandaloneWidgetIds::kEnv2Copy;
    config.pasteWidgetId = PluginDescriptors::StandaloneWidgetIds::kEnv2Paste;
    
    config.parameters = {
        {PluginDescriptors::ParameterIds::kEnv2Delay, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv2Attack, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv2Decay, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv2Sustain, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv2Release, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv2Amplitude, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv2AmplitudeModByVelocity, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv2TriggerMode, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kEnv2EnvelopeMode, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kEnv2Lfo1Trigger, ModulePanelParameterType::ComboBox}
    };
    
    return config;
}

Env2Panel::Env2Panel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : BaseModulePanel(theme, widgetFactory, apvts, createConfig(), getWidth(), getHeight())
{
}
