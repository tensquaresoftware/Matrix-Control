#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class ModulationBusHeader : public juce::Component
    {
    public:
        enum class ColourVariant
        {
            Blue,
            Orange
        };

        explicit ModulationBusHeader(Theme& inTheme, ColourVariant variant = ColourVariant::Blue);
        ~ModulationBusHeader() override = default;

        void setTheme(Theme& inTheme);

        void paint(juce::Graphics& g) override;

        static constexpr int getWidth() { return kWidth; }
        static constexpr int getHeight() { return kHeight; }

    private:
        inline constexpr static int kWidth = 275;
        inline constexpr static int kHeight = 30;
        
        inline constexpr static int kBusNumberTextWidth = 15;
        inline constexpr static int kBusSourceTextWidth = 65;
        inline constexpr static int kBusAmountTextWidth = 65;
        inline constexpr static int kBusDestinationTextWidth = 110;
                
        inline constexpr static float kTextLeftPadding = 2.0f;
        inline constexpr static float kTextAreaHeight = 20.0f;
        inline constexpr static float kLineThickness = 4.0f;

        Theme* theme = nullptr;
        juce::String busNumberText;
        juce::String busSourceText;
        juce::String busAmountText;
        juce::String busDestinationText;
        ColourVariant colourVariant;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        
        void drawBusNumberText(juce::Graphics& g, float x, float y);
        void drawBusSourceText(juce::Graphics& g, float x, float y);
        void drawBusAmountText(juce::Graphics& g, float x, float y);
        void drawBusDestinationText(juce::Graphics& g, float x, float y);
        
        juce::Colour getLineColour() const;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationBusHeader)
    };
}

