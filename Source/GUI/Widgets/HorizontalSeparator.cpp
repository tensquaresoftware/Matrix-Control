#include "HorizontalSeparator.h"

#include "../Themes/Theme.h"

namespace tss
{
    HorizontalSeparator::HorizontalSeparator(Theme& inTheme, SeparatorWidth width)
        : separatorWidth(width)
        , theme(&inTheme)
    {
        setSize(getWidth(width), kHeight);
    }

    void HorizontalSeparator::setTheme(Theme& inTheme)
    {
        theme = &inTheme;
        repaint();
    }

    void HorizontalSeparator::paint(juce::Graphics& g)
    {
        if (theme == nullptr)
        {
            return;
        }

        const auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
        drawLine(g, bounds);
    }

    void HorizontalSeparator::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto baseColour = theme->getHorizontalSeparatorBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void HorizontalSeparator::drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto lineColour = theme->getHorizontalSeparatorLineColour();
        const auto lineY = bounds.getCentreY();
        
        g.setColour(lineColour);
        g.drawLine(bounds.getX(), lineY, bounds.getRight(), lineY, kLineThickness);
    }

    int HorizontalSeparator::getWidth() const
    {
        return getWidth(separatorWidth);
    }
}

