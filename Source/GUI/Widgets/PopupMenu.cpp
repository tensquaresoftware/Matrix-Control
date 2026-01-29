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
    PopupMenu::PopupMenu(ComboBox& comboBox_Ref, DisplayMode displayMode)
        : comboBox_(comboBox_Ref)
        , displayMode_(displayMode)
    {
        setWantsKeyboardFocus(true);
        setAlwaysOnTop(true);
        setInterceptsMouseClicks(true, true);
        setOpaque(true);
        
        if (auto* parent [[maybe_unused]] = comboBox_.getParentComponent())
        {
            theme_ = comboBox_.theme_;
            if (auto* currentTheme = theme_)
                cachedFont_ = currentTheme->getBaseFont();
        }
        
        calculateColumnLayout();
        
        if (displayMode_ == DisplayMode::SingleColumnScrollable)
        {
            setupScrollableContent();
        }
        
        auto selectedIndex = comboBox_.getSelectedItemIndex();
        if (isValidItemIndex(selectedIndex))
        {
            highlightedItemIndex_ = selectedIndex;
        }
        else if (comboBox_.getNumItems() > 0)
        {
            highlightedItemIndex_ = 0;
        }
        
        if (displayMode_ == DisplayMode::SingleColumnScrollable && viewport_ != nullptr && highlightedItemIndex_ >= 0)
        {
            auto itemY = highlightedItemIndex_ * kItemHeight_;
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
        
        g.fillAll(theme_->getPopupMenuBackgroundColour());
        
        drawBackground(g, bounds.reduced(borderThicknessInt));
        
        if (displayMode_ == DisplayMode::SingleColumnScrollable)
        {
            // Content is drawn by ScrollableContentComponent
        }
        else
        {
            auto contentBounds = bounds.reduced(borderThicknessInt);
            drawItems(g, contentBounds);
            
            if (columnCount_ > 1)
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
        
        comboBox_.isPopupOpen_ = false;
        comboBox_.repaint();
        
        comboBox_.grabKeyboardFocus();
        
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
            
            comboBox_.isPopupOpen_ = false;
            comboBox_.repaint();
            
            comboBox_.grabKeyboardFocus();
            
            if (hasValidParent())
            {
                getParentComponent()->removeChildComponent(this);
            }
            delete this;
            return true;
        }
        
        if (key.getKeyCode() == juce::KeyPress::returnKey)
        {
            if (highlightedItemIndex_ >= 0)
            {
                selectItem(highlightedItemIndex_);
            }
            return true;
        }
        
        handleKeyboardNavigation(key);
        
        return false;
    }

    void PopupMenu::calculateColumnLayout()
    {
        auto numItems = comboBox_.getNumItems();
        if (numItems == 0)
        {
            return;
        }
        
        if (displayMode_ == DisplayMode::SingleColumnScrollable)
        {
            columnCount_ = 1;
            itemsPerColumn_ = numItems;
            columnWidth_ = comboBox_.getBounds().getWidth();
            scrollableContentHeight_ = numItems * kItemHeight_;
        }
        else
        {
            columnCount_ = calculateColumnCount(numItems);
            itemsPerColumn_ = calculateItemsPerColumn(numItems, columnCount_);
            columnWidth_ = comboBox_.getBounds().getWidth();
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
        auto* component = comboBox_.getParentComponent();
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
        auto comboBox_ScreenBounds = comboBox_.getScreenBounds();
        auto topLevelScreenBounds = topLevelComponent->getScreenBounds();
        
        // Calculate all needed values from bounds
        auto comboX = comboBox_ScreenBounds.getX() - topLevelScreenBounds.getX();
        auto comboY = comboBox_ScreenBounds.getY() - topLevelScreenBounds.getY();
        auto comboWidth = comboBox_ScreenBounds.getWidth();
        auto comboHeight = comboBox_ScreenBounds.getHeight();
        
        // Calculate X position
        auto spaceRight = topLevelScreenBounds.getRight() - comboBox_ScreenBounds.getRight();
        auto spaceLeft = comboX;
        auto x = (spaceRight < popupWidth && spaceRight < spaceLeft) 
                 ? comboX + comboWidth - popupWidth 
                 : comboX;
        
        // Calculate Y position
        auto verticalMargin = ComboBox::getVerticalMargin();
        auto spaceBelow = topLevelScreenBounds.getBottom() - comboBox_ScreenBounds.getBottom();
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
        
        if (columnCount_ == 1)
        {
            auto y = contentBounds.getY() + itemIndex * itemHeight;
            return juce::Rectangle<int>(contentBounds.getX(), y, contentBounds.getWidth(), itemHeight);
        }
        else
        {
            auto column = itemIndex / itemsPerColumn_;
            auto row = itemIndex % itemsPerColumn_;
            
            auto x = contentBounds.getX() + column * columnWidth_ + column * separatorWidth;
            auto y = contentBounds.getY() + row * itemHeight;
            
            return juce::Rectangle<int>(x, y, columnWidth_, itemHeight);
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
                    if (row >= 0 && row < comboBox_.getNumItems())
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
        
        if (column < 0 || column >= columnCount_ || row < 0 || row >= itemsPerColumn_)
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
        if (columnCount_ == 1)
        {
            return 0;
        }
        
        auto relativeX = x;
        
        if (relativeX < columnWidth_)
        {
            return 0;
        }
        
        relativeX -= columnWidth_;
        
        auto separatorWidth = kSeparatorWidth_;
        for (int column = 1; column < columnCount_; ++column)
        {
            if (relativeX < separatorWidth)
            {
                return column - 1;
            }
            
            relativeX -= separatorWidth;
            
            if (relativeX < columnWidth_)
            {
                return column;
            }
            
            relativeX -= columnWidth_;
        }
        
        return columnCount_ - 1;
    }

    int PopupMenu::getRowFromY(int y) const
    {
        return y / kItemHeight_;
    }

    int PopupMenu::getItemIndexFromColumnAndRow(int column, int row) const
    {
        if (columnCount_ == 1)
        {
            return row;
        }
        
        return column * itemsPerColumn_ + row;
    }

    void PopupMenu::updateHighlightedItem(int itemIndex)
    {
        if (highlightedItemIndex_ != itemIndex)
        {
            highlightedItemIndex_ = itemIndex;
            
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
        
            comboBox_.isPopupOpen_ = false;
        comboBox_.repaint();
        
        if (isValidItemIndex(itemIndex))
        {
            auto itemId = comboBox_.getItemId(itemIndex);
            comboBox_.setSelectedId(itemId, juce::sendNotificationSync);
        }
        
        comboBox_.grabKeyboardFocus();
        
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
        
        auto backgroundColour = theme_->getPopupMenuBackgroundColour();
        g.setColour(backgroundColour);
        g.fillRect(bounds);
    }

    void PopupMenu::drawBorder(juce::Graphics& g, const juce::Rectangle<int>& bounds)
    {
        if (! hasValidLookAndFeel())
        {
            return;
        }
        
        auto borderColour = theme_->getPopupMenuBorderColour();
        g.setColour(borderColour);
        g.drawRect(bounds.toFloat(), kBorderThickness_);
    }

    void PopupMenu::drawItems(juce::Graphics& g, const juce::Rectangle<int>&)
    {
        auto numItems = comboBox_.getNumItems();
        
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
        
        auto isHighlighted = (highlightedItemIndex_ == itemIndex);
        auto isActive = comboBox_.getItemId(itemIndex) != 0;
        
        if (isHighlighted && isActive)
        {
            auto hooverBackgroundColour = theme_->getPopupMenuBackgroundHooverColour();
            auto hooverBounds = itemBounds.reduced(1);
            g.setColour(hooverBackgroundColour);
            g.fillRect(hooverBounds);
            
            auto hooverTextColour = theme_->getPopupMenuTextHooverColour();
            g.setColour(hooverTextColour);
            g.setFont(cachedFont_);
            
            auto textBounds = itemBounds;
            textBounds.removeFromLeft(kTextLeftPadding_);
            g.drawText(comboBox_.getItemText(itemIndex), textBounds, juce::Justification::centredLeft, false);
        }
        else
        {
            auto textColour = theme_->getPopupMenuTextColour();
            if (! isActive)
            {
                textColour = textColour.withAlpha(0.5f);
            }
            
            g.setColour(textColour);
            g.setFont(cachedFont_);
            
            auto textBounds = itemBounds;
            textBounds.removeFromLeft(kTextLeftPadding_);
            g.drawText(comboBox_.getItemText(itemIndex), textBounds, juce::Justification::centredLeft, false);
        }
    }

    void PopupMenu::drawVerticalSeparators(juce::Graphics& g, const juce::Rectangle<int>& contentBounds)
    {
        if (! hasValidLookAndFeel() || columnCount_ <= 1)
        {
            return;
        }
        
        auto separatorColour = theme_->getPopupMenuSeparatorColour();
        g.setColour(separatorColour);
        
        auto separatorWidth = kSeparatorWidth_;
        for (int i = 1; i < columnCount_; ++i)
        {
            auto separatorX = contentBounds.getX() + i * columnWidth_ + (i - 1) * separatorWidth;
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
        if (highlightedItemIndex_ < 0)
        {
            if (comboBox_.getNumItems() > 0)
            {
                updateHighlightedItem(0);
            }
            return;
        }
        
        if (columnCount_ == 1)
        {
            if (highlightedItemIndex_ > 0)
            {
                updateHighlightedItem(highlightedItemIndex_ - 1);
            }
        }
        else
        {
            auto column = highlightedItemIndex_ / itemsPerColumn_;
            auto row = highlightedItemIndex_ % itemsPerColumn_;
            
            if (row > 0)
            {
                auto newIndex = column * itemsPerColumn_ + (row - 1);
                if (isValidItemIndex(newIndex))
                {
                    updateHighlightedItem(newIndex);
                }
            }
        }
    }

    void PopupMenu::navigateDown()
    {
        if (highlightedItemIndex_ < 0)
        {
            if (comboBox_.getNumItems() > 0)
            {
                updateHighlightedItem(0);
            }
            return;
        }
        
        if (columnCount_ == 1)
        {
            if (highlightedItemIndex_ < comboBox_.getNumItems() - 1)
            {
                updateHighlightedItem(highlightedItemIndex_ + 1);
            }
        }
        else
        {
            auto column = highlightedItemIndex_ / itemsPerColumn_;
            auto lastItemInColumn = juce::jmin((column + 1) * itemsPerColumn_ - 1, comboBox_.getNumItems() - 1);
            
            if (highlightedItemIndex_ < lastItemInColumn)
            {
                updateHighlightedItem(highlightedItemIndex_ + 1);
            }
        }
    }

    void PopupMenu::navigateLeft()
    {
        if (columnCount_ <= 1 || highlightedItemIndex_ < 0)
        {
            return;
        }
        
        auto column = highlightedItemIndex_ / itemsPerColumn_;
        auto row = highlightedItemIndex_ % itemsPerColumn_;
        
        if (column > 0)
        {
            auto newIndex = (column - 1) * itemsPerColumn_ + row;
            if (isValidItemIndex(newIndex))
            {
                updateHighlightedItem(newIndex);
            }
        }
    }

    void PopupMenu::navigateRight()
    {
        if (columnCount_ <= 1 || highlightedItemIndex_ < 0)
        {
            return;
        }
        
        auto column = highlightedItemIndex_ / itemsPerColumn_;
        auto row = highlightedItemIndex_ % itemsPerColumn_;
        
        if (column < columnCount_ - 1)
        {
            auto newIndex = (column + 1) * itemsPerColumn_ + row;
            if (isValidItemIndex(newIndex))
            {
                updateHighlightedItem(newIndex);
            }
        }
    }

    bool PopupMenu::isValidItemIndex(int itemIndex) const
    {
        return itemIndex >= 0 && itemIndex < comboBox_.getNumItems();
    }

    bool PopupMenu::hasValidLookAndFeel() const
    {
        return theme_ != nullptr;
    }

    bool PopupMenu::hasValidParent() const
    {
        return getParentComponent() != nullptr;
    }

    void PopupMenu::setupScrollableContent()
    {
        contentComponent_ = std::make_unique<ScrollableContentComponent>(*this);
        contentComponent_->setSize(columnWidth_, scrollableContentHeight_);
        
        viewport_ = std::make_unique<juce::Viewport>();
        viewport_->setViewedComponent(contentComponent_.get(), false);
        viewport_->setScrollBarsShown(true, false);
        viewport_->setScrollBarThickness(8);
        addAndMakeVisible(*viewport_);
    }

    void PopupMenu::show(ComboBox& comboBox_Ref, DisplayMode displayMode)
    {
        if (comboBox_Ref.getNumItems() == 0)
        {
            return;
        }

        auto* themePtr = comboBox_Ref.theme_;
        if (themePtr == nullptr)
        {
            return;
        }

        auto* parent = comboBox_Ref.getParentComponent();
        if (parent == nullptr)
        {
            return;
        }

        auto* topLevelComponent = comboBox_Ref.getTopLevelComponent();
        if (topLevelComponent == nullptr)
        {
            return;
        }

        auto popupMenu = std::make_unique<PopupMenu>(comboBox_Ref, displayMode);
        auto* rawPtr = popupMenu.get();
        
        // Calculate size before adding to parent (layout is already calculated in constructor)
        auto columnCount_ = rawPtr->columnCount_;
        auto itemsPerColumn_ = rawPtr->itemsPerColumn_;
        auto columnWidth_ = rawPtr->columnWidth_;
        auto separatorWidth = kSeparatorWidth_;
        auto itemHeight = kItemHeight_;
        auto borderThickness = kBorderThickness_;
        auto totalWidth = columnCount_ * columnWidth_ + (columnCount_ - 1) * separatorWidth;
        
        int totalHeight;
        if (displayMode == DisplayMode::SingleColumnScrollable)
        {
            totalHeight = juce::jmin(rawPtr->scrollableContentHeight_, kMaxScrollableHeight_);
        }
        else
        {
            totalHeight = itemsPerColumn_ * itemHeight;
        }
        
        auto popupSize = juce::Point<int>(totalWidth + static_cast<int>(borderThickness * 2.0f),
                                          totalHeight + static_cast<int>(borderThickness * 2.0f));
        
        // Pre-calculate position before adding to parent
        auto topLevelScreenBounds = topLevelComponent->getScreenBounds();
        auto verticalMargin = ComboBox::getVerticalMargin();
        
        auto desiredPopupTopLeft = comboBox_Ref.localPointToGlobal(juce::Point<int>(0, comboBox_Ref.getHeight() + verticalMargin));
        auto popupWidth = popupSize.getX();
        auto popupHeight = popupSize.getY();
        
        // Adjust X position
        if (desiredPopupTopLeft.getX() + popupWidth > topLevelScreenBounds.getRight())
        {
            desiredPopupTopLeft.setX(comboBox_Ref.localPointToGlobal(juce::Point<int>(comboBox_Ref.getBounds().getWidth() - popupWidth, 0)).getX());
        }

        // Adjust Y position
        if (desiredPopupTopLeft.getY() + popupHeight > topLevelScreenBounds.getBottom())
        {
            desiredPopupTopLeft.setY(comboBox_Ref.localPointToGlobal(juce::Point<int>(0, -popupHeight - verticalMargin)).getY());
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

