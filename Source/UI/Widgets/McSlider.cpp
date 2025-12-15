#include "McSlider.h"
#include "../LookAndFeel/McLookAndFeel.h"

McSlider::McSlider(double defaultValueValue)
    : juce::Slider(juce::Slider::LinearBarVertical, juce::Slider::NoTextBox)
    , defaultValue(defaultValueValue)
{
    setSize(kDefaultWidth, kDefaultHeight);
    setWantsKeyboardFocus(true);
    setInterceptsMouseClicks(true, false);
}

void McSlider::setLookAndFeel(McLookAndFeel* lookAndFeel)
{
    mcLookAndFeel = lookAndFeel;
}

void McSlider::paint(juce::Graphics& g)
{
    if (mcLookAndFeel == nullptr)
    {
        return;
    }

    auto bounds = getLocalBounds().toFloat();
    auto enabled = isEnabled();

    auto borderBounds = getBorderBounds(bounds);
    auto backgroundBounds = getBackgroundBounds(borderBounds);
    auto trackAreaBounds = getTrackAreaBounds(backgroundBounds);

    drawBorder(g, borderBounds);
    drawBackground(g, backgroundBounds, enabled);
    drawTrack(g, trackAreaBounds, enabled);
    drawValueText(g, bounds, enabled);
}

juce::Rectangle<float> McSlider::getBorderBounds(const juce::Rectangle<float>& bounds) const
{
    return bounds;
}

juce::Rectangle<float> McSlider::getBackgroundBounds(const juce::Rectangle<float>& borderBounds) const
{
    return borderBounds.reduced(static_cast<float>(kBorderMargin));
}

juce::Rectangle<float> McSlider::getTrackAreaBounds(const juce::Rectangle<float>& backgroundBounds) const
{
    return backgroundBounds.reduced(static_cast<float>(kTrackAreaMargin));
}

void McSlider::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    focusableWidget.drawFocusBorder(g, bounds, mcLookAndFeel);
}

void McSlider::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
{
    auto backgroundColour = mcLookAndFeel->getSliderBackgroundColour(enabled);
    g.setColour(backgroundColour);
    g.fillRect(bounds);
}

void McSlider::drawTrack(juce::Graphics& g, const juce::Rectangle<float>& trackAreaBounds, bool enabled)
{
    auto range = getRange();
    auto rangeLength = range.getLength();
    
    if (rangeLength <= 0.0)
    {
        return;
    }

    auto value = getValue();
    auto normalizedValue = static_cast<float>((value - range.getStart()) / rangeLength);
    normalizedValue = juce::jlimit(0.0f, 1.0f, normalizedValue);

    auto trackWidth = trackAreaBounds.getWidth() * normalizedValue;
    auto trackBounds = trackAreaBounds.withWidth(trackWidth);

    auto trackColour = mcLookAndFeel->getSliderTrackColour(enabled);
    g.setColour(trackColour);
    g.fillRect(trackBounds);
}

void McSlider::drawValueText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
{
    auto valueText = juce::String(static_cast<int>(std::round(getValue())));
    auto textColour = mcLookAndFeel->getSliderTextColour(enabled);
    auto font = mcLookAndFeel->getDefaultFont();

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
