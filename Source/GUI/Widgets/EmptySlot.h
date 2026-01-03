#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class EmptySlot : public juce::Component
    {
    public:
        explicit EmptySlot(Theme& newTheme);
        ~EmptySlot() override = default;

        void setTheme(Theme& newTheme);

        void paint(juce::Graphics& g) override;

    private:
        inline constexpr static int kWidth = 150;
        inline constexpr static int kHeight = 20;

        Theme* theme = nullptr;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EmptySlot)
    };
}

