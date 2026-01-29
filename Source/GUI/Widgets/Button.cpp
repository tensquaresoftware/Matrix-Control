#include "Button.h"

#include "GUI/Themes/Theme.h"

namespace tss
{
    Button::Button(Theme& theme, int width, int height, const juce::String& text)
        : juce::Button(text)
        , theme_(&theme)
        , width_(width)
        , height_(height)
    {
        setOpaque(true);
        setSize(width_, height_);
    }

    void Button::setTheme(Theme& theme)
    {
        theme_ = &theme;
    }

    void Button::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        if (theme_ == nullptr)
            return;

        const auto bounds = getLocalBounds().toFloat();
        const auto enabled = isEnabled();
        const auto buttonText = getButtonText();

        g.setColour(getBackgroundColour(enabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown));
        g.fillRect(bounds);

        g.setColour(getBorderColour(enabled));
        g.drawRect(bounds, kBorderThickness_);

        if (!buttonText.isEmpty())
        {
            g.setColour(getTextColour(enabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown));
            g.setFont(theme_->getBaseFont());
            g.drawText(buttonText, bounds, juce::Justification::centred, false);
        }
    }

    juce::Colour Button::getBackgroundColour(bool enabled, bool isHighlighted, bool isDown) const
    {
        if (!enabled)
            return theme_->getButtonBackgroundColourOff();

        if (isDown)
            return theme_->getButtonBackgroundColourClicked();

        if (isHighlighted)
            return theme_->getButtonBackgroundColourHoover();

        return theme_->getButtonBackgroundColourOn();
    }

    juce::Colour Button::getBorderColour(bool enabled) const
    {
        if (!enabled)
            return theme_->getButtonBorderColourOff();

        return theme_->getButtonBorderColourOn();
    }

    juce::Colour Button::getTextColour(bool enabled, bool isHighlighted, bool isDown) const
    {
        if (!enabled)
            return theme_->getButtonTextColourOff();

        if (isDown)
            return theme_->getButtonTextColourClicked();

        if (isHighlighted)
            return theme_->getButtonTextColourHoover();

        return theme_->getButtonTextColourOn();
    }
}

