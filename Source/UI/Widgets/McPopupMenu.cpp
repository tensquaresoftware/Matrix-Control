#include "McPopupMenu.h"
#include "McComboBox.h"
#include "../Themes/McTheme.h"

McPopupMenu::McPopupMenu(McComboBox& comboBoxRef)
    : comboBox(comboBoxRef)
{
    setWantsKeyboardFocus(true);
    setAlwaysOnTop(true);
    setInterceptsMouseClicks(true, true);
    setOpaque(true);
    
    auto* parent = comboBox.getParentComponent();
    if (parent != nullptr)
    {
        mcTheme = comboBox.mcTheme;
        if (mcTheme != nullptr)
        {
            cachedFont = mcTheme->getDefaultFont();
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

void McPopupMenu::paint(juce::Graphics& g)
{
    if (! hasValidLookAndFeel())
    {
        return;
    }

    auto bounds = getLocalBounds();
    
    drawBase(g, bounds);
    drawBackground(g, bounds.reduced(kBorderThickness));
    
    auto contentBounds = bounds.reduced(kBorderThickness);
    drawItems(g, contentBounds);
    
    if (columnCount > 1)
    {
        drawVerticalSeparators(g, contentBounds);
    }
    
    drawBorder(g, bounds);
}

void McPopupMenu::resized()
{
}

void McPopupMenu::mouseMove(const juce::MouseEvent& e)
{
    auto itemIndex = getItemIndexAt(e.getPosition().x, e.getPosition().y);
    updateHighlightedItem(itemIndex);
}

void McPopupMenu::mouseExit(const juce::MouseEvent&)
{
    updateHighlightedItem(-1);
}

void McPopupMenu::mouseUp(const juce::MouseEvent& e)
{
    auto itemIndex = getItemIndexAt(e.getPosition().x, e.getPosition().y);
    if (itemIndex >= 0)
    {
        selectItem(itemIndex);
    }
}

void McPopupMenu::inputAttemptWhenModal()
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

bool McPopupMenu::keyPressed(const juce::KeyPress& key)
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

void McPopupMenu::calculateColumnLayout()
{
    auto numItems = comboBox.getNumItems();
    if (numItems == 0)
    {
        return;
    }
    
    columnCount = calculateColumnCount(numItems);
    itemsPerColumn = calculateItemsPerColumn(numItems, columnCount);
    columnWidth = comboBox.getWidth();
    
    auto totalWidth = columnCount * columnWidth + (columnCount - 1) * kSeparatorWidth;
    auto totalHeight = itemsPerColumn * kItemHeight;
    
    setSize(totalWidth + kBorderThickness * 2, totalHeight + kBorderThickness * 2);
}

int McPopupMenu::calculateColumnCount(int totalItems) const
{
    if (totalItems <= kColumnThreshold)
    {
        return 1;
    }
    
    auto minColumns = (totalItems + kColumnThreshold - 1) / kColumnThreshold;
    return minColumns;
}

int McPopupMenu::calculateItemsPerColumn(int totalItems, int numColumns) const
{
    return (totalItems + numColumns - 1) / numColumns;
}

juce::Point<int> McPopupMenu::calculatePopupPosition() const
{
    auto popupWidth = getWidth();
    auto popupHeight = getHeight();
    
    auto x = calculatePopupX(popupWidth);
    auto y = calculatePopupY(popupHeight);
    
    return juce::Point<int>(x, y);
}

int McPopupMenu::calculatePopupX(int popupWidth) const
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

int McPopupMenu::calculatePopupY(int popupHeight) const
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
    
    auto requiredSpaceBelow = popupHeight + McComboBox::kVerticalMargin;
    auto requiredSpaceAbove = popupHeight + McComboBox::kVerticalMargin;
    
    if (spaceBelow >= requiredSpaceBelow || spaceBelow >= spaceAbove)
    {
        return comboY + comboHeight + McComboBox::kVerticalMargin;
    }
    else if (spaceAbove >= requiredSpaceAbove)
    {
        return comboY - popupHeight - McComboBox::kVerticalMargin;
    }
    else
    {
        return comboY + comboHeight + McComboBox::kVerticalMargin;
    }
}

juce::Rectangle<int> McPopupMenu::getItemBounds(int itemIndex) const
{
    if (! isValidItemIndex(itemIndex))
    {
        return juce::Rectangle<int>();
    }
    
    auto contentBounds = getLocalBounds().reduced(kBorderThickness);
    
    if (columnCount == 1)
    {
        auto y = contentBounds.getY() + itemIndex * kItemHeight;
        return juce::Rectangle<int>(contentBounds.getX(), y, contentBounds.getWidth(), kItemHeight);
    }
    else
    {
        auto column = itemIndex / itemsPerColumn;
        auto row = itemIndex % itemsPerColumn;
        
        auto x = contentBounds.getX() + column * columnWidth + column * kSeparatorWidth;
        auto y = contentBounds.getY() + row * kItemHeight;
        
        return juce::Rectangle<int>(x, y, columnWidth, kItemHeight);
    }
}

int McPopupMenu::getItemIndexAt(int x, int y) const
{
    auto contentBounds = getLocalBounds().reduced(kBorderThickness);
    
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

int McPopupMenu::getColumnFromX(int x) const
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
    
    for (int column = 1; column < columnCount; ++column)
    {
        if (relativeX < kSeparatorWidth)
        {
            return column - 1;
        }
        
        relativeX -= kSeparatorWidth;
        
        if (relativeX < columnWidth)
        {
            return column;
        }
        
        relativeX -= columnWidth;
    }
    
    return columnCount - 1;
}

int McPopupMenu::getRowFromY(int y) const
{
    return y / kItemHeight;
}

int McPopupMenu::getItemIndexFromColumnAndRow(int column, int row) const
{
    if (columnCount == 1)
    {
        return row;
    }
    
    return column * itemsPerColumn + row;
}

void McPopupMenu::updateHighlightedItem(int itemIndex)
{
    if (highlightedItemIndex != itemIndex)
    {
        highlightedItemIndex = itemIndex;
        repaint();
    }
}

void McPopupMenu::selectItem(int itemIndex)
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

void McPopupMenu::drawBase(juce::Graphics& g, const juce::Rectangle<int>& bounds)
{
    if (! hasValidLookAndFeel())
    {
        return;
    }
    
    auto baseColour = mcTheme->getPopupMenuBaseColour();
    g.setColour(baseColour);
    g.fillRect(bounds);
}

void McPopupMenu::drawBackground(juce::Graphics& g, const juce::Rectangle<int>& bounds)
{
    if (! hasValidLookAndFeel())
    {
        return;
    }
    
    auto backgroundColour = mcTheme->getPopupMenuBackgroundColour();
    g.setColour(backgroundColour);
    g.fillRect(bounds);
}

void McPopupMenu::drawBorder(juce::Graphics& g, const juce::Rectangle<int>& bounds)
{
    if (! hasValidLookAndFeel())
    {
        return;
    }
    
    auto borderColour = mcTheme->getPopupMenuBorderColour();
    g.setColour(borderColour);
    g.drawRect(bounds.toFloat(), static_cast<float>(kBorderThickness));
}

void McPopupMenu::drawItems(juce::Graphics& g, const juce::Rectangle<int>&)
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

void McPopupMenu::drawItem(juce::Graphics& g, int itemIndex, const juce::Rectangle<int>& itemBounds)
{
    if (! hasValidLookAndFeel())
    {
        return;
    }
    
    auto isHighlighted = (highlightedItemIndex == itemIndex);
    auto isActive = comboBox.getItemId(itemIndex) != 0;
    
    if (isHighlighted && isActive)
    {
        auto hooverBackgroundColour = mcTheme->getPopupMenuBackgroundHooverColour();
        auto hooverBounds = itemBounds.reduced(1);
        g.setColour(hooverBackgroundColour);
        g.fillRect(hooverBounds);
        
        auto hooverTextColour = mcTheme->getPopupMenuTextHooverColour();
        g.setColour(hooverTextColour);
        g.setFont(cachedFont);
        
        auto textBounds = itemBounds;
        textBounds.removeFromLeft(kTextLeftPadding);
        g.drawText(comboBox.getItemText(itemIndex), textBounds, juce::Justification::centredLeft, false);
    }
    else
    {
        auto textColour = mcTheme->getPopupMenuTextColour();
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

void McPopupMenu::drawVerticalSeparators(juce::Graphics& g, const juce::Rectangle<int>& contentBounds)
{
    if (! hasValidLookAndFeel() || columnCount <= 1)
    {
        return;
    }
    
    auto separatorColour = mcTheme->getPopupMenuSeparatorColour();
    g.setColour(separatorColour);
    
    for (int i = 1; i < columnCount; ++i)
    {
        auto separatorX = contentBounds.getX() + i * columnWidth + (i - 1) * kSeparatorWidth;
        g.fillRect(separatorX, contentBounds.getY(), kSeparatorWidth, contentBounds.getHeight());
    }
}

void McPopupMenu::handleKeyboardNavigation(const juce::KeyPress& key)
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

void McPopupMenu::navigateUp()
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

void McPopupMenu::navigateDown()
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

void McPopupMenu::navigateLeft()
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

void McPopupMenu::navigateRight()
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

bool McPopupMenu::isValidItemIndex(int itemIndex) const
{
    return itemIndex >= 0 && itemIndex < comboBox.getNumItems();
}

bool McPopupMenu::hasValidLookAndFeel() const
{
    return mcTheme != nullptr;
}

bool McPopupMenu::hasValidParent() const
{
    return getParentComponent() != nullptr;
}

void McPopupMenu::show(McComboBox& comboBoxRef)
{
    if (comboBoxRef.getNumItems() == 0)
    {
        return;
    }

    auto* lookAndFeel = comboBoxRef.mcTheme;
    if (lookAndFeel == nullptr)
    {
        return;
    }

    auto* parent = comboBoxRef.getParentComponent();
    if (parent == nullptr)
    {
        return;
    }

    auto popupMenu = std::make_unique<McPopupMenu>(comboBoxRef);
    
    auto popupPosition = popupMenu->calculatePopupPosition();
    
    auto* rawPtr = popupMenu.get();
    parent->addAndMakeVisible(popupMenu.release());
    
    rawPtr->setTopLeftPosition(popupPosition);
    rawPtr->toFront(false);
    rawPtr->grabKeyboardFocus();
    
    rawPtr->enterModalState(false, nullptr, true);
}
