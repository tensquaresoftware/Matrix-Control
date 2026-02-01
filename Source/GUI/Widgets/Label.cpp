#include "Label.h"

#include "GUI/Themes/Theme.h"

namespace tss
{
    Label::Label(Theme& theme, int width, int height, const juce::String& text)
        : theme_(&theme)
        , width_(width)
        , height_(height)
        , labelText_(text)
    {
        setOpaque(false);
        setSize(width_, height_);
    }

    void Label::setTheme(Theme& theme)
    {
        theme_ = &theme;
        invalidateCache();
    }

    void Label::setText(const juce::String& text)
    {
        if (labelText_ != text)
        {
            labelText_ = text;
            invalidateCache();
            repaint();
        }
    }

    void Label::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr || labelText_.isEmpty())
            return;

        if (!cacheValid_)
            regenerateCache();

        if (cachedImage_.isValid())
            g.drawImageAt(cachedImage_, 0, 0);
    }
    
    void Label::resized()
    {
        invalidateCache();
    }
    
    void Label::regenerateCache()
    {
        const auto width = getWidth();
        const auto height = getHeight();
        
        if (width <= 0 || height <= 0)
            return;
        
        cachedImage_ = juce::Image(juce::Image::ARGB, width, height, true);
        juce::Graphics g(cachedImage_);
        
        const auto textColour = theme_->getLabelTextColour();
        const auto font = theme_->getBaseFont();
        
        auto textBounds = cachedImage_.getBounds().toFloat();
        textBounds.removeFromLeft(kTextLeftPadding_);
        
        g.setColour(textColour);
        g.setFont(font);
        g.drawText(labelText_, textBounds, juce::Justification::centredLeft, false);
        
        cacheValid_ = true;
    }
    
    void Label::invalidateCache()
    {
        cacheValid_ = false;
    }
}

