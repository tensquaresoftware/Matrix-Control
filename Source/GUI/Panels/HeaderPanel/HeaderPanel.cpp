#include "HeaderPanel.h"

#include "../../Widgets/Button.h"
#include "../../Themes/Theme.h"

using tss::Theme;

HeaderPanel::HeaderPanel(Theme& newTheme)
    : theme(&newTheme)
    , buttonBlack(newTheme, getButtonWidth(), "BLACK")
    , buttonCream(newTheme, getButtonWidth(), "CREAM")
    , buttonDebug(newTheme, getButtonWidth(), "DEBUG")
    , buttonDisabled(newTheme, getButtonWidth(), "DISABLED")
{
    addAndMakeVisible(buttonBlack);
    addAndMakeVisible(buttonCream);
    addAndMakeVisible(buttonDebug);
    buttonDisabled.setEnabled(false);
    addAndMakeVisible(buttonDisabled);
}

void HeaderPanel::paint(juce::Graphics& g)
{
        g.fillAll(theme->getHeaderPanelBackgroundColour());
}

void HeaderPanel::resized()
{
    auto buttonSpacing = getButtonSpacing();
    auto buttonWidth = getButtonWidth();
    auto buttonHeight = getButtonHeight();
    auto currentX = buttonSpacing;
    auto buttonY = (getHeight() - buttonHeight) / 2;
    auto buttonStep = buttonWidth + buttonSpacing;
    
    buttonBlack.setBounds(currentX, buttonY, buttonWidth, buttonHeight);
    currentX += buttonStep;
    
    buttonCream.setBounds(currentX, buttonY, buttonWidth, buttonHeight);
    currentX += buttonStep;
    
    buttonDebug.setBounds(currentX, buttonY, buttonWidth, buttonHeight);
    currentX += buttonStep;
    
    buttonDisabled.setBounds(currentX, buttonY, buttonWidth, buttonHeight);
}

void HeaderPanel::setTheme(Theme& newTheme)
{
        theme = &newTheme;
    buttonBlack.setTheme(newTheme);
    buttonCream.setTheme(newTheme);
    buttonDebug.setTheme(newTheme);
    buttonDisabled.setTheme(newTheme);
    repaint();
}

