#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "McFocusableWidget.h"

class McTheme;

class McSlider : public juce::Slider
{
public:
    explicit McSlider(double defaultValueValue = 0.0);
    ~McSlider() override = default;

    void setTheme(McTheme* theme);

    void paint(juce::Graphics& g) override;

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent&) override;
    void mouseDoubleClick(const juce::MouseEvent&) override;

    void focusGained(juce::Component::FocusChangeType cause) override;
    void focusLost(juce::Component::FocusChangeType cause) override;

    bool keyPressed(const juce::KeyPress& key) override;

private:
    static constexpr int kWidth              = 60;
    static constexpr int kHeight             = 20;
    static constexpr double kDragSensitivity = 0.5;
    static constexpr double kShiftKeyStep    = 10.0;

    void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
    void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool hasFocus);
    void drawTrack(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
    void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
    
    juce::Rectangle<float> calculateTrackBounds(const juce::Rectangle<float>& backgroundBounds, bool enabled) const;
    
    double calculateStepForRange(double rangeLength, bool isShiftPressed) const;
    bool isIncrementKey(int keyCode) const;
    bool isDecrementKey(int keyCode) const;
    void updateValueWithStep(double step, bool increment);
    void resetToDefaultValue();

    McTheme* mcTheme = nullptr;
    McFocusableWidget focusableWidget;
    
    double defaultValue = 0.0;
    double dragStartValue = 0.0;
    juce::Point<int> dragStartPosition;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(McSlider)
};
