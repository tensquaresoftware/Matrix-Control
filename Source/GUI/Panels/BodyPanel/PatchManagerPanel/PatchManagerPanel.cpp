#include "PatchManagerPanel.h"

#include "../../../Themes/Theme.h"

using tss::Theme;

PatchManagerPanel::PatchManagerPanel(Theme& inTheme)
    : theme(&inTheme)
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

void PatchManagerPanel::setTheme(Theme& inTheme)
{
        theme = &inTheme;
    repaint();
}

