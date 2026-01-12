#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class Label : public juce::Component
    {
    public:
        enum class LabelWidth
        {
            PatchEditModule,
            MasterEditModule,
            ModulationBusNumber,
            PatchManagerBankSelector
        };

        explicit Label(Theme& inTheme, LabelWidth inWidth, const juce::String& text = juce::String());
        ~Label() override = default;

        void setTheme(Theme& inTheme);
        
        void setText(const juce::String& newText);
        juce::String getText() const { return labelText; }

        void paint(juce::Graphics& g) override;

        static constexpr int getWidth(LabelWidth width)
        {
            switch (width)
            {
                case LabelWidth::ModulationBusNumber:
                    return kBusNumberWidth;
                case LabelWidth::PatchManagerBankSelector:
                    return kPatchManagerBankSelectWidth;
                case LabelWidth::PatchEditModule:
                    return kPatchEditModuleWidth;
                case LabelWidth::MasterEditModule:
                    return kMasterEditModuleWidth;
                default:
                    return kPatchEditModuleWidth;
            }
        }

        static constexpr int getHeight() { return kHeight; }

    private:
        inline constexpr static int kBusNumberWidth = 15;
        inline constexpr static int kPatchManagerBankSelectWidth = 75;
        inline constexpr static int kPatchEditModuleWidth = 90;
        inline constexpr static int kMasterEditModuleWidth = 100;
        inline constexpr static int kHeight = 20;
        inline constexpr static float kBackgroundPadding = 1.0f;
        inline constexpr static float kTextLeftPadding = 2.0f;

        LabelWidth width;
        Theme* theme = nullptr;
        juce::String labelText;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Label)
    };
}

