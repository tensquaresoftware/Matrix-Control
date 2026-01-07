#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class Label : public juce::Component
    {
    public:
        enum class Type
        {
            Parameter,
            BusNumber
        };

        explicit Label(Theme& inTheme, Type inType, const juce::String& text = juce::String());
        ~Label() override = default;

        void setTheme(Theme& inTheme);
        
        void setText(const juce::String& newText);
        juce::String getText() const { return labelText; }

        void paint(juce::Graphics& g) override;

        static constexpr int getWidth(Type inType)
        {
            return inType == Type::Parameter ? kParameterWidth : kBusNumberWidth;
        }
        static constexpr int getHeight() { return kHeight; }

    private:
        inline constexpr static int kParameterWidth = 90;
        inline constexpr static int kBusNumberWidth = 15;
        inline constexpr static int kHeight = 20;
        inline constexpr static float kBackgroundPadding = 1.0f;
        inline constexpr static float kTextLeftPadding = 2.0f;

        Type type;
        Theme* theme = nullptr;
        juce::String labelText;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Label)
    };

    // Type aliases pour faciliter l'utilisation
    using ParameterLabel = Label;
    using ModulationBusLabel = Label;
}

