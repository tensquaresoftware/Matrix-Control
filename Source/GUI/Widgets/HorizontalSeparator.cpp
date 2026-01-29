#include "HorizontalSeparator.h"

#include "GUI/Themes/Theme.h"

namespace tss
{
    HorizontalSeparator::HorizontalSeparator(Theme& theme, int width, int height)
        : theme_(&theme)
        , width_(width)
        , height_(height)
    {
        setOpaque(false);
        setSize(width_, height_);
    }

    void HorizontalSeparator::setTheme(Theme& theme)
    {
        theme_ = &theme;
    }

    void HorizontalSeparator::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr)
            return;

        const auto bounds = getLocalBounds().toFloat();
        const auto lineY = bounds.getCentreY();
        
        g.setColour(theme_->getHorizontalSeparatorLineColour());
        g.drawLine(bounds.getX(), lineY, bounds.getRight(), lineY, kLineThickness_);
    }
}

