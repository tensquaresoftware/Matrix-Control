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
            case ColourVariant::Debug:
                return juce::Colour(element.debugVariant);
        }
        return juce::Colour(element.blackVariant);
    }

    
    //=========================================================================
    // COLOURS
    //=========================================================================
    
    // GUI colours ------------------------------------------------------------
    juce::Colour Theme::getGuiBackgroundColour() const
    {
        return getColour(ThemeColours::Gui::kGuiBackground);
    }

    // Panel colours ----------------------------------------------------------
    juce::Colour Theme::getHeaderPanelBackgroundColour() const
    {
        return getColour(ThemeColours::Panel::kHeaderPanelBackground);
    }

    juce::Colour Theme::getBodyPanelBackgroundColour() const
    {
        return getColour(ThemeColours::Panel::kBodyPanelBackground);
    }

    juce::Colour Theme::getFooterPanelBackgroundColour() const
    {
        return getColour(ThemeColours::Panel::kFooterPanelBackground);
    }

    juce::Colour Theme::getPatchEditPanelBackgroundColour() const
    {
        return getColour(ThemeColours::Panel::kPatchEditPanelBackground);
    }

    juce::Colour Theme::getPatchEditModulePanelBackgroundColour() const
    {
        return getColour(ThemeColours::Panel::kPatchEditModulePanelBackground);
    }

    juce::Colour Theme::getMatrixModulationPanelBackgroundColour() const
    {
        return getColour(ThemeColours::Panel::kMatrixModulationPanelBackground);
    }

    juce::Colour Theme::getPatchManagerPanelBackgroundColour() const
    {
        return getColour(ThemeColours::Panel::kPatchManagerPanelBackground);
    }

    juce::Colour Theme::getMasterEditPanelBackgroundColour() const
    {
        return getColour(ThemeColours::Panel::kMasterEditPanelBackground);
    }

    // SectionHeader colours ----------------------------------------------------
    juce::Colour Theme::getSectionHeaderBaseColour() const
    {
        return getColour(ThemeColours::SectionHeader::kBase);
    }

    juce::Colour Theme::getSectionHeaderContentAreaColour() const
    {
        return getColour(ThemeColours::SectionHeader::kContentArea);
    }

    juce::Colour Theme::getSectionHeaderTextColour() const
    {
        return getColour(ThemeColours::SectionHeader::kText);
    }

    juce::Colour Theme::getSectionHeaderLineColourBlue() const
    {
        return getColour(ThemeColours::SectionHeader::kLineBlue);
    }

    juce::Colour Theme::getSectionHeaderLineColourOrange() const
    {
        return getColour(ThemeColours::SectionHeader::kLineOrange);
    }

    // ModuleHeader colours -----------------------------------------------------
    juce::Colour Theme::getModuleHeaderBaseColour() const
    {
        return getColour(ThemeColours::ModuleHeader::kBase);
    }

    juce::Colour Theme::getModuleHeaderTextColour() const
    {
        return getColour(ThemeColours::ModuleHeader::kText);
    }

    juce::Colour Theme::getModuleHeaderLineColourBlue() const
    {
        return getColour(ThemeColours::ModuleHeader::kLineBlue);
    }

    juce::Colour Theme::getModuleHeaderLineColourOrange() const
    {
        return getColour(ThemeColours::ModuleHeader::kLineOrange);
    }

    // GroupLabel colours -----------------------------------------------------
    juce::Colour Theme::getGroupLabelBaseColour() const
    {
        return getColour(ThemeColours::GroupLabel::kBase);
    }

    juce::Colour Theme::getGroupLabelTextColour() const
    {
        return getColour(ThemeColours::GroupLabel::kText);
    }

    juce::Colour Theme::getGroupLabelLineColour() const
    {
        return getColour(ThemeColours::GroupLabel::kLine);
    }

    // ParameterLabel colours -------------------------------------------------
    juce::Colour Theme::getParameterLabelBackgroundColour() const
    {
        return getColour(ThemeColours::ParameterLabel::kBackground);
    }

    juce::Colour Theme::getParameterLabelTextColour() const
    {
        return getColour(ThemeColours::ParameterLabel::kText);
    }

    juce::Colour Theme::getParameterLabelBaseColour() const
    {
        return getColour(ThemeColours::ParameterLabel::kBase);
    }

    // PanelSeparator colours -------------------------------------------------
    juce::Colour Theme::getPanelSeparatorBaseColour() const
    {
        return getColour(ThemeColours::PanelSeparator::kBase);
    }

    juce::Colour Theme::getPanelSeparatorLineColour() const
    {
        return getColour(ThemeColours::PanelSeparator::kLine);
    }

    // ParameterSeparator colours ---------------------------------------------
    juce::Colour Theme::getParameterSeparatorBaseColour() const
    {
        return getColour(ThemeColours::ParameterSeparator::kBase);
    }

    juce::Colour Theme::getParameterSeparatorLineColour() const
    {
        return getColour(ThemeColours::ParameterSeparator::kLine);
    }

    // Button colours ---------------------------------------------------------
    juce::Colour Theme::getButtonBaseColour() const
    {
        return getColour(ThemeColours::Button::kBase);
    }

    juce::Colour Theme::getButtonBackgroundColourOff() const
    {
        return getColour(ThemeColours::Button::kBackgroundDisabled);
    }

    juce::Colour Theme::getButtonBorderColourOff() const
    {
        return getColour(ThemeColours::Button::kBorderDisabled);
    }

    juce::Colour Theme::getButtonTextColourOff() const
    {
        return getColour(ThemeColours::Button::kTextDisabled);
    }

    juce::Colour Theme::getButtonBackgroundColourOn() const
    {
        return getColour(ThemeColours::Button::kBackground);
    }

    juce::Colour Theme::getButtonBorderColourOn() const
    {
        return getColour(ThemeColours::Button::kBorder);
    }

    juce::Colour Theme::getButtonTextColourOn() const
    {
        return getColour(ThemeColours::Button::kText);
    }

    juce::Colour Theme::getButtonBackgroundColourHoover() const
    {
        return getColour(ThemeColours::Button::kBackgroundHoover);
    }

    juce::Colour Theme::getButtonTextColourHoover() const
    {
        return getColour(ThemeColours::Button::kTextHoover);
    }

    juce::Colour Theme::getButtonBackgroundColourClicked() const
    {
        return getColour(ThemeColours::Button::kBackgroundClicked);
    }

    juce::Colour Theme::getButtonTextColourClicked() const
    {
        return getColour(ThemeColours::Button::kTextClicked);
    }

    // Slider colours ---------------------------------------------------------
    juce::Colour Theme::getSliderBaseColour() const
    {
        return getColour(ThemeColours::Slider::kBase);
    }

    juce::Colour Theme::getSliderBackgroundColour(bool isEnabled) const
    {
        return getColour(isEnabled ? ThemeColours::Slider::kBackground : ThemeColours::Slider::kBackgroundDisabled);
    }

    juce::Colour Theme::getSliderBorderColour(bool isEnabled, bool hasFocus) const
    {
        if (hasFocus)
        {
            return getColour(ThemeColours::Slider::kBorderFocus);
        }
        return getColour(isEnabled ? ThemeColours::Slider::kBorder : ThemeColours::Slider::kBorderDisabled);
    }

    juce::Colour Theme::getSliderTrackColour(bool isEnabled) const
    {
        return getColour(isEnabled ? ThemeColours::Slider::kTrack : ThemeColours::Slider::kTrackDisabled);
    }

    juce::Colour Theme::getSliderTextColour(bool isEnabled) const
    {
        return getColour(isEnabled ? ThemeColours::Slider::kText : ThemeColours::Slider::kTextDisabled);
    }

    juce::Colour Theme::getSliderFocusBorderColour() const
    {
        return getSliderBorderColour(true, true);
    }

    // ComboBox colours -------------------------------------------------------
    juce::Colour Theme::getComboBoxBaseColour() const
    {
        return getColour(ThemeColours::ComboBox::kBase);
    }

    juce::Colour Theme::getComboBoxBackgroundColour(bool isEnabled) const
    {
        return getColour(isEnabled ? ThemeColours::ComboBox::kBackground : ThemeColours::ComboBox::kBackgroundDisabled);
    }

    juce::Colour Theme::getComboBoxBorderColour(bool isEnabled, bool hasFocus) const
    {
        if (hasFocus)
        {
            return getColour(ThemeColours::ComboBox::kBorderFocus);
        }
        return getColour(isEnabled ? ThemeColours::ComboBox::kBorder : ThemeColours::ComboBox::kBorderDisabled);
    }

    juce::Colour Theme::getComboBoxTriangleColour(bool isEnabled) const
    {
        return getColour(isEnabled ? ThemeColours::ComboBox::kTriangle : ThemeColours::ComboBox::kTriangleDisabled);
    }

    juce::Colour Theme::getComboBoxTextColour(bool isEnabled) const
    {
        return getColour(isEnabled ? ThemeColours::ComboBox::kText : ThemeColours::ComboBox::kTextDisabled);
    }

    // PopupMenu colours ------------------------------------------------------
    juce::Colour Theme::getPopupMenuBaseColour() const
    {
        return getColour(ThemeColours::PopupMenu::kBase);
    }

    juce::Colour Theme::getPopupMenuBackgroundColour() const
    {
        return getColour(ThemeColours::PopupMenu::kBackground);
    }

    juce::Colour Theme::getPopupMenuBorderColour() const
    {
        return getColour(ThemeColours::PopupMenu::kBorder);
    }

    juce::Colour Theme::getPopupMenuSeparatorColour() const
    {
        return getColour(ThemeColours::PopupMenu::kSeparator);
    }

    juce::Colour Theme::getPopupMenuTextColour() const
    {
        return getColour(ThemeColours::PopupMenu::kText);
    }

    juce::Colour Theme::getPopupMenuBackgroundHooverColour() const
    {
        return getColour(ThemeColours::PopupMenu::kBackgroundHoover);
    }

    juce::Colour Theme::getPopupMenuTextHooverColour() const
    {
        return getColour(ThemeColours::PopupMenu::kTextHoover);
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

