#include "PatchManagerPanel.h"

#include "../../../Themes/Theme.h"

using tss::Theme;

PatchManagerPanel::PatchManagerPanel(Theme& newTheme)
    : theme(&newTheme)
{
    setSize(getWidth(), getHeight());
}

void PatchManagerPanel::paint(juce::Graphics& g)
{
        if (theme == nullptr)
        {
            return;
        }
        
        g.fillAll(theme->getPatchManagerPanelBackgroundColour());
}

void PatchManagerPanel::setTheme(Theme& newTheme)
{
        theme = &newTheme;
    repaint();
}

