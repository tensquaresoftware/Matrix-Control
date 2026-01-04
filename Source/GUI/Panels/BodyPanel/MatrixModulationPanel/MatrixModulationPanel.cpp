#include "MatrixModulationPanel.h"

#include "../../../Themes/Theme.h"

using tss::Theme;

MatrixModulationPanel::MatrixModulationPanel(Theme& inTheme)
    : theme(&inTheme)
{
    setSize(getWidth(), getHeight());
}

void MatrixModulationPanel::paint(juce::Graphics& g)
{
        if (theme == nullptr)
        {
            return;
        }
        
        g.fillAll(theme->getMatrixModulationPanelBackgroundColour());
}

void MatrixModulationPanel::setTheme(Theme& inTheme)
{
        theme = &inTheme;
    repaint();
}

