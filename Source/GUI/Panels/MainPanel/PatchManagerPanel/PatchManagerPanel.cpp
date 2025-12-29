#include "PatchManagerPanel.h"

#include "../../../Skin/Skin.h"

using tss::Skin;

PatchManagerPanel::PatchManagerPanel(Skin& newSkin)
    : skin(&newSkin)
{
    setSize(275, 415);
}

void PatchManagerPanel::paint(juce::Graphics& g)
{
        if (skin == nullptr)
        {
            return;
        }
        
        g.fillAll(skin->getPatchManagerPanelBackgroundColour());
}

void PatchManagerPanel::setSkin(Skin& newSkin)
{
        skin = &newSkin;
    repaint();
}

