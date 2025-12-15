#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class McLookAndFeel;
class McComboBox;

class McPopupMenu : public juce::Component
{
public:
    explicit McPopupMenu(McComboBox& comboBox);
    ~McPopupMenu() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void mouseMove(const juce::MouseEvent& e) override;
    void mouseExit(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    
    void inputAttemptWhenModal() override;
    bool keyPressed(const juce::KeyPress& key) override;

    static void show(McComboBox& comboBox);

private:
    static constexpr int kItemHeight = 20;
    static constexpr int kBorderSize = 1;
    static constexpr int kTextLeftMargin = 4;
    static constexpr int kMaxVisibleRows = 12;
    static constexpr int kColumnSpacing = 2;

    void updateHighlightedItem(int itemIndex);
    int getItemIndexAt(int x, int y) const;
    void selectItem(int itemIndex);
    static juce::Point<int> calculatePopupPosition(McComboBox& comboBox, int popupWidth, int popupHeight);
    
    int calculateOptimalColumnCount(int totalItems, int availableWidth) const;
    void calculateColumnLayout();
    int getItemColumn(int itemIndex) const;
    int getItemRowInColumn(int itemIndex) const;
    juce::Rectangle<int> getItemBounds(int itemIndex) const;
    
    void initializeMultiColumnLayout(int numItems, int comboWidth);
    void initializeSingleColumnLayout(int numItems, int comboWidth);
    void initializeHighlightedItem(int selectedIndex);
    
    void drawAllItems(juce::Graphics& g, const juce::Rectangle<int>& contentBounds) const;
    void drawItem(juce::Graphics& g, int itemIndex, const juce::Rectangle<int>& itemBounds) const;
    void drawItemHighlight(juce::Graphics& g, const juce::Rectangle<int>& itemBounds, int column) const;
    void drawVerticalSeparators(juce::Graphics& g, const juce::Rectangle<int>& contentBounds) const;
    void drawPopupBorder(juce::Graphics& g, const juce::Rectangle<int>& bounds) const;
    
    int findColumnFromXPosition(int relativeX) const;
    int findRowFromYPosition(int relativeY) const;
    int calculateItemIndexFromColumnAndRow(int column, int row) const;
    int findItemIndexInSingleColumn(int relativeY) const;
    
    static int calculatePopupXPosition(McComboBox&, int popupWidth, const juce::Rectangle<int>& parentBounds, const juce::Point<int>& comboBoxPosition, const juce::Rectangle<int>& comboBoxBounds);
    static int calculatePopupYPosition(McComboBox&, int popupHeight, const juce::Rectangle<int>& parentBounds, const juce::Point<int>& comboBoxPosition, const juce::Rectangle<int>& comboBoxBounds);
    static int clampPopupXToParentBounds(int x, int popupWidth, const juce::Rectangle<int>& parentBounds);
    static int clampPopupYToParentBounds(int y, int popupHeight, const juce::Rectangle<int>& parentBounds);
    
    void handleMultiColumnNavigation(const juce::KeyPress& key);
    void handleSingleColumnNavigation(const juce::KeyPress& key);
    void navigateUpInColumn();
    void navigateDownInColumn();
    void navigateToPreviousColumn();
    void navigateToNextColumn();
    void navigateUpInSingleColumn();
    void navigateDownInSingleColumn();
    
    void recalculateColumnLayoutIfNeeded();
    
    static int calculateCeilingDivision(int dividend, int divisor);
    
    bool isValidItemIndex(int itemIndex) const;
    bool hasValidLookAndFeel() const;
    bool hasValidParent() const;
    bool isPositionInsideBounds(int relativeX, int relativeY, const juce::Rectangle<int>& bounds) const;
    bool isValidRow(int row) const;
    bool hasItems() const;
    bool hasHighlightedItem() const;
    bool canNavigateUp() const;
    bool canNavigateDown() const;
    bool canNavigateToPreviousColumn() const;
    bool canNavigateToNextColumn() const;
    
    juce::Rectangle<int> calculateHighlightBoundsForItem(const juce::Rectangle<int>& itemBounds, int column) const;

    McComboBox& comboBox;
    McLookAndFeel* mcLookAndFeel = nullptr;
    int highlightedItemIndex = -1;
    juce::Font cachedFont;
    
    // Multi-column layout data
    int columnCount = 1;
    int itemsPerColumn = 0;
    int columnWidth = 0;
    int totalPopupWidth = 0;
    bool useMultiColumn = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(McPopupMenu)
};
