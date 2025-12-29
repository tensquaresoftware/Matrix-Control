#include "SectionName.h"

#include "../Skin/Skin.h"
#include "../Skin/SkinDimensions.h"

namespace tss
{
    SectionName::SectionName(Skin& newSkin, int width, const juce::String& text, ColourVariant variant)
        : skin(&newSkin)
        , name(text)
        , colourVariant(variant)
    {
        setSize(width, SkinDimensions::Widget::SectionName::kHeight);
    }

    void SectionName::setSkin(Skin& newSkin)
    {
        skin = &newSkin;
        repaint();
    }

    void SectionName::paint(juce::Graphics& g)
    {
        if (skin == nullptr)
        {
            return;
        }

        auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
        drawLeftLine(g, bounds);
        drawText(g, bounds);
        drawRightLine(g, bounds);
    }

    void SectionName::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        g.setColour(skin->getSectionNameBaseColour());
        g.fillRect(bounds);
    }

    void SectionName::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (name.isEmpty())
        {
            return;
        }

        auto leftLineWidth = skin->getSectionNameLeftLineWidth();
        auto textSpacing = skin->getSectionNameTextSpacing();
        auto contentArea = getContentArea(bounds);
        auto textBounds = contentArea;
        textBounds.removeFromLeft(leftLineWidth + textSpacing);
        textBounds.setWidth(calculateTextWidth());

        g.setColour(skin->getSectionNameTextColour());
        g.setFont(skin->getSectionNameFont());
        g.drawText(name, textBounds, juce::Justification::centredLeft, false);
    }

    void SectionName::drawLeftLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        g.setColour(getLineColour());

        auto leftLineWidth = skin->getSectionNameLeftLineWidth();
        auto lineHeight = skin->getSectionNameLineHeight();
        auto contentArea = getContentArea(bounds);
        auto line = contentArea;
        line.setWidth(leftLineWidth);
        line.setHeight(lineHeight);
        
        g.fillRect(line);
    }

    void SectionName::drawRightLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto textWidth = calculateTextWidth();
        auto leftLineWidth = skin->getSectionNameLeftLineWidth();
        auto textSpacing = skin->getSectionNameTextSpacing();
        auto lineStartX = leftLineWidth + textSpacing + textWidth + textSpacing;
        auto remainingWidth = bounds.getWidth() - lineStartX;

        if (remainingWidth > 0.0f)
        {
            g.setColour(getLineColour());

            auto lineHeight = skin->getSectionNameLineHeight();
            auto contentArea = getContentArea(bounds);
            auto line = contentArea;
            line.removeFromLeft(lineStartX);
            line.setHeight(lineHeight);
            
            g.fillRect(line);
        }
    }

    juce::Colour SectionName::getLineColour() const
    {
        return (colourVariant == ColourVariant::Blue) 
            ? skin->getSectionNameLineColourBlue() 
            : skin->getSectionNameLineColourOrange();
    }

    juce::Rectangle<float> SectionName::getContentArea(const juce::Rectangle<float>& bounds) const
    {
        auto contentHeight = skin->getSectionNameContentHeight();
        auto contentArea = bounds;
        contentArea.setHeight(contentHeight);
        return contentArea;
    }

    float SectionName::calculateTextWidth() const
    {
        if (name.isEmpty() || skin == nullptr)
        {
            return 0.0f;
        }

        auto font = skin->getSectionNameFont();
        juce::GlyphArrangement glyphArrangement;
        glyphArrangement.addLineOfText(font, name, 0.0f, 0.0f);
        auto bounds = glyphArrangement.getBoundingBox(0, -1, true);
        return bounds.getWidth();
    }
}

