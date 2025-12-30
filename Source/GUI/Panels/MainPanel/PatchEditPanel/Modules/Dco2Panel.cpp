#include "Dco2Panel.h"

#include "../../../../Skin/Skin.h"
#include "../../../../Skin/SkinDimensions.h"

using tss::Skin;

Dco2Panel::Dco2Panel(Skin& newSkin)
    : skin(&newSkin)
{
    setSize(tss::SkinDimensions::PatchEditModulePanel::kWidth, tss::SkinDimensions::PatchEditModulePanel::kHeight);
}

void Dco2Panel::paint(juce::Graphics& g)
{
    if (skin == nullptr)
    {
        return;
    }

    g.fillAll(skin->getPatchEditModulePanelBackgroundColour());
}

void Dco2Panel::setSkin(Skin& newSkin)
{
    skin = &newSkin;
    repaint();
}

