#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Skin;
    class ComboBox;

    class PopupMenuRenderer
    {
    public:
        PopupMenuRenderer(Skin& skin, bool isButtonLike);

        void drawBackground(juce::Graphics& g, const juce::Rectangle<int>& bounds) const;
        void drawBorder(juce::Graphics& g, const juce::Rectangle<int>& bounds) const;
        void drawItem(juce::Graphics& g, const ComboBox& comboBox, int itemIndex, 
                     const juce::Rectangle<int>& itemBounds, int highlightedItemIndex, 
                     const juce::Font& font) const;
        void drawVerticalSeparators(juce::Graphics& g, const juce::Rectangle<int>& contentBounds,
                                   int columnCount, int columnWidth, int separatorWidth) const;

    private:
        inline constexpr static int kTextLeftPadding_ = 3;

        Skin& skin_;
        bool isButtonLike_;
    };
}
