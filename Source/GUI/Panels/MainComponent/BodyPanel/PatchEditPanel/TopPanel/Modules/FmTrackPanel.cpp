#include "FmTrackPanel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/BaseModulePanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

ModulePanelConfig FmTrackPanel::createConfig()
{
    ModulePanelConfig config;
    config.moduleId = PluginDescriptors::ModuleIds::kFmTrack;
    config.buttonSet = ModulePanelButtonSet::InitOnly;
    config.moduleType = ModulePanelModuleType::PatchEdit;
    config.initWidgetId = PluginDescriptors::StandaloneWidgetIds::kFmTrackInit;
    
    config.parameters = {
        {PluginDescriptors::ParameterIds::kFmAmount, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kFmModByEnv3, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kFmModByPressure, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kTrackPoint1, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kTrackPoint2, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kTrackPoint3, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kTrackPoint4, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kTrackPoint5, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kTrackInput, ModulePanelParameterType::ComboBox},
        {"", ModulePanelParameterType::None}
    };
    
    return config;
}

FmTrackPanel::FmTrackPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : BaseModulePanel(theme, widgetFactory, apvts, createConfig(), getWidth(), getHeight())
{
}
