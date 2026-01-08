#include "Slider.h"

#include "../Themes/Theme.h"

namespace tss
{
    Slider::Slider(Theme& inTheme, double initValue)
        : juce::Slider(juce::Slider::LinearBarVertical, juce::Slider::NoTextBox)
        , theme(&inTheme)
        , defaultValue(initValue)
    {
        setSize(kWidth, kHeight);
        setWantsKeyboardFocus(true);
        setInterceptsMouseClicks(true, false);
    }

    void Slider::setTheme(Theme& inTheme)
    {
        theme = &inTheme;
        repaint();
    }

    void Slider::setUnit(const juce::String& inUnit)
    {
        unit = inUnit;
        repaint();
    }

    juce::String Slider::getUnit() const
    {
        return unit;
    }

    void Slider::paint(juce::Graphics& g)
    {
        if (theme == nullptr)
        {
            return;
        }

        const auto bounds = getLocalBounds().toFloat();
        const auto enabled = isEnabled();
        const auto hasFocus = focusableWidget.hasFocus();

        const auto backgroundBounds = calculateBackgroundBounds(bounds);
        const auto trackBounds = calculateTrackBounds(backgroundBounds, enabled);

        drawBase(g, bounds);
        drawBackground(g, backgroundBounds, enabled);
        drawBorder(g, bounds, backgroundBounds, enabled, hasFocus);
        drawTrack(g, trackBounds, enabled);
        drawText(g, bounds, enabled);
    }

    juce::Rectangle<float> Slider::calculateBackgroundBounds(const juce::Rectangle<float>& bounds) const
    {
        const auto backgroundWidth = static_cast<float>(kBackgroundWidth);
        const auto backgroundHeight = static_cast<float>(kBackgroundHeight);
        const auto backgroundX = (bounds.getWidth() - backgroundWidth) / 2.0f;
        const auto backgroundY = (bounds.getHeight() - backgroundHeight) / 2.0f;
        return juce::Rectangle<float>(bounds.getX() + backgroundX, bounds.getY() + backgroundY, backgroundWidth, backgroundHeight);
    }

    juce::Rectangle<float> Slider::calculateTrackBounds(const juce::Rectangle<float>& backgroundBounds, bool enabled) const
    {
        if (! enabled)
        {
            return juce::Rectangle<float>();
        }

        const auto range = getRange();
        const auto rangeLength = range.getLength();
        
        if (rangeLength <= 0.0)
        {
            return juce::Rectangle<float>();
        }

        const auto trackArea = backgroundBounds.reduced(1.0f);
        const auto value = getValue();
        auto normalizedValue = static_cast<float>((value - range.getStart()) / rangeLength);
        normalizedValue = juce::jlimit(0.0f, 1.0f, normalizedValue);
        const auto trackWidth = trackArea.getWidth() * normalizedValue;
        
        return juce::Rectangle<float>(trackArea.getX(), trackArea.getY(), trackWidth, trackArea.getHeight());
    }

    void Slider::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto baseColour = theme->getSliderBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void Slider::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        const auto backgroundColour = theme->getSliderBackgroundColour(enabled);
        g.setColour(backgroundColour);
        g.fillRect(bounds);
    }

    void Slider::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, const juce::Rectangle<float>& backgroundBounds, bool enabled, bool hasFocus)
    {
        const auto borderColour = theme->getSliderBorderColour(enabled, false);
        g.setColour(borderColour);
        g.drawRect(bounds, 1.0f);

        if (hasFocus)
        {
            const auto focusBorderColour = theme->getSliderBorderColour(enabled, true);
            g.setColour(focusBorderColour);
            g.drawRect(backgroundBounds, 1.0f);
        }
    }

    void Slider::drawTrack(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        if (bounds.isEmpty())
        {
            return;
        }

        const auto trackColour = theme->getSliderTrackColour(enabled);
        g.setColour(trackColour);
        g.fillRect(bounds);
    }

    void Slider::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        auto valueText = juce::String(static_cast<int>(std::round(getValue())));
        
        if (unit.isNotEmpty())
        {
            valueText += " " + unit;
        }
        
        const auto textColour = theme->getSliderTextColour(enabled);
        const auto font = theme->getBaseFont();

        g.setColour(textColour);
        g.setFont(font);
        g.drawText(valueText, bounds, juce::Justification::centred, false);
    }

    void Slider::mouseDown(const juce::MouseEvent& e)
    {
        if (! isEnabled())
        {
            return;
        }
        
        grabKeyboardFocus();
        dragStartValue = getValue();
        dragStartPosition = e.getPosition();
    }

    void Slider::mouseDrag(const juce::MouseEvent& e)
    {
        if (! isEnabled())
        {
            return;
        }
        
        const auto dragDistance = dragStartPosition.y - e.getPosition().y;
        const auto valueDelta = dragDistance * kDragSensitivity;
        
        const auto range = getRange();
        auto newValue = dragStartValue + valueDelta;
        newValue = juce::jlimit(range.getStart(), range.getEnd(), newValue);
        
        setValue(newValue, juce::sendNotificationSync);
    }

    void Slider::mouseUp(const juce::MouseEvent&)
    {
        // Focus is handled by focusGained/focusLost callbacks
    }

    void Slider::mouseDoubleClick(const juce::MouseEvent&)
    {
        if (! isEnabled())
        {
            return;
        }
        
        resetToDefaultValue();
    }

    void Slider::resetToDefaultValue()
    {
        setValue(defaultValue, juce::sendNotificationSync);
    }

    void Slider::focusGained(juce::Component::FocusChangeType)
    {
        focusableWidget.handleFocusGained(this);
    }

    void Slider::focusLost(juce::Component::FocusChangeType)
    {
        focusableWidget.handleFocusLost(this);
    }

    bool Slider::keyPressed(const juce::KeyPress& key)
    {
        if (! isEnabled() || ! hasKeyboardFocus(true))
        {
            return false;
        }
        
        if (key == juce::KeyPress::returnKey)
        {
            resetToDefaultValue();
            return true;
        }
        
        const auto range = getRange();
        const auto rangeLength = range.getLength();
        const auto modifiers = key.getModifiers();
        const bool isShiftPressed = modifiers.isShiftDown();
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
            return kShiftKeyStep;
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

