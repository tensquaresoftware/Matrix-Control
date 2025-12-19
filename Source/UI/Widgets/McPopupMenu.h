#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class McTheme;
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
    static constexpr int kBorderThickness = 1;
    static constexpr int kSeparatorWidth = 1;
    static constexpr int kTextLeftPadding = 3;
    static constexpr int kColumnThreshold = 10;

    void calculateColumnLayout();
    int calculateColumnCount(int totalItems) const;
    int calculateItemsPerColumn(int totalItems, int columnCount) const;
    
    juce::Point<int> calculatePopupPosition() const;
    int calculatePopupX(int popupWidth) const;
    int calculatePopupY(int popupHeight) const;
    
    juce::Rectangle<int> getItemBounds(int itemIndex) const;
    int getItemIndexAt(int x, int y) const;
    int getColumnFromX(int x) const;
    int getRowFromY(int y) const;
    int getItemIndexFromColumnAndRow(int column, int row) const;
    
    void updateHighlightedItem(int itemIndex);
    void selectItem(int itemIndex);
    
    void drawBase(juce::Graphics& g, const juce::Rectangle<int>& bounds);
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

    McComboBox& comboBox;
    McTheme* mcTheme = nullptr;
    int highlightedItemIndex = -1;
    juce::Font cachedFont;
    
    int columnCount = 1;
    int itemsPerColumn = 0;
    int columnWidth = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(McPopupMenu)
};
