#include "ComboBoxControlPainter.h"

#include "GUI/Layout/ScaledDrawing.h"

namespace TSS
{
    namespace
    {
        juce::Path createTrianglePath(float x, float y, float baseSize)
        {
            juce::Path path;
            const auto height = baseSize * ComboBoxControlMetrics::kTriangleHeightFactor;

            path.startNewSubPath(x, y);
            path.lineTo(x + baseSize, y);
            path.lineTo(x + baseSize * 0.5f, y + height);
            path.closeSubPath();

            return path;
        }

        juce::Colour textColourForStyle(ComboBoxControlStyle style, const ComboBoxLook& look, bool enabled)
        {
            if (style == ComboBoxControlStyle::ButtonLike)
                return enabled ? look.buttonLikeText : look.buttonLikeTextDisabled;

            return enabled ? look.textEnabled : look.textDisabled;
        }

        juce::Colour triangleColourForStyle(ComboBoxControlStyle style, const ComboBoxLook& look, bool enabled)
        {
            if (style == ComboBoxControlStyle::ButtonLike)
                return enabled ? look.buttonLikeTriangle : look.buttonLikeTriangleDisabled;

            return enabled ? look.triangleEnabled : look.triangleDisabled;
        }

        juce::Colour backgroundColourForStyle(ComboBoxControlStyle style, const ComboBoxLook& look, bool enabled)
        {
            if (style == ComboBoxControlStyle::ButtonLike)
                return enabled ? look.buttonLikeBackground : look.buttonLikeBackgroundDisabled;

            return enabled ? look.backgroundEnabled : look.backgroundDisabled;
        }

        juce::Colour borderColourForStyle(ComboBoxControlStyle style, const ComboBoxLook& look, bool enabled)
        {
            if (style == ComboBoxControlStyle::ButtonLike)
            {
                juce::ignoreUnused(enabled);
                // Disabled border matches the enabled ButtonLike border.
                return look.buttonLikeBorder;
            }

            return enabled ? look.borderEnabled : look.borderDisabled;
        }

        juce::Rectangle<float> calculateTextBounds(juce::Rectangle<float> bounds, float uiScale)
        {
            const float leftPad = static_cast<float>(ComboBoxControlMetrics::kLeftPadding) * uiScale;
            const float triangleSpace = static_cast<float>(ComboBoxControlMetrics::kTriangleBaseSize) * uiScale;
            const float rightPad = static_cast<float>(ComboBoxControlMetrics::kRightPadding) * uiScale;

            bounds.removeFromLeft(leftPad);
            bounds.removeFromRight(triangleSpace);
            bounds.removeFromRight(rightPad);
            return bounds;
        }
    }

    void ComboBoxControlPainter::paintClosedState(juce::Graphics& g, const ClosedStateArgs& args)
    {
        const auto backgroundBounds = args.bounds;
        g.setColour(backgroundColourForStyle(args.style, args.look, args.enabled));
        g.fillRect(backgroundBounds);

        const auto textBounds = calculateTextBounds(args.bounds, args.uiScale);
        g.setColour(textColourForStyle(args.style, args.look, args.enabled));
        g.setFont(args.look.font.withHeight(args.look.font.getHeight() * args.uiScale));
        g.drawText(args.text, textBounds, juce::Justification::centredLeft, false);

        g.setColour(triangleColourForStyle(args.style, args.look, args.enabled));
        const float triangleBaseSize = static_cast<float>(ComboBoxControlMetrics::kTriangleBaseSize) * args.uiScale;
        const float triangleHeight = triangleBaseSize * ComboBoxControlMetrics::kTriangleHeightFactor;
        const float rightPad = static_cast<float>(ComboBoxControlMetrics::kRightPadding) * args.uiScale;
        const float triangleX = args.bounds.getRight() - triangleBaseSize - rightPad;
        const float triangleY = args.bounds.getCentreY() - triangleHeight * 0.5f;
        g.fillPath(createTrianglePath(triangleX, triangleY, triangleBaseSize));

        const float systemDisplayScale = ScaledDrawing::systemDisplayScaleForComponent(args.component);

        if (args.style == ComboBoxControlStyle::ButtonLike)
        {
            const float thickness = ScaledDrawing::snappedStrokeThicknessFromDesign(
                static_cast<float>(ComboBoxControlMetrics::kBorderThicknessButtonLike),
                args.uiScale,
                systemDisplayScale,
                ScaledDrawing::StrokeSnapPolicy::kRound);
            g.setColour(borderColourForStyle(args.style, args.look, args.enabled));
            g.drawRect(args.bounds, thickness);
            return;
        }

        if (args.hasFocus)
        {
            const float thickness = ScaledDrawing::snappedStrokeThicknessFromDesign(
                static_cast<float>(ComboBoxControlMetrics::kBorderThickness),
                args.uiScale,
                systemDisplayScale,
                ScaledDrawing::StrokeSnapPolicy::kRound);
            g.setColour(args.look.focusBorder);
            g.drawRect(backgroundBounds, thickness);
        }
    }
}
