#include "BankUtilityPanel.h"

#include "../../../../Themes/Theme.h"
#include "../../../../Widgets/ModuleHeader.h"
#include "../../../../Widgets/Label.h"
#include "../../../../Widgets/Button.h"
#include "../../../../../Shared/PluginDescriptors.h"
#include "../../../../Factories/WidgetFactory.h"

using tss::Theme;

BankUtilityPanel::BankUtilityPanel(Theme& inTheme, WidgetFactory& widgetFactory)
    : theme(&inTheme)
{
    setupModuleHeader(inTheme, widgetFactory, PluginDescriptors::ModuleIds::kBankUtility);
    setupBankSelectorLabel(inTheme);
    setupSelectBankButtons(inTheme, widgetFactory);

    setSize(getWidth(), getHeight());
}

BankUtilityPanel::~BankUtilityPanel() = default;

void BankUtilityPanel::paint(juce::Graphics& g)
{
    if (auto* currentTheme = theme)
        g.fillAll(currentTheme->getPatchManagerPanelBackgroundColour());
}

void BankUtilityPanel::resized()
{
    const auto moduleHeaderHeight = tss::ModuleHeader::getHeight();
    const auto moduleHeaderWidth = tss::ModuleHeader::getWidth(tss::ModuleHeader::ModuleWidth::PatchManager);
    const auto labelWidth = tss::Label::getWidth(tss::Label::LabelWidth::PatchManagerBankSelector);
    const auto labelHeight = tss::Label::getHeight();
    const auto buttonWidth = tss::Button::getWidth(tss::Button::ButtonWidth::PatchManagerBankSelect);
    const auto buttonHeight = tss::Button::getHeight();

    int y = 0;

    if (auto* header = bankUtilityModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    y = moduleHeaderHeight + kTopPadding;

    if (auto* label = bankSelectorLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    int x = labelWidth + kSpacing;

    if (auto* button = selectBank0Button.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    x += buttonWidth + kSpacing;

    if (auto* button = selectBank1Button.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    x += buttonWidth + kSpacing;

    if (auto* button = selectBank2Button.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    x += buttonWidth + kSpacing;

    if (auto* button = selectBank3Button.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    x += buttonWidth + kSpacing;

    if (auto* button = selectBank4Button.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    y = moduleHeaderHeight + kTopPadding + buttonHeight + kSpacing;
    x = 0;

    const auto unlockButtonWidth = tss::Button::getWidth(tss::Button::ButtonWidth::PatchManagerUnlock);

    if (auto* button = unlockBankButton.get())
        button->setBounds(x, y, unlockButtonWidth, buttonHeight);

    x += unlockButtonWidth + kSpacing;

    if (auto* button = selectBank5Button.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    x += buttonWidth + kSpacing;

    if (auto* button = selectBank6Button.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    x += buttonWidth + kSpacing;

    if (auto* button = selectBank7Button.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    x += buttonWidth + kSpacing;

    if (auto* button = selectBank8Button.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);

    x += buttonWidth + kSpacing;

    if (auto* button = selectBank9Button.get())
        button->setBounds(x, y, buttonWidth, buttonHeight);
}

void BankUtilityPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = bankUtilityModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* label = bankSelectorLabel.get())
        label->setTheme(inTheme);

    if (auto* button = selectBank0Button.get())
        button->setTheme(inTheme);

    if (auto* button = selectBank1Button.get())
        button->setTheme(inTheme);

    if (auto* button = selectBank2Button.get())
        button->setTheme(inTheme);

    if (auto* button = selectBank3Button.get())
        button->setTheme(inTheme);

    if (auto* button = selectBank4Button.get())
        button->setTheme(inTheme);

    if (auto* button = unlockBankButton.get())
        button->setTheme(inTheme);

    if (auto* button = selectBank5Button.get())
        button->setTheme(inTheme);

    if (auto* button = selectBank6Button.get())
        button->setTheme(inTheme);

    if (auto* button = selectBank7Button.get())
        button->setTheme(inTheme);

    if (auto* button = selectBank8Button.get())
        button->setTheme(inTheme);

    if (auto* button = selectBank9Button.get())
        button->setTheme(inTheme);

    repaint();
}

void BankUtilityPanel::setupModuleHeader(Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    bankUtilityModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme,
        widgetFactory.getGroupDisplayName(moduleId),
        tss::ModuleHeader::ModuleWidth::PatchManager,
        tss::ModuleHeader::ColourVariant::Blue);
    addAndMakeVisible(*bankUtilityModuleHeader);
}

void BankUtilityPanel::setupBankSelectorLabel(Theme& inTheme)
{
    bankSelectorLabel = std::make_unique<tss::Label>(
        inTheme,
        tss::Label::LabelWidth::PatchManagerBankSelector,
        PluginDescriptors::StandaloneWidgetDisplayNames::kBankSelector);
    addAndMakeVisible(*bankSelectorLabel);
}

void BankUtilityPanel::setupSelectBankButtons(Theme& inTheme, WidgetFactory& widgetFactory)
{
    selectBank0Button = std::make_unique<tss::Button>(
        inTheme,
        tss::Button::ButtonWidth::PatchManagerBankSelect,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank0));
    addAndMakeVisible(*selectBank0Button);

    selectBank1Button = std::make_unique<tss::Button>(
        inTheme,
        tss::Button::ButtonWidth::PatchManagerBankSelect,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank1));
    addAndMakeVisible(*selectBank1Button);

    selectBank2Button = std::make_unique<tss::Button>(
        inTheme,
        tss::Button::ButtonWidth::PatchManagerBankSelect,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank2));
    addAndMakeVisible(*selectBank2Button);

    selectBank3Button = std::make_unique<tss::Button>(
        inTheme,
        tss::Button::ButtonWidth::PatchManagerBankSelect,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank3));
    addAndMakeVisible(*selectBank3Button);

    selectBank4Button = std::make_unique<tss::Button>(
        inTheme,
        tss::Button::ButtonWidth::PatchManagerBankSelect,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank4));
    addAndMakeVisible(*selectBank4Button);

    unlockBankButton = std::make_unique<tss::Button>(
        inTheme,
        tss::Button::ButtonWidth::PatchManagerUnlock,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kUnlockBank));
    addAndMakeVisible(*unlockBankButton);

    selectBank5Button = std::make_unique<tss::Button>(
        inTheme,
        tss::Button::ButtonWidth::PatchManagerBankSelect,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank5));
    addAndMakeVisible(*selectBank5Button);

    selectBank6Button = std::make_unique<tss::Button>(
        inTheme,
        tss::Button::ButtonWidth::PatchManagerBankSelect,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank6));
    addAndMakeVisible(*selectBank6Button);

    selectBank7Button = std::make_unique<tss::Button>(
        inTheme,
        tss::Button::ButtonWidth::PatchManagerBankSelect,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank7));
    addAndMakeVisible(*selectBank7Button);

    selectBank8Button = std::make_unique<tss::Button>(
        inTheme,
        tss::Button::ButtonWidth::PatchManagerBankSelect,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank8));
    addAndMakeVisible(*selectBank8Button);

    selectBank9Button = std::make_unique<tss::Button>(
        inTheme,
        tss::Button::ButtonWidth::PatchManagerBankSelect,
        widgetFactory.getStandaloneWidgetDisplayName(PluginDescriptors::StandaloneWidgetIds::kSelectBank9));
    addAndMakeVisible(*selectBank9Button);
}
