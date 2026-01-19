#include "Dco1Panel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/ModuleHeaderPanel.h"
#include "GUI/Panels/Reusable/ParameterPanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

Dco1Panel::Dco1Panel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
    , apvts_(apvts)
{
    moduleHeaderPanel_ = std::make_unique<ModuleHeaderPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ModuleIds::kDco1,
        ModuleHeaderPanel::ButtonSet::InitCopyPaste,
        ModuleHeaderPanel::ModuleType::PatchEdit,
        apvts_,
        PluginDescriptors::StandaloneWidgetIds::kDco1Init,
        PluginDescriptors::StandaloneWidgetIds::kDco1Copy,
        PluginDescriptors::StandaloneWidgetIds::kDco1Paste);
    addAndMakeVisible(*moduleHeaderPanel_);

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco1Frequency,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco1FrequencyModByLfo1,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco1Sync,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco1PulseWidth,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco1PulseWidthModByLfo2,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco1WaveShape,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco1WaveSelect,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco1Levers,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco1KeyboardPortamento,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kDco1KeyClick,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    setSize(getWidth(), getHeight());
}

Dco1Panel::~Dco1Panel() = default;

void Dco1Panel::paint(juce::Graphics& g)
{
    g.fillAll(theme_->getPatchEditModulePanelBackgroundColour());
}

void Dco1Panel::resized()
{
    auto bounds = getLocalBounds();

    if (auto* header = moduleHeaderPanel_.get())
        header->setBounds(bounds.removeFromTop(header->getHeight()));

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setBounds(bounds.removeFromTop(paramPanel->getTotalHeight()));
}

void Dco1Panel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = moduleHeaderPanel_.get())
        header->setTheme(theme);

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setTheme(theme);

    repaint();
}

