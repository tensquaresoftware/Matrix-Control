#include "Button.h"

#include "../Themes/Theme.h"

namespace tss
{
    Button::Button(Theme& inTheme, int width, const juce::String& text)
        : juce::Button(text)
        , theme(&inTheme)
    {
        setSize(width, kHeight);
    }

    void Button::setTheme(Theme& inTheme)
    {
        theme = &inTheme;
        repaint();
    }

    void Button::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        if (theme == nullptr)
        {
            return;
        }

        auto bounds = getLocalBounds().toFloat();
        auto enabled = isEnabled();

        drawBase(g, bounds);
        drawBackground(g, bounds, enabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
        drawBorder(g, bounds, enabled);
        drawText(g, bounds, enabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
    }

    void Button::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto baseColour = theme->getButtonBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void Button::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isHighlighted, bool isDown)
    {
        juce::Colour backgroundColour;
        
        if (!enabled)
        {
            backgroundColour = theme->getButtonBackgroundColourOff();
        }
        else if (isDown)
        {
            backgroundColour = theme->getButtonBackgroundColourClicked();
        }
        else if (isHighlighted)
        {
            backgroundColour = theme->getButtonBackgroundColourHoover();
        }
        else
        {
            backgroundColour = theme->getButtonBackgroundColourOn();
        }
        
        g.setColour(backgroundColour);
        g.fillRect(bounds);
    }

    void Button::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        juce::Colour borderColour;
        
        if (!enabled)
        {
            borderColour = theme->getButtonBorderColourOff();
        }
        else
        {
            borderColour = theme->getButtonBorderColourOn();
        }
        
        g.setColour(borderColour);
        
        auto borderThickness = kBorderThickness;
        
        auto topBorder = juce::Rectangle<float>(bounds.getX(), bounds.getY(), bounds.getWidth(), borderThickness);
        auto bottomBorder = juce::Rectangle<float>(bounds.getX(), bounds.getBottom() - borderThickness, bounds.getWidth(), borderThickness);
        auto leftBorder = juce::Rectangle<float>(bounds.getX(), bounds.getY() + borderThickness, borderThickness, bounds.getHeight() - 2.0f * borderThickness);
        auto rightBorder = juce::Rectangle<float>(bounds.getRight() - borderThickness, bounds.getY() + borderThickness, borderThickness, bounds.getHeight() - 2.0f * borderThickness);
        
        g.fillRect(topBorder);
        g.fillRect(bottomBorder);
        g.fillRect(leftBorder);
        g.fillRect(rightBorder);
    }

    void Button::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isHighlighted, bool isDown)
    {
        auto buttonText = getButtonText();
        if (buttonText.isEmpty())
        {
            return;
        }

        juce::Colour textColour;
        
        if (!enabled)
        {
            textColour = theme->getButtonTextColourOff();
        }
        else if (isDown)
        {
            textColour = theme->getButtonTextColourClicked();
        }
        else if (isHighlighted)
        {
            textColour = theme->getButtonTextColourHoover();
        }
        else
        {
            textColour = theme->getButtonTextColourOn();
        }

        auto font = theme->getBaseFont();

        g.setColour(textColour);
        g.setFont(font);
        g.drawText(buttonText, bounds, juce::Justification::centred, false);
    }
}

