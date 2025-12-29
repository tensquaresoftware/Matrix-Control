#include "PatchEditPanel.h"

#include "../../../Skin/Skin.h"
#include "../../../Skin/SkinDimensions.h"

using tss::Skin;

PatchEditPanel::PatchEditPanel(Skin& newSkin)
    : skin(&newSkin)
{
    setSize(tss::SkinDimensions::PatchEditPanel::kWidth, tss::SkinDimensions::PatchEditPanel::kHeight);
}

void PatchEditPanel::paint(juce::Graphics& g)
{
        if (skin == nullptr)
        {
            return;
        }
        
        g.fillAll(skin->getPatchEditPanelBackgroundColour());
}

void PatchEditPanel::setSkin(Skin& newSkin)
{
        skin = &newSkin;
    repaint();
}

