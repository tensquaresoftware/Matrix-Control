#include "BodyPanel.h"

#include "../../Skin/Skin.h"
#include "../../Widgets/PanelSeparator.h"
#include "PatchEditPanel/PatchEditPanel.h"
#include "MatrixModulationPanel/MatrixModulationPanel.h"
#include "MasterEditPanel/MasterEditPanel.h"
#include "PatchManagerPanel/PatchManagerPanel.h"
#include "../../Skin/SkinDimensions.h"
#include "../../../Shared/WidgetFactory.h"

using tss::Skin;
using tss::PanelSeparator;

BodyPanel::BodyPanel(Skin& newSkin, WidgetFactory& widgetFactory)
    : skin(&newSkin)
{
    patchEditPanel = std::make_unique<PatchEditPanel>(newSkin, widgetFactory);
    addAndMakeVisible(*patchEditPanel);

    panelSeparator1 = std::make_unique<PanelSeparator>(newSkin);
    addAndMakeVisible(*panelSeparator1);

    matrixModulationPanel = std::make_unique<MatrixModulationPanel>(newSkin);
    addAndMakeVisible(*matrixModulationPanel);

    patchManagerPanel = std::make_unique<PatchManagerPanel>(newSkin);
    addAndMakeVisible(*patchManagerPanel);

    panelSeparator2 = std::make_unique<PanelSeparator>(newSkin);
    addAndMakeVisible(*panelSeparator2);

    masterEditPanel = std::make_unique<MasterEditPanel>(newSkin);
    addAndMakeVisible(*masterEditPanel);
}

BodyPanel::~BodyPanel() = default;

void BodyPanel::paint(juce::Graphics& g)
{
        g.fillAll(skin->getBodyPanelBackgroundColour());
}

void BodyPanel::resized()
{
    auto spacing = tss::SkinDimensions::BodyPanel::kSpacing;
    auto patchEditPanelX = spacing;
    auto patchEditPanelY = spacing;
    patchEditPanel->setBounds(patchEditPanelX, patchEditPanelY, tss::SkinDimensions::PatchEditPanel::kWidth, tss::SkinDimensions::PatchEditPanel::kHeight);

    auto separatorX = patchEditPanelX + tss::SkinDimensions::PatchEditPanel::kWidth;
    auto separatorY = spacing;
    panelSeparator1->setTopLeftPosition(separatorX, separatorY);

    auto matrixModulationPanelX = separatorX + panelSeparator1->getWidth();
    auto matrixModulationPanelY = spacing;
    matrixModulationPanel->setBounds(matrixModulationPanelX, matrixModulationPanelY, 275, 315);

    auto patchManagerPanelX = matrixModulationPanelX;
    auto patchManagerPanelY = matrixModulationPanelY + 315;
    patchManagerPanel->setBounds(patchManagerPanelX, patchManagerPanelY, 275, 415);

    auto separator2X = matrixModulationPanelX + 275;
    auto separator2Y = spacing;
    panelSeparator2->setTopLeftPosition(separator2X, separator2Y);

    auto masterEditPanelX = separator2X + panelSeparator2->getWidth();
    auto masterEditPanelY = spacing;
    masterEditPanel->setBounds(masterEditPanelX, masterEditPanelY, tss::SkinDimensions::MasterEditPanel::kWidth, tss::SkinDimensions::MasterEditPanel::kHeight);
}

void BodyPanel::setSkin(Skin& newSkin)
{
        skin = &newSkin;

    if (patchEditPanel != nullptr)
    {
        patchEditPanel->setSkin(*skin);
    }

    if (panelSeparator1 != nullptr)
    {
        panelSeparator1->setSkin(*skin);
    }

    if (matrixModulationPanel != nullptr)
    {
        matrixModulationPanel->setSkin(*skin);
    }

    if (panelSeparator2 != nullptr)
    {
        panelSeparator2->setSkin(*skin);
    }

    if (masterEditPanel != nullptr)
    {
        masterEditPanel->setSkin(*skin);
    }

    if (patchManagerPanel != nullptr)
    {
        patchManagerPanel->setSkin(*skin);
    }

    repaint();
}

