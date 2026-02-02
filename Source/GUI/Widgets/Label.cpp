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
        updateThemeCache();
    }

    void Label::setTheme(Theme& theme)
    {
        theme_ = &theme;
        updateThemeCache();
        invalidateCache();
        repaint();
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

        // Create image at component size - JUCE handles HiDPI automatically
        cachedImage_ = juce::Image(juce::Image::ARGB, width, height, true);
        juce::Graphics g(cachedImage_);

        auto textBounds = juce::Rectangle<float>(0.0f, 0.0f, 
                                                   static_cast<float>(width), 
                                                   static_cast<float>(height));
        textBounds.removeFromLeft(static_cast<float>(kTextLeftPadding_));

        g.setColour(cachedTextColour_);
        g.setFont(cachedFont_);
        g.drawText(labelText_, textBounds, juce::Justification::centredLeft, false);

        cacheValid_ = true;
    }

    void Label::invalidateCache()
    {
        cacheValid_ = false;
    }

    void Label::updateThemeCache()
    {
        if (theme_ == nullptr)
            return;

        cachedTextColour_ = theme_->getLabelTextColour();
        cachedFont_ = theme_->getBaseFont();
    }

    float Label::getPixelScale() const
    {
        const auto* display = juce::Desktop::getInstance()
                                  .getDisplays()
                                  .getDisplayForRect(getScreenBounds());

        return display != nullptr ? static_cast<float>(display->scale) : 1.0f;
    }
}

