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
    }

    void ModuleHeader::setTheme(Theme& theme)
    {
        theme_ = &theme;
    }

    void ModuleHeader::setText(const juce::String& text)
    {
        if (text_ != text)
        {
            text_ = text;
            repaint();
        }
    }

    void ModuleHeader::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr)
            return;

        const auto bounds = getLocalBounds().toFloat();

        drawText(g, bounds);
        drawLine(g, bounds);
    }

    void ModuleHeader::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (text_.isEmpty())
            return;

        const auto font = theme_->getBaseFont().withHeight(kTextFontHeight_).boldened();

        auto textBounds = bounds;
        textBounds.setHeight(kTextAreaHeight_);
        textBounds.removeFromLeft(kTextLeftPadding_);

        g.setColour(theme_->getModuleHeaderTextColour());
        g.setFont(font);
        g.drawText(text_, textBounds, juce::Justification::centredLeft, false);
    }

    void ModuleHeader::drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto lineAreaHeight = bounds.getHeight() - kTextAreaHeight_;
        const auto verticalOffset = kTextAreaHeight_ + (lineAreaHeight - kLineThickness_) * 0.5f;
        
        auto lineBounds = bounds;
        lineBounds.setHeight(kLineThickness_);
        lineBounds.translate(0.0f, verticalOffset);

        g.setColour(getLineColour());
        g.fillRect(lineBounds);
    }

    juce::Colour ModuleHeader::getLineColour() const
    {
        return (colourVariant_ == ColourVariant::Blue) 
            ? theme_->getModuleHeaderLineColourBlue() 
            : theme_->getModuleHeaderLineColourOrange();
    }
}

