#include "ComboBox.h"
#include "MultiColumnPopupMenu.h"
#include "ScrollablePopupMenu.h"

#include "GUI/Themes/Theme.h"

namespace tss
{
    ComboBox::ComboBox(Theme& theme, int width, int height, Style style)
        : juce::ComboBox()
        , theme_(&theme)
        , width_(width)
        , height_(height)
        , style_(style)
    {
        setOpaque(false);
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
            return;

        const auto bounds = getLocalBounds().toFloat();
        const auto enabled = isEnabled();
        const auto hasFocus = hasFocus_ || isPopupOpen_;
        const auto backgroundBounds = calculateBackgroundBounds(bounds);

        drawBackground(g, backgroundBounds, enabled);
        drawText(g, bounds, enabled);
        drawTriangle(g, bounds, enabled);
        drawBorderIfNeeded(g, bounds, backgroundBounds, enabled, hasFocus);
    }


    void ComboBox::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        const auto isButtonLike = (style_ == Style::ButtonLike);
        const auto backgroundColour = theme_->getComboBoxBackgroundColour(enabled, isButtonLike);
        
        g.setColour(backgroundColour);
        g.fillRect(bounds);
    }

    void ComboBox::drawBorderIfNeeded(juce::Graphics& g, const juce::Rectangle<float>& bounds, const juce::Rectangle<float>& backgroundBounds, bool enabled, bool hasFocus)
    {
        const auto isButtonLike = (style_ == Style::ButtonLike);
        
        if (isButtonLike)
        {
            const auto borderColour = theme_->getComboBoxBorderColour(enabled, isButtonLike);
            g.setColour(borderColour);
            g.drawRect(bounds, static_cast<float>(kBorderThicknessButtonLike_));
            return;
        }

        if (hasFocus)
        {
            const auto focusBorderColour = theme_->getComboBoxFocusBorderColour(isButtonLike);
            g.setColour(focusBorderColour);
            g.drawRect(backgroundBounds, static_cast<float>(kBorderThickness_));
        }
    }

    void ComboBox::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        const auto text = getSelectedItemText();
        const auto textColour = getTextColourForCurrentStyle(enabled);
        const auto textBounds = calculateTextBounds(bounds);
        drawTextInBounds(g, text, textBounds, textColour);
    }

    juce::String ComboBox::getSelectedItemText() const
    {
        const auto selectedIndex = getSelectedItemIndex();
        if (selectedIndex >= 0)
        {
            return getItemText(selectedIndex);
        }
        return juce::String();
    }

    juce::Colour ComboBox::getTextColourForCurrentStyle(bool enabled) const
    {
        const auto isButtonLike = (style_ == Style::ButtonLike);
        return theme_->getComboBoxTextColour(enabled, isButtonLike);
    }

    juce::Rectangle<float> ComboBox::calculateTextBounds(const juce::Rectangle<float>& bounds) const
    {
        auto textBounds = bounds;
        textBounds.removeFromLeft(kLeftPadding_);
        textBounds.removeFromRight(kTriangleBaseSize_);
        textBounds.removeFromRight(kRightPadding_);
        return textBounds;
    }

    void ComboBox::drawTextInBounds(juce::Graphics& g, const juce::String& text, const juce::Rectangle<float>& textBounds, const juce::Colour& textColour) const
    {
        const auto font = theme_->getBaseFont();
        g.setColour(textColour);
        g.setFont(font);
        g.drawText(text, textBounds, juce::Justification::centredLeft, false);
    }

    void ComboBox::drawTriangle(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        const auto isButtonLike = (style_ == Style::ButtonLike);
        const auto triangleColour = theme_->getComboBoxTriangleColour(enabled, isButtonLike);
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
        if (style_ == Style::ButtonLike)
            return bounds;
        
        const auto backgroundHeight = static_cast<float>(kBackgroundHeight_);
        const auto backgroundY = (bounds.getHeight() - backgroundHeight) * 0.5f;
        return juce::Rectangle<float>(bounds.getX(), bounds.getY() + backgroundY, bounds.getWidth(), backgroundHeight);
    }

    void ComboBox::showPopup()
    {
        if (! canShowPopup())
        {
            return;
        }

        showPopupAsynchronously();
        repaint();
    }

    bool ComboBox::canShowPopup() const
    {
        return isEnabled() && getNumItems() > 0;
    }

    void ComboBox::showPopupAsynchronously()
    {
        const auto useScrollableMode = (style_ == Style::ButtonLike);
        
        juce::MessageManager::callAsync([safePointer = SafePointer<ComboBox>(this), useScrollableMode]()
        {
            if (safePointer != nullptr && safePointer->canShowPopup())
            {
                safePointer->isPopupOpen_ = true;
                
                if (useScrollableMode)
                {
                    ScrollablePopupMenu::show(*safePointer);
                }
                else
                {
                    MultiColumnPopupMenu::show(*safePointer);
                }
            }
        });
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
        if (isEnabled() && !hasFocus_)
        {
            hasFocus_ = true;
            repaint();
        }
    }

    void ComboBox::focusLost(juce::Component::FocusChangeType)
    {
        hasFocus_ = false;
        repaint();
    }
}

