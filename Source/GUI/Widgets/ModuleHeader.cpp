#include "ModuleHeader.h"

#include "../Themes/Theme.h"

namespace tss
{
    ModuleHeader::ModuleHeader(Theme& inTheme, const juce::String& text, Size size, ColourVariant variant)
        : theme(&inTheme)
        , text(text)
        , colourVariant(variant)
    {
        auto width = (size == Size::Normal) ? kNormalWidth : kLargeWidth;
        setSize(width, kHeight);
    }

    void ModuleHeader::setTheme(Theme& inTheme)
    {
        theme = &inTheme;
        repaint();
    }

    void ModuleHeader::setText(const juce::String& newText)
    {
        if (text != newText)
        {
            text = newText;
            repaint();
        }
    }

    void ModuleHeader::paint(juce::Graphics& g)
    {
        if (theme == nullptr)
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
        auto baseColour = theme->getModuleHeaderBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void ModuleHeader::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (text.isEmpty())
        {
            return;
        }

        auto textColour = theme->getModuleHeaderTextColour();
        auto font = theme->getBaseFont().withHeight(16.0f).boldened();

        auto textBounds = bounds;
        textBounds.setHeight(kTextAreaHeight);
        textBounds.removeFromLeft(kTextLeftPadding);

        g.setColour(textColour);
        g.setFont(font);
        g.drawText(text, textBounds, juce::Justification::centredLeft, false);
    }

    void ModuleHeader::drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto lineColour = getLineColour();
        
        auto lineThickness = kLineThickness;
        auto lineAreaHeight = bounds.getHeight() - kTextAreaHeight;
        auto verticalOffset = kTextAreaHeight + (lineAreaHeight - lineThickness) / 2.0f;
        
        auto lineBounds = bounds;
        lineBounds.setHeight(lineThickness);
        lineBounds.translate(0.0f, verticalOffset);

        g.setColour(lineColour);
        g.fillRect(lineBounds);
    }

    juce::Colour ModuleHeader::getLineColour() const
    {
        return (colourVariant == ColourVariant::Blue) 
            ? theme->getModuleHeaderLineColourBlue() 
            : theme->getModuleHeaderLineColourOrange();
    }
}

