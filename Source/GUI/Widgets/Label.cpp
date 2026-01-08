#include "Label.h"

#include "../Themes/Theme.h"

namespace tss
{
    Label::Label(Theme& inTheme, LabelWidth inWidth, const juce::String& text)
        : width(inWidth)
        , theme(&inTheme)
        , labelText(text)
    {
        setSize(getWidth(width), kHeight);
    }

    void Label::setTheme(Theme& inTheme)
    {
        theme = &inTheme;
        repaint();
    }

    void Label::setText(const juce::String& newText)
    {
        if (labelText != newText)
        {
            labelText = newText;
            repaint();
        }
    }

    void Label::paint(juce::Graphics& g)
    {
        if (theme == nullptr)
        {
            return;
        }

        const auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
        drawBackground(g, bounds);
        drawText(g, bounds);
    }

    void Label::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto baseColour = theme->getLabelBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void Label::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto backgroundColour = theme->getLabelBackgroundColour();
        const auto backgroundBounds = bounds.reduced(kBackgroundPadding);
        
        g.setColour(backgroundColour);
        g.fillRect(backgroundBounds);
    }

    void Label::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (labelText.isEmpty())
        {
            return;
        }

        const auto textColour = theme->getLabelTextColour();
        const auto font = theme->getBaseFont();

        auto textBounds = bounds.reduced(kBackgroundPadding);
        textBounds.removeFromLeft(kTextLeftPadding);

        g.setColour(textColour);
        g.setFont(font);
        g.drawText(labelText, textBounds, juce::Justification::centredLeft, false);
    }
}

