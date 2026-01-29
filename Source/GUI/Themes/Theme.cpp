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
    
    // GUI colours ------------------------------------------------------------
    juce::Colour Theme::getGuiBackgroundColour() const
    {
        return getColour(ThemeColours::Gui::kGuiBackground);
    }

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

    juce::Colour Theme::getPatchEditPanelBackgroundColour() const
    {
        return getColour(ThemeColours::Panels::kPatchEditPanelBackground);
    }

    juce::Colour Theme::getPatchEditModulePanelBackgroundColour() const
    {
        return getColour(ThemeColours::Panels::kPatchEditModulePanelBackground);
    }

    juce::Colour Theme::getMatrixModulationPanelBackgroundColour() const
    {
        return getColour(ThemeColours::Panels::kMatrixModulationPanelBackground);
    }

    juce::Colour Theme::getPatchManagerPanelBackgroundColour() const
    {
        return getColour(ThemeColours::Panels::kPatchManagerPanelBackground);
    }

    juce::Colour Theme::getMasterEditPanelBackgroundColour() const
    {
        return getColour(ThemeColours::Panels::kMasterEditPanelBackground);
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
    juce::Colour Theme::getComboBoxBackgroundColour(bool isEnabled) const
    {
        return getColour(isEnabled ? ThemeColours::Widgets::ComboBox::kBackground : ThemeColours::Widgets::ComboBox::kBackgroundDisabled);
    }

    juce::Colour Theme::getComboBoxFocusBorderColour() const
    {
        return getColour(ThemeColours::Widgets::ComboBox::kFocusBorder);
    }

    juce::Colour Theme::getComboBoxTriangleColour(bool isEnabled) const
    {
        return getColour(isEnabled ? ThemeColours::Widgets::ComboBox::kTriangle : ThemeColours::Widgets::ComboBox::kTriangleDisabled);
    }

    juce::Colour Theme::getComboBoxTextColour(bool isEnabled) const
    {
        return getColour(isEnabled ? ThemeColours::Widgets::ComboBox::kText : ThemeColours::Widgets::ComboBox::kTextDisabled);
    }

    // PopupMenu colours ------------------------------------------------------
    juce::Colour Theme::getPopupMenuBackgroundColour() const
    {
        return getColour(ThemeColours::Widgets::PopupMenu::kBackground);
    }

    juce::Colour Theme::getPopupMenuBorderColour() const
    {
        return getColour(ThemeColours::Widgets::PopupMenu::kBorder);
    }

    juce::Colour Theme::getPopupMenuSeparatorColour() const
    {
        return getColour(ThemeColours::Widgets::PopupMenu::kSeparator);
    }

    juce::Colour Theme::getPopupMenuTextColour() const
    {
        return getColour(ThemeColours::Widgets::PopupMenu::kText);
    }

    juce::Colour Theme::getPopupMenuBackgroundHooverColour() const
    {
        return getColour(ThemeColours::Widgets::PopupMenu::kBackgroundHoover);
    }

    juce::Colour Theme::getPopupMenuTextHooverColour() const
    {
        return getColour(ThemeColours::Widgets::PopupMenu::kTextHoover);
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

