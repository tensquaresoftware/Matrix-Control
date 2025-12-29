#include "Button.h"

#include "../Skin/Skin.h"
#include "../Skin/SkinDimensions.h"

namespace tss
{
    Button::Button(Skin& newSkin, int width, const juce::String& text)
        : juce::Button(text)
        , skin(&newSkin)
    {
        setSize(width, SkinDimensions::Widget::Button::kHeight);
    }

    void Button::setSkin(Skin& newSkin)
    {
        skin = &newSkin;
        repaint();
    }

    void Button::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        if (skin == nullptr)
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
        auto baseColour = skin->getButtonBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void Button::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isHighlighted, bool isDown)
    {
        juce::Colour backgroundColour;
        
        if (!enabled)
        {
            backgroundColour = skin->getButtonBackgroundColourOff();
        }
        else if (isDown)
        {
            backgroundColour = skin->getButtonBackgroundColourClicked();
        }
        else if (isHighlighted)
        {
            backgroundColour = skin->getButtonBackgroundColourHoover();
        }
        else
        {
            backgroundColour = skin->getButtonBackgroundColourOn();
        }
        
        g.setColour(backgroundColour);
        g.fillRect(bounds);
    }

    void Button::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        juce::Colour borderColour;
        
        if (!enabled)
        {
            borderColour = skin->getButtonBorderColourOff();
        }
        else
        {
            borderColour = skin->getButtonBorderColourOn();
        }
        
        g.setColour(borderColour);
        
        auto borderThickness = skin->getButtonBorderThickness();
        
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
            textColour = skin->getButtonTextColourOff();
        }
        else if (isDown)
        {
            textColour = skin->getButtonTextColourClicked();
        }
        else if (isHighlighted)
        {
            textColour = skin->getButtonTextColourHoover();
        }
        else
        {
            textColour = skin->getButtonTextColourOn();
        }

        auto font = skin->getButtonFont();

        g.setColour(textColour);
        g.setFont(font);
        g.drawText(buttonText, bounds, juce::Justification::centred, false);
    }
}

