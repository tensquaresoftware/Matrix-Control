#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

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

private:
    McLookAndFeel* mcLookAndFeel = nullptr;
    
    double defaultValue = 0.0;
    double dragStartValue = 0.0;
    juce::Point<int> dragStartPosition;
    bool hasFocus = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(McSlider)
};
