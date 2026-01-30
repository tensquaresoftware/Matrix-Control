#include "FmTrackPanel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/ModuleHeaderPanel.h"
#include "GUI/Panels/Reusable/ParameterPanel.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

FmTrackPanel::FmTrackPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
    , apvts_(apvts)
{
    setOpaque(true);
    moduleHeaderPanel_ = std::make_unique<ModuleHeaderPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ModuleIds::kFmTrack,
        ModuleHeaderPanel::ButtonSet::InitOnly,
        ModuleHeaderPanel::ModuleType::PatchEdit,
        apvts_,
        PluginDescriptors::StandaloneWidgetIds::kFmTrackInit);
    addAndMakeVisible(*moduleHeaderPanel_);

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kFmAmount,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kFmModByEnv3,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kFmModByPressure,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kTrackPoint1,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kTrackPoint2,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kTrackPoint3,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kTrackPoint4,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kTrackPoint5,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kTrackInput,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        juce::String(),
        ParameterPanel::ParameterType::None,
        ParameterPanel::ModuleType::PatchEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    setSize(getWidth(), getHeight());
}

FmTrackPanel::~FmTrackPanel() = default;

void FmTrackPanel::resized()
{
    auto bounds = getLocalBounds();

    if (auto* header = moduleHeaderPanel_.get())
        header->setBounds(bounds.removeFromTop(header->getHeight()));

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setBounds(bounds.removeFromTop(paramPanel->getTotalHeight()));
}

void FmTrackPanel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = moduleHeaderPanel_.get())
        header->setTheme(theme);

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setTheme(theme);

    repaint();
}
