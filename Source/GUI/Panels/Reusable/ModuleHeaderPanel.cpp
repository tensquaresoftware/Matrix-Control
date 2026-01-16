#include "ModuleHeaderPanel.h"

#include "../../Themes/Theme.h"
#include "../../Widgets/ModuleHeader.h"
#include "../../Widgets/Button.h"
#include "../../../Shared/PluginDimensions.h"
#include "../../Factories/WidgetFactory.h"

using tss::Theme;

ModuleHeaderPanel::~ModuleHeaderPanel() = default;

ModuleHeaderPanel::ModuleHeaderPanel(Theme& theme,
                                     WidgetFactory& factory,
                                     const juce::String& moduleId,
                                     ButtonSet buttonSet,
                                     juce::AudioProcessorValueTreeState& apvts,
                                     const juce::String& initWidgetId,
                                     const juce::String& copyWidgetId,
                                     const juce::String& pasteWidgetId)
    : buttonSet_(buttonSet)
    , apvts_(apvts)
{
    createModuleHeader(theme, factory, moduleId);
    createInitButton(theme, factory, initWidgetId);

    if (buttonSet == ButtonSet::InitCopyPaste)
        createCopyPasteButtons(theme, factory, copyWidgetId, pasteWidgetId);
}

void ModuleHeaderPanel::createModuleHeader(Theme& theme, WidgetFactory& factory, const juce::String& moduleId)
{
    moduleHeader_ = std::make_unique<tss::ModuleHeader>(
        theme,
        factory.getGroupDisplayName(moduleId),
        PluginDimensions::Widgets::Widths::ModuleHeader::kPatchEditModule,
        PluginDimensions::Widgets::Heights::kModuleHeader);
    addAndMakeVisible(*moduleHeader_);
}

void ModuleHeaderPanel::createInitButton(Theme& theme, WidgetFactory& factory, const juce::String& initWidgetId)
{
    initButton_ = factory.createStandaloneButton(initWidgetId, theme, PluginDimensions::Widgets::Heights::kButton);
    initButton_->onClick = [this, initWidgetId]
    {
        apvts_.state.setProperty(initWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*initButton_);
}

void ModuleHeaderPanel::createCopyPasteButtons(Theme& theme, WidgetFactory& factory, const juce::String& copyWidgetId, const juce::String& pasteWidgetId)
{
    copyButton_ = factory.createStandaloneButton(copyWidgetId, theme, PluginDimensions::Widgets::Heights::kButton);
    copyButton_->onClick = [this, copyWidgetId]
    {
        apvts_.state.setProperty(copyWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*copyButton_);

    pasteButton_ = factory.createStandaloneButton(pasteWidgetId, theme, PluginDimensions::Widgets::Heights::kButton);
    pasteButton_->onClick = [this, pasteWidgetId]
    {
        apvts_.state.setProperty(pasteWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*pasteButton_);
}

void ModuleHeaderPanel::resized()
{
    layoutModuleHeader();

    if (buttonSet_ == ButtonSet::InitCopyPaste)
        layoutInitCopyPasteButtons();
    else
        layoutInitOnlyButtons();
}

void ModuleHeaderPanel::layoutModuleHeader()
{
    const auto moduleHeaderWidth = PluginDimensions::Widgets::Widths::ModuleHeader::kPatchEditModule;
    const auto moduleHeaderHeight = PluginDimensions::Widgets::Heights::kModuleHeader;
    const int y = 0;

    if (auto* header = moduleHeader_.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);
}

void ModuleHeaderPanel::layoutInitOnlyButtons()
{
    const auto initButtonWidth = PluginDimensions::Widgets::Widths::Button::kInit;
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;
    const auto panelWidth = getWidth();
    const int y = 0;

    if (auto* button = initButton_.get())
        button->setBounds(panelWidth - initButtonWidth, y, initButtonWidth, buttonHeight);
}

void ModuleHeaderPanel::layoutInitCopyPasteButtons()
{
    const auto initButtonWidth = PluginDimensions::Widgets::Widths::Button::kInit;
    const auto copyButtonWidth = PluginDimensions::Widgets::Widths::Button::kCopy;
    const auto pasteButtonWidth = PluginDimensions::Widgets::Widths::Button::kPaste;
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;
    const auto panelWidth = getWidth();
    const int y = 0;

    if (auto* button = pasteButton_.get())
        button->setBounds(panelWidth - pasteButtonWidth, y, pasteButtonWidth, buttonHeight);

    if (auto* button = copyButton_.get())
        button->setBounds(panelWidth - pasteButtonWidth - copyButtonWidth, y, copyButtonWidth, buttonHeight);

    if (auto* button = initButton_.get())
        button->setBounds(panelWidth - pasteButtonWidth - copyButtonWidth - initButtonWidth, y, initButtonWidth, buttonHeight);
}

void ModuleHeaderPanel::setTheme(Theme& theme)
{
    if (auto* header = moduleHeader_.get())
        header->setTheme(theme);

    if (auto* button = initButton_.get())
        button->setTheme(theme);

    if (auto* button = copyButton_.get())
        button->setTheme(theme);

    if (auto* button = pasteButton_.get())
        button->setTheme(theme);

    repaint();
}
