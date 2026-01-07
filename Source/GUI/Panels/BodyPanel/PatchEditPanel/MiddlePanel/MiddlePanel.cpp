#include "MiddlePanel.h"

#include "../../../../Themes/Theme.h"

using tss::Theme;

MiddlePanel::~MiddlePanel() = default;

MiddlePanel::MiddlePanel(Theme& inTheme)
    : theme(&inTheme)
{
    setSize(getWidth(), getHeight());
}

void MiddlePanel::paint(juce::Graphics& g)
{
    if (theme == nullptr)
    {
        return;
    }

    g.fillAll(theme->getPatchEditPanelBackgroundColour());
}

void MiddlePanel::resized()
{
}

void MiddlePanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;
    repaint();
}

