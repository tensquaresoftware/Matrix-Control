#include "ParameterSeparator.h"

#include "../Themes/Theme.h"

namespace tss
{
    ParameterSeparator::ParameterSeparator(Theme& newTheme)
        : theme(&newTheme)
    {
        setSize(kWidth, kHeight);
    }

    void ParameterSeparator::setTheme(Theme& newTheme)
    {
        theme = &newTheme;
        repaint();
    }

    void ParameterSeparator::paint(juce::Graphics& g)
    {
        if (theme == nullptr)
        {
            return;
        }

        auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
        drawLine(g, bounds);
    }

    void ParameterSeparator::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto baseColour = theme->getParameterSeparatorBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void ParameterSeparator::drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto lineColour = theme->getParameterSeparatorLineColour();
        auto lineY = bounds.getCentreY();
        auto lineThickness = kLineThickness;
        
        g.setColour(lineColour);
        g.drawLine(bounds.getX(), lineY, bounds.getRight(), lineY, lineThickness);
    }
}

