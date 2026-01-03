#include "FooterPanel.h"

#include "../../Themes/Theme.h"

using tss::Theme;

FooterPanel::FooterPanel(Theme& newTheme)
    : theme(&newTheme)
{
}

void FooterPanel::paint(juce::Graphics& g)
{
        g.fillAll(theme->getFooterPanelBackgroundColour());
}

void FooterPanel::resized()
{
}

void FooterPanel::setTheme(Theme& newTheme)
{
        theme = &newTheme;
    repaint();
}

