#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "McFocusableWidget.h"

class McTheme;

class McComboBox : public juce::ComboBox
{
public:
    enum class Size
    {
        Normal,
        Large
    };

    explicit McComboBox(Size size = Size::Normal);
    ~McComboBox() override = default;

    void setTheme(McTheme* theme);

    void paint(juce::Graphics& g) override;
    void showPopup() override;

    void mouseDown(const juce::MouseEvent& e) override;

    void focusGained(juce::Component::FocusChangeType cause) override;
    void focusLost(juce::Component::FocusChangeType cause) override;

private:
    static constexpr int kNormalWidth        = 60;
    static constexpr int kLargeWidth         = 105;
    static constexpr int kHeight             = 20;
    static constexpr int kLeftPadding        = 3;
    static constexpr int kRightPadding       = 3;
    static constexpr int kBorderThickness    = 1;
    static constexpr int kVerticalMargin     = 4;
    static constexpr float kTriangleSideSize = 7.0f;

    void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
    void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool hasFocus);
    void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
    void drawTriangle(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
    juce::Path createTrianglePath(float x, float y, float sideSize) const;

    McTheme* mcTheme = nullptr;
    McFocusableWidget focusableWidget;
    Size comboSize;
    bool isPopupOpen = false;

    friend class McPopupMenu;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(McComboBox)
};
