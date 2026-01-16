#include "GroupLabel.h"

#include "../Themes/Theme.h"

namespace tss
{
    GroupLabel::GroupLabel(Theme& theme, int width, int height, const juce::String& text)
        : theme_(&theme)
        ,width_(width)
        , height_(height)
        , labelText_(text)
    {
        setSize(width_, height_);
    }

    void GroupLabel::setTheme(Theme& theme)
    {
        theme_ = &theme;
        repaint();
    }

    void GroupLabel::setText(const juce::String& text)
    {
        if (labelText_ != text)
        {
            labelText_ = text;
            repaint();
        }
    }

    void GroupLabel::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr)
        {
            return;
        }

        const auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
        drawText(g, bounds);
        drawLines(g, bounds);
    }

    void GroupLabel::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        g.setColour(theme_->getGroupLabelBaseColour());
        g.fillRect(bounds);
    }

    void GroupLabel::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (labelText_.isEmpty())
        {
            return;
        }

        const auto textArea = getContentArea(bounds);
        const auto font = theme_->getBaseFont();

        g.setColour(theme_->getGroupLabelTextColour());
        g.setFont(font);
        g.drawText(labelText_, textArea, juce::Justification::centred, false);
    }

    void GroupLabel::drawLines(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (labelText_.isEmpty())
        {
            return;
        }

        const auto textArea = getContentArea(bounds);
        const auto textWidth = calculateTextWidth();
        const auto textCentreX = textArea.getCentreX();
        const auto lineY = textArea.getCentreY();
        const auto halfTextWidth = textWidth * 0.5f;

        g.setColour(theme_->getGroupLabelLineColour());

        drawLeftLine(g, textArea, textCentreX, halfTextWidth, lineY);
        drawRightLine(g, textArea, textCentreX, halfTextWidth, lineY);
    }

    void GroupLabel::drawLeftLine(juce::Graphics& g, const juce::Rectangle<float>& textArea, float textCentreX, float halfTextWidth, float lineY)
    {
        const auto leftLineEndX = textCentreX - halfTextWidth - kTextSpacing_;
        const auto leftLineWidth = leftLineEndX - textArea.getX();

        if (leftLineWidth > 0.0f)
        {
            const auto lineThicknessHalf = kLineThickness_ * 0.5f;
            auto leftLine = textArea;
            leftLine.setWidth(leftLineWidth);
            leftLine.setY(lineY - lineThicknessHalf);
            leftLine.setHeight(kLineThickness_);
            g.fillRect(leftLine);
        }
    }

    void GroupLabel::drawRightLine(juce::Graphics& g, const juce::Rectangle<float>& textArea, float textCentreX, float halfTextWidth, float lineY)
    {
        const auto rightLineStartX = textCentreX + halfTextWidth + kTextSpacing_;
        const auto rightLineWidth = textArea.getRight() - rightLineStartX;

        if (rightLineWidth > 0.0f)
        {
            const auto lineThicknessHalf = kLineThickness_ * 0.5f;
            auto rightLine = textArea;
            rightLine.removeFromLeft(rightLineStartX - textArea.getX());
            rightLine.setY(lineY - lineThicknessHalf);
            rightLine.setHeight(kLineThickness_);
            g.fillRect(rightLine);
        }
    }

    juce::Rectangle<float> GroupLabel::getContentArea(const juce::Rectangle<float>& bounds) const
    {
        const auto topAreaHeight = kTopAreaHeight_;
        const auto contentHeight = kContentHeight_;
        auto textArea = bounds;
        textArea.removeFromTop(topAreaHeight);
        textArea.setHeight(contentHeight);
        return textArea;
    }

    float GroupLabel::calculateTextWidth() const
    {
        if (labelText_.isEmpty() || theme_ == nullptr)
        {
            return 0.0f;
        }

        const auto font = theme_->getBaseFont();
        juce::GlyphArrangement glyphArrangement;
        glyphArrangement.addLineOfText(font, labelText_, 0.0f, 0.0f);
        const auto bounds = glyphArrangement.getBoundingBox(0, -1, true);
        return bounds.getWidth();
    }
}

