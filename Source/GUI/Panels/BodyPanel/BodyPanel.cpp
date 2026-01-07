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

    matrixModulationPanel = std::make_unique<MatrixModulationPanel>(inTheme, widgetFactory, apvts);
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
    if (auto* currentTheme = theme)
        g.fillAll(currentTheme->getBodyPanelBackgroundColour());
}

void BodyPanel::resized()
{
    const auto bounds = getLocalBounds();
    const auto padding = getPadding();
    
    const auto patchEditPanelX = padding;
    const auto patchEditPanelY = padding;
    patchEditPanel->setBounds(
        bounds.getX() + patchEditPanelX,
        bounds.getY() + patchEditPanelY,
        PatchEditPanel::getWidth(),
        PatchEditPanel::getHeight()
    );
    
    const auto panelSeparator1X = patchEditPanelX + PatchEditPanel::getWidth();
    const auto panelSeparator1Y = padding;
    panelSeparator1->setTopLeftPosition(
        bounds.getX() + panelSeparator1X,
        bounds.getY() + panelSeparator1Y
    );
    
    const auto matrixModulationPanelX = panelSeparator1X + panelSeparator1->getWidth();
    const auto matrixModulationPanelY = padding;
    matrixModulationPanel->setBounds(
        bounds.getX() + matrixModulationPanelX,
        bounds.getY() + matrixModulationPanelY,
        MatrixModulationPanel::getWidth(),
        MatrixModulationPanel::getHeight()
    );
    
    const auto patchManagerPanelX = matrixModulationPanelX;
    const auto patchManagerPanelY = matrixModulationPanelY + MatrixModulationPanel::getHeight();
    patchManagerPanel->setBounds(
        bounds.getX() + patchManagerPanelX,
        bounds.getY() + patchManagerPanelY,
        PatchManagerPanel::getWidth(),
        PatchManagerPanel::getHeight()
    );
    
    const auto panelSeparator2X = matrixModulationPanelX + MatrixModulationPanel::getWidth();
    const auto panelSeparator2Y = padding;
    panelSeparator2->setTopLeftPosition(
        bounds.getX() + panelSeparator2X,
        bounds.getY() + panelSeparator2Y
    );
    
    const auto masterEditPanelX = panelSeparator2X + panelSeparator2->getWidth();
    const auto masterEditPanelY = padding;
    masterEditPanel->setBounds(
        bounds.getX() + masterEditPanelX,
        bounds.getY() + masterEditPanelY,
        MasterEditPanel::getWidth(),
        MasterEditPanel::getHeight()
    );
}

void BodyPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* panel = patchEditPanel.get())
        panel->setTheme(inTheme);

    if (auto* separator = panelSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* panel = matrixModulationPanel.get())
        panel->setTheme(inTheme);

    if (auto* separator = panelSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* panel = masterEditPanel.get())
        panel->setTheme(inTheme);

    if (auto* panel = patchManagerPanel.get())
        panel->setTheme(inTheme);

    repaint();
}

