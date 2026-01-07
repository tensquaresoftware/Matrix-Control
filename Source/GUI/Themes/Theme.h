#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

#include "ThemeColours.h"

namespace tss
{
    class Theme
    {
    public:
        enum class ColourVariant
        {
            Black,
            Cream,
            Debug
        };

        explicit Theme(ColourVariant colourVariant);

        static std::unique_ptr<Theme> create(ColourVariant colourVariant);

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
        
        
        //---------------------------------------------------------------------
        // FONTS
        //---------------------------------------------------------------------
        juce::Font getBaseFont() const;

    private:
        ColourVariant currentColourVariant;

        juce::Colour getColour(const ThemeColours::ColourElement& element) const;
    };
}

