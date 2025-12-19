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

    enum class PopupDisplayMode
    {
        SingleColumn,
        MultiColumn
    };

    explicit McComboBox(Size size = Size::Normal);
    ~McComboBox() override = default;

    void setLookAndFeel(McTheme* themes);
    void setPopupDisplayMode(PopupDisplayMode mode);
    PopupDisplayMode getPopupDisplayMode() const { return popupDisplayMode; }

    void paint(juce::Graphics& g) override;

    void showPopup() override;

    void mouseDown(const juce::MouseEvent& e) override;

    void focusGained(juce::Component::FocusChangeType cause) override;
    void focusLost(juce::Component::FocusChangeType cause) override;

private:
    static constexpr int kDefaultHeight       = 20;
    static constexpr int kNormalWidth         = 60;
    static constexpr int kLargeWidth          = 105;
    static constexpr float kTriangleSideSize  = 7.0f;
    static constexpr int kTextLeftMargin      = 3;
    static constexpr int kTextRightMargin     = 3;
    static constexpr int kTriangleRightMargin = 3;
    static constexpr int kBorderMargin        = 1;

    void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
    void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
    void drawTriangle(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
    juce::Path createTrianglePath(float x, float y, float sideSize) const;

    McTheme* mcTheme = nullptr;
    McFocusableWidget focusableWidget;
    Size comboSize;
    PopupDisplayMode popupDisplayMode = PopupDisplayMode::SingleColumn;

    friend class McPopupMenu;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(McComboBox)
};



