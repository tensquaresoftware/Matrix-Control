#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Skin;

    class ParameterLabel : public juce::Component
    {
    public:
        explicit ParameterLabel(Skin& newSkin, const juce::String& text = juce::String());
        ~ParameterLabel() override = default;

        void setSkin(Skin& newSkin);
        
        void setText(const juce::String& newText);
        juce::String getText() const { return labelText; }

        void paint(juce::Graphics& g) override;

    private:
        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);

        Skin* skin = nullptr;
        juce::String labelText;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterLabel)
    };
}

