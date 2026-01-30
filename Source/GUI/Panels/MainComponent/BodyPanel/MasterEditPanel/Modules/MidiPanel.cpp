#include "MidiPanel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/BaseModulePanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

ModulePanelConfig MidiPanel::createConfig()
{
    ModulePanelConfig config;
    config.moduleId = PluginDescriptors::ModuleIds::kMidi;
    config.buttonSet = ModulePanelButtonSet::InitOnly;
    config.moduleType = ModulePanelModuleType::MasterEdit;
    config.initWidgetId = PluginDescriptors::StandaloneWidgetIds::kMidiInit;
    
    config.parameters = {
        {PluginDescriptors::ParameterIds::kMidiChannel, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kMidiEcho, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kMidiControllers, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kMidiPatchChanges, ModulePanelParameterType::ComboBox},
        {PluginDescriptors::ParameterIds::kMidiPedal1Select, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kMidiPedal2Select, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kMidiLever2Select, ModulePanelParameterType::Slider},
        {PluginDescriptors::ParameterIds::kMidiLever3Select, ModulePanelParameterType::Slider}
    };
    
    return config;
}

MidiPanel::MidiPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : BaseModulePanel(theme, widgetFactory, apvts, createConfig(), getWidth(), getHeight())
{
}
