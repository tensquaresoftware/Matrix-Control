#include "McComboBox.h"
#include "McPopupMenu.h"
#include "../Themes/McTheme.h"

McComboBox::McComboBox(Size size)
    : juce::ComboBox()
    , comboSize(size)
{
    auto width = (size == Size::Normal) ? kNormalWidth : kLargeWidth;
    setSize(width, kHeight);
    setWantsKeyboardFocus(true);
    
    setColour(juce::ComboBox::textColourId, juce::Colours::transparentBlack);
}

void McComboBox::setTheme(McTheme* theme)
{
    mcTheme = theme;
    setColour(juce::ComboBox::textColourId, juce::Colours::transparentBlack);
}

void McComboBox::paint(juce::Graphics& g)
{
    if (mcTheme == nullptr)
    {
        return;
    }

    auto bounds = getLocalBounds().toFloat();
    auto enabled = isEnabled();
    auto hasFocus = focusableWidget.hasFocus() || isPopupOpen;

    drawBase(g, bounds);
    
    auto backgroundBounds = bounds.reduced(static_cast<float>(kBorderThickness));
   
    drawBackground(g, backgroundBounds, enabled);
    drawBorder(g, bounds, enabled, hasFocus);
    drawText(g, bounds, enabled);
    drawTriangle(g, bounds, enabled);
}

void McComboBox::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto baseColour = mcTheme->getComboBoxBaseColour();
    g.setColour(baseColour);
    g.fillRect(bounds);
}

void McComboBox::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
{
    auto backgroundColour = mcTheme->getComboBoxBackgroundColour(enabled);
    g.setColour(backgroundColour);
    g.fillRect(bounds);
}

void McComboBox::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool hasFocus)
{
    auto borderColour = mcTheme->getComboBoxBorderColour(enabled, hasFocus);
    g.setColour(borderColour);
    g.drawRect(bounds, static_cast<float>(kBorderThickness));
}

void McComboBox::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
{
    juce::String text;
    
    auto selectedIndex = getSelectedItemIndex();
    if (selectedIndex >= 0)
    {
        text = getItemText(selectedIndex);
    }

    auto textColour = mcTheme->getComboBoxTextColour(enabled);
    auto font = mcTheme->getDefaultFont();

    auto textBounds = bounds;
    textBounds.removeFromLeft(static_cast<float>(kLeftPadding));
    textBounds.removeFromRight(kTriangleSideSize + static_cast<float>(kRightPadding));

    g.setColour(textColour);
    g.setFont(font);
    g.drawText(text, textBounds, juce::Justification::centredLeft, false);
}

void McComboBox::drawTriangle(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
{
    auto triangleColour = mcTheme->getComboBoxTriangleColour(enabled);
    g.setColour(triangleColour);

    auto triangleHeight = kTriangleSideSize * 0.866f;
    auto triangleX = bounds.getRight() - kTriangleSideSize - static_cast<float>(kRightPadding);
    auto triangleY = bounds.getCentreY() - triangleHeight * 0.5f;

    auto trianglePath = createTrianglePath(triangleX, triangleY, kTriangleSideSize);
    g.fillPath(trianglePath);
}

juce::Path McComboBox::createTrianglePath(float x, float y, float sideSize) const
{
    juce::Path path;
    
    auto height = sideSize * 0.866f;
    
    path.startNewSubPath(x, y);
    path.lineTo(x + sideSize, y);
    path.lineTo(x + sideSize * 0.5f, y + height);
    path.closeSubPath();
    
    return path;
}

void McComboBox::showPopup()
{
    if (isEnabled() && getNumItems() > 0)
    {
        isPopupOpen = true;
        repaint();
        McPopupMenu::show(*this);
    }
}

void McComboBox::mouseDown(const juce::MouseEvent& e)
{
    if (isEnabled())
    {
        grabKeyboardFocus();
        focusableWidget.handleFocusGained(this);
        
        if (e.mods.isLeftButtonDown())
        {
            showPopup();
        }
    }
}

void McComboBox::focusGained(juce::Component::FocusChangeType)
{
    if (! focusableWidget.hasFocus())
    {
        focusableWidget.handleFocusGained(this);
    }
}

void McComboBox::focusLost(juce::Component::FocusChangeType)
{
    focusableWidget.handleFocusLost(this);
}
