#include "HeaderPanel.h"

#include "../../Widgets/Button.h"
#include "../../Themes/Theme.h"

using tss::Theme;

HeaderPanel::HeaderPanel(Theme& inTheme)
    : theme(&inTheme)
    , blackThemeButton(inTheme, tss::Button::ButtonWidth::HeaderPanelTheme, "BLACK")
    , creamThemeButton(inTheme, tss::Button::ButtonWidth::HeaderPanelTheme, "CREAM")
    , debugThemeButton(inTheme, tss::Button::ButtonWidth::HeaderPanelTheme, "DEBUG")
    , someDisabledButton(inTheme, tss::Button::ButtonWidth::HeaderPanelTheme, "DISABLED")
{
    addAndMakeVisible(blackThemeButton);
    addAndMakeVisible(creamThemeButton);
    addAndMakeVisible(debugThemeButton);
    someDisabledButton.setEnabled(false);
    addAndMakeVisible(someDisabledButton);
}

void HeaderPanel::paint(juce::Graphics& g)
{
    g.fillAll(theme->getHeaderPanelBackgroundColour());
}

void HeaderPanel::resized()
{
    const auto bounds = getLocalBounds();
    const auto buttonSpacing = getButtonSpacing();
    const auto buttonWidth = tss::Button::getWidth(tss::Button::ButtonWidth::HeaderPanelTheme);
    const auto buttonHeight = tss::Button::getHeight();
    const auto buttonY = (getHeight() - buttonHeight) / 2;
    
    const auto blackThemeButtonX = buttonSpacing;
    blackThemeButton.setBounds(
        bounds.getX() + blackThemeButtonX,
        bounds.getY() + buttonY,
        buttonWidth,
        buttonHeight
    );
    
    const auto creamThemeButtonX = blackThemeButtonX + buttonWidth + buttonSpacing;
    creamThemeButton.setBounds(
        bounds.getX() + creamThemeButtonX,
        bounds.getY() + buttonY,
        buttonWidth,
        buttonHeight
    );
    
    const auto debugThemeButtonX = creamThemeButtonX + buttonWidth + buttonSpacing;
    debugThemeButton.setBounds(
        bounds.getX() + debugThemeButtonX,
        bounds.getY() + buttonY,
        buttonWidth,
        buttonHeight
    );
    
    const auto someDisabledButtonX = debugThemeButtonX + buttonWidth + buttonSpacing;
    someDisabledButton.setBounds(
        bounds.getX() + someDisabledButtonX,
        bounds.getY() + buttonY,
        buttonWidth,
        buttonHeight
    );
}

void HeaderPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;
    blackThemeButton.setTheme(inTheme);
    creamThemeButton.setTheme(inTheme);
    debugThemeButton.setTheme(inTheme);
    someDisabledButton.setTheme(inTheme);
    repaint();
}

