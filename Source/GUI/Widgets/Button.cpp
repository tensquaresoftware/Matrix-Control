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
        invalidateCache();
    }

    void Button::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        if (theme_ == nullptr)
            return;

        if (!cacheValid_)
            regenerateStateCache();

        const auto enabled = isEnabled();
        const auto state = getCurrentState(enabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
        const auto stateIndex = static_cast<size_t>(state);

        if (cachedStates_[stateIndex].isValid())
            g.drawImageAt(cachedStates_[stateIndex], 0, 0);
    }
    
    void Button::resized()
    {
        invalidateCache();
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
    
    void Button::regenerateStateCache()
    {
        const auto width = getWidth();
        const auto height = getHeight();
        
        if (width <= 0 || height <= 0)
            return;
        
        const auto buttonText = getButtonText();
        const auto bounds = juce::Rectangle<float>(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
        
        for (size_t i = 0; i < 4; ++i)
        {
            const auto state = static_cast<ButtonState>(i);
            bool enabled = true;
            bool highlighted = false;
            bool down = false;
            
            switch (state)
            {
                case ButtonState::Normal:
                    enabled = true;
                    highlighted = false;
                    down = false;
                    break;
                case ButtonState::Hover:
                    enabled = true;
                    highlighted = true;
                    down = false;
                    break;
                case ButtonState::Down:
                    enabled = true;
                    highlighted = false;
                    down = true;
                    break;
                case ButtonState::Disabled:
                    enabled = false;
                    highlighted = false;
                    down = false;
                    break;
            }
            
            cachedStates_[i] = juce::Image(juce::Image::ARGB, width, height, true);
            juce::Graphics g(cachedStates_[i]);
            
            g.setColour(getBackgroundColour(enabled, highlighted, down));
            g.fillRect(bounds);
            
            g.setColour(getBorderColour(enabled));
            g.drawRect(bounds, kBorderThickness_);
            
            if (!buttonText.isEmpty())
            {
                g.setColour(getTextColour(enabled, highlighted, down));
                g.setFont(theme_->getBaseFont());
                g.drawText(buttonText, bounds, juce::Justification::centred, false);
            }
        }
        
        cacheValid_ = true;
    }
    
    void Button::invalidateCache()
    {
        cacheValid_ = false;
    }
    
    Button::ButtonState Button::getCurrentState(bool enabled, bool isHighlighted, bool isDown) const
    {
        if (!enabled)
            return ButtonState::Disabled;
        
        if (isDown)
            return ButtonState::Down;
        
        if (isHighlighted)
            return ButtonState::Hover;
        
        return ButtonState::Normal;
    }
}

