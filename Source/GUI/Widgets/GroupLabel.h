#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Skin;

    class GroupLabel : public juce::Component
    {
    public:
        explicit GroupLabel(Skin& newSkin, int width, const juce::String& text = juce::String());
        ~GroupLabel() override = default;

        void setSkin(Skin& newSkin);

        void setText(const juce::String& newText);
        juce::String getText() const { return labelText; }

        void paint(juce::Graphics& g) override;

    private:
        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawLeftLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawRightLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        juce::Rectangle<float> getContentArea(const juce::Rectangle<float>& bounds) const;
        float calculateTextWidth() const;

        Skin* skin = nullptr;
        juce::String labelText;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GroupLabel)
    };
}

