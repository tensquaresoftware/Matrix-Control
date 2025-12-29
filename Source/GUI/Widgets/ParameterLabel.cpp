#include "ParameterLabel.h"

#include "../Skin/Skin.h"
#include "../Skin/SkinDimensions.h"

namespace tss
{
    ParameterLabel::ParameterLabel(Skin& newSkin, const juce::String& text)
        : skin(&newSkin)
        , labelText(text)
    {
        setSize(SkinDimensions::Widget::ParameterLabel::kWidth, SkinDimensions::Widget::ParameterLabel::kHeight);
    }

    void ParameterLabel::setSkin(Skin& newSkin)
    {
        skin = &newSkin;
        repaint();
    }

    void ParameterLabel::setText(const juce::String& newText)
    {
        if (labelText != newText)
        {
            labelText = newText;
            repaint();
        }
    }

    void ParameterLabel::paint(juce::Graphics& g)
    {
        if (skin == nullptr)
        {
            return;
        }

        auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
        drawBackground(g, bounds);
        drawText(g, bounds);
    }

    void ParameterLabel::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto baseColour = skin->getParameterLabelBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void ParameterLabel::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto backgroundColour = skin->getParameterLabelBackgroundColour();
        auto backgroundMargin = skin->getParameterLabelBackgroundPadding();
        auto backgroundBounds = bounds.reduced(backgroundMargin);
        
        g.setColour(backgroundColour);
        g.fillRect(backgroundBounds);
    }

    void ParameterLabel::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (labelText.isEmpty())
        {
            return;
        }

        auto textColour = skin->getParameterLabelTextColour();
        auto font = skin->getParameterLabelFont();

        auto backgroundMargin = skin->getParameterLabelBackgroundPadding();
        auto textBounds = bounds.reduced(backgroundMargin);
        textBounds.removeFromLeft(skin->getParameterLabelTextLeftPadding());

        g.setColour(textColour);
        g.setFont(font);
        g.drawText(labelText, textBounds, juce::Justification::centredLeft, false);
    }
}

