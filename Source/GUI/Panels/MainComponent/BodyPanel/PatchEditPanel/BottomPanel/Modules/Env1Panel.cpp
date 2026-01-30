#include "Env1Panel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/BaseModulePanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

ModulePanelConfig Env1Panel::createConfig()
{
    ModulePanelConfig config;
    config.moduleId = PluginDescriptors::ModuleIds::kEnvelope1;
    config.buttonSet = ModulePanelButtonSet::InitCopyPaste;
    config.moduleType = ModulePanelModuleType::PatchEdit;
    config.initWidgetId = PluginDescriptors::StandaloneWidgetIds::kEnv1Init;
    config.copyWidgetId = PluginDescriptors::StandaloneWidgetIds::kEnv1Copy;
    config.pasteWidgetId = PluginDescriptors::StandaloneWidgetIds::kEnv1Paste;
    
    config.parameters = {
        {PluginDescriptors::ParameterIds::kEnv1Delay, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv1Attack, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv1Decay, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv1Sustain, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv1Release, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv1Amplitude, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv1AmplitudeModByVelocity, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv1TriggerMode, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kEnv1EnvelopeMode, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kEnv1Lfo1Trigger, ModulePanelParameterType::ComboBox}
    };
    
    return config;
}

Env1Panel::Env1Panel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : BaseModulePanel(theme, widgetFactory, apvts, createConfig(), getWidth(), getHeight())
{
}
