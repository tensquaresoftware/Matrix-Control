#include "GroupLabel.h"

#include "../Themes/Theme.h"

namespace tss
{
    GroupLabel::GroupLabel(Theme& newTheme, int width, const juce::String& text)
        : theme(&newTheme)
        , labelText(text)
    {
        setSize(width, kHeight);
    }

    void GroupLabel::setTheme(Theme& newTheme)
    {
        theme = &newTheme;
        repaint();
    }

    void GroupLabel::setText(const juce::String& newText)
    {
        if (labelText != newText)
        {
            labelText = newText;
            repaint();
        }
    }

    void GroupLabel::paint(juce::Graphics& g)
    {
        if (theme == nullptr)
        {
            return;
        }

        auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
        drawText(g, bounds);
        drawLeftLine(g, bounds);
        drawRightLine(g, bounds);
    }

    void GroupLabel::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        g.setColour(theme->getGroupLabelBaseColour());
        g.fillRect(bounds);
    }

    void GroupLabel::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (labelText.isEmpty())
        {
            return;
        }

        auto textArea = getContentArea(bounds);
        auto font = theme->getBaseFont();

        g.setColour(theme->getGroupLabelTextColour());
        g.setFont(font);
        g.drawText(labelText, textArea, juce::Justification::centred, false);
    }

    void GroupLabel::drawLeftLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (labelText.isEmpty())
        {
            return;
        }

        auto textArea = getContentArea(bounds);
        auto textWidth = calculateTextWidth();
        auto textCentreX = textArea.getCentreX();
        auto lineY = textArea.getCentreY();
        auto leftLineEndX = textCentreX - textWidth * 0.5f - kTextSpacing;
        auto remainingWidth = leftLineEndX - textArea.getX();

        if (remainingWidth > 0.0f)
        {
            g.setColour(theme->getGroupLabelLineColour());

            auto leftLine = textArea;
            leftLine.setWidth(remainingWidth);
            leftLine.setY(lineY - kLineThickness * 0.5f);
            leftLine.setHeight(kLineThickness);

            g.fillRect(leftLine);
        }
    }

    void GroupLabel::drawRightLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (labelText.isEmpty())
        {
            return;
        }

        auto textArea = getContentArea(bounds);
        auto textWidth = calculateTextWidth();
        auto textCentreX = textArea.getCentreX();
        auto lineY = textArea.getCentreY();
        auto rightLineStartX = textCentreX + textWidth * 0.5f + kTextSpacing;
        auto remainingWidth = textArea.getRight() - rightLineStartX;

        if (remainingWidth > 0.0f)
        {
            g.setColour(theme->getGroupLabelLineColour());

            auto rightLine = textArea;
            rightLine.removeFromLeft(rightLineStartX - textArea.getX());
            rightLine.setY(lineY - kLineThickness * 0.5f);
            rightLine.setHeight(kLineThickness);

            g.fillRect(rightLine);
        }
    }

    juce::Rectangle<float> GroupLabel::getContentArea(const juce::Rectangle<float>& bounds) const
    {
        auto topAreaHeight = kTopAreaHeight;
        auto contentHeight = kContentHeight;
        auto textArea = bounds;
        textArea.removeFromTop(topAreaHeight);
        textArea.setHeight(contentHeight);
        return textArea;
    }

    float GroupLabel::calculateTextWidth() const
    {
        if (labelText.isEmpty() || theme == nullptr)
        {
            return 0.0f;
        }

        auto font = theme->getBaseFont();
        juce::GlyphArrangement glyphArrangement;
        glyphArrangement.addLineOfText(font, labelText, 0.0f, 0.0f);
        auto bounds = glyphArrangement.getBoundingBox(0, -1, true);
        return bounds.getWidth();
    }
}

