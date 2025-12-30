#include "FmTrackPanel.h"

#include "../../../../Skin/Skin.h"
#include "../../../../Skin/SkinDimensions.h"

using tss::Skin;

FmTrackPanel::FmTrackPanel(Skin& newSkin)
    : skin(&newSkin)
{
    setSize(tss::SkinDimensions::PatchEditModulePanel::kWidth, tss::SkinDimensions::PatchEditModulePanel::kHeight);
}

void FmTrackPanel::paint(juce::Graphics& g)
{
    if (skin == nullptr)
    {
        return;
    }

    g.fillAll(skin->getPatchEditModulePanelBackgroundColour());
}

void FmTrackPanel::setSkin(Skin& newSkin)
{
    skin = &newSkin;
    repaint();
}

