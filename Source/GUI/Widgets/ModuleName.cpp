#include "ModuleName.h"

#include "../Skin/Skin.h"
#include "../Skin/SkinDimensions.h"

namespace tss
{
    ModuleName::ModuleName(Skin& newSkin, const juce::String& text, Size size, ColourVariant variant)
        : skin(&newSkin)
        , name(text)
        , colourVariant(variant)
    {
        auto width = (size == Size::Normal) ? SkinDimensions::Widget::ModuleName::kNormalWidth : SkinDimensions::Widget::ModuleName::kLargeWidth;
        setSize(width, SkinDimensions::Widget::ModuleName::kHeight);
    }

    void ModuleName::setSkin(Skin& newSkin)
    {
        skin = &newSkin;
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
        if (skin == nullptr)
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
        auto baseColour = skin->getModuleNameBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void ModuleName::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (name.isEmpty())
        {
            return;
        }

        auto textColour = skin->getModuleNameTextColour();
        auto font = skin->getModuleNameFont();

        auto textAreaHeight = skin->getModuleNameTextAreaHeight();
        auto textLeftPadding = skin->getModuleNameTextLeftPadding();
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
        
        auto lineThickness = skin->getModuleNameLineThickness();
        auto lineBounds = bounds;
        lineBounds.removeFromTop(bounds.getHeight() - lineThickness);

        g.setColour(lineColour);
        g.fillRect(lineBounds);
    }

    juce::Colour ModuleName::getLineColour() const
    {
        return (colourVariant == ColourVariant::Blue) 
            ? skin->getModuleNameLineColourBlue() 
            : skin->getModuleNameLineColourOrange();
    }
}

