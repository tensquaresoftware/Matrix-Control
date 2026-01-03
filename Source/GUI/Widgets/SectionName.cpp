#include "SectionName.h"

#include "../Themes/Theme.h"

namespace tss
{
    SectionName::SectionName(Theme& newTheme, int width, const juce::String& text, ColourVariant variant)
        : theme(&newTheme)
        , name(text)
        , colourVariant(variant)
    {
        setSize(width, kHeight);
    }

    void SectionName::setTheme(Theme& newTheme)
    {
        theme = &newTheme;
        repaint();
    }

    void SectionName::paint(juce::Graphics& g)
    {
        if (theme == nullptr)
        {
            return;
        }

        auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
        drawContentArea(g, bounds);
        drawLeftLine(g, bounds);
        drawText(g, bounds);
        drawRightLine(g, bounds);
    }

    void SectionName::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        g.setColour(theme->getSectionNameBaseColour());
        g.fillRect(bounds);
    }

    void SectionName::drawContentArea(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto contentArea = getContentArea(bounds);
        g.setColour(theme->getSectionNameContentAreaColour());
        g.fillRect(contentArea);
    }

    void SectionName::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (name.isEmpty())
        {
            return;
        }

        auto leftLineWidth = kLeftLineWidth;
        auto textSpacing = kTextSpacing;
        auto contentArea = getContentArea(bounds);
        auto textBounds = contentArea;
        textBounds.removeFromLeft(leftLineWidth + textSpacing);
        textBounds.setWidth(calculateTextWidth());

        g.setColour(theme->getSectionNameTextColour());
        g.setFont(theme->getBaseFont().withHeight(20.0f));
        g.drawText(name, textBounds, juce::Justification::topLeft, false);
    }

    void SectionName::drawLeftLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        g.setColour(getLineColour());

        auto leftLineWidth = kLeftLineWidth;
        auto lineHeight = kLineHeight;
        auto contentArea = getContentArea(bounds);
        auto verticalOffset = (contentArea.getHeight() - lineHeight) / 2.0f;
        
        auto line = contentArea;
        line.setWidth(leftLineWidth);
        line.setHeight(lineHeight);
        line.translate(0.0f, verticalOffset);
        
        g.fillRect(line);
    }

    void SectionName::drawRightLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto textWidth = calculateTextWidth();
        auto leftLineWidth = kLeftLineWidth;
        auto textSpacing = kTextSpacing;
        auto lineStartX = leftLineWidth + textSpacing + textWidth + textSpacing;
        auto remainingWidth = bounds.getWidth() - lineStartX;

        if (remainingWidth > 0.0f)
        {
            g.setColour(getLineColour());

            auto contentArea = getContentArea(bounds);
            auto verticalOffset = (contentArea.getHeight() - kLineHeight) / 2.0f;
            
            auto line = contentArea;
            line.removeFromLeft(lineStartX);
            line.setHeight(kLineHeight);
            line.translate(0.0f, verticalOffset);
            
            g.fillRect(line);
        }
    }

    juce::Colour SectionName::getLineColour() const
    {
        return (colourVariant == ColourVariant::Blue) 
            ? theme->getSectionNameLineColourBlue() 
            : theme->getSectionNameLineColourOrange();
    }

    juce::Rectangle<float> SectionName::getContentArea(const juce::Rectangle<float>& bounds) const
    {
        auto contentArea = bounds;
        contentArea.setHeight(kContentHeight);
        return contentArea;
    }

    float SectionName::calculateTextWidth() const
    {
        if (name.isEmpty() || theme == nullptr)
        {
            return 0.0f;
        }

        auto font = theme->getBaseFont().withHeight(20.0f);
        juce::GlyphArrangement glyphArrangement;
        glyphArrangement.addLineOfText(font, name, 0.0f, 0.0f);
        auto bounds = glyphArrangement.getBoundingBox(0, -1, true);
        return bounds.getWidth();
    }
}

