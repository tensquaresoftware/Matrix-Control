#include "ModuleName.h"

#include "../Themes/Theme.h"

namespace tss
{
    ModuleName::ModuleName(Theme& newTheme, const juce::String& text, Size size, ColourVariant variant)
        : theme(&newTheme)
        , name(text)
        , colourVariant(variant)
    {
        auto width = (size == Size::Normal) ? kNormalWidth : kLargeWidth;
        setSize(width, kHeight);
    }

    void ModuleName::setTheme(Theme& newTheme)
    {
        theme = &newTheme;
        repaint();
    }

    void ModuleName::setText(const juce::String& newText)
    {
        if (name != newText)
        {
            name = newText;
            repaint();
        }
    }

    void ModuleName::paint(juce::Graphics& g)
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

    void ModuleName::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto baseColour = theme->getModuleNameBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void ModuleName::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (name.isEmpty())
        {
            return;
        }

        auto textColour = theme->getModuleNameTextColour();
        auto font = theme->getBaseFont().withHeight(16.0f).boldened();

        auto textAreaHeight = kTextAreaHeight;
        auto textLeftPadding = kTextLeftPadding;
        auto textBounds = bounds;
        textBounds.setHeight(textAreaHeight);
        textBounds.removeFromLeft(textLeftPadding);

        g.setColour(textColour);
        g.setFont(font);
        g.drawText(name, textBounds, juce::Justification::centredLeft, false);
    }

    void ModuleName::drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto lineColour = getLineColour();
        
        auto textAreaHeight = kTextAreaHeight;
        auto lineThickness = kLineThickness;
        auto lineAreaHeight = bounds.getHeight() - textAreaHeight;
        auto verticalOffset = textAreaHeight + (lineAreaHeight - lineThickness) / 2.0f;
        
        auto lineBounds = bounds;
        lineBounds.setHeight(lineThickness);
        lineBounds.translate(0.0f, verticalOffset);

        g.setColour(lineColour);
        g.fillRect(lineBounds);
    }

    juce::Colour ModuleName::getLineColour() const
    {
        return (colourVariant == ColourVariant::Blue) 
            ? theme->getModuleNameLineColourBlue() 
            : theme->getModuleNameLineColourOrange();
    }
}

