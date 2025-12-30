#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Skin;

    class SectionName : public juce::Component
    {
    public:
        enum class ColourVariant
        {
            Blue,
            Orange
        };

        explicit SectionName(Skin& newSkin, int width, const juce::String& text = juce::String(), ColourVariant variant = ColourVariant::Blue);
        ~SectionName() override = default;

        void setSkin(Skin& newSkin);

        void paint(juce::Graphics& g) override;

    private:
        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawContentArea(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawLeftLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawRightLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        
        juce::Colour getLineColour() const;
        juce::Rectangle<float> getContentArea(const juce::Rectangle<float>& bounds) const;
        float calculateTextWidth() const;

        Skin* skin = nullptr;
        juce::String name;
        ColourVariant colourVariant;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SectionName)
    };
}

