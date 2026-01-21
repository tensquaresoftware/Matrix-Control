#include "PopupMenu.h"
#include "ComboBox.h"

#include "GUI/Themes/Theme.h"

namespace tss
{
    class PopupMenu::ScrollableContentComponent : public juce::Component
    {
    public:
        explicit ScrollableContentComponent(PopupMenu& popupMenu)
            : popupMenu_(popupMenu)
        {
        }

        void paint(juce::Graphics& g) override
        {
            popupMenu_.drawItems(g, getLocalBounds());
        }

    private:
        PopupMenu& popupMenu_;
    };
    PopupMenu::PopupMenu(ComboBox& comboBoxRef, DisplayMode displayMode)
        : comboBox(comboBoxRef)
        , displayMode_(displayMode)
    {
        setWantsKeyboardFocus(true);
        setAlwaysOnTop(true);
        setInterceptsMouseClicks(true, true);
        setOpaque(true);
        
        if (auto* parent [[maybe_unused]] = comboBox.getParentComponent())
        {
            theme = comboBox.theme_;
            if (auto* currentTheme = theme)
                cachedFont = currentTheme->getBaseFont();
        }
        
        calculateColumnLayout();
        
        if (displayMode_ == DisplayMode::SingleColumnScrollable)
        {
            setupScrollableContent();
        }
        
        auto selectedIndex = comboBox.getSelectedItemIndex();
        if (isValidItemIndex(selectedIndex))
        {
            highlightedItemIndex = selectedIndex;
        }
        else if (comboBox.getNumItems() > 0)
        {
            highlightedItemIndex = 0;
        }
        
        if (displayMode_ == DisplayMode::SingleColumnScrollable && viewport_ != nullptr && highlightedItemIndex >= 0)
        {
            auto itemY = highlightedItemIndex * kItemHeight_;
            viewport_->setViewPosition(0, itemY);
        }
    }

    void PopupMenu::paint(juce::Graphics& g)
    {
        if (! hasValidLookAndFeel())
        {
            return;
        }

        auto bounds = getLocalBounds();
        auto borderThickness = kBorderThickness_;
        auto borderThicknessInt = static_cast<int>(borderThickness);
        
        g.fillAll(theme->getPopupMenuBackgroundColour());
        
        drawBackground(g, bounds.reduced(borderThicknessInt));
        
        if (displayMode_ == DisplayMode::SingleColumnScrollable)
        {
            // Content is drawn by ScrollableContentComponent
        }
        else
        {
            auto contentBounds = bounds.reduced(borderThicknessInt);
            drawItems(g, contentBounds);
            
            if (columnCount > 1)
            {
                drawVerticalSeparators(g, contentBounds);
            }
        }
        
        drawBorder(g, bounds);
    }

    void PopupMenu::resized()
    {
        if (displayMode_ == DisplayMode::SingleColumnScrollable && viewport_ != nullptr)
        {
            auto borderThickness = static_cast<int>(kBorderThickness_);
            auto contentBounds = getLocalBounds().reduced(borderThickness);
            viewport_->setBounds(contentBounds);
        }
    }

    void PopupMenu::mouseMove(const juce::MouseEvent& e)
    {
        if (displayMode_ == DisplayMode::SingleColumnScrollable && viewport_ != nullptr)
        {
            auto viewportPoint = e.getPosition();
            if (viewport_->getBounds().contains(viewportPoint))
            {
                auto localPoint = viewport_->getLocalPoint(this, viewportPoint);
                auto contentPoint = viewport_->getViewPosition() + localPoint;
                auto itemIndex = getItemIndexAt(contentPoint.getX(), contentPoint.getY());
                updateHighlightedItem(itemIndex);
            }
            else
            {
                updateHighlightedItem(-1);
            }
        }
        else
        {
            auto itemIndex = getItemIndexAt(e.getPosition().x, e.getPosition().y);
            updateHighlightedItem(itemIndex);
        }
    }

    void PopupMenu::mouseExit(const juce::MouseEvent&)
    {
        updateHighlightedItem(-1);
    }

    void PopupMenu::mouseUp(const juce::MouseEvent& e)
    {
        if (displayMode_ == DisplayMode::SingleColumnScrollable && viewport_ != nullptr)
        {
            auto viewportPoint = e.getPosition();
            if (viewport_->getBounds().contains(viewportPoint))
            {
                auto localPoint = viewport_->getLocalPoint(this, viewportPoint);
                auto contentPoint = viewport_->getViewPosition() + localPoint;
                auto itemIndex = getItemIndexAt(contentPoint.getX(), contentPoint.getY());
                if (itemIndex >= 0)
                {
                    selectItem(itemIndex);
                }
            }
        }
        else
        {
            auto itemIndex = getItemIndexAt(e.getPosition().x, e.getPosition().y);
            if (itemIndex >= 0)
            {
                selectItem(itemIndex);
            }
        }
    }

    void PopupMenu::inputAttemptWhenModal()
    {
        exitModalState(0);
        
        comboBox.isPopupOpen_ = false;
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
            
            comboBox.isPopupOpen_ = false;
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
        
        if (displayMode_ == DisplayMode::SingleColumnScrollable)
        {
            columnCount = 1;
            itemsPerColumn = numItems;
            columnWidth = comboBox.getBounds().getWidth();
            scrollableContentHeight = numItems * kItemHeight_;
        }
        else
        {
            columnCount = calculateColumnCount(numItems);
            itemsPerColumn = calculateItemsPerColumn(numItems, columnCount);
            columnWidth = comboBox.getBounds().getWidth();
        }
        
        // Note: setSize() is now called in show() after adding to parent
        // to avoid triggering expensive events before the component is in the hierarchy
    }

    int PopupMenu::calculateColumnCount(int totalItems) const
    {
        auto columnThreshold = kColumnThreshold_;
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

    juce::Component* PopupMenu::findTopLevelComponent() const
    {
        auto* component = comboBox.getParentComponent();
        while (component != nullptr)
        {
            auto* parent = component->getParentComponent();
            if (parent == nullptr)
            {
                return component;
            }
            component = parent;
        }
        return nullptr;
    }

    juce::Point<int> PopupMenu::calculatePopupPosition(juce::Component* topLevelComponent) const
    {
        if (topLevelComponent == nullptr)
        {
            return juce::Point<int>();
        }

        auto popupWidth = getWidth();
        auto popupHeight = getHeight();
        
        // Single call to getScreenBounds() for each component
        auto comboBoxScreenBounds = comboBox.getScreenBounds();
        auto topLevelScreenBounds = topLevelComponent->getScreenBounds();
        
        // Calculate all needed values from bounds
        auto comboX = comboBoxScreenBounds.getX() - topLevelScreenBounds.getX();
        auto comboY = comboBoxScreenBounds.getY() - topLevelScreenBounds.getY();
        auto comboWidth = comboBoxScreenBounds.getWidth();
        auto comboHeight = comboBoxScreenBounds.getHeight();
        
        // Calculate X position
        auto spaceRight = topLevelScreenBounds.getRight() - comboBoxScreenBounds.getRight();
        auto spaceLeft = comboX;
        auto x = (spaceRight < popupWidth && spaceRight < spaceLeft) 
                 ? comboX + comboWidth - popupWidth 
                 : comboX;
        
        // Calculate Y position
        auto verticalMargin = ComboBox::getVerticalMargin();
        auto spaceBelow = topLevelScreenBounds.getBottom() - comboBoxScreenBounds.getBottom();
        auto spaceAbove = comboY;
        auto requiredSpace = popupHeight + verticalMargin;
        auto y = comboY + comboHeight + verticalMargin;
        if (spaceBelow < requiredSpace && spaceAbove >= requiredSpace)
        {
            y = comboY - popupHeight - verticalMargin;
        }
        
        return juce::Point<int>(x, y);
    }

    juce::Rectangle<int> PopupMenu::getItemBounds(int itemIndex) const
    {
        if (! isValidItemIndex(itemIndex))
        {
            return juce::Rectangle<int>();
        }
        
        auto itemHeight = kItemHeight_;
        auto separatorWidth = kSeparatorWidth_;
        
        if (displayMode_ == DisplayMode::SingleColumnScrollable)
        {
            if (contentComponent_ != nullptr)
            {
                auto y = itemIndex * itemHeight;
                return juce::Rectangle<int>(0, y, contentComponent_->getWidth(), itemHeight);
            }
            return juce::Rectangle<int>();
        }
        
        auto borderThickness = kBorderThickness_;
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
        if (displayMode_ == DisplayMode::SingleColumnScrollable)
        {
            if (contentComponent_ != nullptr)
            {
                // Coordinates are already in content component space
                if (contentComponent_->getBounds().contains(x, y))
                {
                    auto row = y / kItemHeight_;
                    if (row >= 0 && row < comboBox.getNumItems())
                    {
                        return row;
                    }
                }
            }
            return -1;
        }
        
        auto contentBounds = getLocalBounds().reduced(static_cast<int>(kBorderThickness_));
        
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
        
        auto separatorWidth = kSeparatorWidth_;
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
        return y / kItemHeight_;
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
            
            if (displayMode_ == DisplayMode::SingleColumnScrollable && contentComponent_ != nullptr)
            {
                contentComponent_->repaint();
                
                if (itemIndex >= 0 && viewport_ != nullptr)
                {
                    auto itemY = itemIndex * kItemHeight_;
                    auto viewportY = viewport_->getViewPositionY();
                    auto viewportHeight = viewport_->getHeight();
                    
                    if (itemY < viewportY || itemY + kItemHeight_ > viewportY + viewportHeight)
                    {
                        viewport_->setViewPosition(0, juce::jmax(0, itemY - viewportHeight / 2));
                    }
                }
            }
            else
            {
                repaint();
            }
        }
    }

    void PopupMenu::selectItem(int itemIndex)
    {
        exitModalState(0);
        
            comboBox.isPopupOpen_ = false;
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
        g.drawRect(bounds.toFloat(), kBorderThickness_);
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
            textBounds.removeFromLeft(kTextLeftPadding_);
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
            textBounds.removeFromLeft(kTextLeftPadding_);
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
        
        auto separatorWidth = kSeparatorWidth_;
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

    void PopupMenu::setupScrollableContent()
    {
        contentComponent_ = std::make_unique<ScrollableContentComponent>(*this);
        contentComponent_->setSize(columnWidth, scrollableContentHeight);
        
        viewport_ = std::make_unique<juce::Viewport>();
        viewport_->setViewedComponent(contentComponent_.get(), false);
        viewport_->setScrollBarsShown(true, false);
        viewport_->setScrollBarThickness(8);
        addAndMakeVisible(*viewport_);
    }

    void PopupMenu::show(ComboBox& comboBoxRef, DisplayMode displayMode)
    {
        if (comboBoxRef.getNumItems() == 0)
        {
            return;
        }

        auto* themePtr = comboBoxRef.theme_;
        if (themePtr == nullptr)
        {
            return;
        }

        auto* parent = comboBoxRef.getParentComponent();
        if (parent == nullptr)
        {
            return;
        }

        auto* topLevelComponent = comboBoxRef.getTopLevelComponent();
        if (topLevelComponent == nullptr)
        {
            return;
        }

        auto popupMenu = std::make_unique<PopupMenu>(comboBoxRef, displayMode);
        auto* rawPtr = popupMenu.get();
        
        // Calculate size before adding to parent (layout is already calculated in constructor)
        auto columnCount = rawPtr->columnCount;
        auto itemsPerColumn = rawPtr->itemsPerColumn;
        auto columnWidth = rawPtr->columnWidth;
        auto separatorWidth = kSeparatorWidth_;
        auto itemHeight = kItemHeight_;
        auto borderThickness = kBorderThickness_;
        auto totalWidth = columnCount * columnWidth + (columnCount - 1) * separatorWidth;
        
        int totalHeight;
        if (displayMode == DisplayMode::SingleColumnScrollable)
        {
            totalHeight = juce::jmin(rawPtr->scrollableContentHeight, kMaxScrollableHeight_);
        }
        else
        {
            totalHeight = itemsPerColumn * itemHeight;
        }
        
        auto popupSize = juce::Point<int>(totalWidth + static_cast<int>(borderThickness * 2.0f),
                                          totalHeight + static_cast<int>(borderThickness * 2.0f));
        
        // Pre-calculate position before adding to parent
        auto topLevelScreenBounds = topLevelComponent->getScreenBounds();
        auto verticalMargin = ComboBox::getVerticalMargin();
        
        auto desiredPopupTopLeft = comboBoxRef.localPointToGlobal(juce::Point<int>(0, comboBoxRef.getHeight() + verticalMargin));
        auto popupWidth = popupSize.getX();
        auto popupHeight = popupSize.getY();
        
        // Adjust X position
        if (desiredPopupTopLeft.getX() + popupWidth > topLevelScreenBounds.getRight())
        {
            desiredPopupTopLeft.setX(comboBoxRef.localPointToGlobal(juce::Point<int>(comboBoxRef.getBounds().getWidth() - popupWidth, 0)).getX());
        }

        // Adjust Y position
        if (desiredPopupTopLeft.getY() + popupHeight > topLevelScreenBounds.getBottom())
        {
            desiredPopupTopLeft.setY(comboBoxRef.localPointToGlobal(juce::Point<int>(0, -popupHeight - verticalMargin)).getY());
        }
        
        // Convert screen position to top-level component coordinates
        auto popupPosition = desiredPopupTopLeft - topLevelScreenBounds.getPosition();
        
        topLevelComponent->addAndMakeVisible(popupMenu.release());
        
        // Use setBounds() instead of separate setSize() and setTopLeftPosition() calls
        // This is more efficient as it combines both operations
        rawPtr->setBounds(popupPosition.getX(), popupPosition.getY(), popupSize.getX(), popupSize.getY());
        
        if (displayMode == DisplayMode::SingleColumnScrollable && rawPtr->viewport_ != nullptr)
        {
            rawPtr->resized();
        }
        
        rawPtr->toFront(false);
        rawPtr->grabKeyboardFocus();
        
        rawPtr->enterModalState(false, nullptr, true);
    }
}

