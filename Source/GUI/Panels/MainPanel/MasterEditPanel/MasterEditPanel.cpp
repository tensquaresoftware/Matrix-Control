#include "MasterEditPanel.h"

#include "../../../Skin/Skin.h"
#include "../../../Skin/SkinDimensions.h"

using tss::Skin;

MasterEditPanel::MasterEditPanel(Skin& newSkin)
    : skin(&newSkin)
{
    setSize(tss::SkinDimensions::MasterEditPanel::kWidth, tss::SkinDimensions::MasterEditPanel::kHeight);
}

void MasterEditPanel::paint(juce::Graphics& g)
{
        if (skin == nullptr)
        {
            return;
        }
        
        g.fillAll(skin->getMasterEditPanelBackgroundColour());
}

void MasterEditPanel::setSkin(Skin& newSkin)
{
        skin = &newSkin;
    repaint();
}

