#include "Dco2Panel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/ModuleHeaderPanel.h"
#include "GUI/Panels/Reusable/ParameterPanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

Dco2Panel::Dco2Panel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
    , apvts_(apvts)
{
    setOpaque(true);
    moduleHeaderPanel_ = std::make_unique<ModuleHeaderPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ModuleIds::kDco2,
        ModuleHeaderPanel::ButtonSet::InitCopyPaste,
        ModuleHeaderPanel::ModuleType::PatchEdit,
        apvts_,
        PluginDescriptors::StandaloneWidgetIds::kDco2Init,
        PluginDescriptors::StandaloneWidgetIds::kDco2Copy,
        PluginDescriptors::StandaloneWidgetIds::kDco2Paste);
    addAndMakeVisible(*moduleHeaderPanel_);

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco2Frequency,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco2FrequencyModByLfo1,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco2Detune,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco2PulseWidth,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco2PulseWidthModByLfo2,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco2WaveShape,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco2WaveSelect,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco2Levers,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco2KeyboardPortamento,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco2KeyClick,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    setSize(getWidth(), getHeight());
}

Dco2Panel::~Dco2Panel() = default;

void Dco2Panel::paint(juce::Graphics& g)
{
    if (theme_ == nullptr)
    {
        return;
    }
    g.fillAll(theme_->getPatchEditModulePanelBackgroundColour());
}

void Dco2Panel::resized()
{
    auto bounds = getLocalBounds();

    if (auto* header = moduleHeaderPanel_.get())
        header->setBounds(bounds.removeFromTop(header->getHeight()));

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setBounds(bounds.removeFromTop(paramPanel->getTotalHeight()));
}

void Dco2Panel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = moduleHeaderPanel_.get())
        header->setTheme(theme);

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setTheme(theme);

    repaint();
}
