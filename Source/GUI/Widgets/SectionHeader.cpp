#include "SectionHeader.h"

#include "../Themes/Theme.h"

namespace tss
{
    SectionHeader::SectionHeader(Theme& inTheme, SectionWidth width, const juce::String& inText, ColourVariant variant)
        : theme(&inTheme)
        , text(inText)
        , colourVariant(variant)
    {
        setSize(getWidth(width), kHeight);
    }

    void SectionHeader::setTheme(Theme& inTheme)
    {
        theme = &inTheme;
        repaint();
    }

    void SectionHeader::paint(juce::Graphics& g)
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

    void SectionHeader::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        g.setColour(theme->getSectionHeaderBaseColour());
        g.fillRect(bounds);
    }

    void SectionHeader::drawContentArea(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto contentArea = getContentArea(bounds);
        g.setColour(theme->getSectionHeaderContentAreaColour());
        g.fillRect(contentArea);
    }

    void SectionHeader::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (text.isEmpty())
        {
            return;
        }

        auto leftLineWidth = kLeftLineWidth;
        auto textSpacing = kTextSpacing;
        auto contentArea = getContentArea(bounds);
        auto textBounds = contentArea;
        textBounds.removeFromLeft(leftLineWidth + textSpacing);
        textBounds.setWidth(calculateTextWidth());

        g.setColour(theme->getSectionHeaderTextColour());
        g.setFont(theme->getBaseFont().withHeight(20.0f));
        g.drawText(text, textBounds, juce::Justification::topLeft, false);
    }

    void SectionHeader::drawLeftLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
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

    void SectionHeader::drawRightLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
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

    juce::Colour SectionHeader::getLineColour() const
    {
        return (colourVariant == ColourVariant::Blue) 
            ? theme->getSectionHeaderLineColourBlue() 
            : theme->getSectionHeaderLineColourOrange();
    }

    juce::Rectangle<float> SectionHeader::getContentArea(const juce::Rectangle<float>& bounds) const
    {
        auto contentArea = bounds;
        contentArea.setHeight(kContentHeight);
        return contentArea;
    }

    float SectionHeader::calculateTextWidth() const
    {
        if (text.isEmpty() || theme == nullptr)
        {
            return 0.0f;
        }

        auto font = theme->getBaseFont().withHeight(20.0f);
        juce::GlyphArrangement glyphArrangement;
        glyphArrangement.addLineOfText(font, text, 0.0f, 0.0f);
        auto bounds = glyphArrangement.getBoundingBox(0, -1, true);
        return bounds.getWidth();
    }
}

