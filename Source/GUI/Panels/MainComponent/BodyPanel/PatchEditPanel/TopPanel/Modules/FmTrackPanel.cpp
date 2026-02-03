#include "FmTrackPanel.h"

#include "GUI/Themes/Skin.h"
#include "GUI/Panels/Reusable/BaseModulePanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"


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

FmTrackPanel::FmTrackPanel(tss::Skin& skin, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : BaseModulePanel(skin, widgetFactory, apvts, createConfig(), getWidth(), getHeight())
{
}
