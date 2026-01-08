#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "FocusableWidget.h"

namespace tss
{
    class Theme;
    class PopupMenu;

    class ComboBox : public juce::ComboBox
    {
    public:
        enum class ComboBoxWidth
        {
            PatchEditModule,
            MasterEditModule,
            MatrixModulationAmount,
            MatrixModulationDestination
        };

        explicit ComboBox(Theme& inTheme, ComboBoxWidth width = ComboBoxWidth::PatchEditModule);
        ~ComboBox() override = default;

        void setTheme(Theme& inTheme);

        void paint(juce::Graphics& g) override;
        void showPopup() override;

        void mouseDown(const juce::MouseEvent& e) override;

        void focusGained(juce::Component::FocusChangeType cause) override;
        void focusLost(juce::Component::FocusChangeType cause) override;

        static constexpr int getWidth(ComboBoxWidth width)
        {
            switch (width)
            {
                case ComboBoxWidth::PatchEditModule:
                case ComboBoxWidth::MasterEditModule:
                case ComboBoxWidth::MatrixModulationAmount:
                    return kNormalWidth;
                case ComboBoxWidth::MatrixModulationDestination:
                    return kLargeWidth;
                default:
                    return kNormalWidth;
            }
        }

        static constexpr int getHeight() { return kHeight; }
        static constexpr int getVerticalMargin() { return kVerticalMargin; }

    private:
        inline constexpr static int kNormalWidth = 60;
        inline constexpr static int kLargeWidth = 105;
        inline constexpr static int kHeight = 20;

        inline constexpr static int kNormalBackgroundWidth = 60;
        inline constexpr static int kLargeBackgroundWidth = 105;
        inline constexpr static int kBackgroundHeight = 16;
        
        inline constexpr static float kLeftPadding = 3.0f;
        inline constexpr static float kRightPadding = 3.0f;
        inline constexpr static float kBorderThickness = 1.0f;
        inline constexpr static int kVerticalMargin = 4;
        inline constexpr static float kTriangleBaseSize = 7.0f;
        inline constexpr static float kTriangleHeightFactor = 0.8660254f; // √3 / 2

        Theme* theme = nullptr;
        FocusableWidget focusableWidget;
        ComboBoxWidth comboWidth;
        bool isPopupOpen = false;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
        void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, const juce::Rectangle<float>& backgroundBounds, bool enabled, bool hasFocus);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
        void drawTriangle(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
        juce::Path createTrianglePath(float x, float y, float baseSize) const;
        
        juce::Rectangle<float> calculateBackgroundBounds(const juce::Rectangle<float>& bounds) const;

        friend class PopupMenu;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComboBox)
    };
}

