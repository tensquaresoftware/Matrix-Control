#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class HorizontalSeparator : public juce::Component
    {
    public:
        enum class SeparatorWidth
        {
            PatchEditModule,
            MasterEditModule,
            MatrixModulationBus
        };

        HorizontalSeparator(Theme& inTheme, SeparatorWidth width);
        ~HorizontalSeparator() override = default;

        void setTheme(Theme& inTheme);

        void paint(juce::Graphics& g) override;

        int getWidth() const;
        static constexpr int getHeight() { return kHeight; }

        static constexpr int getWidth(SeparatorWidth width)
        {
            switch (width)
            {
                case SeparatorWidth::PatchEditModule:
                    return kPatchEditModuleWidth;
                case SeparatorWidth::MasterEditModule:
                    return kMasterEditModuleWidth;
                case SeparatorWidth::MatrixModulationBus:
                    return kMatrixModulationBusWidth;
                default:
                    return kPatchEditModuleWidth;
            }
        }

    private:
        inline constexpr static int kPatchEditModuleWidth = 150;
        inline constexpr static int kMasterEditModuleWidth = 160;
        inline constexpr static int kMatrixModulationBusWidth = 275;
        inline constexpr static int kHeight = 5;
        inline constexpr static float kLineThickness = 1.0f;

        SeparatorWidth separatorWidth;
        Theme* theme = nullptr;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HorizontalSeparator)
    };
}

