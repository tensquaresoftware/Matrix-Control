#include "Env3Panel.h"

#include "GUI/Themes/Skin.h"
#include "GUI/Panels/Reusable/BaseModulePanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"


ModulePanelConfig Env3Panel::createConfig()
{
    ModulePanelConfig config;
    config.moduleId = PluginDescriptors::ModuleIds::kEnvelope3;
    config.buttonSet = ModulePanelButtonSet::InitCopyPaste;
    config.moduleType = ModulePanelModuleType::PatchEdit;
    config.initWidgetId = PluginDescriptors::StandaloneWidgetIds::kEnv3Init;
    config.copyWidgetId = PluginDescriptors::StandaloneWidgetIds::kEnv3Copy;
    config.pasteWidgetId = PluginDescriptors::StandaloneWidgetIds::kEnv3Paste;
    
    config.parameters = {
        {PluginDescriptors::ParameterIds::kEnv3Delay, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv3Attack, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv3Decay, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv3Sustain, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv3Release, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv3Amplitude, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv3AmplitudeModByVelocity, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kEnv3TriggerMode, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kEnv3EnvelopeMode, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kEnv3Lfo1Trigger, ModulePanelParameterType::ComboBox}
    };
    
    return config;
}

Env3Panel::Env3Panel(tss::Skin& skin, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : BaseModulePanel(skin, widgetFactory, apvts, createConfig(), getWidth(), getHeight())
{
}
