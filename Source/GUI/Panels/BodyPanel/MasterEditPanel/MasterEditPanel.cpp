#include "MasterEditPanel.h"

#include "../../../Themes/Theme.h"

using tss::Theme;

MasterEditPanel::MasterEditPanel(Theme& newTheme)
    : theme(&newTheme)
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

void MasterEditPanel::setTheme(Theme& newTheme)
{
        theme = &newTheme;
    repaint();
}

