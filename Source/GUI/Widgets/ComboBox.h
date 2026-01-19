#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "FocusableWidget.h"

namespace tss
{
    class Theme;
    class PopupMenu;
}

// Include PopupMenu.h to use PopupMenu::DisplayMode in class declaration
#include "PopupMenu.h"

namespace tss
{

    class ComboBox : public juce::ComboBox
    {
    public:
        enum class Style
        {
            Standard,
            ButtonLike
        };

        explicit ComboBox(Theme& theme, int width, int height, Style style = Style::Standard);
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
        inline constexpr static int kLeftPadding_ = 4;
        inline constexpr static int kRightPadding_ = 4;
        inline constexpr static int kBorderThickness_ = 1;
        inline constexpr static int kBorderThicknessButtonLike_ = 2;
        inline constexpr static int kTriangleBaseSize_ = 7;
        inline constexpr static float kTriangleHeightFactor_ = 0.8660254f;

        Theme* theme_ = nullptr;
        FocusableWidget focusableWidget_;
        int width_;
        int height_;
        Style style_;
        bool isPopupOpen_ = false;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
        void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, const juce::Rectangle<float>& backgroundBounds, bool enabled, bool hasFocus);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
        void drawTriangle(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
        juce::Path createTrianglePath(float x, float y, float baseSize) const;
        
        juce::Rectangle<float> calculateBackgroundBounds(const juce::Rectangle<float>& bounds) const;

        struct BorderStyle
        {
            juce::Colour colour;
            float thickness;
        };
        BorderStyle getBorderColourAndThickness(bool enabled) const;
        void drawMainBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, const BorderStyle& borderStyle) const;
        void drawFocusBorderIfNeeded(juce::Graphics& g, const juce::Rectangle<float>& backgroundBounds, bool enabled, bool hasFocus, float borderThickness) const;

        juce::String getSelectedItemText() const;
        juce::Colour getTextColourForCurrentStyle(bool enabled) const;
        juce::Rectangle<float> calculateTextBounds(const juce::Rectangle<float>& bounds) const;
        void drawTextInBounds(juce::Graphics& g, const juce::String& text, const juce::Rectangle<float>& textBounds, const juce::Colour& textColour) const;

        bool canShowPopup() const;
        
        PopupMenu::DisplayMode determineDisplayMode() const;
        void showPopupAsynchronously(PopupMenu::DisplayMode displayMode);

        friend class PopupMenu;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComboBox)
    };
}
