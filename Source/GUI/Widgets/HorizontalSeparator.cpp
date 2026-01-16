#include "HorizontalSeparator.h"

#include "GUI/Themes/Theme.h"

namespace tss
{
    HorizontalSeparator::HorizontalSeparator(Theme& theme, int width, int height)
        : theme_(&theme)
        ,width_(width)
        , height_(height)
    {
        setSize(width_, height_);
    }

    void HorizontalSeparator::setTheme(Theme& theme)
    {
        theme_ = &theme;
        repaint();
    }

    void HorizontalSeparator::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr)
        {
            return;
        }

        const auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
        drawLine(g, bounds);
    }

    void HorizontalSeparator::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto baseColour = theme_->getHorizontalSeparatorBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void HorizontalSeparator::drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto lineColour = theme_->getHorizontalSeparatorLineColour();
        const auto lineY = bounds.getCentreY();
        
        g.setColour(lineColour);
        g.drawLine(bounds.getX(), lineY, bounds.getRight(), lineY, kLineThickness_);
    }
}

