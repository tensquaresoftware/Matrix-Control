#include "MiscPanel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/ModuleHeaderPanel.h"
#include "GUI/Panels/Reusable/ParameterPanel.h"
#include "GUI/Widgets/Slider.h"
#include "Shared/PluginDescriptors.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

MiscPanel::MiscPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
    , apvts_(apvts)
{
    setOpaque(true);
    moduleHeaderPanel_ = std::make_unique<ModuleHeaderPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ModuleIds::kMisc,
        ModuleHeaderPanel::ButtonSet::InitOnly,
        ModuleHeaderPanel::ModuleType::MasterEdit,
        apvts_,
        PluginDescriptors::StandaloneWidgetIds::kMiscInit);
    addAndMakeVisible(*moduleHeaderPanel_);

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kMasterTune,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::MasterEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kMasterTranspose,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::MasterEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());
    if (auto* slider = parameterPanels_.back()->getSlider())
        slider->setUnit("st");

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kBendRange,
        ParameterPanel::ParameterType::Slider,
        ParameterPanel::ModuleType::MasterEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());
    if (auto* slider = parameterPanels_.back()->getSlider())
        slider->setUnit("st");

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kUnisonEnable,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::MasterEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kVolumeInvertEnable,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::MasterEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kBankLockEnable,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::MasterEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    parameterPanels_.push_back(std::make_unique<ParameterPanel>(
        theme,
        widgetFactory,
        PluginDescriptors::ParameterIds::kMemoryProtectEnable,
        ParameterPanel::ParameterType::ComboBox,
        ParameterPanel::ModuleType::MasterEdit,
        apvts_));
    addAndMakeVisible(*parameterPanels_.back());

    setSize(getWidth(), getHeight());
}

MiscPanel::~MiscPanel() = default;

void MiscPanel::resized()
{
    auto bounds = getLocalBounds();

    if (auto* header = moduleHeaderPanel_.get())
        header->setBounds(bounds.removeFromTop(header->getHeight()));

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setBounds(bounds.removeFromTop(paramPanel->getTotalHeight()));
}

void MiscPanel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = moduleHeaderPanel_.get())
        header->setTheme(theme);

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setTheme(theme);

    repaint();
}
