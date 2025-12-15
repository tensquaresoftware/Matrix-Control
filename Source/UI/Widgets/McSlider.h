#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "McFocusableWidget.h"

class McLookAndFeel;

class McSlider : public juce::Slider
{
public:
    explicit McSlider(double defaultValueValue = 0.0);
    ~McSlider() override = default;

    void setLookAndFeel(McLookAndFeel* lookAndFeel);

    void paint(juce::Graphics& g) override;

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseDoubleClick(const juce::MouseEvent& e) override;

    void focusGained(juce::Component::FocusChangeType cause) override;
    void focusLost(juce::Component::FocusChangeType cause) override;

    bool keyPressed(const juce::KeyPress& key) override;

private:
    static constexpr int kDefaultWidth = 60;
    static constexpr int kDefaultHeight = 20;
    static constexpr double kDragSensitivity = 0.5;
    static constexpr double kShiftKeyStep = 10.0;
    static constexpr int kBorderMargin = 1;
    static constexpr int kTrackAreaMargin = 1;

    void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
    void drawTrack(juce::Graphics& g, const juce::Rectangle<float>& trackAreaBounds, bool enabled);
    void drawValueText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
    
    juce::Rectangle<float> getBorderBounds(const juce::Rectangle<float>& bounds) const;
    juce::Rectangle<float> getBackgroundBounds(const juce::Rectangle<float>& borderBounds) const;
    juce::Rectangle<float> getTrackAreaBounds(const juce::Rectangle<float>& backgroundBounds) const;
    
    double calculateStepForRange(double rangeLength, bool isShiftPressed) const;
    bool isIncrementKey(int keyCode) const;
    bool isDecrementKey(int keyCode) const;
    void updateValueWithStep(double step, bool increment);
    void resetToDefaultValue();

    McLookAndFeel* mcLookAndFeel = nullptr;
    McFocusableWidget focusableWidget;
    
    double defaultValue = 0.0;
    double dragStartValue = 0.0;
    juce::Point<int> dragStartPosition;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(McSlider)
};
