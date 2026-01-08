#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class ModuleHeader : public juce::Component
    {
    public:
        enum class ModuleWidth
        {
            PatchEdit,
            MasterEdit,
            PatchManager
        };

        enum class ColourVariant
        {
            Blue,
            Orange
        };

        explicit ModuleHeader(Theme& inTheme, const juce::String& inText = juce::String(), ModuleWidth width = ModuleWidth::PatchEdit, ColourVariant variant = ColourVariant::Blue);
        ~ModuleHeader() override = default;

        void setTheme(Theme& inTheme);
        
        void setText(const juce::String& newText);
        juce::String getText() const { return text; }

        void paint(juce::Graphics& g) override;

        static constexpr int getWidth(ModuleWidth width)
        {
            switch (width)
            {
                case ModuleWidth::PatchEdit:
                    return kPatchEditWidth;
                case ModuleWidth::MasterEdit:
                    return kMasterEditWidth;
                case ModuleWidth::PatchManager:
                    return kPatchManagerWidth;
                default:
                    return kPatchEditWidth;
            }
        }

        static constexpr int getHeight() { return kHeight; }

    private:
        inline constexpr static int kPatchEditWidth = 150;
        inline constexpr static int kMasterEditWidth = 160;
        inline constexpr static int kPatchManagerWidth = 275;
        inline constexpr static int kHeight = 30;
        
        inline constexpr static float kTextLeftPadding = 2.0f;
        inline constexpr static float kTextAreaHeight = 20.0f;
        inline constexpr static float kLineThickness = 4.0f;

        Theme* theme = nullptr;
        juce::String text;
        ColourVariant colourVariant;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        
        juce::Colour getLineColour() const;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleHeader)
    };
}

