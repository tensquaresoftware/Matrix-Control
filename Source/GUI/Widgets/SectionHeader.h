#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class SectionHeader : public juce::Component
    {
    public:
        enum class SectionWidth
        {
            PatchEdit,
            MatrixModulation,
            PatchManager,
            MasterEdit
        };

        enum class ColourVariant
        {
            Blue,
            Orange
        };

        explicit SectionHeader(Theme& inTheme, SectionWidth width, const juce::String& inText = juce::String(), ColourVariant variant = ColourVariant::Blue);
        ~SectionHeader() override = default;

        void setTheme(Theme& inTheme);

        void paint(juce::Graphics& g) override;

        static constexpr int getWidth(SectionWidth width)
        {
            switch (width)
            {
                case SectionWidth::PatchEdit:
                    return kPatchEditWidth;
                case SectionWidth::MatrixModulation:
                case SectionWidth::PatchManager:
                    return kMatrixModulationWidth;
                case SectionWidth::MasterEdit:
                    return kMasterEditWidth;
                default:
                    return kPatchEditWidth;
            }
        }

        static constexpr int getHeight() { return kHeight; }

    private:
        inline constexpr static int kPatchEditWidth = 810;
        inline constexpr static int kMatrixModulationWidth = 275;
        inline constexpr static int kMasterEditWidth = 160;
        inline constexpr static int kHeight = 30;
        inline constexpr static float kContentHeight = 20.0f;
        inline constexpr static float kLineHeight = 12.0f;
        inline constexpr static float kLeftLineWidth = 20.0f;
        inline constexpr static float kTextSpacing = 8.0f;

        Theme* theme = nullptr;
        juce::String text;
        ColourVariant colourVariant;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawContentArea(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawLeftLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawRightLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        
        juce::Colour getLineColour() const;
        juce::Rectangle<float> getContentArea(const juce::Rectangle<float>& bounds) const;
        float calculateTextWidth() const;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SectionHeader)
    };
}

