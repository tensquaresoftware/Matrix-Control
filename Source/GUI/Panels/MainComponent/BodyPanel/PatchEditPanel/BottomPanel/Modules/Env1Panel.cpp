#include "Env1Panel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/ModuleHeaderPanel.h"
#include "GUI/Panels/Reusable/ParameterPanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

Env1Panel::Env1Panel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
    , apvts_(apvts)
{
    setOpaque(true);
    moduleHeaderPanel_ = std::make_unique<ModuleHeaderPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ModuleIds::kEnvelope1,
        ModuleHeaderPanel::ButtonSet::InitCopyPaste,
        ModuleHeaderPanel::ModuleType::PatchEdit,
        apvts_,
        PluginDescriptors::StandaloneWidgetIds::kEnv1Init,
        PluginDescriptors::StandaloneWidgetIds::kEnv1Copy,
        PluginDescriptors::StandaloneWidgetIds::kEnv1Paste);
    addAndMakeVisible(*moduleHeaderPanel_);

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv1Delay,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv1Attack,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv1Decay,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv1Sustain,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv1Release,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv1Amplitude,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv1AmplitudeModByVelocity,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv1TriggerMode,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv1EnvelopeMode,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kEnv1Lfo1Trigger,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    setSize(getWidth(), getHeight());
}

Env1Panel::~Env1Panel() = default;

void Env1Panel::paint(juce::Graphics& g)
{
    if (theme_ == nullptr)
    {
        return;
    }
    g.fillAll(theme_->getPatchEditModulePanelBackgroundColour());
}

void Env1Panel::resized()
{
    auto bounds = getLocalBounds();

    if (auto* header = moduleHeaderPanel_.get())
        header->setBounds(bounds.removeFromTop(header->getHeight()));

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setBounds(bounds.removeFromTop(paramPanel->getTotalHeight()));
}

void Env1Panel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = moduleHeaderPanel_.get())
        header->setTheme(theme);

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setTheme(theme);

    repaint();
}
