#include "PanelSeparator.h"

#include "../Themes/Theme.h"

namespace tss
{
    PanelSeparator::PanelSeparator(Theme& inTheme)
        : theme(&inTheme)
    {
        setSize(kWidth, kHeight);
    }

    void PanelSeparator::setTheme(Theme& inTheme)
    {
        theme = &inTheme;
        repaint();
    }

    void PanelSeparator::paint(juce::Graphics& g)
    {
        if (theme == nullptr)
        {
            return;
        }

        const auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
        drawLine(g, bounds);
    }

    void PanelSeparator::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto baseColour = theme->getPanelSeparatorBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void PanelSeparator::drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto lineColour = theme->getPanelSeparatorLineColour();
        const auto lineWidth = kLineWidth;
        const auto lineX = bounds.getCentreX() - lineWidth / 2.0f;
        
        auto line = bounds;
        line.setX(lineX);
        line.setWidth(lineWidth);
        
        g.setColour(lineColour);
        g.fillRect(line);
    }
}

