#include "GroupLabel.h"

#include "../Skin/Skin.h"
#include "../Skin/SkinDimensions.h"

namespace tss
{
    GroupLabel::GroupLabel(Skin& newSkin, int width, const juce::String& text)
        : skin(&newSkin)
        , labelText(text)
    {
        setSize(width, SkinDimensions::Widget::GroupLabel::kHeight);
    }

    void GroupLabel::setSkin(Skin& newSkin)
    {
        skin = &newSkin;
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
        if (skin == nullptr)
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
        g.setColour(skin->getGroupLabelBaseColour());
        g.fillRect(bounds);
    }

    void GroupLabel::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (labelText.isEmpty())
        {
            return;
        }

        auto textArea = getContentArea(bounds);
        auto font = skin->getGroupLabelFont();

        g.setColour(skin->getGroupLabelTextColour());
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
        auto textSpacing = skin->getGroupLabelTextSpacing();
        auto leftLineEndX = textCentreX - textWidth * 0.5f - textSpacing;
        auto remainingWidth = leftLineEndX - textArea.getX();

        if (remainingWidth > 0.0f)
        {
            g.setColour(skin->getGroupLabelLineColour());

            auto lineThickness = skin->getGroupLabelLineThickness();
            auto leftLine = textArea;
            leftLine.setWidth(remainingWidth);
            leftLine.setY(lineY - lineThickness * 0.5f);
            leftLine.setHeight(lineThickness);

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
        auto textSpacing = skin->getGroupLabelTextSpacing();
        auto rightLineStartX = textCentreX + textWidth * 0.5f + textSpacing;
        auto remainingWidth = textArea.getRight() - rightLineStartX;

        if (remainingWidth > 0.0f)
        {
            g.setColour(skin->getGroupLabelLineColour());

            auto lineThickness = skin->getGroupLabelLineThickness();
            auto rightLine = textArea;
            rightLine.removeFromLeft(rightLineStartX - textArea.getX());
            rightLine.setY(lineY - lineThickness * 0.5f);
            rightLine.setHeight(lineThickness);

            g.fillRect(rightLine);
        }
    }

    juce::Rectangle<float> GroupLabel::getContentArea(const juce::Rectangle<float>& bounds) const
    {
        auto topAreaHeight = skin->getGroupLabelTopAreaHeight();
        auto contentHeight = skin->getGroupLabelContentHeight();
        auto textArea = bounds;
        textArea.removeFromTop(topAreaHeight);
        textArea.setHeight(contentHeight);
        return textArea;
    }

    float GroupLabel::calculateTextWidth() const
    {
        if (labelText.isEmpty() || skin == nullptr)
        {
            return 0.0f;
        }

        auto font = skin->getGroupLabelFont();
        juce::GlyphArrangement glyphArrangement;
        glyphArrangement.addLineOfText(font, labelText, 0.0f, 0.0f);
        auto bounds = glyphArrangement.getBoundingBox(0, -1, true);
        return bounds.getWidth();
    }
}

