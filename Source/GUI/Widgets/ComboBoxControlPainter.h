#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "GUI/Looks/WidgetLooks.h"

namespace TSS
{
    struct ComboBoxControlMetrics
    {
        static constexpr int kDefaultWidth = 100;
        static constexpr int kDefaultHeight = 20;
        static constexpr int kLeftPadding = 4;
        static constexpr int kRightPadding = 4;
        static constexpr int kBorderThickness = 1;
        static constexpr int kBorderThicknessButtonLike = 2;
        static constexpr int kTriangleBaseSize = 7;
        static constexpr float kTriangleHeightFactor = 0.8660254f;
    };

    enum class ComboBoxControlStyle
    {
        Standard,
        ButtonLike
    };

    class ComboBoxControlPainter
    {
    public:
        struct ClosedStateArgs
        {
            const juce::Component& component;
            juce::Rectangle<float> bounds;
            ComboBoxControlStyle style = ComboBoxControlStyle::Standard;
            const ComboBoxLook& look;
            float uiScale = 1.0f;
            juce::String text;
            bool enabled = true;
            bool hasFocus = false;
        };

        static void paintClosedState(juce::Graphics& g, const ClosedStateArgs& args);
    };
}
