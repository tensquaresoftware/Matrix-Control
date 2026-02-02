#include "Button.h"

#include <map>

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
        invalidateCache();
        repaint();
    }

    void Button::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        if (theme_ == nullptr)
            return;

        if (!cacheValid_)
            regenerateCache();

        const auto state = getCurrentState(isEnabled(), shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
        const auto it = cachedImages_.find(state);

        if (it != cachedImages_.end() && it->second.isValid())
        {
            g.drawImage(it->second, getLocalBounds().toFloat(),
                       juce::RectanglePlacement::fillDestination);
        }
    }

    void Button::resized()
    {
        invalidateCache();
    }

    void Button::regenerateCache()
    {
        const auto width = getWidth();
        const auto height = getHeight();

        if (width <= 0 || height <= 0)
            return;

        const float pixelScale = getPixelScale();
        const int imageWidth = juce::roundToInt(width * pixelScale);
        const int imageHeight = juce::roundToInt(height * pixelScale);

        // Pre-render all button states
        cachedImages_.clear();

        for (auto state : {ButtonState::Normal, ButtonState::Hover, ButtonState::Pressed, ButtonState::Disabled})
        {
            juce::Image stateImage(juce::Image::ARGB, imageWidth, imageHeight, true);
            juce::Graphics g(stateImage);
            g.addTransform(juce::AffineTransform::scale(pixelScale));

            renderButtonState(g, state);

            cachedImages_[state] = std::move(stateImage);
        }

        cacheValid_ = true;
    }

    void Button::invalidateCache()
    {
        cacheValid_ = false;
    }

    float Button::getPixelScale() const
    {
        const auto* display = juce::Desktop::getInstance()
                                  .getDisplays()
                                  .getDisplayForRect(getScreenBounds());
        const float displayScale = display != nullptr ? static_cast<float>(display->scale) : 1.0f;
        return displayScale;
    }

    Button::ButtonState Button::getCurrentState(bool enabled, bool isHighlighted, bool isDown) const
    {
        if (!enabled)
            return ButtonState::Disabled;
        if (isDown)
            return ButtonState::Pressed;
        if (isHighlighted)
            return ButtonState::Hover;
        return ButtonState::Normal;
    }

    void Button::renderButtonState(juce::Graphics& g, ButtonState state)
    {
        const auto bounds = juce::Rectangle<float>(0.0f, 0.0f, 
                                                    static_cast<float>(width_), 
                                                    static_cast<float>(height_));
        const auto buttonText = getButtonText();

        bool enabled = (state != ButtonState::Disabled);
        bool isHighlighted = (state == ButtonState::Hover);
        bool isDown = (state == ButtonState::Pressed);

        g.setColour(getBackgroundColour(enabled, isHighlighted, isDown));
        g.fillRect(bounds);

        g.setColour(getBorderColour(enabled));
        g.drawRect(bounds, kBorderThickness_);

        if (!buttonText.isEmpty())
        {
            g.setColour(getTextColour(enabled, isHighlighted, isDown));
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

