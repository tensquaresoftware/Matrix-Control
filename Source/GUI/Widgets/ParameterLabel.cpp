#include "ParameterLabel.h"

#include "../Themes/Theme.h"

namespace tss
{
    ParameterLabel::ParameterLabel(Theme& newTheme, const juce::String& text)
        : theme(&newTheme)
        , labelText(text)
    {
        setSize(kWidth, kHeight);
    }

    void ParameterLabel::setTheme(Theme& newTheme)
    {
        theme = &newTheme;
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
        if (theme == nullptr)
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
        auto baseColour = theme->getParameterLabelBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void ParameterLabel::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto backgroundColour = theme->getParameterLabelBackgroundColour();
        auto backgroundMargin = kBackgroundPadding;
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

        auto textColour = theme->getParameterLabelTextColour();
        auto font = theme->getBaseFont();

        auto backgroundMargin = kBackgroundPadding;
        auto textBounds = bounds.reduced(backgroundMargin);
        textBounds.removeFromLeft(kTextLeftPadding);

        g.setColour(textColour);
        g.setFont(font);
        g.drawText(labelText, textBounds, juce::Justification::centredLeft, false);
    }
}

