#include "Slider.h"

#include "../Skin/Skin.h"
#include "../Skin/SkinDimensions.h"

namespace tss
{
    Slider::Slider(Skin& newSkin, double initValue)
        : juce::Slider(juce::Slider::LinearBarVertical, juce::Slider::NoTextBox)
        , skin(&newSkin)
        , defaultValue(initValue)
    {
        setSize(SkinDimensions::Widget::Slider::kWidth, SkinDimensions::Widget::Slider::kHeight);
        setWantsKeyboardFocus(true);
        setInterceptsMouseClicks(true, false);
    }

    void Slider::setSkin(Skin& newSkin)
    {
        skin = &newSkin;
        repaint();
    }

    void Slider::paint(juce::Graphics& g)
    {
        if (skin == nullptr)
        {
            return;
        }

        auto bounds = getLocalBounds().toFloat();
        auto enabled = isEnabled();
        auto hasFocus = focusableWidget.hasFocus();

        auto backgroundBounds = calculateBackgroundBounds(bounds);
        auto trackBounds = calculateTrackBounds(backgroundBounds, enabled);

        drawBase(g, bounds);
        drawBackground(g, backgroundBounds, enabled);
        drawBorder(g, bounds, backgroundBounds, enabled, hasFocus);
        drawTrack(g, trackBounds, enabled);
        drawText(g, bounds, enabled);
    }

    juce::Rectangle<float> Slider::calculateBackgroundBounds(const juce::Rectangle<float>& bounds) const
    {
        auto backgroundWidth = static_cast<float>(skin->getSliderBackgroundWidth());
        auto backgroundHeight = static_cast<float>(skin->getSliderBackgroundHeight());
        auto backgroundX = (bounds.getWidth() - backgroundWidth) / 2.0f;
        auto backgroundY = (bounds.getHeight() - backgroundHeight) / 2.0f;
        return juce::Rectangle<float>(bounds.getX() + backgroundX, bounds.getY() + backgroundY, backgroundWidth, backgroundHeight);
    }

    juce::Rectangle<float> Slider::calculateTrackBounds(const juce::Rectangle<float>& backgroundBounds, bool enabled) const
    {
        if (! enabled)
        {
            return juce::Rectangle<float>();
        }

        auto range = getRange();
        auto rangeLength = range.getLength();
        
        if (rangeLength <= 0.0)
        {
            return juce::Rectangle<float>();
        }

        auto trackArea = backgroundBounds.reduced(1.0f);
        auto value = getValue();
        auto normalizedValue = static_cast<float>((value - range.getStart()) / rangeLength);
        normalizedValue = juce::jlimit(0.0f, 1.0f, normalizedValue);
        auto trackWidth = trackArea.getWidth() * normalizedValue;
        
        return juce::Rectangle<float>(trackArea.getX(), trackArea.getY(), trackWidth, trackArea.getHeight());
    }

    void Slider::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto baseColour = skin->getSliderBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void Slider::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        auto backgroundColour = skin->getSliderBackgroundColour(enabled);
        g.setColour(backgroundColour);
        g.fillRect(bounds);
    }

    void Slider::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, const juce::Rectangle<float>& backgroundBounds, bool enabled, bool hasFocus)
    {
        auto borderColour = skin->getSliderBorderColour(enabled, false);
        g.setColour(borderColour);
        g.drawRect(bounds, 1.0f);

        if (hasFocus)
        {
            auto focusBorderColour = skin->getSliderBorderColour(enabled, true);
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

        auto trackColour = skin->getSliderTrackColour(enabled);
        g.setColour(trackColour);
        g.fillRect(bounds);
    }

    void Slider::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        auto valueText = juce::String(static_cast<int>(std::round(getValue())));
        auto textColour = skin->getSliderTextColour(enabled);
        auto font = skin->getSliderFont();

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
        
        auto dragDistance = dragStartPosition.y - e.getPosition().y;
        auto valueDelta = dragDistance * skin->getSliderDragSensitivity();
        
        auto range = getRange();
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
        
        auto range = getRange();
        auto rangeLength = range.getLength();
        auto modifiers = key.getModifiers();
        bool isShiftPressed = modifiers.isShiftDown();
        auto step = calculateStepForRange(rangeLength, isShiftPressed);
        
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
            return skin->getSliderShiftKeyStep();
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
        auto range = getRange();
        auto currentValue = getValue();
        auto newValue = increment ? currentValue + step : currentValue - step;
        newValue = juce::jlimit(range.getStart(), range.getEnd(), newValue);
        setValue(newValue, juce::sendNotificationSync);
    }
}

