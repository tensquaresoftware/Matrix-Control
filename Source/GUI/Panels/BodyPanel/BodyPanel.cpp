#include "BodyPanel.h"

#include "../../Themes/Theme.h"
#include "../../Widgets/VerticalSeparator.h"
#include "PatchEditPanel/PatchEditPanel.h"
#include "MatrixModulationPanel/MatrixModulationPanel.h"
#include "MasterEditPanel/MasterEditPanel.h"
#include "PatchManagerPanel/PatchManagerPanel.h"
#include "../../../GUI/Factories/WidgetFactory.h"

using tss::Theme;
using tss::VerticalSeparator;

BodyPanel::BodyPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
{
    patchEditPanel_ = std::make_unique<PatchEditPanel>(theme, widgetFactory, apvts);
    addAndMakeVisible(*patchEditPanel_);

    verticalSeparator1_ = std::make_unique<VerticalSeparator>(theme);
    addAndMakeVisible(*verticalSeparator1_);

    matrixModulationPanel_ = std::make_unique<MatrixModulationPanel>(theme, widgetFactory, apvts);
    addAndMakeVisible(*matrixModulationPanel_);

    patchManagerPanel_ = std::make_unique<PatchManagerPanel>(theme, widgetFactory);
    addAndMakeVisible(*patchManagerPanel_);

    verticalSeparator2_ = std::make_unique<VerticalSeparator>(theme);
    addAndMakeVisible(*verticalSeparator2_);

    masterEditPanel_ = std::make_unique<MasterEditPanel>(theme, widgetFactory, apvts);
    addAndMakeVisible(*masterEditPanel_);
}

BodyPanel::~BodyPanel() = default;

void BodyPanel::paint(juce::Graphics& g)
{
    if (auto* currentTheme = theme_)
        g.fillAll(currentTheme->getBodyPanelBackgroundColour());
}

void BodyPanel::resized()
{
    const auto bounds = getLocalBounds();
    const auto padding = getPadding();
    
    const auto patchEditPanelX = padding;
    const auto patchEditPanelY = padding;
    patchEditPanel_->setBounds(
        bounds.getX() + patchEditPanelX,
        bounds.getY() + patchEditPanelY,
        PatchEditPanel::getWidth(),
        PatchEditPanel::getHeight()
    );
    
    const auto verticalSeparator1X = patchEditPanelX + PatchEditPanel::getWidth();
    const auto verticalSeparator1Y = padding;
    verticalSeparator1_->setTopLeftPosition(
        bounds.getX() + verticalSeparator1X,
        bounds.getY() + verticalSeparator1Y
    );
    
    const auto matrixModulationPanelX = verticalSeparator1X + VerticalSeparator::getWidth();
    const auto matrixModulationPanelY = padding;
    matrixModulationPanel_->setBounds(
        bounds.getX() + matrixModulationPanelX,
        bounds.getY() + matrixModulationPanelY,
        MatrixModulationPanel::getWidth(),
        MatrixModulationPanel::getHeight()
    );
    
    const auto patchManagerPanelX = matrixModulationPanelX;
    const auto patchManagerPanelY = matrixModulationPanelY + MatrixModulationPanel::getHeight();
    patchManagerPanel_->setBounds(
        bounds.getX() + patchManagerPanelX,
        bounds.getY() + patchManagerPanelY,
        PatchManagerPanel::getWidth(),
        PatchManagerPanel::getHeight()
    );
    
    const auto verticalSeparator2X = matrixModulationPanelX + MatrixModulationPanel::getWidth();
    const auto verticalSeparator2Y = padding;
    verticalSeparator2_->setTopLeftPosition(
        bounds.getX() + verticalSeparator2X,
        bounds.getY() + verticalSeparator2Y
    );
    
    const auto masterEditPanelX = verticalSeparator2X + VerticalSeparator::getWidth();
    const auto masterEditPanelY = padding;
    masterEditPanel_->setBounds(
        bounds.getX() + masterEditPanelX,
        bounds.getY() + masterEditPanelY,
        MasterEditPanel::getWidth(),
        MasterEditPanel::getHeight()
    );
}

void BodyPanel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* panel = patchEditPanel_.get())
        panel->setTheme(theme);

    if (auto* separator = verticalSeparator1_.get())
        separator->setTheme(theme);

    if (auto* panel = matrixModulationPanel_.get())
        panel->setTheme(theme);

    if (auto* separator = verticalSeparator2_.get())
        separator->setTheme(theme);

    if (auto* panel = masterEditPanel_.get())
        panel->setTheme(theme);

    if (auto* panel = patchManagerPanel_.get())
        panel->setTheme(theme);

    repaint();
}

