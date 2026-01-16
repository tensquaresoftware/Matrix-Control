#include "MiddlePanel.h"

#include "../../../../Themes/Theme.h"

using tss::Theme;

MiddlePanel::~MiddlePanel() = default;

MiddlePanel::MiddlePanel(Theme& theme)
    : theme_(&theme)
{
    setSize(getWidth(), getHeight());
}

void MiddlePanel::paint(juce::Graphics& g)
{
    if (theme_ == nullptr)
    {
        return;
    }

    g.fillAll(theme_->getPatchEditPanelBackgroundColour());
}

void MiddlePanel::resized()
{
}

void MiddlePanel::setTheme(Theme& theme)
{
    theme_ = &theme;
    repaint();
}

