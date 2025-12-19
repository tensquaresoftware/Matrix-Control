#include "McSlider.h"
#include "../Themes/McTheme.h"

McSlider::McSlider(double defaultValueValue)
    : juce::Slider(juce::Slider::LinearBarVertical, juce::Slider::NoTextBox)
    , defaultValue(defaultValueValue)
{
    setSize(kWidth, kHeight);
    setWantsKeyboardFocus(true);
    setInterceptsMouseClicks(true, false);
}

void McSlider::setTheme(McTheme* theme)
{
    mcTheme = theme;
}

void McSlider::paint(juce::Graphics& g)
{
    if (mcTheme == nullptr)
    {
        return;
    }

    auto bounds = getLocalBounds().toFloat();
    auto enabled = isEnabled();
    auto hasFocus = focusableWidget.hasFocus();

    auto backgroundBounds = bounds.reduced(1.0f);
    auto trackBounds = calculateTrackBounds(backgroundBounds, enabled);

    drawBase(g, bounds);
    drawBackground(g, backgroundBounds, enabled);
    drawBorder(g, bounds, enabled, hasFocus);
    drawTrack(g, trackBounds, enabled);
    drawText(g, bounds, enabled);
}

juce::Rectangle<float> McSlider::calculateTrackBounds(const juce::Rectangle<float>& backgroundBounds, bool enabled) const
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

void McSlider::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto baseColour = mcTheme->getSliderBaseColour();
    g.setColour(baseColour);
    g.fillRect(bounds);
}

void McSlider::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
{
    auto backgroundColour = mcTheme->getSliderBackgroundColour(enabled);
    g.setColour(backgroundColour);
    g.fillRect(bounds);
}

void McSlider::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool hasFocus)
{
    auto borderColour = mcTheme->getSliderBorderColour(enabled, hasFocus);
    g.setColour(borderColour);
    g.drawRect(bounds, 1.0f);
}

void McSlider::drawTrack(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
{
    if (bounds.isEmpty())
    {
        return;
    }

    auto trackColour = mcTheme->getSliderTrackColour(enabled);
    g.setColour(trackColour);
    g.fillRect(bounds);
}

void McSlider::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
{
    auto valueText = juce::String(static_cast<int>(std::round(getValue())));
    auto textColour = mcTheme->getSliderTextColour(enabled);
    auto font = mcTheme->getDefaultFont();

    g.setColour(textColour);
    g.setFont(font);
    g.drawText(valueText, bounds, juce::Justification::centred, false);
}

void McSlider::mouseDown(const juce::MouseEvent& e)
{
    if (! isEnabled())
    {
        return;
    }
    
    grabKeyboardFocus();
    dragStartValue = getValue();
    dragStartPosition = e.getPosition();
}

void McSlider::mouseDrag(const juce::MouseEvent& e)
{
    if (! isEnabled())
    {
        return;
    }
    
    auto dragDistance = dragStartPosition.y - e.getPosition().y;
    auto valueDelta = dragDistance * kDragSensitivity;
    
    auto range = getRange();
    auto newValue = dragStartValue + valueDelta;
    newValue = juce::jlimit(range.getStart(), range.getEnd(), newValue);
    
    setValue(newValue, juce::sendNotificationSync);
}

void McSlider::mouseUp(const juce::MouseEvent&)
{
    // Focus is handled by focusGained/focusLost callbacks
}

void McSlider::mouseDoubleClick(const juce::MouseEvent&)
{
    if (! isEnabled())
    {
        return;
    }
    
    resetToDefaultValue();
}

void McSlider::resetToDefaultValue()
{
    setValue(defaultValue, juce::sendNotificationSync);
}

void McSlider::focusGained(juce::Component::FocusChangeType)
{
    focusableWidget.handleFocusGained(this);
}

void McSlider::focusLost(juce::Component::FocusChangeType)
{
    focusableWidget.handleFocusLost(this);
}

bool McSlider::keyPressed(const juce::KeyPress& key)
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

double McSlider::calculateStepForRange(double rangeLength, bool isShiftPressed) const
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

bool McSlider::isIncrementKey(int keyCode) const
{
    return keyCode == juce::KeyPress::upKey || keyCode == juce::KeyPress::rightKey;
}

bool McSlider::isDecrementKey(int keyCode) const
{
    return keyCode == juce::KeyPress::downKey || keyCode == juce::KeyPress::leftKey;
}

void McSlider::updateValueWithStep(double step, bool increment)
{
    auto range = getRange();
    auto currentValue = getValue();
    auto newValue = increment ? currentValue + step : currentValue - step;
    newValue = juce::jlimit(range.getStart(), range.getEnd(), newValue);
    setValue(newValue, juce::sendNotificationSync);
}
