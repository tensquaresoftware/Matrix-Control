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

        explicit ModulationBusHeader(Theme& inTheme, int width, int height, ColourVariant variant = ColourVariant::Blue);
        ~ModulationBusHeader() override = default;

        void setTheme(Theme& inTheme);

        void paint(juce::Graphics& g) override;

    private:
        inline constexpr static int kBusNumberTextWidth_ = 15;
        inline constexpr static int kBusSourceTextWidth_ = 65;
        inline constexpr static int kBusAmountTextWidth_ = 65;
        inline constexpr static int kBusDestinationTextWidth_ = 110;
                
        inline constexpr static int kTextLeftPadding_ = 2;
        inline constexpr static int kTextAreaHeight_ = 20;
        inline constexpr static int kLineThickness_ = 4;

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

