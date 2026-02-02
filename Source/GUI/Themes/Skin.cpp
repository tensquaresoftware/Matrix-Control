#include "Skin.h"

#include "ThemeColours.h"

namespace tss
{
    Skin::Skin(ColourVariant variant)
        : currentVariant_(variant)
    {
        initializeDefaultValues();
        initializeDefaultColours();
    }

    std::unique_ptr<Skin> Skin::create(ColourVariant variant)
    {
        return std::make_unique<Skin>(variant);
    }

    float Skin::getValue(SkinValueId valueId) const
    {
        const auto it = values_.find(valueId);
        if (it != values_.end())
            return it->second;

        const int index = static_cast<int>(valueId);
        if (index >= 0 && index < static_cast<int>(SkinValueId::kNumValues))
            return kDefaultValues[index];

        return 0.0f;
    }

    juce::Colour Skin::getColour(SkinColourId colourId) const
    {
        const auto it = colours_.find(colourId);
        if (it != colours_.end())
            return it->second;

        return juce::Colours::white;
    }

    void Skin::setValue(SkinValueId valueId, float value)
    {
        values_[valueId] = value;
    }

    void Skin::setColour(SkinColourId colourId, juce::Colour colour)
    {
        colours_[colourId] = colour;
    }

    juce::Font Skin::getBaseFont() const
    {
        return juce::Font(juce::FontOptions{}.withHeight(10.0f));
    }

    void Skin::initializeDefaultValues()
    {
        for (int i = 0; i < static_cast<int>(SkinValueId::kNumValues); ++i)
        {
            values_[static_cast<SkinValueId>(i)] = kDefaultValues[i];
        }
    }

    void Skin::initializeDefaultColours()
    {
        if (currentVariant_ == ColourVariant::Black)
            initializeBlackVariantColours();
        else
            initializeCreamVariantColours();
    }

    void Skin::initializeBlackVariantColours()
    {
        using namespace ThemeColours;

        colours_[SkinColourId::kHeaderPanelBackground] = juce::Colour(Panels::kHeaderPanelBackground.blackVariant);
        colours_[SkinColourId::kBodyPanelBackground] = juce::Colour(Panels::kBodyPanelBackground.blackVariant);
        colours_[SkinColourId::kFooterPanelBackground] = juce::Colour(Panels::kFooterPanelBackground.blackVariant);

        colours_[SkinColourId::kSectionHeaderText] = juce::Colour(Widgets::SectionHeader::kText.blackVariant);
        colours_[SkinColourId::kSectionHeaderLineBlue] = juce::Colour(Widgets::SectionHeader::kLineBlue.blackVariant);
        colours_[SkinColourId::kSectionHeaderLineOrange] = juce::Colour(Widgets::SectionHeader::kLineOrange.blackVariant);

        colours_[SkinColourId::kModuleHeaderText] = juce::Colour(Widgets::ModuleHeader::kText.blackVariant);
        colours_[SkinColourId::kModuleHeaderLineBlue] = juce::Colour(Widgets::ModuleHeader::kLineBlue.blackVariant);
        colours_[SkinColourId::kModuleHeaderLineOrange] = juce::Colour(Widgets::ModuleHeader::kLineOrange.blackVariant);

        colours_[SkinColourId::kGroupLabelText] = juce::Colour(Widgets::GroupLabel::kText.blackVariant);
        colours_[SkinColourId::kGroupLabelLine] = juce::Colour(Widgets::GroupLabel::kLine.blackVariant);

        colours_[SkinColourId::kLabelText] = juce::Colour(Widgets::Label::kText.blackVariant);

        colours_[SkinColourId::kVerticalSeparatorLine] = juce::Colour(Widgets::VerticalSeparator::kLine.blackVariant);
        colours_[SkinColourId::kHorizontalSeparatorLine] = juce::Colour(Widgets::HorizontalSeparator::kLine.blackVariant);

        colours_[SkinColourId::kButtonBackgroundOff] = juce::Colour(Widgets::Button::kBackground.blackVariant);
        colours_[SkinColourId::kButtonBorderOff] = juce::Colour(Widgets::Button::kBorder.blackVariant);
        colours_[SkinColourId::kButtonTextOff] = juce::Colour(Widgets::Button::kText.blackVariant);
        colours_[SkinColourId::kButtonBackgroundOn] = juce::Colour(Widgets::Button::kBackground.blackVariant);
        colours_[SkinColourId::kButtonBorderOn] = juce::Colour(Widgets::Button::kBorder.blackVariant);
        colours_[SkinColourId::kButtonTextOn] = juce::Colour(Widgets::Button::kText.blackVariant);
        colours_[SkinColourId::kButtonBackgroundHover] = juce::Colour(Widgets::Button::kBackgroundHoover.blackVariant);
        colours_[SkinColourId::kButtonTextHover] = juce::Colour(Widgets::Button::kTextHoover.blackVariant);
        colours_[SkinColourId::kButtonBackgroundClicked] = juce::Colour(Widgets::Button::kBackgroundClicked.blackVariant);
        colours_[SkinColourId::kButtonTextClicked] = juce::Colour(Widgets::Button::kTextClicked.blackVariant);

        colours_[SkinColourId::kSliderTrackEnabled] = juce::Colour(Widgets::Slider::kTrack.blackVariant);
        colours_[SkinColourId::kSliderTrackDisabled] = juce::Colour(Widgets::Slider::kTrackDisabled.blackVariant);
        colours_[SkinColourId::kSliderValueBarEnabled] = juce::Colour(Widgets::Slider::kValueBar.blackVariant);
        colours_[SkinColourId::kSliderValueBarDisabled] = juce::Colour(Widgets::Slider::kValueBarDisabled.blackVariant);
        colours_[SkinColourId::kSliderTextEnabled] = juce::Colour(Widgets::Slider::kText.blackVariant);
        colours_[SkinColourId::kSliderTextDisabled] = juce::Colour(Widgets::Slider::kTextDisabled.blackVariant);
        colours_[SkinColourId::kSliderFocusBorder] = juce::Colour(Widgets::Slider::kFocusBorder.blackVariant);

        colours_[SkinColourId::kComboBoxBackgroundEnabled] = juce::Colour(Widgets::ComboBox::Standard::kBackground.blackVariant);
        colours_[SkinColourId::kComboBoxBackgroundDisabled] = juce::Colour(Widgets::ComboBox::Standard::kBackgroundDisabled.blackVariant);
        colours_[SkinColourId::kComboBoxBorderEnabled] = juce::Colour(Widgets::ComboBox::ButtonLike::kBorder.blackVariant);
        colours_[SkinColourId::kComboBoxBorderDisabled] = juce::Colour(Widgets::ComboBox::ButtonLike::kBorderDisabled.blackVariant);
        colours_[SkinColourId::kComboBoxFocusBorder] = juce::Colour(Widgets::ComboBox::Standard::kFocusBorder.blackVariant);
        colours_[SkinColourId::kComboBoxTriangleEnabled] = juce::Colour(Widgets::ComboBox::Standard::kTriangle.blackVariant);
        colours_[SkinColourId::kComboBoxTriangleDisabled] = juce::Colour(Widgets::ComboBox::Standard::kTriangleDisabled.blackVariant);
        colours_[SkinColourId::kComboBoxTextEnabled] = juce::Colour(Widgets::ComboBox::Standard::kText.blackVariant);
        colours_[SkinColourId::kComboBoxTextDisabled] = juce::Colour(Widgets::ComboBox::Standard::kTextDisabled.blackVariant);

        colours_[SkinColourId::kPopupMenuBackground] = juce::Colour(Widgets::PopupMenu::Standard::kBackground.blackVariant);
        colours_[SkinColourId::kPopupMenuBorder] = juce::Colour(Widgets::PopupMenu::Standard::kBorder.blackVariant);
        colours_[SkinColourId::kPopupMenuSeparator] = juce::Colour(Widgets::PopupMenu::Standard::kSeparator.blackVariant);
        colours_[SkinColourId::kPopupMenuText] = juce::Colour(Widgets::PopupMenu::Standard::kText.blackVariant);
        colours_[SkinColourId::kPopupMenuBackgroundHover] = juce::Colour(Widgets::PopupMenu::Standard::kBackgroundHoover.blackVariant);
        colours_[SkinColourId::kPopupMenuTextHover] = juce::Colour(Widgets::PopupMenu::Standard::kTextHoover.blackVariant);
        colours_[SkinColourId::kPopupMenuScrollbar] = juce::Colour(Widgets::PopupMenu::ButtonLike::kScrollbar.blackVariant);

        colours_[SkinColourId::kNumberBoxText] = juce::Colour(Widgets::NumberBox::kText.blackVariant);
        colours_[SkinColourId::kNumberBoxDot] = juce::Colour(Widgets::NumberBox::kDot.blackVariant);
        colours_[SkinColourId::kNumberBoxEditorBackground] = juce::Colour(Widgets::NumberBox::kEditorBackground.blackVariant);
        colours_[SkinColourId::kNumberBoxEditorSelectionBackground] = juce::Colour(Widgets::NumberBox::kEditorSelectionBackground.blackVariant);
        colours_[SkinColourId::kNumberBoxEditorText] = juce::Colour(Widgets::NumberBox::kEditorText.blackVariant);

        colours_[SkinColourId::kEnvelopeDisplayBackground] = juce::Colour(Widgets::EnvelopeDisplay::kBackground.blackVariant);
        colours_[SkinColourId::kEnvelopeDisplayBorder] = juce::Colour(Widgets::EnvelopeDisplay::kBorder.blackVariant);
        colours_[SkinColourId::kEnvelopeDisplayEnvelope] = juce::Colour(Widgets::EnvelopeDisplay::kEnvelope.blackVariant);

        colours_[SkinColourId::kPatchNameDisplayBackground] = juce::Colour(Widgets::PatchNameDisplay::kBackground.blackVariant);
        colours_[SkinColourId::kPatchNameDisplayBorder] = juce::Colour(Widgets::PatchNameDisplay::kBorder.blackVariant);
        colours_[SkinColourId::kPatchNameDisplayText] = juce::Colour(Widgets::PatchNameDisplay::kText.blackVariant);

        colours_[SkinColourId::kTrackGeneratorDisplayBackground] = juce::Colour(Widgets::TrackGeneratorDisplay::kBackground.blackVariant);
        colours_[SkinColourId::kTrackGeneratorDisplayBorder] = juce::Colour(Widgets::TrackGeneratorDisplay::kBorder.blackVariant);
        colours_[SkinColourId::kTrackGeneratorDisplayShaper] = juce::Colour(Widgets::TrackGeneratorDisplay::kShaper.blackVariant);
    }

    void Skin::initializeCreamVariantColours()
    {
        using namespace ThemeColours;

        colours_[SkinColourId::kHeaderPanelBackground] = juce::Colour(Panels::kHeaderPanelBackground.creamVariant);
        colours_[SkinColourId::kBodyPanelBackground] = juce::Colour(Panels::kBodyPanelBackground.creamVariant);
        colours_[SkinColourId::kFooterPanelBackground] = juce::Colour(Panels::kFooterPanelBackground.creamVariant);

        colours_[SkinColourId::kSectionHeaderText] = juce::Colour(Widgets::SectionHeader::kText.creamVariant);
        colours_[SkinColourId::kSectionHeaderLineBlue] = juce::Colour(Widgets::SectionHeader::kLineBlue.creamVariant);
        colours_[SkinColourId::kSectionHeaderLineOrange] = juce::Colour(Widgets::SectionHeader::kLineOrange.creamVariant);

        colours_[SkinColourId::kModuleHeaderText] = juce::Colour(Widgets::ModuleHeader::kText.creamVariant);
        colours_[SkinColourId::kModuleHeaderLineBlue] = juce::Colour(Widgets::ModuleHeader::kLineBlue.creamVariant);
        colours_[SkinColourId::kModuleHeaderLineOrange] = juce::Colour(Widgets::ModuleHeader::kLineOrange.creamVariant);

        colours_[SkinColourId::kGroupLabelText] = juce::Colour(Widgets::GroupLabel::kText.creamVariant);
        colours_[SkinColourId::kGroupLabelLine] = juce::Colour(Widgets::GroupLabel::kLine.creamVariant);

        colours_[SkinColourId::kLabelText] = juce::Colour(Widgets::Label::kText.creamVariant);

        colours_[SkinColourId::kVerticalSeparatorLine] = juce::Colour(Widgets::VerticalSeparator::kLine.creamVariant);
        colours_[SkinColourId::kHorizontalSeparatorLine] = juce::Colour(Widgets::HorizontalSeparator::kLine.creamVariant);

        colours_[SkinColourId::kButtonBackgroundOff] = juce::Colour(Widgets::Button::kBackground.creamVariant);
        colours_[SkinColourId::kButtonBorderOff] = juce::Colour(Widgets::Button::kBorder.creamVariant);
        colours_[SkinColourId::kButtonTextOff] = juce::Colour(Widgets::Button::kText.creamVariant);
        colours_[SkinColourId::kButtonBackgroundOn] = juce::Colour(Widgets::Button::kBackground.creamVariant);
        colours_[SkinColourId::kButtonBorderOn] = juce::Colour(Widgets::Button::kBorder.creamVariant);
        colours_[SkinColourId::kButtonTextOn] = juce::Colour(Widgets::Button::kText.creamVariant);
        colours_[SkinColourId::kButtonBackgroundHover] = juce::Colour(Widgets::Button::kBackgroundHoover.creamVariant);
        colours_[SkinColourId::kButtonTextHover] = juce::Colour(Widgets::Button::kTextHoover.creamVariant);
        colours_[SkinColourId::kButtonBackgroundClicked] = juce::Colour(Widgets::Button::kBackgroundClicked.creamVariant);
        colours_[SkinColourId::kButtonTextClicked] = juce::Colour(Widgets::Button::kTextClicked.creamVariant);

        colours_[SkinColourId::kSliderTrackEnabled] = juce::Colour(Widgets::Slider::kTrack.creamVariant);
        colours_[SkinColourId::kSliderTrackDisabled] = juce::Colour(Widgets::Slider::kTrackDisabled.creamVariant);
        colours_[SkinColourId::kSliderValueBarEnabled] = juce::Colour(Widgets::Slider::kValueBar.creamVariant);
        colours_[SkinColourId::kSliderValueBarDisabled] = juce::Colour(Widgets::Slider::kValueBarDisabled.creamVariant);
        colours_[SkinColourId::kSliderTextEnabled] = juce::Colour(Widgets::Slider::kText.creamVariant);
        colours_[SkinColourId::kSliderTextDisabled] = juce::Colour(Widgets::Slider::kTextDisabled.creamVariant);
        colours_[SkinColourId::kSliderFocusBorder] = juce::Colour(Widgets::Slider::kFocusBorder.creamVariant);

        colours_[SkinColourId::kComboBoxBackgroundEnabled] = juce::Colour(Widgets::ComboBox::Standard::kBackground.creamVariant);
        colours_[SkinColourId::kComboBoxBackgroundDisabled] = juce::Colour(Widgets::ComboBox::Standard::kBackgroundDisabled.creamVariant);
        colours_[SkinColourId::kComboBoxBorderEnabled] = juce::Colour(Widgets::ComboBox::ButtonLike::kBorder.creamVariant);
        colours_[SkinColourId::kComboBoxBorderDisabled] = juce::Colour(Widgets::ComboBox::ButtonLike::kBorderDisabled.creamVariant);
        colours_[SkinColourId::kComboBoxFocusBorder] = juce::Colour(Widgets::ComboBox::Standard::kFocusBorder.creamVariant);
        colours_[SkinColourId::kComboBoxTriangleEnabled] = juce::Colour(Widgets::ComboBox::Standard::kTriangle.creamVariant);
        colours_[SkinColourId::kComboBoxTriangleDisabled] = juce::Colour(Widgets::ComboBox::Standard::kTriangleDisabled.creamVariant);
        colours_[SkinColourId::kComboBoxTextEnabled] = juce::Colour(Widgets::ComboBox::Standard::kText.creamVariant);
        colours_[SkinColourId::kComboBoxTextDisabled] = juce::Colour(Widgets::ComboBox::Standard::kTextDisabled.creamVariant);

        colours_[SkinColourId::kPopupMenuBackground] = juce::Colour(Widgets::PopupMenu::Standard::kBackground.creamVariant);
        colours_[SkinColourId::kPopupMenuBorder] = juce::Colour(Widgets::PopupMenu::Standard::kBorder.creamVariant);
        colours_[SkinColourId::kPopupMenuSeparator] = juce::Colour(Widgets::PopupMenu::Standard::kSeparator.creamVariant);
        colours_[SkinColourId::kPopupMenuText] = juce::Colour(Widgets::PopupMenu::Standard::kText.creamVariant);
        colours_[SkinColourId::kPopupMenuBackgroundHover] = juce::Colour(Widgets::PopupMenu::Standard::kBackgroundHoover.creamVariant);
        colours_[SkinColourId::kPopupMenuTextHover] = juce::Colour(Widgets::PopupMenu::Standard::kTextHoover.creamVariant);
        colours_[SkinColourId::kPopupMenuScrollbar] = juce::Colour(Widgets::PopupMenu::ButtonLike::kScrollbar.creamVariant);

        colours_[SkinColourId::kNumberBoxText] = juce::Colour(Widgets::NumberBox::kText.creamVariant);
        colours_[SkinColourId::kNumberBoxDot] = juce::Colour(Widgets::NumberBox::kDot.creamVariant);
        colours_[SkinColourId::kNumberBoxEditorBackground] = juce::Colour(Widgets::NumberBox::kEditorBackground.creamVariant);
        colours_[SkinColourId::kNumberBoxEditorSelectionBackground] = juce::Colour(Widgets::NumberBox::kEditorSelectionBackground.creamVariant);
        colours_[SkinColourId::kNumberBoxEditorText] = juce::Colour(Widgets::NumberBox::kEditorText.creamVariant);

        colours_[SkinColourId::kEnvelopeDisplayBackground] = juce::Colour(Widgets::EnvelopeDisplay::kBackground.creamVariant);
        colours_[SkinColourId::kEnvelopeDisplayBorder] = juce::Colour(Widgets::EnvelopeDisplay::kBorder.creamVariant);
        colours_[SkinColourId::kEnvelopeDisplayEnvelope] = juce::Colour(Widgets::EnvelopeDisplay::kEnvelope.creamVariant);

        colours_[SkinColourId::kPatchNameDisplayBackground] = juce::Colour(Widgets::PatchNameDisplay::kBackground.creamVariant);
        colours_[SkinColourId::kPatchNameDisplayBorder] = juce::Colour(Widgets::PatchNameDisplay::kBorder.creamVariant);
        colours_[SkinColourId::kPatchNameDisplayText] = juce::Colour(Widgets::PatchNameDisplay::kText.creamVariant);

        colours_[SkinColourId::kTrackGeneratorDisplayBackground] = juce::Colour(Widgets::TrackGeneratorDisplay::kBackground.creamVariant);
        colours_[SkinColourId::kTrackGeneratorDisplayBorder] = juce::Colour(Widgets::TrackGeneratorDisplay::kBorder.creamVariant);
        colours_[SkinColourId::kTrackGeneratorDisplayShaper] = juce::Colour(Widgets::TrackGeneratorDisplay::kShaper.creamVariant);
    }
}
