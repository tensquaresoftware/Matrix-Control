#include "BankUtilityPanel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Widgets/ModuleHeader.h"
#include "GUI/Widgets/Label.h"
#include "GUI/Widgets/Button.h"
#include "Shared/PluginDescriptors.h"
#include "Shared/PluginDimensions.h"
#include "GUI/Factories/WidgetFactory.h"
#include <juce_core/juce_core.h>

using tss::Theme;

BankUtilityPanel::BankUtilityPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
    , apvts_(apvts)
{
    setupModuleHeader(theme, widgetFactory, PluginDescriptors::ModuleIds::kBankUtility);
    setupBankSelectorLabel(theme);
    setupSelectBankButtons(theme, widgetFactory);

    setSize(getWidth(), getHeight());
}

BankUtilityPanel::~BankUtilityPanel() = default;

void BankUtilityPanel::paint(juce::Graphics& g)
{
    if (auto* currentTheme = theme_)
        g.fillAll(currentTheme->getPatchManagerPanelBackgroundColour());
}

void BankUtilityPanel::resized()
{
    const auto moduleHeaderHeight = PluginDimensions::Widgets::Heights::kModuleHeader;
    const auto moduleHeaderWidth = PluginDimensions::Widgets::Widths::ModuleHeader::kPatchManagerModule;
    const auto labelWidth = PluginDimensions::Widgets::Widths::Label::kPatchManagerBankSelector;
    const auto labelHeight = PluginDimensions::Widgets::Heights::kLabel;
    const auto buttonWidth = PluginDimensions::Widgets::Widths::Button::kPatchManagerBankSelect;
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;

    int y = 0;

    if (auto* header = bankUtilityModuleHeader_.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    y = moduleHeaderHeight + kTopPadding_;

    if (auto* label = bankSelectorLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    int x = labelWidth + kSpacing_;

    if (auto* button = selectBank0Button_.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    x += buttonWidth + kSpacing_;

    if (auto* button = selectBank1Button_.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    x += buttonWidth + kSpacing_;

    if (auto* button = selectBank2Button_.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    x += buttonWidth + kSpacing_;

    if (auto* button = selectBank3Button_.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    x += buttonWidth + kSpacing_;

    if (auto* button = selectBank4Button_.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    y = moduleHeaderHeight + kTopPadding_ + buttonHeight + kSpacing_;
    x = 0;

    const auto unlockButtonWidth = PluginDimensions::Widgets::Widths::Button::kPatchManagerUnlockBank;

    if (auto* button = unlockBankButton_.get())
        button->setBounds(x, y, unlockButtonWidth, buttonHeight);

    x += unlockButtonWidth + kSpacing_;

    if (auto* button = selectBank5Button_.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    x += buttonWidth + kSpacing_;

    if (auto* button = selectBank6Button_.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    x += buttonWidth + kSpacing_;

    if (auto* button = selectBank7Button_.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    x += buttonWidth + kSpacing_;

    if (auto* button = selectBank8Button_.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    x += buttonWidth + kSpacing_;

    if (auto* button = selectBank9Button_.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);
}

void BankUtilityPanel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = bankUtilityModuleHeader_.get())
        header->setTheme(theme);

    if (auto* label = bankSelectorLabel_.get())
        label->setTheme(theme);

    if (auto* button = selectBank0Button_.get())
        button->setTheme(theme);

    if (auto* button = selectBank1Button_.get())
        button->setTheme(theme);

    if (auto* button = selectBank2Button_.get())
        button->setTheme(theme);

    if (auto* button = selectBank3Button_.get())
        button->setTheme(theme);

    if (auto* button = selectBank4Button_.get())
        button->setTheme(theme);

    if (auto* button = unlockBankButton_.get())
        button->setTheme(theme);

    if (auto* button = selectBank5Button_.get())
        button->setTheme(theme);

    if (auto* button = selectBank6Button_.get())
        button->setTheme(theme);

    if (auto* button = selectBank7Button_.get())
        button->setTheme(theme);

    if (auto* button = selectBank8Button_.get())
        button->setTheme(theme);

    if (auto* button = selectBank9Button_.get())
        button->setTheme(theme);

    repaint();
}

void BankUtilityPanel::setupModuleHeader(Theme& theme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    bankUtilityModuleHeader_ = std::make_unique<tss::ModuleHeader>(
        theme, 
        widgetFactory.getGroupDisplayName(moduleId),
        PluginDimensions::Widgets::Widths::ModuleHeader::kPatchManagerModule,
        PluginDimensions::Widgets::Heights::kModuleHeader,
        tss::ModuleHeader::ColourVariant::Blue);
    addAndMakeVisible(*bankUtilityModuleHeader_);
}

void BankUtilityPanel::setupBankSelectorLabel(Theme& theme)
{
    bankSelectorLabel_ = std::make_unique<tss::Label>(
        theme,
        PluginDimensions::Widgets::Widths::Label::kPatchManagerBankSelector,
        PluginDimensions::Widgets::Heights::kLabel,
        PluginDescriptors::StandaloneWidgetDisplayNames::kBankSelector);
    addAndMakeVisible(*bankSelectorLabel_);
}

void BankUtilityPanel::setupSelectBankButtons(Theme& theme, WidgetFactory& widgetFactory)
{
    selectBank0Button_ = std::make_unique<tss::Button>(
        theme,
        PluginDimensions::Widgets::Widths::Button::kPatchManagerBankSelect,
        PluginDimensions::Widgets::Heights::kButton,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank0));
    selectBank0Button_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kSelectBank0,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*selectBank0Button_);

    selectBank1Button_ = std::make_unique<tss::Button>(
        theme,
        PluginDimensions::Widgets::Widths::Button::kPatchManagerBankSelect,
        PluginDimensions::Widgets::Heights::kButton,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank1));
    selectBank1Button_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kSelectBank1,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*selectBank1Button_);

    selectBank2Button_ = std::make_unique<tss::Button>(
        theme,
        PluginDimensions::Widgets::Widths::Button::kPatchManagerBankSelect,
        PluginDimensions::Widgets::Heights::kButton,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank2));
    selectBank2Button_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kSelectBank2,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*selectBank2Button_);

    selectBank3Button_ = std::make_unique<tss::Button>(
        theme,
        PluginDimensions::Widgets::Widths::Button::kPatchManagerBankSelect,
        PluginDimensions::Widgets::Heights::kButton,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank3));
    selectBank3Button_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kSelectBank3,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*selectBank3Button_);

    selectBank4Button_ = std::make_unique<tss::Button>(
        theme,
        PluginDimensions::Widgets::Widths::Button::kPatchManagerBankSelect,
        PluginDimensions::Widgets::Heights::kButton,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank4));
    selectBank4Button_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kSelectBank4,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*selectBank4Button_);

    unlockBankButton_ = std::make_unique<tss::Button>(
        theme,
        PluginDimensions::Widgets::Widths::Button::kPatchManagerUnlockBank,
        PluginDimensions::Widgets::Heights::kButton,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kUnlockBank));
    unlockBankButton_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kUnlockBank,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*unlockBankButton_);

    selectBank5Button_ = std::make_unique<tss::Button>(
        theme,
        PluginDimensions::Widgets::Widths::Button::kPatchManagerBankSelect,
        PluginDimensions::Widgets::Heights::kButton,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank5));
    selectBank5Button_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kSelectBank5,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*selectBank5Button_);

    selectBank6Button_ = std::make_unique<tss::Button>(
        theme,
        PluginDimensions::Widgets::Widths::Button::kPatchManagerBankSelect,
        PluginDimensions::Widgets::Heights::kButton,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank6));
    selectBank6Button_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kSelectBank6,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*selectBank6Button_);

    selectBank7Button_ = std::make_unique<tss::Button>(
        theme,
        PluginDimensions::Widgets::Widths::Button::kPatchManagerBankSelect,
        PluginDimensions::Widgets::Heights::kButton,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank7));
    selectBank7Button_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kSelectBank7,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*selectBank7Button_);

    selectBank8Button_ = std::make_unique<tss::Button>(
        theme,
        PluginDimensions::Widgets::Widths::Button::kPatchManagerBankSelect,
        PluginDimensions::Widgets::Heights::kButton,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank8));
    selectBank8Button_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kSelectBank8,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*selectBank8Button_);

    selectBank9Button_ = std::make_unique<tss::Button>(
        theme,
        PluginDimensions::Widgets::Widths::Button::kPatchManagerBankSelect,
        PluginDimensions::Widgets::Heights::kButton,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank9));
    selectBank9Button_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kSelectBank9,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*selectBank9Button_);
}
