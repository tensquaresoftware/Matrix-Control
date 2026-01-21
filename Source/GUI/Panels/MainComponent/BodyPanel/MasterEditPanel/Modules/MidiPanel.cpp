#include "MidiPanel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/ModuleHeaderPanel.h"
#include "GUI/Panels/Reusable/ParameterPanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

MidiPanel::MidiPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
    , apvts_(apvts)
{
    setOpaque(true);
    moduleHeaderPanel_ = std::make_unique<ModuleHeaderPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ModuleIds::kMidi,
        ModuleHeaderPanel::ButtonSet::InitOnly,
        ModuleHeaderPanel::ModuleType::MasterEdit,
        apvts_,
        PluginDescriptors::StandaloneWidgetIds::kMidiInit);
    addAndMakeVisible(*moduleHeaderPanel_);

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kMidiChannel,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::MasterEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kMidiEcho,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::MasterEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kMidiControllers,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::MasterEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kMidiPatchChanges,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::MasterEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kMidiPedal1Select,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::MasterEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kMidiPedal2Select,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::MasterEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kMidiLever2Select,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::MasterEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kMidiLever3Select,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::MasterEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    setSize(getWidth(), getHeight());
}

MidiPanel::~MidiPanel() = default;

void MidiPanel::paint(juce::Graphics& g)
{
    g.fillAll(theme_->getMasterEditPanelBackgroundColour());
}

void MidiPanel::resized()
{
    auto bounds = getLocalBounds();

    if (auto* header = moduleHeaderPanel_.get())
        header->setBounds(bounds.removeFromTop(header->getHeight()));

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setBounds(bounds.removeFromTop(paramPanel->getTotalHeight()));
}

void MidiPanel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = moduleHeaderPanel_.get())
        header->setTheme(theme);

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setTheme(theme);

    repaint();
}
