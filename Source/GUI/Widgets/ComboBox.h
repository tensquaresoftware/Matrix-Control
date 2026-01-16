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
        explicit ComboBox(Theme& theme, int width, int height);
        ~ComboBox() override = default;

        void setTheme(Theme& theme);

        void paint(juce::Graphics& g) override;
        void showPopup() override;

        void mouseDown(const juce::MouseEvent& e) override;

        void focusGained(juce::Component::FocusChangeType cause) override;
        void focusLost(juce::Component::FocusChangeType cause) override;

        int getWidth() const { return width_; }
        int getHeight() const { return height_; }
        static constexpr int getVerticalMargin() { return kVerticalMargin_; }

    private:
        inline constexpr static int kVerticalMargin_ = 4;
        inline constexpr static int kBackgroundHeight_ = 16;
        inline constexpr static int kLeftPadding_ = 3;
        inline constexpr static int kRightPadding_ = 3;
        inline constexpr static int kBorderThickness_ = 1;
        inline constexpr static int kTriangleBaseSize_ = 7;
        inline constexpr static float kTriangleHeightFactor_ = 0.8660254f;

        Theme* theme_ = nullptr;
        FocusableWidget focusableWidget_;
        int width_;
        int height_;
        bool isPopupOpen_ = false;

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

