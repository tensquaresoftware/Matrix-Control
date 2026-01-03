#include "VcfVcaPanel.h"

#include "../../../../Themes/Theme.h"

using tss::Theme;

VcfVcaPanel::VcfVcaPanel(Theme& newTheme)
    : theme(&newTheme)
{
    setSize(getWidth(), getHeight());
}

void VcfVcaPanel::paint(juce::Graphics& g)
{
    if (theme == nullptr)
    {
        return;
    }

    g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void VcfVcaPanel::setTheme(Theme& newTheme)
{
    theme = &newTheme;
    repaint();
}

