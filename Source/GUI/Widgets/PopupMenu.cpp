#include "PopupMenu.h"
#include "ComboBox.h"

#include "../Themes/Theme.h"

namespace tss
{
    PopupMenu::PopupMenu(ComboBox& comboBoxRef)
        : comboBox(comboBoxRef)
    {
        setWantsKeyboardFocus(true);
        setAlwaysOnTop(true);
        setInterceptsMouseClicks(true, true);
        setOpaque(true);
        
        auto* parent = comboBox.getParentComponent();
        if (parent != nullptr)
        {
            theme = comboBox.theme;
            if (theme != nullptr)
            {
                cachedFont = theme->getBaseFont();
            }
        }
        
        calculateColumnLayout();
        
        auto selectedIndex = comboBox.getSelectedItemIndex();
        if (isValidItemIndex(selectedIndex))
        {
            highlightedItemIndex = selectedIndex;
        }
        else if (comboBox.getNumItems() > 0)
        {
            highlightedItemIndex = 0;
        }
    }

    void PopupMenu::paint(juce::Graphics& g)
    {
        if (! hasValidLookAndFeel())
        {
            return;
        }

        auto bounds = getLocalBounds();
        auto borderThickness = kBorderThickness;
        auto borderThicknessInt = static_cast<int>(borderThickness);
        
        drawBase(g, bounds);
        drawBackground(g, bounds.reduced(borderThicknessInt));
        
        auto contentBounds = bounds.reduced(borderThicknessInt);
        drawItems(g, contentBounds);
        
        if (columnCount > 1)
        {
            drawVerticalSeparators(g, contentBounds);
        }
        
        drawBorder(g, bounds);
    }

    void PopupMenu::resized()
    {
    }

    void PopupMenu::mouseMove(const juce::MouseEvent& e)
    {
        auto itemIndex = getItemIndexAt(e.getPosition().x, e.getPosition().y);
        updateHighlightedItem(itemIndex);
    }

    void PopupMenu::mouseExit(const juce::MouseEvent&)
    {
        updateHighlightedItem(-1);
    }

    void PopupMenu::mouseUp(const juce::MouseEvent& e)
    {
        auto itemIndex = getItemIndexAt(e.getPosition().x, e.getPosition().y);
        if (itemIndex >= 0)
        {
            selectItem(itemIndex);
        }
    }

    void PopupMenu::inputAttemptWhenModal()
    {
        exitModalState(0);
        
        comboBox.isPopupOpen = false;
        comboBox.repaint();
        
        comboBox.grabKeyboardFocus();
        
        if (hasValidParent())
        {
            getParentComponent()->removeChildComponent(this);
        }
        delete this;
    }

    bool PopupMenu::keyPressed(const juce::KeyPress& key)
    {
        if (key.getKeyCode() == juce::KeyPress::escapeKey)
        {
            exitModalState(0);
            
            comboBox.isPopupOpen = false;
            comboBox.repaint();
            
            comboBox.grabKeyboardFocus();
            
            if (hasValidParent())
            {
                getParentComponent()->removeChildComponent(this);
            }
            delete this;
            return true;
        }
        
        if (key.getKeyCode() == juce::KeyPress::returnKey)
        {
            if (highlightedItemIndex >= 0)
            {
                selectItem(highlightedItemIndex);
            }
            return true;
        }
        
        handleKeyboardNavigation(key);
        
        return false;
    }

    void PopupMenu::calculateColumnLayout()
    {
        auto numItems = comboBox.getNumItems();
        if (numItems == 0)
        {
            return;
        }
        
        columnCount = calculateColumnCount(numItems);
        itemsPerColumn = calculateItemsPerColumn(numItems, columnCount);
        columnWidth = comboBox.getWidth();
        
        auto separatorWidth = kSeparatorWidth;
        auto itemHeight = kItemHeight;
        auto borderThickness = kBorderThickness;
        auto totalWidth = columnCount * columnWidth + (columnCount - 1) * separatorWidth;
        auto totalHeight = itemsPerColumn * itemHeight;
        
        setSize(totalWidth + static_cast<int>(borderThickness * 2.0f), totalHeight + static_cast<int>(borderThickness * 2.0f));
    }

    int PopupMenu::calculateColumnCount(int totalItems) const
    {
        auto columnThreshold = kColumnThreshold;
        if (totalItems <= columnThreshold)
        {
            return 1;
        }
        
        auto minColumns = (totalItems + columnThreshold - 1) / columnThreshold;
        return minColumns;
    }

    int PopupMenu::calculateItemsPerColumn(int totalItems, int numColumns) const
    {
        return (totalItems + numColumns - 1) / numColumns;
    }

    juce::Point<int> PopupMenu::calculatePopupPosition() const
    {
        auto popupWidth = getWidth();
        auto popupHeight = getHeight();
        
        auto x = calculatePopupX(popupWidth);
        auto y = calculatePopupY(popupHeight);
        
        return juce::Point<int>(x, y);
    }

    int PopupMenu::calculatePopupX(int popupWidth) const
    {
        auto* parent = comboBox.getParentComponent();
        if (parent == nullptr)
        {
            return comboBox.getX();
        }
        
        auto comboBounds = comboBox.getBounds();
        auto comboX = comboBox.getX();
        auto comboWidth = comboBounds.getWidth();
        auto parentBounds = parent->getBounds();
        
        auto spaceRight = parentBounds.getRight() - (comboX + comboWidth);
        auto spaceLeft = comboX - parentBounds.getX();
        
        if (spaceRight >= popupWidth || spaceRight >= spaceLeft)
        {
            return comboX;
        }
        else
        {
            return comboX + comboWidth - popupWidth;
        }
    }

    int PopupMenu::calculatePopupY(int popupHeight) const
    {
        auto* parent = comboBox.getParentComponent();
        if (parent == nullptr)
        {
            return comboBox.getBottom();
        }
        
        auto comboBounds = comboBox.getBounds();
        auto comboY = comboBox.getY();
        auto comboHeight = comboBounds.getHeight();
        auto parentBounds = parent->getBounds();
        
        auto spaceBelow = parentBounds.getBottom() - (comboY + comboHeight);
        auto spaceAbove = comboY - parentBounds.getY();
        
        auto verticalMargin = ComboBox::getVerticalMargin();
        auto requiredSpaceBelow = popupHeight + verticalMargin;
        auto requiredSpaceAbove = popupHeight + verticalMargin;
        
        if (spaceBelow >= requiredSpaceBelow || spaceBelow >= spaceAbove)
        {
            return comboY + comboHeight + verticalMargin;
        }
        else if (spaceAbove >= requiredSpaceAbove)
        {
            return comboY - popupHeight - verticalMargin;
        }
        else
        {
            return comboY + comboHeight + verticalMargin;
        }
    }

    juce::Rectangle<int> PopupMenu::getItemBounds(int itemIndex) const
    {
        if (! isValidItemIndex(itemIndex))
        {
            return juce::Rectangle<int>();
        }
        
        auto borderThickness = kBorderThickness;
        auto itemHeight = kItemHeight;
        auto separatorWidth = kSeparatorWidth;
        auto contentBounds = getLocalBounds().reduced(static_cast<int>(borderThickness));
        
        if (columnCount == 1)
        {
            auto y = contentBounds.getY() + itemIndex * itemHeight;
            return juce::Rectangle<int>(contentBounds.getX(), y, contentBounds.getWidth(), itemHeight);
        }
        else
        {
            auto column = itemIndex / itemsPerColumn;
            auto row = itemIndex % itemsPerColumn;
            
            auto x = contentBounds.getX() + column * columnWidth + column * separatorWidth;
            auto y = contentBounds.getY() + row * itemHeight;
            
            return juce::Rectangle<int>(x, y, columnWidth, itemHeight);
        }
    }

    int PopupMenu::getItemIndexAt(int x, int y) const
    {
        auto borderThickness = kBorderThickness;
        auto contentBounds = getLocalBounds().reduced(static_cast<int>(borderThickness));
        
        if (! contentBounds.contains(x, y))
        {
            return -1;
        }
        
        auto relativeX = x - contentBounds.getX();
        auto relativeY = y - contentBounds.getY();
        
        auto column = getColumnFromX(relativeX);
        auto row = getRowFromY(relativeY);
        
        if (column < 0 || column >= columnCount || row < 0 || row >= itemsPerColumn)
        {
            return -1;
        }
        
        auto itemIndex = getItemIndexFromColumnAndRow(column, row);
        
        if (isValidItemIndex(itemIndex))
        {
            return itemIndex;
        }
        
        return -1;
    }

    int PopupMenu::getColumnFromX(int x) const
    {
        if (columnCount == 1)
        {
            return 0;
        }
        
        auto relativeX = x;
        
        if (relativeX < columnWidth)
        {
            return 0;
        }
        
        relativeX -= columnWidth;
        
        auto separatorWidth = kSeparatorWidth;
        for (int column = 1; column < columnCount; ++column)
        {
            if (relativeX < separatorWidth)
            {
                return column - 1;
            }
            
            relativeX -= separatorWidth;
            
            if (relativeX < columnWidth)
            {
                return column;
            }
            
            relativeX -= columnWidth;
        }
        
        return columnCount - 1;
    }

    int PopupMenu::getRowFromY(int y) const
    {
        auto itemHeight = kItemHeight;
        return y / itemHeight;
    }

    int PopupMenu::getItemIndexFromColumnAndRow(int column, int row) const
    {
        if (columnCount == 1)
        {
            return row;
        }
        
        return column * itemsPerColumn + row;
    }

    void PopupMenu::updateHighlightedItem(int itemIndex)
    {
        if (highlightedItemIndex != itemIndex)
        {
            highlightedItemIndex = itemIndex;
            repaint();
        }
    }

    void PopupMenu::selectItem(int itemIndex)
    {
        exitModalState(0);
        
        comboBox.isPopupOpen = false;
        comboBox.repaint();
        
        if (isValidItemIndex(itemIndex))
        {
            auto itemId = comboBox.getItemId(itemIndex);
            comboBox.setSelectedId(itemId, juce::sendNotificationSync);
        }
        
        comboBox.grabKeyboardFocus();
        
        if (hasValidParent())
        {
            getParentComponent()->removeChildComponent(this);
        }
        
        delete this;
    }

    void PopupMenu::drawBase(juce::Graphics& g, const juce::Rectangle<int>& bounds)
    {
        if (! hasValidLookAndFeel())
        {
            return;
        }
        
        auto baseColour = theme->getPopupMenuBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void PopupMenu::drawBackground(juce::Graphics& g, const juce::Rectangle<int>& bounds)
    {
        if (! hasValidLookAndFeel())
        {
            return;
        }
        
        auto backgroundColour = theme->getPopupMenuBackgroundColour();
        g.setColour(backgroundColour);
        g.fillRect(bounds);
    }

    void PopupMenu::drawBorder(juce::Graphics& g, const juce::Rectangle<int>& bounds)
    {
        if (! hasValidLookAndFeel())
        {
            return;
        }
        
        auto borderColour = theme->getPopupMenuBorderColour();
        g.setColour(borderColour);
        auto borderThickness = kBorderThickness;
        g.drawRect(bounds.toFloat(), borderThickness);
    }

    void PopupMenu::drawItems(juce::Graphics& g, const juce::Rectangle<int>&)
    {
        auto numItems = comboBox.getNumItems();
        
        for (int i = 0; i < numItems; ++i)
        {
            auto itemBounds = getItemBounds(i);
            if (itemBounds.isEmpty())
            {
                continue;
            }
            
            drawItem(g, i, itemBounds);
        }
    }

    void PopupMenu::drawItem(juce::Graphics& g, int itemIndex, const juce::Rectangle<int>& itemBounds)
    {
        if (! hasValidLookAndFeel())
        {
            return;
        }
        
        auto isHighlighted = (highlightedItemIndex == itemIndex);
        auto isActive = comboBox.getItemId(itemIndex) != 0;
        
        if (isHighlighted && isActive)
        {
            auto hooverBackgroundColour = theme->getPopupMenuBackgroundHooverColour();
            auto hooverBounds = itemBounds.reduced(1);
            g.setColour(hooverBackgroundColour);
            g.fillRect(hooverBounds);
            
            auto hooverTextColour = theme->getPopupMenuTextHooverColour();
            g.setColour(hooverTextColour);
            g.setFont(cachedFont);
            
            auto textBounds = itemBounds;
            textBounds.removeFromLeft(kTextLeftPadding);
            g.drawText(comboBox.getItemText(itemIndex), textBounds, juce::Justification::centredLeft, false);
        }
        else
        {
            auto textColour = theme->getPopupMenuTextColour();
            if (! isActive)
            {
                textColour = textColour.withAlpha(0.5f);
            }
            
            g.setColour(textColour);
            g.setFont(cachedFont);
            
            auto textBounds = itemBounds;
            textBounds.removeFromLeft(kTextLeftPadding);
            g.drawText(comboBox.getItemText(itemIndex), textBounds, juce::Justification::centredLeft, false);
        }
    }

    void PopupMenu::drawVerticalSeparators(juce::Graphics& g, const juce::Rectangle<int>& contentBounds)
    {
        if (! hasValidLookAndFeel() || columnCount <= 1)
        {
            return;
        }
        
        auto separatorColour = theme->getPopupMenuSeparatorColour();
        g.setColour(separatorColour);
        
        auto separatorWidth = kSeparatorWidth;
        for (int i = 1; i < columnCount; ++i)
        {
            auto separatorX = contentBounds.getX() + i * columnWidth + (i - 1) * separatorWidth;
            g.fillRect(separatorX, contentBounds.getY(), separatorWidth, contentBounds.getHeight());
        }
    }

    void PopupMenu::handleKeyboardNavigation(const juce::KeyPress& key)
    {
        if (key.getKeyCode() == juce::KeyPress::upKey)
        {
            navigateUp();
        }
        else if (key.getKeyCode() == juce::KeyPress::downKey)
        {
            navigateDown();
        }
        else if (key.getKeyCode() == juce::KeyPress::leftKey)
        {
            navigateLeft();
        }
        else if (key.getKeyCode() == juce::KeyPress::rightKey)
        {
            navigateRight();
        }
    }

    void PopupMenu::navigateUp()
    {
        if (highlightedItemIndex < 0)
        {
            if (comboBox.getNumItems() > 0)
            {
                updateHighlightedItem(0);
            }
            return;
        }
        
        if (columnCount == 1)
        {
            if (highlightedItemIndex > 0)
            {
                updateHighlightedItem(highlightedItemIndex - 1);
            }
        }
        else
        {
            auto column = highlightedItemIndex / itemsPerColumn;
            auto row = highlightedItemIndex % itemsPerColumn;
            
            if (row > 0)
            {
                auto newIndex = column * itemsPerColumn + (row - 1);
                if (isValidItemIndex(newIndex))
                {
                    updateHighlightedItem(newIndex);
                }
            }
        }
    }

    void PopupMenu::navigateDown()
    {
        if (highlightedItemIndex < 0)
        {
            if (comboBox.getNumItems() > 0)
            {
                updateHighlightedItem(0);
            }
            return;
        }
        
        if (columnCount == 1)
        {
            if (highlightedItemIndex < comboBox.getNumItems() - 1)
            {
                updateHighlightedItem(highlightedItemIndex + 1);
            }
        }
        else
        {
            auto column = highlightedItemIndex / itemsPerColumn;
            auto lastItemInColumn = juce::jmin((column + 1) * itemsPerColumn - 1, comboBox.getNumItems() - 1);
            
            if (highlightedItemIndex < lastItemInColumn)
            {
                updateHighlightedItem(highlightedItemIndex + 1);
            }
        }
    }

    void PopupMenu::navigateLeft()
    {
        if (columnCount <= 1 || highlightedItemIndex < 0)
        {
            return;
        }
        
        auto column = highlightedItemIndex / itemsPerColumn;
        auto row = highlightedItemIndex % itemsPerColumn;
        
        if (column > 0)
        {
            auto newIndex = (column - 1) * itemsPerColumn + row;
            if (isValidItemIndex(newIndex))
            {
                updateHighlightedItem(newIndex);
            }
        }
    }

    void PopupMenu::navigateRight()
    {
        if (columnCount <= 1 || highlightedItemIndex < 0)
        {
            return;
        }
        
        auto column = highlightedItemIndex / itemsPerColumn;
        auto row = highlightedItemIndex % itemsPerColumn;
        
        if (column < columnCount - 1)
        {
            auto newIndex = (column + 1) * itemsPerColumn + row;
            if (isValidItemIndex(newIndex))
            {
                updateHighlightedItem(newIndex);
            }
        }
    }

    bool PopupMenu::isValidItemIndex(int itemIndex) const
    {
        return itemIndex >= 0 && itemIndex < comboBox.getNumItems();
    }

    bool PopupMenu::hasValidLookAndFeel() const
    {
        return theme != nullptr;
    }

    bool PopupMenu::hasValidParent() const
    {
        return getParentComponent() != nullptr;
    }

    void PopupMenu::show(ComboBox& comboBoxRef)
    {
        if (comboBoxRef.getNumItems() == 0)
        {
            return;
        }

        auto* themePtr = comboBoxRef.theme;
        if (themePtr == nullptr)
        {
            return;
        }

        auto* parent = comboBoxRef.getParentComponent();
        if (parent == nullptr)
        {
            return;
        }

        auto popupMenu = std::make_unique<PopupMenu>(comboBoxRef);
        
        auto popupPosition = popupMenu->calculatePopupPosition();
        
        auto* rawPtr = popupMenu.get();
        parent->addAndMakeVisible(popupMenu.release());
        
        rawPtr->setTopLeftPosition(popupPosition);
        rawPtr->toFront(false);
        rawPtr->grabKeyboardFocus();
        
        rawPtr->enterModalState(false, nullptr, true);
    }
}

