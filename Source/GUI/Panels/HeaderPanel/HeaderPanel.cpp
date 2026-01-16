#include "HeaderPanel.h"

#include "../../Widgets/Button.h"
#include "../../Themes/Theme.h"
#include "../../../Shared/PluginDimensions.h"

using tss::Theme;

HeaderPanel::HeaderPanel(Theme& theme)
    : theme_(&theme)
    , blackThemeButton_(theme, PluginDimensions::Widgets::Widths::Button::kHeaderPanelTheme, PluginDimensions::Widgets::Heights::kButton, "BLACK")
    , creamThemeButton_(theme, PluginDimensions::Widgets::Widths::Button::kHeaderPanelTheme, PluginDimensions::Widgets::Heights::kButton, "CREAM")
    , debugThemeButton_(theme, PluginDimensions::Widgets::Widths::Button::kHeaderPanelTheme, PluginDimensions::Widgets::Heights::kButton, "DEBUG")
    , someDisabledButton_(theme, PluginDimensions::Widgets::Widths::Button::kHeaderPanelTheme, PluginDimensions::Widgets::Heights::kButton, "DISABLED")
{
    addAndMakeVisible(blackThemeButton_);
    addAndMakeVisible(creamThemeButton_);
    addAndMakeVisible(debugThemeButton_);
    someDisabledButton_.setEnabled(false);
    addAndMakeVisible(someDisabledButton_);
}

void HeaderPanel::paint(juce::Graphics& g)
{
    g.fillAll(theme_->getHeaderPanelBackgroundColour());
}

void HeaderPanel::resized()
{
    const auto bounds = getLocalBounds();
    const auto buttonSpacing = getButtonSpacing();
    const auto buttonWidth = PluginDimensions::Widgets::Widths::Button::kHeaderPanelTheme;
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;
    const auto buttonY = (getHeight() - buttonHeight) / 2;
    
    const auto blackThemeButtonX = buttonSpacing;
    blackThemeButton_.setBounds(
        bounds.getX() + blackThemeButtonX,
        bounds.getY() + buttonY,
        buttonWidth,
        buttonHeight
    );
    
    const auto creamThemeButtonX = blackThemeButtonX + buttonWidth + buttonSpacing;
    creamThemeButton_.setBounds(
        bounds.getX() + creamThemeButtonX,
        bounds.getY() + buttonY,
        buttonWidth,
        buttonHeight
    );
    
    const auto debugThemeButtonX = creamThemeButtonX + buttonWidth + buttonSpacing;
    debugThemeButton_.setBounds(
        bounds.getX() + debugThemeButtonX,
        bounds.getY() + buttonY,
        buttonWidth,
        buttonHeight
    );
    
    const auto someDisabledButtonX = debugThemeButtonX + buttonWidth + buttonSpacing;
    someDisabledButton_.setBounds(
        bounds.getX() + someDisabledButtonX,
        bounds.getY() + buttonY,
        buttonWidth,
        buttonHeight
    );
}

void HeaderPanel::setTheme(Theme& theme)
{
    theme_ = &theme;
    blackThemeButton_.setTheme(theme);
    creamThemeButton_.setTheme(theme);
    debugThemeButton_.setTheme(theme);
    someDisabledButton_.setTheme(theme);
    repaint();
}

