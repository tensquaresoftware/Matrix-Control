#include "VcfVcaPanel.h"

#include "../../../../Skin/Skin.h"
#include "../../../../Skin/SkinDimensions.h"

using tss::Skin;

VcfVcaPanel::VcfVcaPanel(Skin& newSkin)
    : skin(&newSkin)
{
    setSize(tss::SkinDimensions::PatchEditModulePanel::kWidth, tss::SkinDimensions::PatchEditModulePanel::kHeight);
}

void VcfVcaPanel::paint(juce::Graphics& g)
{
    if (skin == nullptr)
    {
        return;
    }

    g.fillAll(skin->getPatchEditModulePanelBackgroundColour());
}

void VcfVcaPanel::setSkin(Skin& newSkin)
{
    skin = &newSkin;
    repaint();
}

