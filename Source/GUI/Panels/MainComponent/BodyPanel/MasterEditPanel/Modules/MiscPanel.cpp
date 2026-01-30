#include "MiscPanel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/BaseModulePanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

ModulePanelConfig MiscPanel::createConfig()
{
    ModulePanelConfig config;
    config.moduleId = PluginDescriptors::ModuleIds::kMisc;
    config.buttonSet = ModulePanelButtonSet::InitOnly;
    config.moduleType = ModulePanelModuleType::MasterEdit;
    config.initWidgetId = PluginDescriptors::StandaloneWidgetIds::kMiscInit;
    
    config.parameters = {
        {PluginDescriptors::ParameterIds::kMasterTune, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kMasterTranspose, ModulePanelParameterType::Slider},
        {"", ModulePanelParameterType::None},
        {PluginDescriptors::ParameterIds::kBendRange, ModulePanelParameterType::Slider},
        {"", ModulePanelParameterType::None},
        {PluginDescriptors::ParameterIds::kUnisonEnable, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kVolumeInvertEnable, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kBankLockEnable, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kMemoryProtectEnable, ModulePanelParameterType::ComboBox}
    };
    
    return config;
}

MiscPanel::MiscPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : BaseModulePanel(theme, widgetFactory, apvts, createConfig(), getWidth(), getHeight())
{
}
