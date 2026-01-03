#include "RampPortamentoPanel.h"

#include "../../../../Themes/Theme.h"

using tss::Theme;

RampPortamentoPanel::RampPortamentoPanel(Theme& newTheme)
    : theme(&newTheme)
{
    setSize(getWidth(), getHeight());
}

void RampPortamentoPanel::paint(juce::Graphics& g)
{
    if (theme == nullptr)
    {
        return;
    }

    g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void RampPortamentoPanel::setTheme(Theme& newTheme)
{
    theme = &newTheme;
    repaint();
}

