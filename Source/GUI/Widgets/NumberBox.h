#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class NumberBox : public juce::Component
    {
    public:
        explicit NumberBox(Theme& inTheme, int width, bool inEditable);
        ~NumberBox() override = default;

        void setTheme(Theme& inTheme);

        void setValue(int newValue);
        int getValue() const { return currentValue; }

        void setShowDot(bool show);
        bool getShowDot() const { return showDot; }

        void paint(juce::Graphics& g) override;
        void mouseDoubleClick(const juce::MouseEvent& e) override;

        static constexpr int getHeight() { return kHeight_; }

    private:
        inline constexpr static int kHeight_ = 20;
        inline constexpr static int kBorderThickness_ = 2;

        Theme* theme = nullptr;
        int currentValue = 0;
        bool editable = false;
        bool showDot = false;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawDot(juce::Graphics& g, const juce::Rectangle<float>& bounds);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NumberBox)
    };
}
