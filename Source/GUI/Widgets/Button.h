#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Skin;

    class Button : public juce::Button
    {
    public:
        explicit Button(Skin& newSkin, int width, const juce::String& text = {});
        ~Button() override = default;

        void setSkin(Skin& newSkin);
        
        void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    private:
        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isHighlighted, bool isDown);
        void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isHighlighted, bool isDown);

        Skin* skin = nullptr;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Button)
    };
}

