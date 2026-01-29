#include "GroupLabel.h"

#include "GUI/Themes/Theme.h"

namespace tss
{
    GroupLabel::GroupLabel(Theme& theme, int width, int height, const juce::String& text)
        : theme_(&theme)
        , width_(width)
        , height_(height)
        , labelText_(text)
    {
        setOpaque(false);
        setSize(width_, height_);
    }

    void GroupLabel::setTheme(Theme& theme)
    {
        theme_ = &theme;
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
        if (theme_ == nullptr || labelText_.isEmpty())
            return;

        const auto contentArea = calculateContentArea();
        const auto textWidth = calculateTextWidth();

        drawText(g, contentArea);
        drawLines(g, contentArea, textWidth);
    }

    void GroupLabel::drawText(juce::Graphics& g, const juce::Rectangle<float>& area)
    {
        g.setColour(theme_->getGroupLabelTextColour());
        g.setFont(theme_->getBaseFont());
        g.drawText(labelText_, area, juce::Justification::centred, false);
    }

    void GroupLabel::drawLines(juce::Graphics& g, const juce::Rectangle<float>& area, float textWidth)
    {
        const auto halfTextWidth = textWidth * 0.5f;
        const auto centreX = area.getCentreX();
        const auto centreY = area.getCentreY();

        g.setColour(theme_->getGroupLabelLineColour());

        drawLeftLine(g, area, centreX, halfTextWidth, centreY);
        drawRightLine(g, area, centreX, halfTextWidth, centreY);
    }

    void GroupLabel::drawLeftLine(juce::Graphics& g, const juce::Rectangle<float>& area, float centreX, float halfTextWidth, float centreY)
    {
        const auto lineEndX = centreX - halfTextWidth - kTextSpacing_;
        const auto lineWidth = lineEndX - area.getX();

        if (lineWidth > 0.0f)
        {
            const auto lineThicknessHalf = kLineThickness_ * 0.5f;
            const auto line = juce::Rectangle<float>(
                area.getX(),
                centreY - lineThicknessHalf,
                lineWidth,
                kLineThickness_
            );
            g.fillRect(line);
        }
    }

    void GroupLabel::drawRightLine(juce::Graphics& g, const juce::Rectangle<float>& area, float centreX, float halfTextWidth, float centreY)
    {
        const auto lineStartX = centreX + halfTextWidth + kTextSpacing_;
        const auto lineWidth = area.getRight() - lineStartX;

        if (lineWidth > 0.0f)
        {
            const auto lineThicknessHalf = kLineThickness_ * 0.5f;
            const auto line = juce::Rectangle<float>(
                lineStartX,
                centreY - lineThicknessHalf,
                lineWidth,
                kLineThickness_
            );
            g.fillRect(line);
        }
    }

    juce::Rectangle<float> GroupLabel::calculateContentArea() const
    {
        auto area = getLocalBounds().toFloat();
        area.removeFromTop(kTopAreaHeight_);
        area.setHeight(kContentHeight_);
        return area;
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

