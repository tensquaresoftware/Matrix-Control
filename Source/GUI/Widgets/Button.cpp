#include "Button.h"

#include "../Themes/Theme.h"

namespace tss
{
    Button::Button(Theme& theme, int width, int height, const juce::String& text)
        : juce::Button(text)
        , theme_(&theme)
        , width_(width)
        , height_(height)
    {
        setSize(width_, height_);
    }

    void Button::setTheme(Theme& theme)
    {
        theme_ = &theme;
        repaint();
    }

    void Button::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        if (theme_ == nullptr)
        {
            return;
        }

        const auto bounds = getLocalBounds().toFloat();
        const auto enabled = isEnabled();

        drawBase(g, bounds);
        drawBackground(g, bounds, enabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
        drawBorder(g, bounds, enabled);
        drawText(g, bounds, enabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
    }

    void Button::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto baseColour = theme_->getButtonBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void Button::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isHighlighted, bool isDown)
    {
        juce::Colour backgroundColour;
        
        if (!enabled)
        {
            backgroundColour = theme_->getButtonBackgroundColourOff();
        }
        else if (isDown)
        {
            backgroundColour = theme_->getButtonBackgroundColourClicked();
        }
        else if (isHighlighted)
        {
            backgroundColour = theme_->getButtonBackgroundColourHoover();
        }
        else
        {
            backgroundColour = theme_->getButtonBackgroundColourOn();
        }
        
        g.setColour(backgroundColour);
        g.fillRect(bounds);
    }

    void Button::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        juce::Colour borderColour;
        
        if (!enabled)
        {
            borderColour = theme_->getButtonBorderColourOff();
        }
        else
        {
            borderColour = theme_->getButtonBorderColourOn();
        }
        
        g.setColour(borderColour);
        g.drawRect(bounds, kBorderThickness_);
    }

    void Button::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isHighlighted, bool isDown)
    {
        const auto buttonText = getButtonText();
        if (buttonText.isEmpty())
        {
            return;
        }

        juce::Colour textColour;
        
        if (!enabled)
        {
            textColour = theme_->getButtonTextColourOff();
        }
        else if (isDown)
        {
            textColour = theme_->getButtonTextColourClicked();
        }
        else if (isHighlighted)
        {
            textColour = theme_->getButtonTextColourHoover();
        }
        else
        {
            textColour = theme_->getButtonTextColourOn();
        }

        const auto font = theme_->getBaseFont();

        g.setColour(textColour);
        g.setFont(font);
        g.drawText(buttonText, bounds, juce::Justification::centred, false);
    }
}

