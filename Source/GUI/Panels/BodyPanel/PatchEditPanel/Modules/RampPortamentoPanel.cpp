#include "RampPortamentoPanel.h"

#include "../../../../Skin/Skin.h"
#include "../../../../Skin/SkinDimensions.h"

using tss::Skin;

RampPortamentoPanel::RampPortamentoPanel(Skin& newSkin)
    : skin(&newSkin)
{
    setSize(tss::SkinDimensions::PatchEditModulePanel::kWidth, tss::SkinDimensions::PatchEditModulePanel::kHeight);
}

void RampPortamentoPanel::paint(juce::Graphics& g)
{
    if (skin == nullptr)
    {
        return;
    }

    g.fillAll(skin->getPatchEditModulePanelBackgroundColour());
}

void RampPortamentoPanel::setSkin(Skin& newSkin)
{
    skin = &newSkin;
    repaint();
}

