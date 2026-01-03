#include "PanelSeparator.h"

#include "../Themes/Theme.h"

namespace tss
{
    PanelSeparator::PanelSeparator(Theme& newTheme)
        : theme(&newTheme)
    {
        setSize(kWidth, kHeight);
    }

    void PanelSeparator::setTheme(Theme& newTheme)
    {
        theme = &newTheme;
        repaint();
    }

    void PanelSeparator::paint(juce::Graphics& g)
    {
        if (theme == nullptr)
        {
            return;
        }

        auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
        drawLine(g, bounds);
    }

    void PanelSeparator::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto baseColour = theme->getPanelSeparatorBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void PanelSeparator::drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto lineColour = theme->getPanelSeparatorLineColour();
        auto lineWidth = kLineWidth;
        auto lineX = bounds.getCentreX() - lineWidth / 2.0f;
        
        auto line = bounds;
        line.setX(lineX);
        line.setWidth(lineWidth);
        
        g.setColour(lineColour);
        g.fillRect(line);
    }
}

