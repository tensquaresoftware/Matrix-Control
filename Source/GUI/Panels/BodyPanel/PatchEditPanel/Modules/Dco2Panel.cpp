#include "Dco2Panel.h"

#include "../../../../Themes/Theme.h"

using tss::Theme;

Dco2Panel::Dco2Panel(Theme& newTheme)
    : theme(&newTheme)
{
    setSize(getWidth(), getHeight());
}

void Dco2Panel::paint(juce::Graphics& g)
{
    if (theme == nullptr)
    {
        return;
    }

    g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void Dco2Panel::resized()
{
}

void Dco2Panel::setTheme(Theme& newTheme)
{
    theme = &newTheme;
    repaint();
}

