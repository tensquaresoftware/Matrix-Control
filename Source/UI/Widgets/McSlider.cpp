#include "McSlider.h"
#include "../LookAndFeel/McLookAndFeel.h"

McSlider::McSlider(double defaultValueValue)
    : juce::Slider(juce::Slider::LinearBarVertical, juce::Slider::NoTextBox)
    , defaultValue(defaultValueValue)
{
    setSize(60, 20);
    setWantsKeyboardFocus(true);
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

    // Border rectangle (60x20)
    auto borderBounds = bounds;
    
    // Background rectangle (58x18, centered)
    auto backgroundBounds = borderBounds.reduced(1.0f);
    
    // Draw border
    if (hasFocus)
    {
        g.setColour(juce::Colour(0xFF06471C));
    }
    else
    {
        g.setColour(juce::Colours::transparentBlack);
    }
    g.drawRect(borderBounds, 1.0f);

    // Draw background
    auto backgroundColour = mcLookAndFeel->getSliderBackgroundColour(enabled);
    g.setColour(backgroundColour);
    g.fillRect(backgroundBounds);

    // Track area (56x16, centered within background)
    auto trackAreaBounds = backgroundBounds.reduced(1.0f);
    
    auto range = getRange();
    auto rangeLength = range.getLength();
    
    if (rangeLength > 0.0)
    {
        auto value = getValue();
        auto normalizedValue = static_cast<float>((value - range.getStart()) / rangeLength);
        normalizedValue = juce::jlimit(0.0f, 1.0f, normalizedValue);

        auto trackWidth = trackAreaBounds.getWidth() * normalizedValue;
        auto trackBounds = trackAreaBounds.withWidth(trackWidth);

        auto trackColour = mcLookAndFeel->getSliderTrackColour(enabled);
        g.setColour(trackColour);
        g.fillRect(trackBounds);
    }

    auto valueText = juce::String(static_cast<int>(std::round(getValue())));
    auto textColour = mcLookAndFeel->getSliderTextColour(enabled);
    auto font = mcLookAndFeel->getDefaultFont();

    g.setColour(textColour);
    g.setFont(font);
    g.drawText(valueText, bounds, juce::Justification::centred, false);
}

void McSlider::mouseDown(const juce::MouseEvent& e)
{
    hasFocus = true;
    repaint();
    grabKeyboardFocus();
    dragStartValue = getValue();
    dragStartPosition = e.getPosition();
}

void McSlider::mouseDrag(const juce::MouseEvent& e)
{
    auto dragDistance = dragStartPosition.y - e.getPosition().y;
    auto sensitivity = 0.5;
    auto valueDelta = dragDistance * sensitivity;
    
    auto range = getRange();
    auto newValue = dragStartValue + valueDelta;
    newValue = juce::jlimit(range.getStart(), range.getEnd(), newValue);
    
    setValue(newValue, juce::sendNotificationSync);
}

void McSlider::mouseUp(const juce::MouseEvent&)
{
    if (! hasKeyboardFocus(true))
    {
        hasFocus = false;
        repaint();
    }
}

void McSlider::mouseDoubleClick(const juce::MouseEvent&)
{
    setValue(defaultValue, juce::sendNotificationSync);
}

void McSlider::focusGained(juce::Component::FocusChangeType)
{
    hasFocus = true;
    repaint();
}

void McSlider::focusLost(juce::Component::FocusChangeType)
{
    hasFocus = false;
    repaint();
}
