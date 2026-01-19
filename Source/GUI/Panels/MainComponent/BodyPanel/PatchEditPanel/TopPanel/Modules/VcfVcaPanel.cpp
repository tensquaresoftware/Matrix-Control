#include "VcfVcaPanel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/ModuleHeaderPanel.h"
#include "GUI/Panels/Reusable/ParameterPanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

VcfVcaPanel::VcfVcaPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
    , apvts_(apvts)
{
    moduleHeaderPanel_ = std::make_unique<ModuleHeaderPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ModuleIds::kVcfVca,
        ModuleHeaderPanel::ButtonSet::InitOnly,
        ModuleHeaderPanel::ModuleType::PatchEdit,
        apvts_,
        PluginDescriptors::StandaloneWidgetIds::kVcfVcaInit);
    addAndMakeVisible(*moduleHeaderPanel_);

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kVcfBalance,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kVcfFequency,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kVcfFrequencyModByEnv1,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kVcfFrequencyModByPressure,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kVcfResonance,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kVca1Volume,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kVca1ModByVelocity,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kVca2ModByEnv2,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kVcfLevers,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kVcfKeyboardPortamento,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    setSize(getWidth(), getHeight());
}

VcfVcaPanel::~VcfVcaPanel() = default;

void VcfVcaPanel::paint(juce::Graphics& g)
{
    g.fillAll(theme_->getPatchEditModulePanelBackgroundColour());
}

void VcfVcaPanel::resized()
{
    auto bounds = getLocalBounds();

    if (auto* header = moduleHeaderPanel_.get())
        header->setBounds(bounds.removeFromTop(header->getHeight()));

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setBounds(bounds.removeFromTop(paramPanel->getTotalHeight()));
}

void VcfVcaPanel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = moduleHeaderPanel_.get())
        header->setTheme(theme);

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setTheme(theme);

    repaint();
}
