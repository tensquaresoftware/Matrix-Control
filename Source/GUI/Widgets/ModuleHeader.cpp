#include "ModuleHeader.h"

#include "../Themes/Theme.h"

namespace tss
{
    ModuleHeader::ModuleHeader(Theme& theme, const juce::String& text, int width, int height, ColourVariant variant)
        : width_(width)
        , height_(height)
        , theme_(&theme)
        , text_(text)
        , colourVariant_(variant)
    {
        setSize(width_, height_);
    }

    void ModuleHeader::setTheme(Theme& theme)
    {
        theme_ = &theme;
        repaint();
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
        {
            return;
        }

        auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
        drawText(g, bounds);
        drawLine(g, bounds);
    }

    void ModuleHeader::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto baseColour = theme_->getModuleHeaderBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void ModuleHeader::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (text_.isEmpty())
        {
            return;
        }

        auto textColour = theme_->getModuleHeaderTextColour();
        auto font = theme_->getBaseFont().withHeight(16.0f).boldened();

        auto textBounds = bounds;
        textBounds.setHeight(kTextAreaHeight_);
        textBounds.removeFromLeft(kTextLeftPadding_);

        g.setColour(textColour);
        g.setFont(font);
        g.drawText(text_, textBounds, juce::Justification::centredLeft, false);
    }

    void ModuleHeader::drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto lineColour = getLineColour();
        
        auto lineThickness = kLineThickness_;
        auto lineAreaHeight = bounds.getHeight() - kTextAreaHeight_;
        auto verticalOffset = kTextAreaHeight_ + (lineAreaHeight - lineThickness) / 2.0f;
        
        auto lineBounds = bounds;
        lineBounds.setHeight(lineThickness);
        lineBounds.translate(0.0f, verticalOffset);

        g.setColour(lineColour);
        g.fillRect(lineBounds);
    }

    juce::Colour ModuleHeader::getLineColour() const
    {
        return (colourVariant_ == ColourVariant::Blue) 
            ? theme_->getModuleHeaderLineColourBlue() 
            : theme_->getModuleHeaderLineColourOrange();
    }
}

