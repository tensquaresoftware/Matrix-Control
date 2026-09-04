#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "GUI/Looks/WidgetLooks.h"

namespace TSS
{
    class NumberBox : public juce::Component
    {
    public:
        using ValueChangedCallback = std::function<void(int)>;

        // What the box shows: a value, the "coordinates unknown" placeholder, or nothing at all
        // for devices where this coordinate does not exist (Matrix-6/6R bank).
        enum class DisplayState
        {
            kValue,
            kUndefined,
            kUnavailable
        };

        explicit NumberBox(int width, int height, const NumberBoxLook& look, bool editable, int minValue, int maxValue);
        ~NumberBox() override = default;

        void setLook(const NumberBoxLook& look);
        void setUiScale(float uiScale);

        void setValue(int newValue);
        int getValue() const { return currentValue_; }

        void setRange(int minValue, int maxValue);

        void setOnValueChanged(ValueChangedCallback callback);

        void setDisplayState(DisplayState state);
        DisplayState getDisplayState() const { return displayState_; }

        // Navigation focus highlight — red text, matching the PATCH NAME display.
        void setFocusHighlight(bool focused);
        bool hasFocusHighlight() const { return focusHighlight_; }

        void paint(juce::Graphics& g) override;
        void resized() override;
        void mouseDoubleClick(const juce::MouseEvent& e) override;

        int getHeight() const { return height_; }

    private:
        inline constexpr static int kDefaultHeight_ = 20;
        inline constexpr static int kBorderThickness_ = 2;
        inline constexpr static float kEditorFontSizeIncrease_ = 4.0f;

        NumberBoxLook look_{};
        int height_ {kDefaultHeight_};
        int currentValue_ = 0;
        int minValue_ = 0;
        int maxValue_ = 99;
        bool editable_ = false;
        DisplayState displayState_ = DisplayState::kValue;
        bool focusHighlight_ = false;
        float uiScale_ = 1.0f;
        std::unique_ptr<juce::TextEditor> editor_;
        ValueChangedCallback onValueChanged_;

        juce::String cachedValueText_;

        void updateValueText();
        int digitCount() const;

        juce::Colour getBorderColour() const;
        juce::Colour getTextColour() const;

        void showEditor();
        void hideEditor();
        void handleEditorReturn();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NumberBox)
    };
}
