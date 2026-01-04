#include "HeaderPanel.h"

#include "../../Widgets/Button.h"
#include "../../Themes/Theme.h"

using tss::Theme;
using tss::Button;

HeaderPanel::HeaderPanel(Theme& inTheme)
    : theme(&inTheme)
    , buttonBlack(inTheme, getButtonWidth(), "BLACK")
    , buttonCream(inTheme, getButtonWidth(), "CREAM")
    , buttonDebug(inTheme, getButtonWidth(), "DEBUG")
    , buttonDisabled(inTheme, getButtonWidth(), "DISABLED")
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

void HeaderPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;
    buttonBlack.setTheme(inTheme);
    buttonCream.setTheme(inTheme);
    buttonDebug.setTheme(inTheme);
    buttonDisabled.setTheme(inTheme);
    repaint();
}

