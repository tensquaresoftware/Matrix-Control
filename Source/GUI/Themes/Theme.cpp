#include "Theme.h"

namespace tss
{
    Theme::Theme(ColourVariant colourVariant)
        : currentColourVariant(colourVariant)
    {
    }

    std::unique_ptr<Theme> Theme::create(ColourVariant colourVariant)
    {
        return std::make_unique<Theme>(colourVariant);
    }

    juce::Colour Theme::getColour(const ThemeColours::ColourElement& element) const
    {
        switch (currentColourVariant)
        {
            case ColourVariant::Black:
                return juce::Colour(element.blackVariant);
            case ColourVariant::Cream:
                return juce::Colour(element.creamVariant);
        }
        return juce::Colour(element.blackVariant);
    }

    
    //=========================================================================
    // COLOURS
    //=========================================================================

    // Panel colours ----------------------------------------------------------
    juce::Colour Theme::getHeaderPanelBackgroundColour() const
    {
        return getColour(ThemeColours::Panels::kHeaderPanelBackground);
    }

    juce::Colour Theme::getBodyPanelBackgroundColour() const
    {
        return getColour(ThemeColours::Panels::kBodyPanelBackground);
    }

    juce::Colour Theme::getFooterPanelBackgroundColour() const
    {
        return getColour(ThemeColours::Panels::kFooterPanelBackground);
    }

    // SectionHeader colours ----------------------------------------------------
    juce::Colour Theme::getSectionHeaderTextColour() const
    {
        return getColour(ThemeColours::Widgets::SectionHeader::kText);
    }

    juce::Colour Theme::getSectionHeaderLineColourBlue() const
    {
        return getColour(ThemeColours::Widgets::SectionHeader::kLineBlue);
    }

    juce::Colour Theme::getSectionHeaderLineColourOrange() const
    {
        return getColour(ThemeColours::Widgets::SectionHeader::kLineOrange);
    }

    // ModuleHeader colours -----------------------------------------------------
    juce::Colour Theme::getModuleHeaderTextColour() const
    {
        return getColour(ThemeColours::Widgets::ModuleHeader::kText);
    }

    juce::Colour Theme::getModuleHeaderLineColourBlue() const
    {
        return getColour(ThemeColours::Widgets::ModuleHeader::kLineBlue);
    }

    juce::Colour Theme::getModuleHeaderLineColourOrange() const
    {
        return getColour(ThemeColours::Widgets::ModuleHeader::kLineOrange);
    }

    // GroupLabel colours -----------------------------------------------------
    juce::Colour Theme::getGroupLabelTextColour() const
    {
        return getColour(ThemeColours::Widgets::GroupLabel::kText);
    }

    juce::Colour Theme::getGroupLabelLineColour() const
    {
        return getColour(ThemeColours::Widgets::GroupLabel::kLine);
    }

    // Label colours ----------------------------------------------------------
    juce::Colour Theme::getLabelTextColour() const
    {
        return getColour(ThemeColours::Widgets::Label::kText);
    }

    // VerticalSeparator colours ----------------------------------------------
    juce::Colour Theme::getVerticalSeparatorLineColour() const
    {
        return getColour(ThemeColours::Widgets::VerticalSeparator::kLine);
    }

    // HorizontalSeparator colours --------------------------------------------
    juce::Colour Theme::getHorizontalSeparatorLineColour() const
    {
        return getColour(ThemeColours::Widgets::HorizontalSeparator::kLine);
    }

    // Button colours ---------------------------------------------------------
    juce::Colour Theme::getButtonBackgroundColourOff() const
    {
        return getColour(ThemeColours::Widgets::Button::kBackgroundDisabled);
    }

    juce::Colour Theme::getButtonBorderColourOff() const
    {
        return getColour(ThemeColours::Widgets::Button::kBorderDisabled);
    }

    juce::Colour Theme::getButtonTextColourOff() const
    {
        return getColour(ThemeColours::Widgets::Button::kTextDisabled);
    }

    juce::Colour Theme::getButtonBackgroundColourOn() const
    {
        return getColour(ThemeColours::Widgets::Button::kBackground);
    }

    juce::Colour Theme::getButtonBorderColourOn() const
    {
        return getColour(ThemeColours::Widgets::Button::kBorder);
    }

    juce::Colour Theme::getButtonTextColourOn() const
    {
        return getColour(ThemeColours::Widgets::Button::kText);
    }

    juce::Colour Theme::getButtonBackgroundColourHoover() const
    {
        return getColour(ThemeColours::Widgets::Button::kBackgroundHoover);
    }

    juce::Colour Theme::getButtonTextColourHoover() const
    {
        return getColour(ThemeColours::Widgets::Button::kTextHoover);
    }

    juce::Colour Theme::getButtonBackgroundColourClicked() const
    {
        return getColour(ThemeColours::Widgets::Button::kBackgroundClicked);
    }

    juce::Colour Theme::getButtonTextColourClicked() const
    {
        return getColour(ThemeColours::Widgets::Button::kTextClicked);
    }

    // Slider colours ---------------------------------------------------------
    juce::Colour Theme::getSliderTrackColour(bool isEnabled) const
    {
        return getColour(isEnabled ? ThemeColours::Widgets::Slider::kTrack : ThemeColours::Widgets::Slider::kTrackDisabled);
    }

    juce::Colour Theme::getSliderValueBarColour(bool isEnabled) const
    {
        return getColour(isEnabled ? ThemeColours::Widgets::Slider::kValueBar : ThemeColours::Widgets::Slider::kValueBarDisabled);
    }

    juce::Colour Theme::getSliderTextColour(bool isEnabled) const
    {
        return getColour(isEnabled ? ThemeColours::Widgets::Slider::kText : ThemeColours::Widgets::Slider::kTextDisabled);
    }

    juce::Colour Theme::getSliderFocusBorderColour() const
    {
        return getColour(ThemeColours::Widgets::Slider::kFocusBorder);
    }

    // ComboBox colours -------------------------------------------------------
    juce::Colour Theme::getComboBoxBackgroundColour(bool isEnabled, bool isButtonLike) const
    {
        if (isButtonLike)
        {
            return getColour(isEnabled ? ThemeColours::Widgets::ComboBox::ButtonLike::kBackground : ThemeColours::Widgets::ComboBox::ButtonLike::kBackgroundDisabled);
        }
        return getColour(isEnabled ? ThemeColours::Widgets::ComboBox::Standard::kBackground : ThemeColours::Widgets::ComboBox::Standard::kBackgroundDisabled);
    }

    juce::Colour Theme::getComboBoxBorderColour(bool isEnabled, bool isButtonLike) const
    {
        if (isButtonLike)
        {
            return getColour(isEnabled ? ThemeColours::Widgets::ComboBox::ButtonLike::kBorder : ThemeColours::Widgets::ComboBox::ButtonLike::kBorderDisabled);
        }
        return getColour(ThemeColours::Widgets::ComboBox::Standard::kBackground);
    }

    juce::Colour Theme::getComboBoxFocusBorderColour(bool isButtonLike) const
    {
        if (isButtonLike)
        {
            return getColour(ThemeColours::Widgets::ComboBox::ButtonLike::kBorder);
        }
        return getColour(ThemeColours::Widgets::ComboBox::Standard::kFocusBorder);
    }

    juce::Colour Theme::getComboBoxTriangleColour(bool isEnabled, bool isButtonLike) const
    {
        if (isButtonLike)
        {
            return getColour(isEnabled ? ThemeColours::Widgets::ComboBox::ButtonLike::kTriangle : ThemeColours::Widgets::ComboBox::ButtonLike::kTriangleDisabled);
        }
        return getColour(isEnabled ? ThemeColours::Widgets::ComboBox::Standard::kTriangle : ThemeColours::Widgets::ComboBox::Standard::kTriangleDisabled);
    }

    juce::Colour Theme::getComboBoxTextColour(bool isEnabled, bool isButtonLike) const
    {
        if (isButtonLike)
        {
            return getColour(isEnabled ? ThemeColours::Widgets::ComboBox::ButtonLike::kText : ThemeColours::Widgets::ComboBox::ButtonLike::kTextDisabled);
        }
        return getColour(isEnabled ? ThemeColours::Widgets::ComboBox::Standard::kText : ThemeColours::Widgets::ComboBox::Standard::kTextDisabled);
    }

    // PopupMenu colours ------------------------------------------------------
    juce::Colour Theme::getPopupMenuBackgroundColour(bool isButtonLike) const
    {
        if (isButtonLike)
        {
            return getColour(ThemeColours::Widgets::PopupMenu::ButtonLike::kBackground);
        }
        return getColour(ThemeColours::Widgets::PopupMenu::Standard::kBackground);
    }

    juce::Colour Theme::getPopupMenuBorderColour(bool isButtonLike) const
    {
        if (isButtonLike)
        {
            return getColour(ThemeColours::Widgets::PopupMenu::ButtonLike::kBorder);
        }
        return getColour(ThemeColours::Widgets::PopupMenu::Standard::kBorder);
    }

    juce::Colour Theme::getPopupMenuSeparatorColour(bool isButtonLike) const
    {
        if (isButtonLike)
        {
            return getColour(ThemeColours::Widgets::PopupMenu::ButtonLike::kSeparator);
        }
        return getColour(ThemeColours::Widgets::PopupMenu::Standard::kSeparator);
    }

    juce::Colour Theme::getPopupMenuTextColour(bool isButtonLike) const
    {
        if (isButtonLike)
        {
            return getColour(ThemeColours::Widgets::PopupMenu::ButtonLike::kText);
        }
        return getColour(ThemeColours::Widgets::PopupMenu::Standard::kText);
    }

    juce::Colour Theme::getPopupMenuBackgroundHooverColour(bool isButtonLike) const
    {
        if (isButtonLike)
        {
            return getColour(ThemeColours::Widgets::PopupMenu::ButtonLike::kBackgroundHoover);
        }
        return getColour(ThemeColours::Widgets::PopupMenu::Standard::kBackgroundHoover);
    }

    juce::Colour Theme::getPopupMenuTextHooverColour(bool isButtonLike) const
    {
        if (isButtonLike)
        {
            return getColour(ThemeColours::Widgets::PopupMenu::ButtonLike::kTextHoover);
        }
        return getColour(ThemeColours::Widgets::PopupMenu::Standard::kTextHoover);
    }

    juce::Colour Theme::getPopupMenuScrollbarColour(bool isButtonLike) const
    {
        jassert(isButtonLike && "PopupMenu scrollbar is only used with ButtonLike style");
        return getColour(ThemeColours::Widgets::PopupMenu::ButtonLike::kScrollbar);
    }

    // NumberBox colours -------------------------------------------------------
    juce::Colour Theme::getNumberBoxTextColour() const
    {
        return getColour(ThemeColours::Widgets::NumberBox::kText);
    }

    juce::Colour Theme::getNumberBoxDotColour() const
    {
        return getColour(ThemeColours::Widgets::NumberBox::kDot);
    }

    juce::Colour Theme::getNumberBoxEditorBackgroundColour() const
    {
        return getColour(ThemeColours::Widgets::NumberBox::kEditorBackground);
    }

    juce::Colour Theme::getNumberBoxEditorSelectionBackgroundColour() const
    {
        return getColour(ThemeColours::Widgets::NumberBox::kEditorSelectionBackground);
    }

    juce::Colour Theme::getNumberBoxEditorTextColour() const
    {
        return getColour(ThemeColours::Widgets::NumberBox::kEditorText);
    }

    // EnvelopeDisplay colours -------------------------------------------------
    juce::Colour Theme::getEnvelopeDisplayBackgroundColour() const
    {
        return getColour(ThemeColours::Widgets::EnvelopeDisplay::kBackground);
    }

    juce::Colour Theme::getEnvelopeDisplayBorderColour() const
    {
        return getColour(ThemeColours::Widgets::EnvelopeDisplay::kBorder);
    }

    juce::Colour Theme::getEnvelopeDisplayEnvelopeColour() const
    {
        return getColour(ThemeColours::Widgets::EnvelopeDisplay::kEnvelope);
    }

    // PatchNameDisplay colours ------------------------------------------------
    juce::Colour Theme::getPatchNameDisplayBackgroundColour() const
    {
        return getColour(ThemeColours::Widgets::PatchNameDisplay::kBackground);
    }

    juce::Colour Theme::getPatchNameDisplayBorderColour() const
    {
        return getColour(ThemeColours::Widgets::PatchNameDisplay::kBorder);
    }

    juce::Colour Theme::getPatchNameDisplayTextColour() const
    {
        return getColour(ThemeColours::Widgets::PatchNameDisplay::kText);
    }

    // TrackGeneratorDisplay colours -------------------------------------------
    juce::Colour Theme::getTrackGeneratorDisplayBackgroundColour() const
    {
        return getColour(ThemeColours::Widgets::TrackGeneratorDisplay::kBackground);
    }

    juce::Colour Theme::getTrackGeneratorDisplayBorderColour() const
    {
        return getColour(ThemeColours::Widgets::TrackGeneratorDisplay::kBorder);
    }

    juce::Colour Theme::getTrackGeneratorDisplayShaperColour() const
    {
        return getColour(ThemeColours::Widgets::TrackGeneratorDisplay::kShaper);
    }

    
    //=========================================================================
    // FONTS
    //=========================================================================
    
    juce::Font Theme::getBaseFont() const
    {
        juce::Font font(juce::FontOptions().withHeight(14.0f));
        
        auto availableTypefaces = juce::Font::findAllTypefaceNames();
        
        for (const auto& name : availableTypefaces)
        {
            if (name.containsIgnoreCase("PT Sans Narrow"))
            {
                font = juce::Font(juce::FontOptions(name, 14.0f, juce::Font::plain));
                break;
            }
        }
        
        return font;
    }
}

