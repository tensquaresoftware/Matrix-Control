#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "FocusableWidget.h"

namespace tss
{
    class Theme;

    class Slider : public juce::Slider
    {
    public:
        explicit Slider(Theme& inTheme, double initValue = 0.0);
        ~Slider() override = default;

        void setTheme(Theme& inTheme);

        void setUnit(const juce::String& inUnit);
        juce::String getUnit() const;

        void paint(juce::Graphics& g) override;

        void mouseDown(const juce::MouseEvent& e) override;
        void mouseDrag(const juce::MouseEvent& e) override;
        void mouseUp(const juce::MouseEvent&) override;
        void mouseDoubleClick(const juce::MouseEvent&) override;

        void focusGained(juce::Component::FocusChangeType cause) override;
        void focusLost(juce::Component::FocusChangeType cause) override;

        bool keyPressed(const juce::KeyPress& key) override;

        static constexpr int getWidth() { return kWidth; }
        static constexpr int getHeight() { return kHeight; }

    private:
        inline constexpr static int kWidth = 60;
        inline constexpr static int kHeight = 20;
        inline constexpr static int kBackgroundWidth = 60;
        inline constexpr static int kBackgroundHeight = 16;
        inline constexpr static double kDragSensitivity = 0.5;
        inline constexpr static double kShiftKeyStep = 10.0;

        Theme* theme = nullptr;
        FocusableWidget focusableWidget;
        
        double defaultValue = 0.0;
        double dragStartValue = 0.0;
        juce::Point<int> dragStartPosition;
        juce::String unit;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
        void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, const juce::Rectangle<float>& backgroundBounds, bool enabled, bool hasFocus);
        void drawTrack(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
        
        juce::Rectangle<float> calculateBackgroundBounds(const juce::Rectangle<float>& bounds) const;
        juce::Rectangle<float> calculateTrackBounds(const juce::Rectangle<float>& backgroundBounds, bool enabled) const;
        
        double calculateStepForRange(double rangeLength, bool isShiftPressed) const;
        bool isIncrementKey(int keyCode) const;
        bool isDecrementKey(int keyCode) const;
        void updateValueWithStep(double step, bool increment);
        void resetToDefaultValue();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Slider)
    };
}

