#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Skin;

    class ParameterSeparator : public juce::Component
    {
    public:
        explicit ParameterSeparator(Skin& newSkin);
        ~ParameterSeparator() override = default;

        void setSkin(Skin& newSkin);

        void paint(juce::Graphics& g) override;

    private:
        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);

        Skin* skin = nullptr;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterSeparator)
    };
}

