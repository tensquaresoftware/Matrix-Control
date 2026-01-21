#include "SectionHeader.h"

#include "GUI/Themes/Theme.h"

namespace tss
{
    SectionHeader::SectionHeader(Theme& theme, int width, int height, const juce::String& text, ColourVariant variant)
        : width_(width)
        , height_(height)
        , theme_(&theme)
        , text_(text)
        , colourVariant_(variant)
    {
        setOpaque(true);
        setSize(width_, height_);
    }

    void SectionHeader::setTheme(Theme& theme)
    {
        theme_ = &theme;
        repaint();
    }

    void SectionHeader::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr)
        {
            return;
        }

        g.fillAll(theme_->getPatchManagerPanelBackgroundColour());

        auto bounds = getLocalBounds().toFloat();

        drawContentArea(g, bounds);
        drawLeftLine(g, bounds);
        drawText(g, bounds);
        drawRightLine(g, bounds);
    }


    void SectionHeader::drawContentArea(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto contentArea = getContentArea(bounds);
        g.setColour(theme_->getSectionHeaderContentAreaColour());
        g.fillRect(contentArea);
    }

    void SectionHeader::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (text_.isEmpty())
        {
            return;
        }

        auto leftLineWidth = kLeftLineWidth_;
        auto textSpacing = kTextSpacing_;
        auto contentArea = getContentArea(bounds);
        auto textBounds = contentArea;
        textBounds.removeFromLeft(leftLineWidth + textSpacing);
        textBounds.setWidth(calculateTextWidth());

        g.setColour(theme_->getSectionHeaderTextColour());
        g.setFont(theme_->getBaseFont().withHeight(20.0f));
        g.drawText(text_, textBounds, juce::Justification::topLeft, false);
    }

    void SectionHeader::drawLeftLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        g.setColour(getLineColour());

        auto leftLineWidth = kLeftLineWidth_;
        auto lineHeight = kLineHeight_;
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
        auto leftLineWidth = kLeftLineWidth_;
        auto textSpacing = kTextSpacing_;
        auto lineStartX = leftLineWidth + textSpacing + textWidth + textSpacing;
        auto remainingWidth = bounds.getWidth() - lineStartX;

        if (remainingWidth > 0.0f)
        {
            g.setColour(getLineColour());

            auto contentArea = getContentArea(bounds);
            auto verticalOffset = (contentArea.getHeight() - kLineHeight_) / 2.0f;
            
            auto line = contentArea;
            line.removeFromLeft(lineStartX);
            line.setHeight(kLineHeight_);
            line.translate(0.0f, verticalOffset);
            
            g.fillRect(line);
        }
    }

    juce::Colour SectionHeader::getLineColour() const
    {
        return (colourVariant_ == ColourVariant::Blue) 
            ? theme_->getSectionHeaderLineColourBlue() 
            : theme_->getSectionHeaderLineColourOrange();
    }

    juce::Rectangle<float> SectionHeader::getContentArea(const juce::Rectangle<float>& bounds) const
    {
        auto contentArea = bounds;
        contentArea.setHeight(kContentHeight_);
        return contentArea;
    }

    float SectionHeader::calculateTextWidth() const
    {
        if (text_.isEmpty() || theme_ == nullptr)
        {
            return 0.0f;
        }

        auto font = theme_->getBaseFont().withHeight(20.0f);
        juce::GlyphArrangement glyphArrangement;
        glyphArrangement.addLineOfText(font, text_, 0.0f, 0.0f);
        auto bounds = glyphArrangement.getBoundingBox(0, -1, true);
        return bounds.getWidth();
    }
}

