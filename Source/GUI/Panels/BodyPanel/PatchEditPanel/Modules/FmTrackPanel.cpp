#include "FmTrackPanel.h"

#include "../../../../Themes/Theme.h"

using tss::Theme;

FmTrackPanel::FmTrackPanel(Theme& newTheme)
    : theme(&newTheme)
{
    setSize(getWidth(), getHeight());
}

void FmTrackPanel::paint(juce::Graphics& g)
{
    if (theme == nullptr)
    {
        return;
    }

    g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void FmTrackPanel::setTheme(Theme& newTheme)
{
    theme = &newTheme;
    repaint();
}

