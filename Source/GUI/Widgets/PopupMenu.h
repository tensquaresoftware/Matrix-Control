#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;
    class ComboBox;

    class PopupMenu : public juce::Component
    {
    public:
        enum class DisplayMode
        {
            MultiColumn,
            SingleColumnScrollable
        };

        explicit PopupMenu(ComboBox& comboBox, DisplayMode displayMode = DisplayMode::MultiColumn);
        ~PopupMenu() override = default;

        static void show(ComboBox& comboBox, DisplayMode displayMode = DisplayMode::MultiColumn);

    private:
        class ScrollableContentComponent;

        void paint(juce::Graphics& g) override;
        void resized() override;

        void mouseMove(const juce::MouseEvent& e) override;
        void mouseExit(const juce::MouseEvent& e) override;
        void mouseUp(const juce::MouseEvent& e) override;
        
        void inputAttemptWhenModal() override;
        bool keyPressed(const juce::KeyPress& key) override;

    private:
        inline constexpr static int kItemHeight_ = 20;
        inline constexpr static float kBorderThickness_ = 1.0f;
        inline constexpr static int kSeparatorWidth_ = 1;
        inline constexpr static int kTextLeftPadding_ = 3;
        inline constexpr static int kColumnThreshold_ = 10;
        inline constexpr static int kMaxScrollableHeight_ = 300;

        ComboBox& comboBox_;
        Theme* theme_ = nullptr;
        DisplayMode displayMode_;
        int highlightedItemIndex_ = -1;
        juce::Font cachedFont_ { juce::FontOptions() };
        
        int columnCount_ = 1;
        int itemsPerColumn_ = 0;
        int columnWidth_ = 0;
        int scrollableContentHeight_ = 0;
        
        std::unique_ptr<juce::Viewport> viewport_;
        std::unique_ptr<ScrollableContentComponent> contentComponent_;

        void calculateColumnLayout();
        int calculateColumnCount(int totalItems) const;
        int calculateItemsPerColumn(int totalItems, int columnCount) const;
        void setupScrollableContent();
        
        juce::Point<int> calculatePopupPosition(juce::Component* topLevelComponent) const;
        juce::Component* findTopLevelComponent() const;
        
        juce::Rectangle<int> getItemBounds(int itemIndex) const;
        int getItemIndexAt(int x, int y) const;
        int getColumnFromX(int x) const;
        int getRowFromY(int y) const;
        int getItemIndexFromColumnAndRow(int column, int row) const;
        
        void updateHighlightedItem(int itemIndex);
        void selectItem(int itemIndex);
        
        void drawBackground(juce::Graphics& g, const juce::Rectangle<int>& bounds);
        void drawBorder(juce::Graphics& g, const juce::Rectangle<int>& bounds);
        void drawItems(juce::Graphics& g, const juce::Rectangle<int>& contentBounds);
        void drawItem(juce::Graphics& g, int itemIndex, const juce::Rectangle<int>& itemBounds);
        void drawVerticalSeparators(juce::Graphics& g, const juce::Rectangle<int>& contentBounds);
        
        void handleKeyboardNavigation(const juce::KeyPress& key);
        void navigateUp();
        void navigateDown();
        void navigateLeft();
        void navigateRight();
        
        bool isValidItemIndex(int itemIndex) const;
        bool hasValidLookAndFeel() const;
        bool hasValidParent() const;

        friend class ScrollableContentComponent;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PopupMenu)
    };
}

