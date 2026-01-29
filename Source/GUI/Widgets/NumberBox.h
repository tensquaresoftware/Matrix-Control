#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class NumberBox : public juce::Component
    {
    public:
        explicit NumberBox(Theme& theme, int width, bool editable);
        ~NumberBox() override = default;

        void setTheme(Theme& theme);

        void setValue(int newValue);
        int getValue() const { return currentValue_; }

        void setShowDot(bool show);
        bool getShowDot() const { return showDot_; }

        void paint(juce::Graphics& g) override;
        void mouseDoubleClick(const juce::MouseEvent& e) override;

        static constexpr int getHeight() { return kHeight_; }

    private:
        inline constexpr static int kHeight_ = 20;
        inline constexpr static int kBorderThickness_ = 2;
        inline constexpr static float kDotRadius_ = 1.5f;
        inline constexpr static float kDotXOffset_ = 3.0f;
        inline constexpr static float kEditorFontSizeIncrease_ = 4.0f;

        Theme* theme_ = nullptr;
        int currentValue_ = 0;
        bool editable_ = false;
        bool showDot_ = false;
        std::unique_ptr<juce::TextEditor> editor_;

        juce::Colour getBorderColour() const;
        float calculateTextWidth(const juce::String& text) const;
        juce::Point<float> calculateDotPosition(const juce::Rectangle<float>& bounds, float textWidth) const;
        
        void showEditor();
        void hideEditor();
        void handleEditorReturn();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NumberBox)
    };
}
