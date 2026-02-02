#include "ModuleHeader.h"

#include "GUI/Themes/Theme.h"

namespace tss
{
    ModuleHeader::ModuleHeader(Theme& theme, const juce::String& text, int width, int height, ColourVariant variant)
        : width_(width)
        , height_(height)
        , theme_(&theme)
        , text_(text)
        , colourVariant_(variant)
    {
        setOpaque(false);
        setSize(width_, height_);
        updateThemeCache();
    }

    void ModuleHeader::setTheme(Theme& theme)
    {
        theme_ = &theme;
        updateThemeCache();
        invalidateCache();
        repaint();
    }

    void ModuleHeader::setText(const juce::String& text)
    {
        if (text_ != text)
        {
            text_ = text;
            invalidateCache();
            repaint();
        }
    }

    void ModuleHeader::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr || text_.isEmpty())
            return;

        if (!cacheValid_)
            regenerateCache();

        if (cachedImage_.isValid())
        {
            g.drawImage(cachedImage_, getLocalBounds().toFloat(),
                       juce::RectanglePlacement::stretchToFit);
        }
    }

    void ModuleHeader::resized()
    {
        invalidateCache();
    }

    void ModuleHeader::regenerateCache()
    {
        const auto width = getWidth();
        const auto height = getHeight();

        if (width <= 0 || height <= 0)
            return;

        const float pixelScale = getPixelScale();
        const int imageWidth = juce::roundToInt(width * pixelScale);
        const int imageHeight = juce::roundToInt(height * pixelScale);

        // Create HiDPI image at physical resolution
        cachedImage_ = juce::Image(juce::Image::ARGB, imageWidth, imageHeight, true);
        juce::Graphics g(cachedImage_);
        
        // Scale graphics context to match physical resolution
        g.addTransform(juce::AffineTransform::scale(pixelScale));

        const auto bounds = juce::Rectangle<float>(0.0f, 0.0f, 
                                                    static_cast<float>(width), 
                                                    static_cast<float>(height));

        drawText(g, bounds);
        drawLine(g, bounds);

        cacheValid_ = true;
    }

    void ModuleHeader::invalidateCache()
    {
        cacheValid_ = false;
    }

    void ModuleHeader::updateThemeCache()
    {
        if (theme_ == nullptr)
            return;

        cachedTextColour_ = theme_->getModuleHeaderTextColour();
        cachedLineColour_ = getLineColour();
        cachedFont_ = theme_->getBaseFont().withHeight(kTextFontHeight_).boldened();
    }

    float ModuleHeader::getPixelScale() const
    {
        const auto* display = juce::Desktop::getInstance()
                                  .getDisplays()
                                  .getDisplayForRect(getScreenBounds());
        const float displayScale = display != nullptr ? static_cast<float>(display->scale) : 1.0f;
        return displayScale;
    }

    void ModuleHeader::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (text_.isEmpty())
            return;

        auto textBounds = bounds;
        textBounds.setHeight(kTextAreaHeight_);
        textBounds.removeFromLeft(kTextLeftPadding_);

        g.setColour(cachedTextColour_);
        g.setFont(cachedFont_);
        g.drawText(text_, textBounds, juce::Justification::centredLeft, false);
    }

    void ModuleHeader::drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto lineAreaHeight = bounds.getHeight() - kTextAreaHeight_;
        const auto verticalOffset = kTextAreaHeight_ + (lineAreaHeight - kLineThickness_) * 0.5f;
        
        auto lineBounds = bounds;
        lineBounds.setHeight(kLineThickness_);
        lineBounds.translate(0.0f, verticalOffset);

        g.setColour(cachedLineColour_);
        g.fillRect(lineBounds);
    }

    juce::Colour ModuleHeader::getLineColour() const
    {
        return (colourVariant_ == ColourVariant::Blue) 
            ? theme_->getModuleHeaderLineColourBlue() 
            : theme_->getModuleHeaderLineColourOrange();
    }
}

