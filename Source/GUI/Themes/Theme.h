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
            Cream
        };

        explicit Theme(ColourVariant colourVariant);

        static std::unique_ptr<Theme> create(ColourVariant colourVariant);

        ColourVariant getColourVariant() const { return currentColourVariant; }


        //=====================================================================
        // COLOURS
        //=====================================================================

        // Panel colours ------------------------------------------------------
        juce::Colour getHeaderPanelBackgroundColour() const;
        juce::Colour getBodyPanelBackgroundColour() const;
        juce::Colour getFooterPanelBackgroundColour() const;

        // SectionHeader colours ------------------------------------------------
        juce::Colour getSectionHeaderTextColour() const;
        juce::Colour getSectionHeaderLineColourBlue() const;
        juce::Colour getSectionHeaderLineColourOrange() const;

        // ModuleHeader colours -------------------------------------------------
        juce::Colour getModuleHeaderTextColour() const;
        juce::Colour getModuleHeaderLineColourBlue() const;
        juce::Colour getModuleHeaderLineColourOrange() const;

        // GroupLabel colours -------------------------------------------------
        juce::Colour getGroupLabelTextColour() const;
        juce::Colour getGroupLabelLineColour() const;

        // Label colours ------------------------------------------------------
        juce::Colour getLabelTextColour() const;

        // VerticalSeparator colours ------------------------------------------
        juce::Colour getVerticalSeparatorLineColour() const;

        // HorizontalSeparator colours -----------------------------------------
        juce::Colour getHorizontalSeparatorLineColour() const;

        // Button colours -----------------------------------------------------
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
        juce::Colour getSliderTrackColour(bool isEnabled = true) const;
        juce::Colour getSliderValueBarColour(bool isEnabled = true) const;
        juce::Colour getSliderTextColour(bool isEnabled = true) const;
        juce::Colour getSliderFocusBorderColour() const;

        // ComboBox colours ---------------------------------------------------
        juce::Colour getComboBoxBackgroundColour(bool isEnabled = true, bool isButtonLike = false) const;
        juce::Colour getComboBoxBorderColour(bool isEnabled = true, bool isButtonLike = false) const;
        juce::Colour getComboBoxFocusBorderColour(bool isButtonLike = false) const;
        juce::Colour getComboBoxTriangleColour(bool isEnabled = true, bool isButtonLike = false) const;
        juce::Colour getComboBoxTextColour(bool isEnabled = true, bool isButtonLike = false) const;

        // PopupMenu colours --------------------------------------------------
        juce::Colour getPopupMenuBackgroundColour(bool isButtonLike = false) const;
        juce::Colour getPopupMenuBorderColour(bool isButtonLike = false) const;
        juce::Colour getPopupMenuSeparatorColour(bool isButtonLike = false) const;
        juce::Colour getPopupMenuTextColour(bool isButtonLike = false) const;
        juce::Colour getPopupMenuBackgroundHooverColour(bool isButtonLike = false) const;
        juce::Colour getPopupMenuTextHooverColour(bool isButtonLike = false) const;
        juce::Colour getPopupMenuScrollbarColour(bool isButtonLike = false) const;

        // NumberBox colours ---------------------------------------------------
        juce::Colour getNumberBoxTextColour() const;
        juce::Colour getNumberBoxDotColour() const;
        juce::Colour getNumberBoxEditorBackgroundColour() const;
        juce::Colour getNumberBoxEditorSelectionBackgroundColour() const;
        juce::Colour getNumberBoxEditorTextColour() const;

        // EnvelopeDisplay colours ----------------------------------------------
        juce::Colour getEnvelopeDisplayBackgroundColour() const;
        juce::Colour getEnvelopeDisplayBorderColour() const;
        juce::Colour getEnvelopeDisplayEnvelopeColour() const;

        // PatchNameDisplay colours ---------------------------------------------
        juce::Colour getPatchNameDisplayBackgroundColour() const;
        juce::Colour getPatchNameDisplayBorderColour() const;
        juce::Colour getPatchNameDisplayTextColour() const;

        // TrackGeneratorDisplay colours ----------------------------------------
        juce::Colour getTrackGeneratorDisplayBackgroundColour() const;
        juce::Colour getTrackGeneratorDisplayBorderColour() const;
        juce::Colour getTrackGeneratorDisplayShaperColour() const;
        
        
        //---------------------------------------------------------------------
        // FONTS
        //---------------------------------------------------------------------
        juce::Font getBaseFont() const;

    private:
        ColourVariant currentColourVariant;

        juce::Colour getColour(const ThemeColours::ColourElement& element) const;
    };
}

