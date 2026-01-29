#include "Slider.h"

#include "GUI/Themes/Theme.h"

namespace tss
{
    Slider::Slider(Theme& theme, int width, int height, double defaultValue)
        : juce::Slider(juce::Slider::LinearBarVertical, juce::Slider::NoTextBox)
        , theme_(&theme)
        , width_(width)
        , height_(height)
        , defaultValue_(defaultValue)
    {
        setOpaque(false);
        setSize(width_, height_);
        setWantsKeyboardFocus(true);
        setInterceptsMouseClicks(true, false);
    }

    void Slider::setTheme(Theme& theme)
    {
        theme_ = &theme;
    }

    void Slider::setUnit(const juce::String& unit)
    {
        unit_ = unit;
        repaint();
    }

    juce::String Slider::getUnit() const
    {
        return unit_;
    }

    void Slider::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr)
            return;

        const auto bounds = getLocalBounds().toFloat();
        const auto enabled = isEnabled();
        const auto trackBounds = calculateTrackBounds(bounds);
        const auto valueBarBounds = calculateValueBarBounds(trackBounds, enabled);

        drawTrack(g, trackBounds, enabled);
        drawValueBar(g, valueBarBounds, enabled);
        drawText(g, bounds, enabled);
        drawFocusBorderIfNeeded(g, trackBounds, hasFocus_);
    }

    juce::Rectangle<float> Slider::calculateTrackBounds(const juce::Rectangle<float>& bounds) const
    {
        const auto trackHeight = static_cast<float>(kTrackHeight_);
        const auto trackY = (bounds.getHeight() - trackHeight) * 0.5f;
        return juce::Rectangle<float>(bounds.getX(), bounds.getY() + trackY, bounds.getWidth(), trackHeight);
    }

    juce::Rectangle<float> Slider::calculateValueBarBounds(const juce::Rectangle<float>& trackBounds, bool enabled) const
    {
        if (!enabled)
            return juce::Rectangle<float>();

        const auto range = getRange();
        const auto rangeLength = range.getLength();
        
        if (rangeLength <= 0.0)
            return juce::Rectangle<float>();

        const auto valueBarArea = trackBounds.reduced(1.0f);
        const auto value = getValue();
        auto normalizedValue = static_cast<float>((value - range.getStart()) / rangeLength);
        normalizedValue = juce::jlimit(0.0f, 1.0f, normalizedValue);
        const auto valueBarWidth = valueBarArea.getWidth() * normalizedValue;
        
        return juce::Rectangle<float>(valueBarArea.getX(), valueBarArea.getY(), valueBarWidth, valueBarArea.getHeight());
    }


    void Slider::drawFocusBorderIfNeeded(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool hasFocus)
    {
        if (hasFocus)
        {
            const auto focusBorderColour = theme_->getSliderFocusBorderColour();
            g.setColour(focusBorderColour);
            g.drawRect(bounds, 1.0f);
        }
    }

    void Slider::drawTrack(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        const auto trackColour = theme_->getSliderTrackColour(enabled);
        g.setColour(trackColour);
        g.fillRect(bounds);
    }

    void Slider::drawValueBar(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        if (bounds.isEmpty())
            return;

        const auto valueBarColour = theme_->getSliderValueBarColour(enabled);
        g.setColour(valueBarColour);
        g.fillRect(bounds);
    }

    void Slider::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        auto valueText = juce::String(static_cast<int>(std::round(getValue())));
        
        if (unit_.isNotEmpty())
            valueText += " " + unit_;
        
        const auto textColour = theme_->getSliderTextColour(enabled);
        const auto font = theme_->getBaseFont();

        g.setColour(textColour);
        g.setFont(font);
        g.drawText(valueText, bounds, juce::Justification::centred, false);
    }

    void Slider::mouseDown(const juce::MouseEvent& e)
    {
        if (!isEnabled())
            return;
        
        grabKeyboardFocus();
        dragStartValue_ = getValue();
        dragStartPosition_ = e.getPosition();
    }

    void Slider::mouseDrag(const juce::MouseEvent& e)
    {
        if (!isEnabled())
            return;
        
        const auto dragDistance = dragStartPosition_.y - e.getPosition().y;
        const auto valueDelta = dragDistance * kDragSensitivity_;
        const auto range = getRange();
        auto newValue = dragStartValue_ + valueDelta;
        newValue = juce::jlimit(range.getStart(), range.getEnd(), newValue);
        
        setValue(newValue, juce::sendNotificationSync);
    }

    void Slider::mouseUp(const juce::MouseEvent&)
    {
        // Focus is handled by focusGained/focusLost callbacks
    }

    void Slider::mouseDoubleClick(const juce::MouseEvent&)
    {
        if (!isEnabled())
            return;
        
        resetToDefaultValue();
    }

    void Slider::resetToDefaultValue()
    {
        setValue(defaultValue_, juce::sendNotificationSync);
    }

    void Slider::focusGained(juce::Component::FocusChangeType)
    {
        if (isEnabled())
        {
            hasFocus_ = true;
            repaint();
        }
    }

    void Slider::focusLost(juce::Component::FocusChangeType)
    {
        hasFocus_ = false;
        repaint();
    }

    bool Slider::keyPressed(const juce::KeyPress& key)
    {
        if (!isEnabled() || !hasKeyboardFocus(true))
            return false;
        
        if (key == juce::KeyPress::returnKey)
        {
            resetToDefaultValue();
            return true;
        }
        
        const auto range = getRange();
        const auto rangeLength = range.getLength();
        const bool isShiftPressed = key.getModifiers().isShiftDown();
        const auto step = calculateStepForRange(rangeLength, isShiftPressed);
        
        if (isIncrementKey(key.getKeyCode()))
        {
            updateValueWithStep(step, true);
            return true;
        }
        
        if (isDecrementKey(key.getKeyCode()))
        {
            updateValueWithStep(step, false);
            return true;
        }
        
        return false;
    }

    double Slider::calculateStepForRange(double rangeLength, bool isShiftPressed) const
    {
        if (isShiftPressed)
        {
            return kShiftKeyStep_;
        }
        
        if (rangeLength <= 100.0)
        {
            return 1.0;
        }
        
        return rangeLength / 100.0;
    }

    bool Slider::isIncrementKey(int keyCode) const
    {
        return keyCode == juce::KeyPress::upKey || keyCode == juce::KeyPress::rightKey;
    }

    bool Slider::isDecrementKey(int keyCode) const
    {
        return keyCode == juce::KeyPress::downKey || keyCode == juce::KeyPress::leftKey;
    }

    void Slider::updateValueWithStep(double step, bool increment)
    {
        const auto range = getRange();
        const auto currentValue = getValue();
        auto newValue = increment ? currentValue + step : currentValue - step;
        newValue = juce::jlimit(range.getStart(), range.getEnd(), newValue);
        setValue(newValue, juce::sendNotificationSync);
    }
}

