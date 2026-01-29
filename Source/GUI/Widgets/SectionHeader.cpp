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
        setOpaque(false);
        setSize(width_, height_);
    }

    void SectionHeader::setTheme(Theme& theme)
    {
        theme_ = &theme;
    }

    void SectionHeader::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr)
            return;

        auto contentArea = getLocalBounds().toFloat();
        contentArea.setHeight(kContentHeight_);

        drawText(g, contentArea);
        drawLines(g, contentArea);
    }

    void SectionHeader::drawText(juce::Graphics& g, const juce::Rectangle<float>& contentArea)
    {
        if (text_.isEmpty())
            return;

        auto textBounds = contentArea;
        textBounds.removeFromLeft(kLeftLineWidth_ + kTextSpacing_);
        textBounds.setWidth(calculateTextWidth());

        g.setColour(theme_->getSectionHeaderTextColour());
        g.setFont(theme_->getBaseFont().withHeight(kTextFontHeight_));
        g.drawText(text_, textBounds, juce::Justification::topLeft, false);
    }

    void SectionHeader::drawLines(juce::Graphics& g, const juce::Rectangle<float>& contentArea)
    {
        g.setColour(getLineColour());
        
        drawLeftLine(g, contentArea);
        drawRightLine(g, contentArea);
    }

    void SectionHeader::drawLeftLine(juce::Graphics& g, const juce::Rectangle<float>& contentArea)
    {
        const auto verticalOffset = (contentArea.getHeight() - kLineHeight_) * 0.5f;
        
        auto line = contentArea;
        line.setWidth(kLeftLineWidth_);
        line.setHeight(kLineHeight_);
        line.translate(0.0f, verticalOffset);
        
        g.fillRect(line);
    }

    void SectionHeader::drawRightLine(juce::Graphics& g, const juce::Rectangle<float>& contentArea)
    {
        const auto textWidth = calculateTextWidth();
        const auto lineStartX = kLeftLineWidth_ + kTextSpacing_ + textWidth + kTextSpacing_;
        const auto remainingWidth = contentArea.getWidth() - lineStartX;

        if (remainingWidth > 0.0f)
        {
            const auto verticalOffset = (contentArea.getHeight() - kLineHeight_) * 0.5f;
            
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

    float SectionHeader::calculateTextWidth() const
    {
        if (text_.isEmpty() || theme_ == nullptr)
            return 0.0f;

        const auto font = theme_->getBaseFont().withHeight(kTextFontHeight_);
        juce::GlyphArrangement glyphArrangement;
        glyphArrangement.addLineOfText(font, text_, 0.0f, 0.0f);
        return glyphArrangement.getBoundingBox(0, -1, true).getWidth();
    }
}

