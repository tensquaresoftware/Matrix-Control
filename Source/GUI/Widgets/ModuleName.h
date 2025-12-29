#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Skin;

    class ModuleName : public juce::Component
    {
    public:
        enum class Size
        {
            Normal,
            Large
        };

        enum class ColourVariant
        {
            Blue,
            Orange
        };

        explicit ModuleName(Skin& newSkin, const juce::String& text = juce::String(), Size size = Size::Normal, ColourVariant variant = ColourVariant::Blue);
        ~ModuleName() override = default;

        void setSkin(Skin& newSkin);
        
        void setText(const juce::String& newText);
        juce::String getText() const { return name; }

        void paint(juce::Graphics& g) override;

    private:
        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        
        juce::Colour getLineColour() const;

        Skin* skin = nullptr;
        juce::String name;
        ColourVariant colourVariant;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleName)
    };
}

