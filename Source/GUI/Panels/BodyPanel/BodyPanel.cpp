#include "BodyPanel.h"

#include "../../Themes/Theme.h"
#include "../../Widgets/PanelSeparator.h"
#include "PatchEditPanel/PatchEditPanel.h"
#include "MatrixModulationPanel/MatrixModulationPanel.h"
#include "MasterEditPanel/MasterEditPanel.h"
#include "PatchManagerPanel/PatchManagerPanel.h"
#include "../../../GUI/Factories/WidgetFactory.h"

using tss::Theme;
using tss::PanelSeparator;

BodyPanel::BodyPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme(&inTheme)
{
    patchEditPanel = std::make_unique<PatchEditPanel>(inTheme, widgetFactory, apvts);
    addAndMakeVisible(*patchEditPanel);

    panelSeparator1 = std::make_unique<PanelSeparator>(inTheme);
    addAndMakeVisible(*panelSeparator1);

    matrixModulationPanel = std::make_unique<MatrixModulationPanel>(inTheme);
    addAndMakeVisible(*matrixModulationPanel);

    patchManagerPanel = std::make_unique<PatchManagerPanel>(inTheme);
    addAndMakeVisible(*patchManagerPanel);

    panelSeparator2 = std::make_unique<PanelSeparator>(inTheme);
    addAndMakeVisible(*panelSeparator2);

    masterEditPanel = std::make_unique<MasterEditPanel>(inTheme);
    addAndMakeVisible(*masterEditPanel);
}

BodyPanel::~BodyPanel() = default;

void BodyPanel::paint(juce::Graphics& g)
{
        g.fillAll(theme->getBodyPanelBackgroundColour());
}

void BodyPanel::resized()
{
    auto spacing = getSpacing();
    auto patchEditPanelX = spacing;
    auto patchEditPanelY = spacing;
    patchEditPanel->setBounds(patchEditPanelX, patchEditPanelY, PatchEditPanel::getWidth(), PatchEditPanel::getHeight());

    auto separatorX = patchEditPanelX + PatchEditPanel::getWidth();
    auto separatorY = spacing;
    panelSeparator1->setTopLeftPosition(separatorX, separatorY);

    auto matrixModulationPanelX = separatorX + panelSeparator1->getWidth();
    auto matrixModulationPanelY = spacing;
    matrixModulationPanel->setBounds(matrixModulationPanelX, matrixModulationPanelY, MatrixModulationPanel::getWidth(), MatrixModulationPanel::getHeight());

    auto patchManagerPanelX = matrixModulationPanelX;
    auto patchManagerPanelY = matrixModulationPanelY + MatrixModulationPanel::getHeight();
    patchManagerPanel->setBounds(patchManagerPanelX, patchManagerPanelY, PatchManagerPanel::getWidth(), PatchManagerPanel::getHeight());

    auto separator2X = matrixModulationPanelX + MatrixModulationPanel::getWidth();
    auto separator2Y = spacing;
    panelSeparator2->setTopLeftPosition(separator2X, separator2Y);

    auto masterEditPanelX = separator2X + panelSeparator2->getWidth();
    auto masterEditPanelY = spacing;
    masterEditPanel->setBounds(masterEditPanelX, masterEditPanelY, MasterEditPanel::getWidth(), MasterEditPanel::getHeight());
}

void BodyPanel::setTheme(Theme& inTheme)
{
        theme = &inTheme;

    if (patchEditPanel != nullptr)
    {
        patchEditPanel->setTheme(*theme);
    }

    if (panelSeparator1 != nullptr)
    {
        panelSeparator1->setTheme(*theme);
    }

    if (matrixModulationPanel != nullptr)
    {
        matrixModulationPanel->setTheme(*theme);
    }

    if (panelSeparator2 != nullptr)
    {
        panelSeparator2->setTheme(*theme);
    }

    if (masterEditPanel != nullptr)
    {
        masterEditPanel->setTheme(*theme);
    }

    if (patchManagerPanel != nullptr)
    {
        patchManagerPanel->setTheme(*theme);
    }

    repaint();
}

