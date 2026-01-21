#include "ComboBox.h"
#include "PopupMenu.h"

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
        setOpaque(true);
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

        g.fillAll(theme_->getGuiBackgroundColour());

        const auto bounds = getLocalBounds().toFloat();
        const auto enabled = isEnabled();
        const auto hasFocus = focusableWidget_.hasFocus() || isPopupOpen_;

        const auto backgroundBounds = calculateBackgroundBounds(bounds);

        drawBackground(g, backgroundBounds, enabled);
        drawBorder(g, bounds, backgroundBounds, enabled, hasFocus);
        drawText(g, bounds, enabled);
        drawTriangle(g, bounds, enabled);
    }


    void ComboBox::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
    {
        juce::Colour backgroundColour;
        
        if (style_ == Style::ButtonLike)
        {
            if (! enabled)
            {
                backgroundColour = theme_->getButtonBackgroundColourOff();
            }
            else
            {
                backgroundColour = theme_->getButtonBackgroundColourOn();
            }
        }
        else
        {
            backgroundColour = theme_->getComboBoxBackgroundColour(enabled);
        }
        
        g.setColour(backgroundColour);
        g.fillRect(bounds);
    }

    void ComboBox::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, const juce::Rectangle<float>& backgroundBounds, bool enabled, bool hasFocus)
    {
        const auto borderStyle = getBorderColourAndThickness(enabled);
        drawMainBorder(g, bounds, borderStyle);
        drawFocusBorderIfNeeded(g, backgroundBounds, enabled, hasFocus, borderStyle.thickness);
    }

    ComboBox::BorderStyle ComboBox::getBorderColourAndThickness(bool enabled) const
    {
        BorderStyle style;
        
        if (style_ == Style::ButtonLike)
        {
            if (! enabled)
            {
                style.colour = theme_->getButtonBorderColourOff();
            }
            else
            {
                style.colour = theme_->getButtonBorderColourOn();
            }
            style.thickness = kBorderThicknessButtonLike_;
        }
        else
        {
            style.colour = theme_->getComboBoxBorderColour(enabled, false);
            style.thickness = kBorderThickness_;
        }
        
        return style;
    }

    void ComboBox::drawMainBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, const BorderStyle& borderStyle) const
    {
        g.setColour(borderStyle.colour);
        g.drawRect(bounds, borderStyle.thickness);
    }

    void ComboBox::drawFocusBorderIfNeeded(juce::Graphics& g, const juce::Rectangle<float>& backgroundBounds, bool enabled, bool hasFocus, float borderThickness) const
    {
        if (hasFocus && style_ != Style::ButtonLike)
        {
            const auto focusBorderColour = theme_->getComboBoxBorderColour(enabled, true);
            g.setColour(focusBorderColour);
            g.drawRect(backgroundBounds, borderThickness);
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
        if (style_ == Style::ButtonLike)
        {
            if (! enabled)
            {
                return theme_->getButtonTextColourOff();
            }
            return theme_->getButtonTextColourOn();
        }
        return theme_->getComboBoxTextColour(enabled);
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
        if (style_ == Style::ButtonLike)
        {
            return bounds;
        }
        
        const auto backgroundWidth = static_cast<float>(width_);
        const auto backgroundHeight = static_cast<float>(kBackgroundHeight_);
        const auto backgroundX = (bounds.getWidth() - backgroundWidth) / 2.0f;
        const auto backgroundY = (bounds.getHeight() - backgroundHeight) / 2.0f;
        return juce::Rectangle<float>(bounds.getX() + backgroundX, bounds.getY() + backgroundY, backgroundWidth, backgroundHeight);
    }

    void ComboBox::showPopup()
    {
        if (! canShowPopup())
        {
            return;
        }

        const auto displayMode = determineDisplayMode();
        showPopupAsynchronously(displayMode);
        repaint();
    }

    bool ComboBox::canShowPopup() const
    {
        return isEnabled() && getNumItems() > 0;
    }

    PopupMenu::DisplayMode ComboBox::determineDisplayMode() const
    {
        if (style_ == Style::ButtonLike)
        {
            return PopupMenu::DisplayMode::SingleColumnScrollable;
        }
        return PopupMenu::DisplayMode::MultiColumn;
    }

    void ComboBox::showPopupAsynchronously(PopupMenu::DisplayMode displayMode)
    {
        juce::MessageManager::callAsync([safePointer = SafePointer<ComboBox>(this), displayMode]()
        {
            if (safePointer != nullptr && safePointer->canShowPopup())
            {
                safePointer->isPopupOpen_ = true;
                PopupMenu::show(*safePointer, displayMode);
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

