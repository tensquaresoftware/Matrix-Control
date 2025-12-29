#include "HeaderPanel.h"

#include "../../Widgets/Button.h"
#include "../../Skin/Skin.h"
#include "../../Skin/SkinDimensions.h"

using tss::Skin;

HeaderPanel::HeaderPanel(Skin& newSkin)
    : skin(&newSkin)
    , buttonBlack(newSkin, tss::SkinDimensions::HeaderPanel::kButtonWidth, "BLACK")
    , buttonCream(newSkin, tss::SkinDimensions::HeaderPanel::kButtonWidth, "CREAM")
    , buttonDebug(newSkin, tss::SkinDimensions::HeaderPanel::kButtonWidth, "DEBUG")
    , buttonDisabled(newSkin, tss::SkinDimensions::HeaderPanel::kButtonWidth, "DISABLED")
{
    addAndMakeVisible(buttonBlack);
    addAndMakeVisible(buttonCream);
    addAndMakeVisible(buttonDebug);
    buttonDisabled.setEnabled(false);
    addAndMakeVisible(buttonDisabled);
}

void HeaderPanel::paint(juce::Graphics& g)
{
        g.fillAll(skin->getHeaderPanelBackgroundColour());
}

void HeaderPanel::resized()
{
    auto buttonSpacing = tss::SkinDimensions::HeaderPanel::kButtonSpacing;
    auto buttonWidth = tss::SkinDimensions::HeaderPanel::kButtonWidth;
    auto buttonHeight = tss::SkinDimensions::HeaderPanel::kButtonHeight;
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

void HeaderPanel::setSkin(Skin& newSkin)
{
        skin = &newSkin;
    buttonBlack.setSkin(newSkin);
    buttonCream.setSkin(newSkin);
    buttonDebug.setSkin(newSkin);
    buttonDisabled.setSkin(newSkin);
    repaint();
}

