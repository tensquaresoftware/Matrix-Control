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
    
    auto numItems = comboBox.getNumItems();
    auto comboWidth = comboBox.getWidth();
    
    useMultiColumn = (comboBox.getPopupDisplayMode() == McComboBox::PopupDisplayMode::MultiColumn);
    
    if (useMultiColumn)
    {
        initializeMultiColumnLayout(numItems, comboWidth);
    }
    else
    {
        initializeSingleColumnLayout(numItems, comboWidth);
    }
    
    initializeHighlightedItem(comboBox.getSelectedItemIndex());
}

void McPopupMenu::paint(juce::Graphics& g)
{
    if (! hasValidLookAndFeel())
    {
        return;
    }

    auto bounds = getLocalBounds();
    auto backgroundColour = mcTheme->getPopupMenuBackgroundColour();

    g.setColour(backgroundColour);
    g.fillRect(bounds);

    auto contentBounds = bounds.reduced(kBorderSize);
    drawAllItems(g, contentBounds);

    if (useMultiColumn && columnCount > 1)
    {
        drawVerticalSeparators(g, contentBounds);
    }

    drawPopupBorder(g, bounds);
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

void McPopupMenu::updateHighlightedItem(int itemIndex)
{
    if (highlightedItemIndex != itemIndex)
    {
        highlightedItemIndex = itemIndex;
        repaint();
    }
}

int McPopupMenu::getItemIndexAt(int x, int y) const
{
    auto bounds = getLocalBounds().reduced(kBorderSize);
    auto relativeX = x - bounds.getX();
    auto relativeY = y - bounds.getY();
    
    if (! isPositionInsideBounds(relativeX, relativeY, bounds))
    {
        return -1;
    }
    
    if (useMultiColumn)
    {
        auto column = findColumnFromXPosition(relativeX);
        auto row = findRowFromYPosition(relativeY);
        
        if (! isValidRow(row))
        {
            return -1;
        }
        
        auto itemIndex = calculateItemIndexFromColumnAndRow(column, row);
        if (isValidItemIndex(itemIndex))
        {
            return itemIndex;
        }
    }
    else
    {
        return findItemIndexInSingleColumn(relativeY);
    }
    
    return -1;
}

void McPopupMenu::selectItem(int itemIndex)
{
    exitModalState(0);
    
    if (isValidItemIndex(itemIndex))
    {
        auto itemId = comboBox.getItemId(itemIndex);
        comboBox.setSelectedId(itemId, juce::sendNotificationSync);
    }
    
    if (hasValidParent())
    {
        getParentComponent()->removeChildComponent(this);
    }
    
    delete this;
}

juce::Point<int> McPopupMenu::calculatePopupPosition(McComboBox& comboBox, int popupWidth, int popupHeight)
{
    auto* parent = comboBox.getParentComponent();
    if (parent == nullptr)
    {
        return comboBox.getPosition();
    }

    auto comboBoxBounds = comboBox.getBounds();
    auto comboBoxPosition = comboBox.getPosition();
    auto parentBounds = parent->getBounds();
    
    auto x = calculatePopupXPosition(comboBox, popupWidth, parentBounds, comboBoxPosition, comboBoxBounds);
    x = clampPopupXToParentBounds(x, popupWidth, parentBounds);
    
    auto y = calculatePopupYPosition(comboBox, popupHeight, parentBounds, comboBoxPosition, comboBoxBounds);
    y = clampPopupYToParentBounds(y, popupHeight, parentBounds);
    
    return juce::Point<int>(x, y);
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
    popupMenu->mcTheme = lookAndFeel;
    popupMenu->cachedFont = lookAndFeel->getDefaultFont();
    
    if (popupMenu->useMultiColumn)
    {
        auto spaceRight = parent->getWidth() - (comboBoxRef.getX() + comboBoxRef.getWidth());
        auto spaceLeft = comboBoxRef.getX();
        auto availableWidth = juce::jmax(spaceRight, spaceLeft);
        popupMenu->columnCount = popupMenu->calculateOptimalColumnCount(comboBoxRef.getNumItems(), availableWidth);
        popupMenu->itemsPerColumn = popupMenu->calculateCeilingDivision(comboBoxRef.getNumItems(), popupMenu->columnCount);
        popupMenu->totalPopupWidth = popupMenu->columnCount * popupMenu->columnWidth + (popupMenu->columnCount - 1) * kColumnSpacing + kBorderSize * 2;
        auto totalHeight = popupMenu->itemsPerColumn * kItemHeight + kBorderSize * 2;
        popupMenu->setSize(popupMenu->totalPopupWidth, totalHeight);
    }

    auto popupWidth = popupMenu->getWidth();
    auto popupHeight = popupMenu->getHeight();
    auto popupPosition = calculatePopupPosition(comboBoxRef, popupWidth, popupHeight);
    
    auto* rawPtr = popupMenu.get();
    parent->addAndMakeVisible(popupMenu.release());
    
    rawPtr->setTopLeftPosition(popupPosition);
    rawPtr->toFront(false);
    rawPtr->grabKeyboardFocus();
    
    rawPtr->enterModalState(false, nullptr, true);
}


void McPopupMenu::inputAttemptWhenModal()
{
    exitModalState(0);
    
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
        
        if (hasValidParent())
        {
            getParentComponent()->removeChildComponent(this);
        }
        delete this;
        return true;
    }
    
    if (key.getKeyCode() == juce::KeyPress::returnKey)
    {
        if (hasHighlightedItem())
        {
            selectItem(highlightedItemIndex);
        }
        return true;
    }
    
    if (useMultiColumn)
    {
        handleMultiColumnNavigation(key);
    }
    else
    {
        handleSingleColumnNavigation(key);
    }
    
    return false;
}

void McPopupMenu::calculateColumnLayout()
{
    if (! useMultiColumn || mcTheme == nullptr)
    {
        return;
    }
    
    columnWidth = comboBox.getWidth();
    totalPopupWidth = columnCount * columnWidth + (columnCount - 1) * kColumnSpacing + kBorderSize * 2;
    auto totalHeight = itemsPerColumn * kItemHeight + kBorderSize * 2;
    setSize(totalPopupWidth, totalHeight);
}

int McPopupMenu::calculateOptimalColumnCount(int totalItems, int availableWidth) const
{
    if (totalItems <= kMaxVisibleRows)
    {
        return 1;
    }
    
    auto minColumns = calculateCeilingDivision(totalItems, kMaxVisibleRows);
    
    auto comboBoxWidth = comboBox.getWidth();
    auto maxColumnsByWidth = (availableWidth - kBorderSize * 2) / (comboBoxWidth + kColumnSpacing);
    if (maxColumnsByWidth < 1)
    {
        maxColumnsByWidth = 1;
    }
    
    return juce::jlimit(1, maxColumnsByWidth, minColumns);
}

int McPopupMenu::getItemColumn(int itemIndex) const
{
    if (! isValidItemIndex(itemIndex))
    {
        return 0;
    }
    return itemIndex / itemsPerColumn;
}

int McPopupMenu::getItemRowInColumn(int itemIndex) const
{
    if (! isValidItemIndex(itemIndex))
    {
        return 0;
    }
    return itemIndex % itemsPerColumn;
}

juce::Rectangle<int> McPopupMenu::getItemBounds(int itemIndex) const
{
    auto bounds = getLocalBounds().reduced(kBorderSize);
    
    if (useMultiColumn)
    {
        auto column = getItemColumn(itemIndex);
        auto row = getItemRowInColumn(itemIndex);
        
        auto x = bounds.getX() + column * (columnWidth + kColumnSpacing);
        auto y = bounds.getY() + row * kItemHeight;
        
        return juce::Rectangle<int>(x, y, columnWidth, kItemHeight);
    }
    else
    {
        auto y = bounds.getY() + itemIndex * kItemHeight;
        return juce::Rectangle<int>(bounds.getX(), y, bounds.getWidth(), kItemHeight);
    }
}

void McPopupMenu::initializeMultiColumnLayout(int numItems, int comboWidth)
{
    columnWidth = comboWidth;
    
    auto estimatedAvailableWidth = comboWidth * 4;
    columnCount = calculateOptimalColumnCount(numItems, estimatedAvailableWidth);
    itemsPerColumn = calculateCeilingDivision(numItems, columnCount);
    
    totalPopupWidth = columnCount * columnWidth + (columnCount - 1) * kColumnSpacing + kBorderSize * 2;
    auto totalHeight = itemsPerColumn * kItemHeight + kBorderSize * 2;
    
    setSize(totalPopupWidth, totalHeight);
}

void McPopupMenu::initializeSingleColumnLayout(int numItems, int comboWidth)
{
    columnCount = 1;
    itemsPerColumn = numItems;
    columnWidth = comboWidth;
    totalPopupWidth = comboWidth;
    auto totalHeight = numItems * kItemHeight + kBorderSize * 2;
    
    setSize(totalPopupWidth, totalHeight);
}

void McPopupMenu::initializeHighlightedItem(int selectedIndex)
{
    if (isValidItemIndex(selectedIndex))
    {
        highlightedItemIndex = selectedIndex;
    }
}

void McPopupMenu::drawAllItems(juce::Graphics& g, const juce::Rectangle<int>& contentBounds) const
{
    auto numItems = comboBox.getNumItems();
    
    if (useMultiColumn)
    {
        for (int i = 0; i < numItems; ++i)
        {
            auto column = getItemColumn(i);
            auto row = getItemRowInColumn(i);
            auto x = contentBounds.getX() + column * (columnWidth + kColumnSpacing);
            auto y = contentBounds.getY() + row * kItemHeight;
            auto itemBounds = juce::Rectangle<int>(x, y, columnWidth, kItemHeight);
            drawItem(g, i, itemBounds);
        }
    }
    else
    {
        for (int i = 0; i < numItems; ++i)
        {
            auto y = contentBounds.getY() + i * kItemHeight;
            auto itemBounds = juce::Rectangle<int>(contentBounds.getX(), y, contentBounds.getWidth(), kItemHeight);
            drawItem(g, i, itemBounds);
        }
    }
}

void McPopupMenu::drawItem(juce::Graphics& g, int itemIndex, const juce::Rectangle<int>& itemBounds) const
{
    if (! hasValidLookAndFeel())
    {
        return;
    }
    
    auto isActive = comboBox.getItemId(itemIndex) != 0;
    auto isHighlighted = (highlightedItemIndex == itemIndex);
    
    if (isHighlighted && isActive)
    {
        auto column = useMultiColumn ? getItemColumn(itemIndex) : 0;
        drawItemHighlight(g, itemBounds, column);
    }
    
    juce::Colour textColour;
    if (isHighlighted && isActive)
    {
        textColour = mcTheme->getPopupMenuTextColourHighlighted();
    }
    else
    {
        textColour = mcTheme->getPopupMenuTextColour();
        if (! isActive)
        {
            textColour = textColour.withAlpha(0.5f);
        }
    }
    
    auto text = comboBox.getItemText(itemIndex);
    
    g.setColour(textColour);
    g.setFont(cachedFont);
    
    auto textArea = itemBounds.reduced(kTextLeftMargin, 0);
    g.drawText(text, textArea, juce::Justification::centredLeft, false);
}

void McPopupMenu::drawItemHighlight(juce::Graphics& g, const juce::Rectangle<int>& itemBounds, int column) const
{
    auto highlightColour = mcTheme->getPopupMenuHighlightColour();
    g.setColour(highlightColour);
    
    auto highlightBounds = calculateHighlightBoundsForItem(itemBounds, column);
    g.fillRect(highlightBounds);
}

void McPopupMenu::drawVerticalSeparators(juce::Graphics& g, const juce::Rectangle<int>& contentBounds) const
{
    auto separatorColour = mcTheme->getPopupMenuSeparatorColour();
    g.setColour(separatorColour);
    
    auto separatorY = contentBounds.getY();
    auto separatorHeight = contentBounds.getHeight();
    
    for (int i = 0; i < columnCount - 1; ++i)
    {
        auto separatorX = contentBounds.getX() + (i + 1) * columnWidth + i * kColumnSpacing;
        g.fillRect(separatorX, separatorY, 1, separatorHeight);
    }
}

void McPopupMenu::drawPopupBorder(juce::Graphics& g, const juce::Rectangle<int>& bounds) const
{
    auto borderColour = mcTheme->getPopupMenuBorderColour();
    g.setColour(borderColour);
    g.drawRect(bounds, kBorderSize);
}

int McPopupMenu::findColumnFromXPosition(int relativeX) const
{
    int currentX = 0;
    int column = 0;
    for (int c = 0; c < columnCount; ++c)
    {
        if (relativeX >= currentX && relativeX < currentX + columnWidth)
        {
            column = c;
            break;
        }
        currentX += columnWidth + kColumnSpacing;
    }
    return column;
}

int McPopupMenu::findRowFromYPosition(int relativeY) const
{
    return relativeY / kItemHeight;
}

int McPopupMenu::calculateItemIndexFromColumnAndRow(int column, int row) const
{
    return column * itemsPerColumn + row;
}

int McPopupMenu::findItemIndexInSingleColumn(int relativeY) const
{
    auto itemIndex = relativeY / kItemHeight;
    if (isValidItemIndex(itemIndex))
    {
        return itemIndex;
    }
    return -1;
}

int McPopupMenu::calculatePopupXPosition(McComboBox&, int popupWidth, const juce::Rectangle<int>& parentBounds, const juce::Point<int>& comboBoxPosition, const juce::Rectangle<int>& comboBoxBounds)
{
    auto xCC = comboBoxPosition.x;
    auto wCC = comboBoxBounds.getWidth();
    auto wPM = popupWidth;
    
    auto spaceRight = parentBounds.getRight() - (xCC + wCC);
    auto spaceLeft = xCC - parentBounds.getX();
    
    if (spaceRight >= wPM || spaceRight >= spaceLeft)
    {
        return xCC;
    }
    else
    {
        return xCC + wCC - wPM;
    }
}

int McPopupMenu::calculatePopupYPosition(McComboBox&, int popupHeight, const juce::Rectangle<int>& parentBounds, const juce::Point<int>& comboBoxPosition, const juce::Rectangle<int>& comboBoxBounds)
{
    auto yCC = comboBoxPosition.y;
    auto hCC = comboBoxBounds.getHeight();
    auto hPM = popupHeight;
    
    auto spaceBelow = parentBounds.getBottom() - (yCC + hCC);
    auto spaceAbove = yCC - parentBounds.getY();
    
    auto requiredSpaceBelow = hPM + kVerticalSpacing;
    auto requiredSpaceAbove = hPM + kVerticalSpacing;
    
    if (spaceBelow >= requiredSpaceBelow || spaceBelow >= spaceAbove)
    {
        return yCC + hCC + kVerticalSpacing;
    }
    else if (spaceAbove >= requiredSpaceAbove)
    {
        return yCC - hPM - kVerticalSpacing;
    }
    else
    {
        return yCC + hCC + kVerticalSpacing;
    }
}

int McPopupMenu::clampPopupXToParentBounds(int x, int popupWidth, const juce::Rectangle<int>& parentBounds)
{
    return juce::jlimit(parentBounds.getX(), parentBounds.getRight() - popupWidth, x);
}

int McPopupMenu::clampPopupYToParentBounds(int y, int popupHeight, const juce::Rectangle<int>& parentBounds)
{
    return juce::jlimit(parentBounds.getY(), parentBounds.getBottom() - popupHeight, y);
}

void McPopupMenu::handleMultiColumnNavigation(const juce::KeyPress& key)
{
    if (key.getKeyCode() == juce::KeyPress::upKey)
    {
        navigateUpInColumn();
    }
    else if (key.getKeyCode() == juce::KeyPress::downKey)
    {
        navigateDownInColumn();
    }
    else if (key.getKeyCode() == juce::KeyPress::leftKey)
    {
        navigateToPreviousColumn();
    }
    else if (key.getKeyCode() == juce::KeyPress::rightKey)
    {
        navigateToNextColumn();
    }
}

void McPopupMenu::handleSingleColumnNavigation(const juce::KeyPress& key)
{
    if (key.getKeyCode() == juce::KeyPress::upKey)
    {
        navigateUpInSingleColumn();
    }
    else if (key.getKeyCode() == juce::KeyPress::downKey)
    {
        navigateDownInSingleColumn();
    }
}

void McPopupMenu::navigateUpInColumn()
{
    if (hasHighlightedItem())
    {
        auto currentColumn = getItemColumn(highlightedItemIndex);
        auto currentRow = getItemRowInColumn(highlightedItemIndex);
        
        if (currentRow > 0)
        {
            auto newIndex = (currentColumn * itemsPerColumn) + (currentRow - 1);
            if (isValidItemIndex(newIndex))
            {
                updateHighlightedItem(newIndex);
            }
        }
    }
    else if (hasItems())
    {
        updateHighlightedItem(0);
    }
}

void McPopupMenu::navigateDownInColumn()
{
    if (hasHighlightedItem())
    {
        auto currentColumn = getItemColumn(highlightedItemIndex);
        auto lastItemInColumn = juce::jmin((currentColumn + 1) * itemsPerColumn - 1, comboBox.getNumItems() - 1);
        
        if (highlightedItemIndex < lastItemInColumn)
        {
            updateHighlightedItem(highlightedItemIndex + 1);
        }
    }
    else if (hasItems())
    {
        updateHighlightedItem(0);
    }
}

void McPopupMenu::navigateToPreviousColumn()
{
    if (! canNavigateToPreviousColumn())
    {
        return;
    }
    
    auto currentColumn = getItemColumn(highlightedItemIndex);
    auto currentRow = getItemRowInColumn(highlightedItemIndex);
    auto newIndex = ((currentColumn - 1) * itemsPerColumn) + currentRow;
    
    if (isValidItemIndex(newIndex))
    {
        updateHighlightedItem(newIndex);
    }
}

void McPopupMenu::navigateToNextColumn()
{
    if (! canNavigateToNextColumn())
    {
        return;
    }
    
    auto currentColumn = getItemColumn(highlightedItemIndex);
    auto currentRow = getItemRowInColumn(highlightedItemIndex);
    auto newIndex = ((currentColumn + 1) * itemsPerColumn) + currentRow;
    
    if (isValidItemIndex(newIndex))
    {
        updateHighlightedItem(newIndex);
    }
}

void McPopupMenu::navigateUpInSingleColumn()
{
    if (canNavigateUp())
    {
        updateHighlightedItem(highlightedItemIndex - 1);
    }
}

void McPopupMenu::navigateDownInSingleColumn()
{
    if (canNavigateDown())
    {
        updateHighlightedItem(highlightedItemIndex + 1);
    }
}

void McPopupMenu::recalculateColumnLayoutIfNeeded()
{
    if (useMultiColumn && mcTheme != nullptr)
    {
        columnWidth = comboBox.getWidth();
        totalPopupWidth = columnCount * columnWidth + (columnCount - 1) * kColumnSpacing + kBorderSize * 2;
        auto totalHeight = itemsPerColumn * kItemHeight + kBorderSize * 2;
        setSize(totalPopupWidth, totalHeight);
    }
}

int McPopupMenu::calculateCeilingDivision(int dividend, int divisor)
{
    return (dividend + divisor - 1) / divisor;
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

bool McPopupMenu::isPositionInsideBounds(int relativeX, int relativeY, const juce::Rectangle<int>& bounds) const
{
    return relativeX >= 0 && relativeY >= 0 && relativeX < bounds.getWidth() && relativeY < bounds.getHeight();
}

bool McPopupMenu::isValidRow(int row) const
{
    return row >= 0 && row < itemsPerColumn;
}

bool McPopupMenu::hasItems() const
{
    return comboBox.getNumItems() > 0;
}

bool McPopupMenu::hasHighlightedItem() const
{
    return highlightedItemIndex >= 0;
}

bool McPopupMenu::canNavigateUp() const
{
    return highlightedItemIndex > 0;
}

bool McPopupMenu::canNavigateDown() const
{
    return highlightedItemIndex < comboBox.getNumItems() - 1;
}

bool McPopupMenu::canNavigateToPreviousColumn() const
{
    if (! hasHighlightedItem())
    {
        return false;
    }
    
    auto currentColumn = getItemColumn(highlightedItemIndex);
    return currentColumn > 0;
}

bool McPopupMenu::canNavigateToNextColumn() const
{
    if (! hasHighlightedItem())
    {
        return false;
    }
    
    auto currentColumn = getItemColumn(highlightedItemIndex);
    return currentColumn < columnCount - 1;
}

juce::Rectangle<int> McPopupMenu::calculateHighlightBoundsForItem(const juce::Rectangle<int>& itemBounds, int column) const
{
    if (useMultiColumn)
    {
        if (column == 0)
        {
            return itemBounds.reduced(1);
        }
        else
        {
            auto highlightBounds = itemBounds;
            highlightBounds.removeFromTop(1);
            highlightBounds.removeFromRight(1);
            highlightBounds.removeFromBottom(1);
            return highlightBounds;
        }
    }
    else
    {
        return itemBounds.reduced(1);
    }
}
