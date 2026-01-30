#include "VibratoPanel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/BaseModulePanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

ModulePanelConfig VibratoPanel::createConfig()
{
    ModulePanelConfig config;
    config.moduleId = PluginDescriptors::ModuleIds::kVibrato;
    config.buttonSet = ModulePanelButtonSet::InitOnly;
    config.moduleType = ModulePanelModuleType::MasterEdit;
    config.initWidgetId = PluginDescriptors::StandaloneWidgetIds::kVibratoInit;
    
    config.parameters = {
        {PluginDescriptors::ParameterIds::kVibratoSpeed, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kVibratoWaveform, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kVibratoAmplitude, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kVibratoSpeedModSource, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kVibratoSpeedModAmount, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kVibratoAmpModSource, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kVibratoAmpModAmount, ModulePanelParameterType::Slider}
    };
    
    return config;
}

VibratoPanel::VibratoPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : BaseModulePanel(theme, widgetFactory, apvts, createConfig(), getWidth(), getHeight())
{
}
