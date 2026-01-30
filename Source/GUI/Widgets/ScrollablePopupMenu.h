#pragma once

#include "PopupMenuBase.h"

namespace tss
{
    class ScrollablePopupMenu : public PopupMenuBase
    {
    public:
        explicit ScrollablePopupMenu(ComboBox& comboBox);
        ~ScrollablePopupMenu() override;

        void paint(juce::Graphics& g) override;
        void resized() override;

        static void show(ComboBox& comboBox);

    protected:
        void handleKeyboardNavigation(const juce::KeyPress& key) override;

    private:
        class ScrollableViewportLookAndFeel;
        class ScrollableContentComponent;

        inline constexpr static int kMaxScrollableHeight_ = 300;

        int columnWidth_ = 0;
        int scrollableContentHeight_ = 0;
        
        std::unique_ptr<juce::Viewport> viewport_;
        std::unique_ptr<ScrollableContentComponent> contentComponent_;
        std::unique_ptr<ScrollableViewportLookAndFeel> scrollbarLookAndFeel_;

        void setupScrollableContent();
        
        juce::Rectangle<int> getItemBounds(int itemIndex) const;
        int getItemIndexAt(int x, int y) const;
        
        void drawItems(juce::Graphics& g);
        
        void navigateUp();
        void navigateDown();

        void navigateAndScroll(int delta);
        void scrollToHighlightedItem();

        friend class ScrollableContentComponent;
    };
}
