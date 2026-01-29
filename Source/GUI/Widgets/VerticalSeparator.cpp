#include "VerticalSeparator.h"

#include "GUI/Themes/Theme.h"

namespace tss
{
    VerticalSeparator::VerticalSeparator(Theme& theme, int width, int height)
        : theme_(&theme)
        , width_(width)
        , height_(height)
    {
        setOpaque(false);
        setSize(width_, height_);
    }

    void VerticalSeparator::setTheme(Theme& theme)
    {
        theme_ = &theme;
    }

    void VerticalSeparator::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr)
            return;

        const auto bounds = getLocalBounds().toFloat();
        const auto lineX = bounds.getCentreX() - kLineWidth_ * 0.5f;
        
        auto line = bounds;
        line.removeFromTop(kTopPadding_);
        line.removeFromBottom(kBottomPadding_);
        line.setX(lineX);
        line.setWidth(kLineWidth_);
        
        g.setColour(theme_->getVerticalSeparatorLineColour());
        g.fillRect(line);
    }
}

