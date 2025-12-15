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
    
    struct ItemComponent : public juce::Component
    {
        ItemComponent(McPopupMenu& parent, int itemIndex);
        ~ItemComponent() override = default;

        void paint(juce::Graphics& g) override;
        void mouseEnter(const juce::MouseEvent&) override;
        void mouseExit(const juce::MouseEvent&) override;
        void mouseUp(const juce::MouseEvent&) override;

        McPopupMenu& parent;
        int itemIndex;
        bool isHighlighted = false;
    };

    void updateHighlightedItem(int itemIndex);
    int getItemIndexAt(int y) const;
    void selectItem(int itemIndex);
    static juce::Point<int> calculatePopupPosition(McComboBox& comboBox, int popupHeight);

    McComboBox& comboBox;
    McLookAndFeel* mcLookAndFeel = nullptr;
    int highlightedItemIndex = -1;
    std::vector<std::unique_ptr<ItemComponent>> itemComponents;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(McPopupMenu)
};
