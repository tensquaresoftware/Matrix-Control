#include "MasterEditPanel.h"

#include "../../../Themes/Theme.h"

using tss::Theme;

MasterEditPanel::MasterEditPanel(Theme& inTheme)
    : theme(&inTheme)
{
    setSize(getWidth(), getHeight());
}

void MasterEditPanel::paint(juce::Graphics& g)
{
        if (theme == nullptr)
        {
            return;
        }
        
        g.fillAll(theme->getMasterEditPanelBackgroundColour());
}

void MasterEditPanel::setTheme(Theme& inTheme)
{
        theme = &inTheme;
    repaint();
}

