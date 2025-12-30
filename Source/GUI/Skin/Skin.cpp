#include "Skin.h"
#include "SkinDimensions.h"
#include "SkinFonts.h"

namespace tss
{
    Skin::Skin(ColourVariant colourVariant)
        : currentColourVariant(colourVariant)
    {
    }

    juce::Colour Skin::getColour(const SkinColours::ColourElement& element) const
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
    juce::Colour Skin::getGuiBackgroundColour() const
    {
        return getColour(SkinColours::Gui::kGuiBackground);
    }

    // Panel colours ----------------------------------------------------------
    juce::Colour Skin::getHeaderPanelBackgroundColour() const
    {
        return getColour(SkinColours::Panel::kHeaderPanelBackground);
    }

    juce::Colour Skin::getMainPanelBackgroundColour() const
    {
        return getColour(SkinColours::Panel::kMainPanelBackground);
    }

    juce::Colour Skin::getFooterPanelBackgroundColour() const
    {
        return getColour(SkinColours::Panel::kFooterPanelBackground);
    }

    juce::Colour Skin::getPatchEditPanelBackgroundColour() const
    {
        return getColour(SkinColours::Panel::kPatchEditPanelBackground);
    }

    juce::Colour Skin::getPatchEditModulePanelBackgroundColour() const
    {
        return getColour(SkinColours::Panel::kPatchEditModulePanelBackground);
    }

    juce::Colour Skin::getMatrixModulationPanelBackgroundColour() const
    {
        return getColour(SkinColours::Panel::kMatrixModulationPanelBackground);
    }

    juce::Colour Skin::getPatchManagerPanelBackgroundColour() const
    {
        return getColour(SkinColours::Panel::kPatchManagerPanelBackground);
    }

    juce::Colour Skin::getMasterEditPanelBackgroundColour() const
    {
        return getColour(SkinColours::Panel::kMasterEditPanelBackground);
    }

    // SectionName colours ----------------------------------------------------
    juce::Colour Skin::getSectionNameBaseColour() const
    {
        return getColour(SkinColours::SectionName::kBase);
    }

    juce::Colour Skin::getSectionNameContentAreaColour() const
    {
        return getColour(SkinColours::SectionName::kContentArea);
    }

    juce::Colour Skin::getSectionNameTextColour() const
    {
        return getColour(SkinColours::SectionName::kText);
    }

    juce::Colour Skin::getSectionNameLineColourBlue() const
    {
        return getColour(SkinColours::SectionName::kLineBlue);
    }

    juce::Colour Skin::getSectionNameLineColourOrange() const
    {
        return getColour(SkinColours::SectionName::kLineOrange);
    }

    // ModuleName colours -----------------------------------------------------
    juce::Colour Skin::getModuleNameBaseColour() const
    {
        return getColour(SkinColours::ModuleName::kBase);
    }

    juce::Colour Skin::getModuleNameTextColour() const
    {
        return getColour(SkinColours::ModuleName::kText);
    }

    juce::Colour Skin::getModuleNameLineColourBlue() const
    {
        return getColour(SkinColours::ModuleName::kLineBlue);
    }

    juce::Colour Skin::getModuleNameLineColourOrange() const
    {
        return getColour(SkinColours::ModuleName::kLineOrange);
    }

    // GroupLabel colours -----------------------------------------------------
    juce::Colour Skin::getGroupLabelBaseColour() const
    {
        return getColour(SkinColours::GroupLabel::kBase);
    }

    juce::Colour Skin::getGroupLabelTextColour() const
    {
        return getColour(SkinColours::GroupLabel::kText);
    }

    juce::Colour Skin::getGroupLabelLineColour() const
    {
        return getColour(SkinColours::GroupLabel::kLine);
    }

    // ParameterLabel colours -------------------------------------------------
    juce::Colour Skin::getParameterLabelBackgroundColour() const
    {
        return getColour(SkinColours::ParameterLabel::kBackground);
    }

    juce::Colour Skin::getParameterLabelTextColour() const
    {
        return getColour(SkinColours::ParameterLabel::kText);
    }

    juce::Colour Skin::getParameterLabelBaseColour() const
    {
        return getColour(SkinColours::ParameterLabel::kBase);
    }

    // PanelSeparator colours -------------------------------------------------
    juce::Colour Skin::getPanelSeparatorBaseColour() const
    {
        return getColour(SkinColours::PanelSeparator::kBase);
    }

    juce::Colour Skin::getPanelSeparatorLineColour() const
    {
        return getColour(SkinColours::PanelSeparator::kLine);
    }

    // ParameterSeparator colours ---------------------------------------------
    juce::Colour Skin::getParameterSeparatorBaseColour() const
    {
        return getColour(SkinColours::ParameterSeparator::kBase);
    }

    juce::Colour Skin::getParameterSeparatorLineColour() const
    {
        return getColour(SkinColours::ParameterSeparator::kLine);
    }

    // Button colours ---------------------------------------------------------
    juce::Colour Skin::getButtonBaseColour() const
    {
        return getColour(SkinColours::Button::kBase);
    }

    juce::Colour Skin::getButtonBackgroundColourOff() const
    {
        return getColour(SkinColours::Button::kBackgroundDisabled);
    }

    juce::Colour Skin::getButtonBorderColourOff() const
    {
        return getColour(SkinColours::Button::kBorderDisabled);
    }

    juce::Colour Skin::getButtonTextColourOff() const
    {
        return getColour(SkinColours::Button::kTextDisabled);
    }

    juce::Colour Skin::getButtonBackgroundColourOn() const
    {
        return getColour(SkinColours::Button::kBackground);
    }

    juce::Colour Skin::getButtonBorderColourOn() const
    {
        return getColour(SkinColours::Button::kBorder);
    }

    juce::Colour Skin::getButtonTextColourOn() const
    {
        return getColour(SkinColours::Button::kText);
    }

    juce::Colour Skin::getButtonBackgroundColourHoover() const
    {
        return getColour(SkinColours::Button::kBackgroundHoover);
    }

    juce::Colour Skin::getButtonTextColourHoover() const
    {
        return getColour(SkinColours::Button::kTextHoover);
    }

    juce::Colour Skin::getButtonBackgroundColourClicked() const
    {
        return getColour(SkinColours::Button::kBackgroundClicked);
    }

    juce::Colour Skin::getButtonTextColourClicked() const
    {
        return getColour(SkinColours::Button::kTextClicked);
    }

    // Slider colours ---------------------------------------------------------
    juce::Colour Skin::getSliderBaseColour() const
    {
        return getColour(SkinColours::Slider::kBase);
    }

    juce::Colour Skin::getSliderBackgroundColour(bool isEnabled) const
    {
        return getColour(isEnabled ? SkinColours::Slider::kBackground : SkinColours::Slider::kBackgroundDisabled);
    }

    juce::Colour Skin::getSliderBorderColour(bool isEnabled, bool hasFocus) const
    {
        if (hasFocus)
        {
            return getColour(SkinColours::Slider::kBorderFocus);
        }
        return getColour(isEnabled ? SkinColours::Slider::kBorder : SkinColours::Slider::kBorderDisabled);
    }

    juce::Colour Skin::getSliderTrackColour(bool isEnabled) const
    {
        return getColour(isEnabled ? SkinColours::Slider::kTrack : SkinColours::Slider::kTrackDisabled);
    }

    juce::Colour Skin::getSliderTextColour(bool isEnabled) const
    {
        return getColour(isEnabled ? SkinColours::Slider::kText : SkinColours::Slider::kTextDisabled);
    }

    juce::Colour Skin::getSliderFocusBorderColour() const
    {
        return getSliderBorderColour(true, true);
    }

    // ComboBox colours -------------------------------------------------------
    juce::Colour Skin::getComboBoxBaseColour() const
    {
        return getColour(SkinColours::ComboBox::kBase);
    }

    juce::Colour Skin::getComboBoxBackgroundColour(bool isEnabled) const
    {
        return getColour(isEnabled ? SkinColours::ComboBox::kBackground : SkinColours::ComboBox::kBackgroundDisabled);
    }

    juce::Colour Skin::getComboBoxBorderColour(bool isEnabled, bool hasFocus) const
    {
        if (hasFocus)
        {
            return getColour(SkinColours::ComboBox::kBorderFocus);
        }
        return getColour(isEnabled ? SkinColours::ComboBox::kBorder : SkinColours::ComboBox::kBorderDisabled);
    }

    juce::Colour Skin::getComboBoxTriangleColour(bool isEnabled) const
    {
        return getColour(isEnabled ? SkinColours::ComboBox::kTriangle : SkinColours::ComboBox::kTriangleDisabled);
    }

    juce::Colour Skin::getComboBoxTextColour(bool isEnabled) const
    {
        return getColour(isEnabled ? SkinColours::ComboBox::kText : SkinColours::ComboBox::kTextDisabled);
    }

    // PopupMenu colours ------------------------------------------------------
    juce::Colour Skin::getPopupMenuBaseColour() const
    {
        return getColour(SkinColours::PopupMenu::kBase);
    }

    juce::Colour Skin::getPopupMenuBackgroundColour() const
    {
        return getColour(SkinColours::PopupMenu::kBackground);
    }

    juce::Colour Skin::getPopupMenuBorderColour() const
    {
        return getColour(SkinColours::PopupMenu::kBorder);
    }

    juce::Colour Skin::getPopupMenuSeparatorColour() const
    {
        return getColour(SkinColours::PopupMenu::kSeparator);
    }

    juce::Colour Skin::getPopupMenuTextColour() const
    {
        return getColour(SkinColours::PopupMenu::kText);
    }

    juce::Colour Skin::getPopupMenuBackgroundHooverColour() const
    {
        return getColour(SkinColours::PopupMenu::kBackgroundHoover);
    }

    juce::Colour Skin::getPopupMenuTextHooverColour() const
    {
        return getColour(SkinColours::PopupMenu::kTextHoover);
    }

    // EmptySlot colours -------------------------------------------------------
    juce::Colour Skin::getEmptySlotBaseColour() const
    {
        return getColour(SkinColours::EmptySlot::kBase);
    }


    //=========================================================================
    // DIMENSIONS
    //=========================================================================
    
    // GUI dimensions ---------------------------------------------------------
    int Skin::getGuiWidth() const
    {
        return SkinDimensions::Gui::kWidth;
    }

    int Skin::getGuiHeight() const
    {
        return SkinDimensions::Gui::kHeight;
    }

    // HeaderPanel dimensions -------------------------------------------------
    int Skin::getHeaderPanelHeight() const
    {
        return SkinDimensions::HeaderPanel::kHeight;
    }

    int Skin::getHeaderButtonWidth() const
    {
        return SkinDimensions::HeaderPanel::kButtonWidth;
    }

    int Skin::getHeaderButtonHeight() const
    {
        return SkinDimensions::HeaderPanel::kButtonHeight;
    }

    int Skin::getHeaderButtonSpacing() const
    {
        return SkinDimensions::HeaderPanel::kButtonSpacing;
    }

    // MainPanel dimensions ---------------------------------------------------
    int Skin::getMainPanelHeight() const
    {
        return SkinDimensions::MainPanel::kHeight;
    }

    int Skin::getMainPanelSpacing() const
    {
        return SkinDimensions::MainPanel::kSpacing;
    }

    // FooterPanel dimensions -------------------------------------------------
    int Skin::getFooterPanelHeight() const
    {
        return SkinDimensions::FooterPanel::kHeight;
    }

    // PatchEditPanel dimensions ----------------------------------------------
    int Skin::getPatchEditPanelWidth() const
    {
        return SkinDimensions::PatchEditPanel::kWidth;
    }

    int Skin::getPatchEditPanelHeight() const
    {
        return SkinDimensions::PatchEditPanel::kHeight;
    }

    // MasterEditPanel dimensions ---------------------------------------------
    int Skin::getMasterEditPanelWidth() const
    {
        return SkinDimensions::MasterEditPanel::kWidth;
    }

    int Skin::getMasterEditPanelHeight() const
    {
        return SkinDimensions::MasterEditPanel::kHeight;
    }

    // PatchEditModulePanel dimensions ----------------------------------------
    int Skin::getPatchEditModulePanelWidth() const
    {
        return SkinDimensions::PatchEditModulePanel::kWidth;
    }

    int Skin::getPatchEditModulePanelHeight() const
    {
        return SkinDimensions::PatchEditModulePanel::kHeight;
    }

    // SectionName dimensions -------------------------------------------------
    int Skin::getSectionNameHeight() const
    {
        return SkinDimensions::Widget::SectionName::kHeight;
    }

    float Skin::getSectionNameContentHeight() const
    {
        return SkinDimensions::Widget::SectionName::kContentHeight;
    }

    float Skin::getSectionNameLineHeight() const
    {
        return SkinDimensions::Widget::SectionName::kLineHeight;
    }

    float Skin::getSectionNameLeftLineWidth() const
    {
        return SkinDimensions::Widget::SectionName::kLeftLineWidth;
    }

    float Skin::getSectionNameTextSpacing() const
    {
        return SkinDimensions::Widget::SectionName::kTextSpacing;
    }

    // ModuleName dimensions --------------------------------------------------
    int Skin::getModuleNameHeight() const
    {
        return SkinDimensions::Widget::ModuleName::kHeight;
    }

    int Skin::getModuleNameNormalWidth() const
    {
        return SkinDimensions::Widget::ModuleName::kNormalWidth;
    }

    int Skin::getModuleNameLargeWidth() const
    {
        return SkinDimensions::Widget::ModuleName::kLargeWidth;
    }

    float Skin::getModuleNameTextLeftPadding() const
    {
        return SkinDimensions::Widget::ModuleName::kTextLeftPadding;
    }

    float Skin::getModuleNameTextAreaHeight() const
    {
        return SkinDimensions::Widget::ModuleName::kTextAreaHeight;
    }

    float Skin::getModuleNameLineThickness() const
    {
        return SkinDimensions::Widget::ModuleName::kLineThickness;
    }

    // GroupLabel dimensions --------------------------------------------------
    int Skin::getGroupLabelHeight() const
    {
        return SkinDimensions::Widget::GroupLabel::kHeight;
    }

    float Skin::getGroupLabelTopAreaHeight() const
    {
        return SkinDimensions::Widget::GroupLabel::kTopAreaHeight;
    }

    float Skin::getGroupLabelContentHeight() const
    {
        return SkinDimensions::Widget::GroupLabel::kContentHeight;
    }

    float Skin::getGroupLabelTextSpacing() const
    {
        return SkinDimensions::Widget::GroupLabel::kTextSpacing;
    }

    float Skin::getGroupLabelLineThickness() const
    {
        return SkinDimensions::Widget::GroupLabel::kLineThickness;
    }

    // ParameterLabel dimensions ----------------------------------------------
    int Skin::getParameterLabelWidth() const
    {
        return SkinDimensions::Widget::ParameterLabel::kWidth;
    }

    int Skin::getParameterLabelHeight() const
    {
        return SkinDimensions::Widget::ParameterLabel::kHeight;
    }

    float Skin::getParameterLabelBackgroundPadding() const
    {
        return SkinDimensions::Widget::ParameterLabel::kBackgroundPadding;
    }

    float Skin::getParameterLabelTextLeftPadding() const
    {
        return SkinDimensions::Widget::ParameterLabel::kTextLeftPadding;
    }

    // PanelSeparator dimensions ----------------------------------------------
    int Skin::getPanelSeparatorWidth() const
    {
        return SkinDimensions::Widget::PanelSeparator::kWidth;
    }

    int Skin::getPanelSeparatorHeight() const
    {
        return SkinDimensions::Widget::PanelSeparator::kHeight;
    }

    float Skin::getPanelSeparatorLineWidth() const
    {
        return SkinDimensions::Widget::PanelSeparator::kLineWidth;
    }

    // ParameterSeparator dimensions ------------------------------------------
    int Skin::getParameterSeparatorWidth() const
    {
        return SkinDimensions::Widget::ParameterSeparator::kWidth;
    }

    int Skin::getParameterSeparatorHeight() const
    {
        return SkinDimensions::Widget::ParameterSeparator::kHeight;
    }

    float Skin::getParameterSeparatorLineThickness() const
    {
        return SkinDimensions::Widget::ParameterSeparator::kLineThickness;
    }

    // Button dimensions ------------------------------------------------------
    int Skin::getButtonHeight() const
    {
        return SkinDimensions::Widget::Button::kHeight;
    }

    int Skin::getButtonDefaultWidth() const
    {
        return SkinDimensions::Widget::Button::kDefaultWidth;
    }

    float Skin::getButtonBorderThickness() const
    {
        return SkinDimensions::Widget::Button::kBorderThickness;
    }

    // FocusableWidget dimensions ---------------------------------------------
    float Skin::getFocusBorderThickness() const
    {
        return SkinDimensions::Widget::FocusableWidget::kBorderThickness;
    }

    // Slider dimensions
    int Skin::getSliderWidth() const
    {
        return SkinDimensions::Widget::Slider::kWidth;
    }

    int Skin::getSliderHeight() const
    {
        return SkinDimensions::Widget::Slider::kHeight;
    }

    int Skin::getSliderBackgroundWidth() const
    {
        return SkinDimensions::Widget::Slider::kBackgroundWidth;
    }

    int Skin::getSliderBackgroundHeight() const
    {
        return SkinDimensions::Widget::Slider::kBackgroundHeight;
    }

    double Skin::getSliderDragSensitivity() const
    {
        return SkinDimensions::Widget::Slider::kDragSensitivity;
    }

    double Skin::getSliderShiftKeyStep() const
    {
        return SkinDimensions::Widget::Slider::kShiftKeyStep;
    }

    // ComboBox dimensions ----------------------------------------------------
    int Skin::getComboBoxNormalWidth() const
    {
        return SkinDimensions::Widget::ComboBox::kNormalWidth;
    }

    int Skin::getComboBoxLargeWidth() const
    {
        return SkinDimensions::Widget::ComboBox::kLargeWidth;
    }

    int Skin::getComboBoxHeight() const
    {
        return SkinDimensions::Widget::ComboBox::kHeight;
    }

    int Skin::getComboBoxNormalBackgroundWidth() const
    {
        return SkinDimensions::Widget::ComboBox::kNormalBackgroundWidth;
    }

    int Skin::getComboBoxLargeBackgroundWidth() const
    {
        return SkinDimensions::Widget::ComboBox::kLargeBackgroundWidth;
    }

    int Skin::getComboBoxBackgroundHeight() const
    {
        return SkinDimensions::Widget::ComboBox::kBackgroundHeight;
    }

    float Skin::getComboBoxLeftPadding() const
    {
        return SkinDimensions::Widget::ComboBox::kLeftPadding;
    }

    float Skin::getComboBoxRightPadding() const
    {
        return SkinDimensions::Widget::ComboBox::kRightPadding;
    }

    float Skin::getComboBoxBorderThickness() const
    {
        return SkinDimensions::Widget::ComboBox::kBorderThickness;
    }

    int Skin::getComboBoxVerticalMargin() const
    {
        return SkinDimensions::Widget::ComboBox::kVerticalMargin;
    }

    float Skin::getComboBoxTriangleBaseSize() const
    {
        return SkinDimensions::Widget::ComboBox::kTriangleBaseSize;
    }

    // PopupMenu dimensions ---------------------------------------------------
    int Skin::getPopupMenuItemHeight() const
    {
        return SkinDimensions::Widget::PopupMenu::kItemHeight;
    }

    float Skin::getPopupMenuBorderThickness() const
    {
        return SkinDimensions::Widget::PopupMenu::kBorderThickness;
    }

    int Skin::getPopupMenuSeparatorWidth() const
    {
        return SkinDimensions::Widget::PopupMenu::kSeparatorWidth;
    }

    int Skin::getPopupMenuTextLeftPadding() const
    {
        return SkinDimensions::Widget::PopupMenu::kTextLeftPadding;
    }

    int Skin::getPopupMenuColumnThreshold() const
    {
        return SkinDimensions::Widget::PopupMenu::kColumnThreshold;
    }

    // EmptySlot dimensions ----------------------------------------------------
    int Skin::getEmptySlotWidth() const
    {
        return SkinDimensions::Widget::EmptySlot::kWidth;
    }

    int Skin::getEmptySlotHeight() const
    {
        return SkinDimensions::Widget::EmptySlot::kHeight;
    }

    //=========================================================================
    // FONTS
    //=========================================================================
    
    juce::Font Skin::getDefaultFont() const
    {
        return SkinFonts::getDefaultFont();
    }

    juce::Font Skin::getSectionNameFont() const
    {
        return SkinFonts::getSectionNameFont();
    }

    juce::Font Skin::getModuleNameFont() const
    {
        return SkinFonts::getModuleNameFont();
    }

    juce::Font Skin::getGroupLabelFont() const
    {
        return SkinFonts::getGroupLabelFont();
    }

    juce::Font Skin::getParameterLabelFont() const
    {
        return SkinFonts::getParameterLabelFont();
    }

    juce::Font Skin::getButtonFont() const
    {
        return SkinFonts::getButtonFont();
    }

    juce::Font Skin::getSliderFont() const
    {
        return SkinFonts::getSliderFont();
    }

    juce::Font Skin::getComboBoxFont() const
    {
        return SkinFonts::getComboBoxFont();
    }

    juce::Font Skin::getPopupMenuFont() const
    {
        return SkinFonts::getPopupMenuFont();
    }
}

