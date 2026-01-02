#include "MatrixModulationPanel.h"

#include "../../../Skin/Skin.h"

using tss::Skin;

MatrixModulationPanel::MatrixModulationPanel(Skin& newSkin)
    : skin(&newSkin)
{
    setSize(275, 315);
}

void MatrixModulationPanel::paint(juce::Graphics& g)
{
        if (skin == nullptr)
        {
            return;
        }
        
        g.fillAll(skin->getMatrixModulationPanelBackgroundColour());
}

void MatrixModulationPanel::setSkin(Skin& newSkin)
{
        skin = &newSkin;
    repaint();
}

