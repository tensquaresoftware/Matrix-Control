#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "SkinColours.h"

namespace tss
{
    class Skin
    {
    public:
        enum class ColourVariant
        {
            Black,
            Cream,
            Debug
        };

        explicit Skin(ColourVariant colourVariant);

        ColourVariant getColourVariant() const { return currentColourVariant; }


        //=====================================================================
        // COLOURS
        //=====================================================================
        
        // GUI colours --------------------------------------------------------
        juce::Colour getGuiBackgroundColour() const;

        // Panel colours ------------------------------------------------------
        juce::Colour getHeaderPanelBackgroundColour() const;
        juce::Colour getBodyPanelBackgroundColour() const;
        juce::Colour getFooterPanelBackgroundColour() const;
        juce::Colour getPatchEditPanelBackgroundColour() const;
        juce::Colour getPatchEditModulePanelBackgroundColour() const;
        juce::Colour getMatrixModulationPanelBackgroundColour() const;
        juce::Colour getPatchManagerPanelBackgroundColour() const;
        juce::Colour getMasterEditPanelBackgroundColour() const;

        // SectionName colours ------------------------------------------------
        juce::Colour getSectionNameBaseColour() const;
        juce::Colour getSectionNameContentAreaColour() const;
        juce::Colour getSectionNameTextColour() const;
        juce::Colour getSectionNameLineColourBlue() const;
        juce::Colour getSectionNameLineColourOrange() const;

        // ModuleName colours -------------------------------------------------
        juce::Colour getModuleNameBaseColour() const;
        juce::Colour getModuleNameTextColour() const;
        juce::Colour getModuleNameLineColourBlue() const;
        juce::Colour getModuleNameLineColourOrange() const;

        // GroupLabel colours -------------------------------------------------
        juce::Colour getGroupLabelBaseColour() const;
        juce::Colour getGroupLabelTextColour() const;
        juce::Colour getGroupLabelLineColour() const;

        // ParameterLabel colours ---------------------------------------------
        juce::Colour getParameterLabelBackgroundColour() const;
        juce::Colour getParameterLabelTextColour() const;
        juce::Colour getParameterLabelBaseColour() const;

        // PanelSeparator colours ---------------------------------------------
        juce::Colour getPanelSeparatorBaseColour() const;
        juce::Colour getPanelSeparatorLineColour() const;

        // ParameterSeparator colours -----------------------------------------
        juce::Colour getParameterSeparatorBaseColour() const;
        juce::Colour getParameterSeparatorLineColour() const;

        // Button colours -----------------------------------------------------
        juce::Colour getButtonBaseColour() const;
        juce::Colour getButtonBackgroundColourOff() const;
        juce::Colour getButtonBorderColourOff() const;
        juce::Colour getButtonTextColourOff() const;
        juce::Colour getButtonBackgroundColourOn() const;
        juce::Colour getButtonBorderColourOn() const;
        juce::Colour getButtonTextColourOn() const;
        juce::Colour getButtonBackgroundColourHoover() const;
        juce::Colour getButtonTextColourHoover() const;
        juce::Colour getButtonBackgroundColourClicked() const;
        juce::Colour getButtonTextColourClicked() const;

        // Slider colours -----------------------------------------------------
        juce::Colour getSliderBaseColour() const;
        juce::Colour getSliderBackgroundColour(bool isEnabled = true) const;
        juce::Colour getSliderBorderColour(bool isEnabled = true, bool hasFocus = false) const;
        juce::Colour getSliderTrackColour(bool isEnabled = true) const;
        juce::Colour getSliderTextColour(bool isEnabled = true) const;
        juce::Colour getSliderFocusBorderColour() const;

        // ComboBox colours ---------------------------------------------------
        juce::Colour getComboBoxBaseColour() const;
        juce::Colour getComboBoxBackgroundColour(bool isEnabled = true) const;
        juce::Colour getComboBoxBorderColour(bool isEnabled = true, bool hasFocus = false) const;
        juce::Colour getComboBoxTriangleColour(bool isEnabled = true) const;
        juce::Colour getComboBoxTextColour(bool isEnabled = true) const;

        // PopupMenu colours --------------------------------------------------
        juce::Colour getPopupMenuBaseColour() const;
        juce::Colour getPopupMenuBackgroundColour() const;
        juce::Colour getPopupMenuBorderColour() const;
        juce::Colour getPopupMenuSeparatorColour() const;
        juce::Colour getPopupMenuTextColour() const;
        juce::Colour getPopupMenuBackgroundHooverColour() const;
        juce::Colour getPopupMenuTextHooverColour() const;

        // EmptySlot colours --------------------------------------------------
        juce::Colour getEmptySlotBaseColour() const;
        
        
        //=====================================================================
        // DIMENSIONS
        //=====================================================================
        
        // GUI dimensions -----------------------------------------------------
        int getGuiWidth() const;
        int getGuiHeight() const;

        // HeaderPanel dimensions ---------------------------------------------
        int getHeaderPanelHeight() const;
        int getHeaderButtonWidth() const;
        int getHeaderButtonHeight() const;
        int getHeaderButtonSpacing() const;

        // BodyPanel dimensions -----------------------------------------------
        int getBodyPanelHeight() const;
        int getBodyPanelSpacing() const;

        // FooterPanel dimensions ---------------------------------------------
        int getFooterPanelHeight() const;

        // PatchEditPanel dimensions ------------------------------------------
        int getPatchEditPanelWidth() const;
        int getPatchEditPanelHeight() const;

        // MasterEditPanel dimensions -----------------------------------------
        int getMasterEditPanelWidth() const;
        int getMasterEditPanelHeight() const;

        // PatchEditModulePanel dimensions ------------------------------------
        int getPatchEditModulePanelWidth() const;
        int getPatchEditModulePanelHeight() const;

        // SectionName dimensions ---------------------------------------------
        int getSectionNameHeight() const;
        float getSectionNameContentHeight() const;
        float getSectionNameLineHeight() const;
        float getSectionNameLeftLineWidth() const;
        float getSectionNameTextSpacing() const;

        // ModuleName dimensions ----------------------------------------------
        int getModuleNameHeight() const;
        int getModuleNameNormalWidth() const;
        int getModuleNameLargeWidth() const;
        float getModuleNameTextLeftPadding() const;
        float getModuleNameTextAreaHeight() const;
        float getModuleNameLineThickness() const;

        // GroupLabel dimensions ----------------------------------------------
        int getGroupLabelHeight() const;
        float getGroupLabelTopAreaHeight() const;
        float getGroupLabelContentHeight() const;
        float getGroupLabelTextSpacing() const;
        float getGroupLabelLineThickness() const;

        // ParameterLabel dimensions ------------------------------------------
        int getParameterLabelWidth() const;
        int getParameterLabelHeight() const;
        float getParameterLabelBackgroundPadding() const;
        float getParameterLabelTextLeftPadding() const;

        // PanelSeparator dimensions ------------------------------------------
        int getPanelSeparatorWidth() const;
        int getPanelSeparatorHeight() const;
        float getPanelSeparatorLineWidth() const;

        // ParameterSeparator dimensions --------------------------------------
        int getParameterSeparatorWidth() const;
        int getParameterSeparatorHeight() const;
        float getParameterSeparatorLineThickness() const;

        // Button dimensions --------------------------------------------------
        int getButtonHeight() const;
        int getButtonDefaultWidth() const;
        float getButtonBorderThickness() const;

        // FocusableWidget dimensions -----------------------------------------
        float getFocusBorderThickness() const;

        // Slider dimensions --------------------------------------------------
        int getSliderWidth() const;
        int getSliderHeight() const;
        int getSliderBackgroundWidth() const;
        int getSliderBackgroundHeight() const;
        double getSliderDragSensitivity() const;
        double getSliderShiftKeyStep() const;

        // ComboBox dimensions ------------------------------------------------
        int getComboBoxNormalWidth() const;
        int getComboBoxLargeWidth() const;
        int getComboBoxHeight() const;
        int getComboBoxNormalBackgroundWidth() const;
        int getComboBoxLargeBackgroundWidth() const;
        int getComboBoxBackgroundHeight() const;
        float getComboBoxLeftPadding() const;
        float getComboBoxRightPadding() const;
        float getComboBoxBorderThickness() const;
        int getComboBoxVerticalMargin() const;
        float getComboBoxTriangleBaseSize() const;

        // PopupMenu dimensions -----------------------------------------------
        int getPopupMenuItemHeight() const;
        float getPopupMenuBorderThickness() const;
        int getPopupMenuSeparatorWidth() const;
        int getPopupMenuTextLeftPadding() const;
        int getPopupMenuColumnThreshold() const;

        // EmptySlot dimensions ------------------------------------------------
        int getEmptySlotWidth() const;
        int getEmptySlotHeight() const;

        
        //---------------------------------------------------------------------
        // FONTS
        //---------------------------------------------------------------------
        juce::Font getDefaultFont() const;
        juce::Font getSectionNameFont() const;
        juce::Font getModuleNameFont() const;
        juce::Font getGroupLabelFont() const;
        juce::Font getParameterLabelFont() const;
        juce::Font getButtonFont() const;
        juce::Font getSliderFont() const;
        juce::Font getComboBoxFont() const;
        juce::Font getPopupMenuFont() const;

    private:
        ColourVariant currentColourVariant;

        juce::Colour getColour(const SkinColours::ColourElement& element) const;
    };
}
