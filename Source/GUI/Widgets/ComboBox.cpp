#include "ComboBox.h"
#include "PopupMenu.h"

#include "../Themes/Theme.h"

namespace tss
{
    ComboBox::ComboBox(Theme& theme, int width, int height)
        : juce::ComboBox()
        , theme_(&theme)
        , width_(width)
        , height_(height)
    {
        setSize(width_, height_);
        setWantsKeyboardFocus(true);
        setColour(juce::ComboBox::textColourId, juce::Colours::transparentBlack);
    }

    void ComboBox::setTheme(Theme& theme)
    {
        theme_ = &theme;
        setColour(juce::ComboBox::textColourId, juce::Colours::transparentBlack);
    }

    void ComboBox::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr)
        {
            return;
        }

        const auto bounds = getLocalBounds().toFloat();
        const auto enabled = isEnabled();
        const auto hasFocus = focusableWidget_.hasFocus() || isPopupOpen_;

        const auto backgroundBounds = calculateBackgroundBounds(bounds);

        drawBase(g, bounds);
        drawBackground(g, backgroundBounds, enabled);
        drawBorder(g, bounds, backgroundBounds, enabled, hasFocus);
        drawText(g, bounds, enabled);
        drawTriangle(g, bounds, enabled);
    }

    void ComboBox::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto baseColour = theme_->getComboBoxBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void ComboBox::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        const auto backgroundColour = theme_->getComboBoxBackgroundColour(enabled);
        g.setColour(backgroundColour);
        g.fillRect(bounds);
    }

    void ComboBox::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, const juce::Rectangle<float>& backgroundBounds, bool enabled, bool hasFocus)
    {
        const auto borderColour = theme_->getComboBoxBorderColour(enabled, false);
        g.setColour(borderColour);
        const auto borderThickness = kBorderThickness_;
        g.drawRect(bounds, borderThickness);

        if (hasFocus)
        {
            const auto focusBorderColour = theme_->getComboBoxBorderColour(enabled, true);
            g.setColour(focusBorderColour);
            g.drawRect(backgroundBounds, borderThickness);
        }
    }

    void ComboBox::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        juce::String text;
        
        const auto selectedIndex = getSelectedItemIndex();
        if (selectedIndex >= 0)
        {
            text = getItemText(selectedIndex);
        }

        const auto textColour = theme_->getComboBoxTextColour(enabled);
        const auto font = theme_->getBaseFont();

        const auto leftPadding = kLeftPadding_;
        const auto rightPadding = kRightPadding_;
        const auto triangleBaseSize = kTriangleBaseSize_;
        auto textBounds = bounds;
        textBounds.removeFromLeft(leftPadding);
        textBounds.removeFromRight(triangleBaseSize + rightPadding);

        g.setColour(textColour);
        g.setFont(font);
        g.drawText(text, textBounds, juce::Justification::centredLeft, false);
    }

    void ComboBox::drawTriangle(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        const auto triangleColour = theme_->getComboBoxTriangleColour(enabled);
        g.setColour(triangleColour);

        const auto triangleBaseSize = kTriangleBaseSize_;
        const auto triangleHeight = triangleBaseSize * kTriangleHeightFactor_;
        const auto triangleX = bounds.getRight() - triangleBaseSize - kRightPadding_;
        const auto triangleY = bounds.getCentreY() - triangleHeight * 0.5f;

        const auto trianglePath = createTrianglePath(triangleX, triangleY, triangleBaseSize);
        g.fillPath(trianglePath);
    }

    juce::Path ComboBox::createTrianglePath(float x, float y, float baseSize) const
    {
        juce::Path path;
        
        const auto height = baseSize * kTriangleHeightFactor_;
        
        path.startNewSubPath(x, y);
        path.lineTo(x + baseSize, y);
        path.lineTo(x + baseSize * 0.5f, y + height);
        path.closeSubPath();
        
        return path;
    }

    juce::Rectangle<float> ComboBox::calculateBackgroundBounds(const juce::Rectangle<float>& bounds) const
    {
        const auto backgroundWidth = static_cast<float>(width_ - static_cast<int>(kLeftPadding_ + kRightPadding_));
        const auto backgroundHeight = static_cast<float>(kBackgroundHeight_);
        const auto backgroundX = (bounds.getWidth() - backgroundWidth) / 2.0f;
        const auto backgroundY = (bounds.getHeight() - backgroundHeight) / 2.0f;
        return juce::Rectangle<float>(bounds.getX() + backgroundX, bounds.getY() + backgroundY, backgroundWidth, backgroundHeight);
    }

    void ComboBox::showPopup()
    {
        if (! isEnabled() || getNumItems() == 0)
        {
            return;
        }

        // Defer popup creation and display asynchronously, just like JUCE does
        // This prevents blocking the message thread during popup creation
        juce::MessageManager::callAsync([safePointer = SafePointer<ComboBox>(this)]()
        {
            if (safePointer != nullptr && safePointer->isEnabled() && safePointer->getNumItems() > 0)
            {
                safePointer->isPopupOpen_ = true;
                PopupMenu::show(*safePointer);
            }
        });
        
        repaint();
    }

    void ComboBox::mouseDown(const juce::MouseEvent& e)
    {
        if (isEnabled())
        {
            if (e.mods.isLeftButtonDown())
            {
                showPopup();
            }
        }
    }

    void ComboBox::focusGained(juce::Component::FocusChangeType)
    {
        if (! focusableWidget_.hasFocus())
        {
            focusableWidget_.handleFocusGained(this);
        }
    }

    void ComboBox::focusLost(juce::Component::FocusChangeType)
    {
        focusableWidget_.handleFocusLost(this);
    }
}

