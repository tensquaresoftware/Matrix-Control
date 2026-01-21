#include "VerticalSeparator.h"

#include "GUI/Themes/Theme.h"

namespace tss
{
    VerticalSeparator::VerticalSeparator(Theme& inTheme)
        : theme(&inTheme)
    {
        setOpaque(true);
        setSize(kWidth_, kHeight_);
    }

    void VerticalSeparator::setTheme(Theme& inTheme)
    {
        theme = &inTheme;
        repaint();
    }

    void VerticalSeparator::paint(juce::Graphics& g)
    {
        if (theme == nullptr)
        {
            return;
        }

        g.fillAll(theme->getGuiBackgroundColour());

        const auto bounds = getLocalBounds().toFloat();

        drawLine(g, bounds);
    }


    void VerticalSeparator::drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto lineColour = theme->getVerticalSeparatorLineColour();
        const auto lineWidth = kLineWidth_;
        const auto lineX = bounds.getCentreX() - lineWidth / 2.0f;
        
        auto line = bounds;
        line.setX(lineX);
        line.setWidth(lineWidth);
        
        g.setColour(lineColour);
        g.fillRect(line);
    }
}

