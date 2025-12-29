#include "FooterPanel.h"

#include "../../Skin/Skin.h"

using tss::Skin;

FooterPanel::FooterPanel(Skin& newSkin)
    : skin(&newSkin)
{
}

void FooterPanel::paint(juce::Graphics& g)
{
        g.fillAll(skin->getFooterPanelBackgroundColour());
}

void FooterPanel::resized()
{
}

void FooterPanel::setSkin(Skin& newSkin)
{
        skin = &newSkin;
    repaint();
}

