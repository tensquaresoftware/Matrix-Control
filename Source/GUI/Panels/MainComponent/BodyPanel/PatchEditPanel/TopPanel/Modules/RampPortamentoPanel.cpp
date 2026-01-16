#include "RampPortamentoPanel.h"

#include "../../../../../../Themes/Theme.h"
#include "../../../../../Reusable/ModuleHeaderPanel.h"
#include "../../../../../Reusable/ParameterPanel.h"
#include "../../../../../../../Shared/PluginDescriptors.h"
#include "../../../../../../../GUI/Factories/WidgetFactory.h"

using tss::Theme;

RampPortamentoPanel::RampPortamentoPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
    , apvts_(apvts)
{
    moduleHeaderPanel_ = std::make_unique<ModuleHeaderPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ModuleIds::kRampPortamento,
        ModuleHeaderPanel::ButtonSet::InitOnly,
        apvts_,
        PluginDescriptors::StandaloneWidgetIds::kRampPortamentoInit);
    addAndMakeVisible(*moduleHeaderPanel_);

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kRamp1Rate,
        ParameterPanel::ParameterType::Slider,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kRamp1Trigger,
        ParameterPanel::ParameterType::ComboBox,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kRamp2Rate,
        ParameterPanel::ParameterType::Slider,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kRamp2Trigger,
        ParameterPanel::ParameterType::ComboBox,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kPortamentoRate,
        ParameterPanel::ParameterType::Slider,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kPortamentoModByVelocity,
        ParameterPanel::ParameterType::Slider,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kPortamentoMode,
        ParameterPanel::ParameterType::ComboBox,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kPortamentoLegato,
        ParameterPanel::ParameterType::ComboBox,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kPortamentoKeyboardMode,
        ParameterPanel::ParameterType::ComboBox,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        juce::String(),
        ParameterPanel::ParameterType::None,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    setSize(getWidth(), getHeight());
}

RampPortamentoPanel::~RampPortamentoPanel() = default;

void RampPortamentoPanel::paint(juce::Graphics& g)
{
    g.fillAll(theme_->getPatchEditModulePanelBackgroundColour());
}

void RampPortamentoPanel::resized()
{
    auto bounds = getLocalBounds();

    if (auto* header = moduleHeaderPanel_.get())
        header->setBounds(bounds.removeFromTop(header->getHeight()));

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setBounds(bounds.removeFromTop(paramPanel->getTotalHeight()));
}

void RampPortamentoPanel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = moduleHeaderPanel_.get())
        header->setTheme(theme);

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setTheme(theme);

    repaint();
}
