#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class ParameterSeparator : public juce::Component
    {
    public:
        enum class Type
        {
            PatchEditModule,
            MasterEditModule,
            MatrixModulationBus
        };

        ParameterSeparator(Theme& inTheme, Type inType);
        ~ParameterSeparator() override = default;

        void setTheme(Theme& inTheme);

        void paint(juce::Graphics& g) override;

        int getWidth() const;
        static constexpr int getHeight() { return kHeight; }

        static int getWidth(Type inType)
        {
            switch (inType)
            {
                case Type::PatchEditModule:
                    return kPatchEditModuleWidth;
                case Type::MasterEditModule:
                    return kMasterEditModuleWidth;
                case Type::MatrixModulationBus:
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

        Type type;
        Theme* theme = nullptr;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterSeparator)
    };
}

