#include "ComboBox.h"
#include "PopupMenu.h"

#include "../Themes/Theme.h"

namespace tss
{
    ComboBox::ComboBox(Theme& newTheme, Size size)
        : juce::ComboBox()
        , theme(&newTheme)
        , comboSize(size)
    {
        auto width = (size == Size::Normal) ? kNormalWidth : kLargeWidth;
        setSize(width, kHeight);
        setWantsKeyboardFocus(true);
        setColour(juce::ComboBox::textColourId, juce::Colours::transparentBlack);
    }

    void ComboBox::setTheme(Theme& newTheme)
    {
        theme = &newTheme;
        setColour(juce::ComboBox::textColourId, juce::Colours::transparentBlack);
    }

    void ComboBox::paint(juce::Graphics& g)
    {
        if (theme == nullptr)
        {
            return;
        }

        auto bounds = getLocalBounds().toFloat();
        auto enabled = isEnabled();
        auto hasFocus = focusableWidget.hasFocus() || isPopupOpen;

        auto backgroundBounds = calculateBackgroundBounds(bounds);

        drawBase(g, bounds);
        drawBackground(g, backgroundBounds, enabled);
        drawBorder(g, bounds, backgroundBounds, enabled, hasFocus);
        drawText(g, bounds, enabled);
        drawTriangle(g, bounds, enabled);
    }

    void ComboBox::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto baseColour = theme->getComboBoxBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void ComboBox::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        auto backgroundColour = theme->getComboBoxBackgroundColour(enabled);
        g.setColour(backgroundColour);
        g.fillRect(bounds);
    }

    void ComboBox::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, const juce::Rectangle<float>& backgroundBounds, bool enabled, bool hasFocus)
    {
        auto borderColour = theme->getComboBoxBorderColour(enabled, false);
        g.setColour(borderColour);
        auto borderThickness = kBorderThickness;
        g.drawRect(bounds, borderThickness);

        if (hasFocus)
        {
            auto focusBorderColour = theme->getComboBoxBorderColour(enabled, true);
            g.setColour(focusBorderColour);
            g.drawRect(backgroundBounds, borderThickness);
        }
    }

    void ComboBox::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        juce::String text;
        
        auto selectedIndex = getSelectedItemIndex();
        if (selectedIndex >= 0)
        {
            text = getItemText(selectedIndex);
        }

        auto textColour = theme->getComboBoxTextColour(enabled);
        auto font = theme->getBaseFont();

        auto leftPadding = kLeftPadding;
        auto rightPadding = kRightPadding;
        auto triangleBaseSize = kTriangleBaseSize;
        auto textBounds = bounds;
        textBounds.removeFromLeft(leftPadding);
        textBounds.removeFromRight(triangleBaseSize + rightPadding);

        g.setColour(textColour);
        g.setFont(font);
        g.drawText(text, textBounds, juce::Justification::centredLeft, false);
    }

    void ComboBox::drawTriangle(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        auto triangleColour = theme->getComboBoxTriangleColour(enabled);
        g.setColour(triangleColour);

        auto triangleBaseSize = kTriangleBaseSize;
        auto triangleHeight = triangleBaseSize * kTriangleHeightFactor;
        auto triangleX = bounds.getRight() - triangleBaseSize - kRightPadding;
        auto triangleY = bounds.getCentreY() - triangleHeight * 0.5f;

        auto trianglePath = createTrianglePath(triangleX, triangleY, triangleBaseSize);
        g.fillPath(trianglePath);
    }

    juce::Path ComboBox::createTrianglePath(float x, float y, float baseSize) const
    {
        juce::Path path;
        
        auto height = baseSize * kTriangleHeightFactor;
        
        path.startNewSubPath(x, y);
        path.lineTo(x + baseSize, y);
        path.lineTo(x + baseSize * 0.5f, y + height);
        path.closeSubPath();
        
        return path;
    }

    juce::Rectangle<float> ComboBox::calculateBackgroundBounds(const juce::Rectangle<float>& bounds) const
    {
        auto backgroundWidth = static_cast<float>((comboSize == Size::Normal) 
            ? kNormalBackgroundWidth 
            : kLargeBackgroundWidth);
        auto backgroundHeight = static_cast<float>(kBackgroundHeight);
        auto backgroundX = (bounds.getWidth() - backgroundWidth) / 2.0f;
        auto backgroundY = (bounds.getHeight() - backgroundHeight) / 2.0f;
        return juce::Rectangle<float>(bounds.getX() + backgroundX, bounds.getY() + backgroundY, backgroundWidth, backgroundHeight);
    }

    void ComboBox::showPopup()
    {
        if (isEnabled() && getNumItems() > 0)
        {
            isPopupOpen = true;
            repaint();
            PopupMenu::show(*this);
        }
    }

    void ComboBox::mouseDown(const juce::MouseEvent& e)
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

    void ComboBox::focusGained(juce::Component::FocusChangeType)
    {
        if (! focusableWidget.hasFocus())
        {
            focusableWidget.handleFocusGained(this);
        }
    }

    void ComboBox::focusLost(juce::Component::FocusChangeType)
    {
        focusableWidget.handleFocusLost(this);
    }
}

