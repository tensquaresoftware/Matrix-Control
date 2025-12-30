#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "FocusableWidget.h"

namespace tss
{
    class Skin;
    class PopupMenu;

    class ComboBox : public juce::ComboBox
    {
    public:
        enum class Size
        {
            Normal,
            Large
        };

        explicit ComboBox(Skin& newSkin, Size size = Size::Normal);
        ~ComboBox() override = default;

        void setSkin(Skin& newSkin);

        void paint(juce::Graphics& g) override;
        void showPopup() override;

        void mouseDown(const juce::MouseEvent& e) override;

        void focusGained(juce::Component::FocusChangeType cause) override;
        void focusLost(juce::Component::FocusChangeType cause) override;

    private:
        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
        void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, const juce::Rectangle<float>& backgroundBounds, bool enabled, bool hasFocus);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
        void drawTriangle(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
        juce::Path createTrianglePath(float x, float y, float baseSize) const;
        
        juce::Rectangle<float> calculateBackgroundBounds(const juce::Rectangle<float>& bounds) const;

        Skin* skin = nullptr;
        FocusableWidget focusableWidget;
        Size comboSize;
        bool isPopupOpen = false;

        friend class PopupMenu;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComboBox)
    };
}

