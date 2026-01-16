#include "Env2Panel.h"

#include "../../../../../../Themes/Theme.h"
#include "../../../../../Reusable/ModuleHeaderPanel.h"
#include "../../../../../Reusable/ParameterPanel.h"
#include "../../../../../../../Shared/PluginDescriptors.h"
#include "../../../../../../../GUI/Factories/WidgetFactory.h"

using tss::Theme;

Env2Panel::Env2Panel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
    , apvts_(apvts)
{
    moduleHeaderPanel_ = std::make_unique<ModuleHeaderPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ModuleIds::kEnvelope2,
        ModuleHeaderPanel::ButtonSet::InitCopyPaste,
        apvts_,
        PluginDescriptors::StandaloneWidgetIds::kEnv2Init,
        PluginDescriptors::StandaloneWidgetIds::kEnv2Copy,
        PluginDescriptors::StandaloneWidgetIds::kEnv2Paste);
    addAndMakeVisible(*moduleHeaderPanel_);

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv2Delay,
        ParameterPanel::ParameterType::Slider,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv2Attack,
        ParameterPanel::ParameterType::Slider,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv2Decay,
        ParameterPanel::ParameterType::Slider,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv2Sustain,
        ParameterPanel::ParameterType::Slider,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv2Release,
        ParameterPanel::ParameterType::Slider,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv2Amplitude,
        ParameterPanel::ParameterType::Slider,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv2AmplitudeModByVelocity,
        ParameterPanel::ParameterType::Slider,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv2TriggerMode,
        ParameterPanel::ParameterType::ComboBox,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv2EnvelopeMode,
        ParameterPanel::ParameterType::ComboBox,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv2Lfo1Trigger,
        ParameterPanel::ParameterType::ComboBox,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    setSize(getWidth(), getHeight());
}

Env2Panel::~Env2Panel() = default;

void Env2Panel::paint(juce::Graphics& g)
{
    g.fillAll(theme_->getPatchEditModulePanelBackgroundColour());
}

void Env2Panel::resized()
{
    auto bounds = getLocalBounds();

    if (auto* header = moduleHeaderPanel_.get())
        header->setBounds(bounds.removeFromTop(header->getHeight()));

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setBounds(bounds.removeFromTop(paramPanel->getTotalHeight()));
}

void Env2Panel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = moduleHeaderPanel_.get())
        header->setTheme(theme);

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setTheme(theme);

    repaint();
}
