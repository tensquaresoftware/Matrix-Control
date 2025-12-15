#include "McPopupMenu.h"
#include "McComboBox.h"
#include "../LookAndFeel/McLookAndFeel.h"

McPopupMenu::McPopupMenu(McComboBox& comboBoxRef)
    : comboBox(comboBoxRef)
{
    setWantsKeyboardFocus(true);
    setAlwaysOnTop(true);
    setInterceptsMouseClicks(true, true);
    
    auto numItems = comboBox.getNumItems();
    auto width = comboBox.getWidth();
    auto totalHeight = numItems * kItemHeight + kBorderSize * 2;
    
    setSize(width, totalHeight);
    
    for (int i = 0; i < numItems; ++i)
    {
        auto itemComponent = std::make_unique<ItemComponent>(*this, i);
        addAndMakeVisible(itemComponent.get());
        itemComponents.push_back(std::move(itemComponent));
    }
    
    auto selectedIndex = comboBox.getSelectedItemIndex();
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(itemComponents.size()))
    {
        highlightedItemIndex = selectedIndex;
        if (itemComponents[static_cast<size_t>(selectedIndex)] != nullptr)
        {
            itemComponents[static_cast<size_t>(selectedIndex)]->isHighlighted = true;
        }
    }
    
    resized();
}

void McPopupMenu::paint(juce::Graphics& g)
{
    if (mcLookAndFeel == nullptr)
    {
        return;
    }

    auto bounds = getLocalBounds();
    auto backgroundColour = mcLookAndFeel->getPopupMenuBackgroundColour();
    auto borderColour = mcLookAndFeel->getPopupMenuBorderColour();

    g.setColour(backgroundColour);
    g.fillRect(bounds);

    g.setColour(borderColour);
    g.drawRect(bounds, kBorderSize);
}

void McPopupMenu::resized()
{
    auto bounds = getLocalBounds().reduced(kBorderSize);
    auto y = 0;
    
    for (auto& itemComponent : itemComponents)
    {
        itemComponent->setBounds(bounds.getX(), bounds.getY() + y, bounds.getWidth(), kItemHeight);
        y += kItemHeight;
    }
}

void McPopupMenu::mouseMove(const juce::MouseEvent& e)
{
    auto itemIndex = getItemIndexAt(e.getPosition().y);
    updateHighlightedItem(itemIndex);
}

void McPopupMenu::mouseExit(const juce::MouseEvent&)
{
    updateHighlightedItem(-1);
}

void McPopupMenu::mouseUp(const juce::MouseEvent& e)
{
    auto itemIndex = getItemIndexAt(e.getPosition().y);
    if (itemIndex >= 0)
    {
        selectItem(itemIndex);
    }
}

void McPopupMenu::updateHighlightedItem(int itemIndex)
{
    if (highlightedItemIndex != itemIndex)
    {
        if (highlightedItemIndex >= 0 && highlightedItemIndex < static_cast<int>(itemComponents.size()))
        {
            auto index = static_cast<size_t>(highlightedItemIndex);
            if (itemComponents[index] != nullptr)
            {
                itemComponents[index]->isHighlighted = false;
                itemComponents[index]->repaint();
            }
        }
        
        highlightedItemIndex = itemIndex;
        
        if (highlightedItemIndex >= 0 && highlightedItemIndex < static_cast<int>(itemComponents.size()))
        {
            auto index = static_cast<size_t>(highlightedItemIndex);
            if (itemComponents[index] != nullptr)
            {
                itemComponents[index]->isHighlighted = true;
                itemComponents[index]->repaint();
            }
        }
    }
}

int McPopupMenu::getItemIndexAt(int y) const
{
    auto bounds = getLocalBounds().reduced(kBorderSize);
    auto relativeY = y - bounds.getY();
    
    if (relativeY < 0)
    {
        return -1;
    }
    
    auto itemIndex = relativeY / kItemHeight;
    if (itemIndex >= 0 && itemIndex < comboBox.getNumItems())
    {
        return itemIndex;
    }
    
    return -1;
}

void McPopupMenu::selectItem(int itemIndex)
{
    exitModalState(0);
    
    if (itemIndex >= 0 && itemIndex < comboBox.getNumItems())
    {
        auto itemId = comboBox.getItemId(itemIndex);
        comboBox.setSelectedId(itemId, juce::sendNotificationSync);
    }
    
    auto* parent = getParentComponent();
    if (parent != nullptr)
    {
        parent->removeChildComponent(this);
    }
    
    delete this;
}

juce::Point<int> McPopupMenu::calculatePopupPosition(McComboBox& comboBox, int popupHeight)
{
    auto* parent = comboBox.getParentComponent();
    if (parent == nullptr)
    {
        return comboBox.getPosition();
    }

    auto comboBoxBounds = comboBox.getBounds();
    auto comboBoxPosition = comboBox.getPosition();
    auto parentBounds = parent->getBounds();
    
    auto spaceBelow = parentBounds.getBottom() - (comboBoxPosition.y + comboBoxBounds.getHeight());
    auto spaceAbove = comboBoxPosition.y - parentBounds.getY();
    
    auto x = comboBoxPosition.x;
    int y;
    
    if (spaceBelow >= popupHeight || spaceBelow >= spaceAbove)
    {
        y = comboBoxPosition.y + comboBoxBounds.getHeight();
    }
    else
    {
        y = comboBoxPosition.y - popupHeight;
    }
    
    auto finalY = juce::jlimit(parentBounds.getY(), parentBounds.getBottom() - popupHeight, y);
    
    return juce::Point<int>(x, finalY);
}

void McPopupMenu::show(McComboBox& comboBoxRef)
{
    if (comboBoxRef.getNumItems() == 0)
    {
        return;
    }

    auto* lookAndFeel = comboBoxRef.mcLookAndFeel;
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
    popupMenu->mcLookAndFeel = lookAndFeel;

    auto popupHeight = popupMenu->getHeight();
    auto popupPosition = calculatePopupPosition(comboBoxRef, popupHeight);
    
    popupMenu->setTopLeftPosition(popupPosition);
    
    auto* rawPtr = popupMenu.get();
    parent->addAndMakeVisible(popupMenu.release());
    
    rawPtr->setVisible(true);
    rawPtr->toFront(true);
    rawPtr->grabKeyboardFocus();
    rawPtr->resized();
    rawPtr->repaint();
    
    rawPtr->enterModalState(true, nullptr, true);
}

McPopupMenu::ItemComponent::ItemComponent(McPopupMenu& parentRef, int index)
    : parent(parentRef)
    , itemIndex(index)
{
    setMouseClickGrabsKeyboardFocus(false);
    setInterceptsMouseClicks(true, false);
}

void McPopupMenu::ItemComponent::paint(juce::Graphics& g)
{
    if (parent.mcLookAndFeel == nullptr)
    {
        return;
    }

    auto bounds = getLocalBounds();
    auto isActive = parent.comboBox.getItemId(itemIndex) != 0;
    
    juce::Colour textColour;
    if (isHighlighted && isActive)
    {
        auto highlightColour = parent.mcLookAndFeel->getPopupMenuHighlightColour();
        g.setColour(highlightColour);
        g.fillRect(bounds);
        
        textColour = parent.mcLookAndFeel->getPopupMenuTextColourHighlighted();
    }
    else
    {
        textColour = parent.mcLookAndFeel->getPopupMenuTextColour();
        if (! isActive)
        {
            textColour = textColour.withAlpha(0.5f);
        }
    }

    auto text = parent.comboBox.getItemText(itemIndex);
    auto font = parent.mcLookAndFeel->getDefaultFont();
    
    g.setColour(textColour);
    g.setFont(font);
    
    auto textArea = bounds.reduced(parent.kTextLeftMargin, 0);
    g.drawText(text, textArea, juce::Justification::centredLeft, false);
}

void McPopupMenu::ItemComponent::mouseEnter(const juce::MouseEvent&)
{
    isHighlighted = true;
    repaint();
}

void McPopupMenu::ItemComponent::mouseExit(const juce::MouseEvent&)
{
    isHighlighted = false;
    repaint();
}

void McPopupMenu::ItemComponent::mouseUp(const juce::MouseEvent&)
{
    parent.selectItem(itemIndex);
}

void McPopupMenu::inputAttemptWhenModal()
{
    exitModalState(0);
    
    auto* parent = getParentComponent();
    if (parent != nullptr)
    {
        parent->removeChildComponent(this);
    }
    delete this;
}

bool McPopupMenu::keyPressed(const juce::KeyPress& key)
{
    if (key.getKeyCode() == juce::KeyPress::escapeKey)
    {
        exitModalState(0);
        
        auto* parent = getParentComponent();
        if (parent != nullptr)
        {
            parent->removeChildComponent(this);
        }
        delete this;
        return true;
    }
    
    return false;
}
