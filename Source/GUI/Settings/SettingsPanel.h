#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "GUI/Widgets/ComboBox.h"
#include "GUI/Widgets/HorizontalSeparator.h"
#include "GUI/Widgets/Label.h"
#include "GUI/Widgets/Slider.h"

namespace TSS
{
    class ISkin;
}

class SettingsPanel : public juce::Component
{
public:
    static constexpr int kDesignWidth = 280;
    static constexpr int kDesignHeight = 390;

    SettingsPanel(TSS::ISkin& skin, bool isPluginMode);
    ~SettingsPanel() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setSkin(TSS::ISkin& skin);
    void setUiScale(float uiScale);
    void setPluginMode(bool isPluginMode);

    TSS::Slider& getHardwareLatencySlider() { return hardwareLatencySlider_; }
    TSS::ComboBox& getNameReconciliationPolicyCombo() { return nameReconciliationPolicyCombo_; }
    TSS::ComboBox& getUnsavedEditWarningPolicyCombo() { return unsavedEditWarningPolicyCombo_; }
    TSS::ComboBox& getMutatorDeleteWarningPolicyCombo() { return mutatorDeleteWarningPolicyCombo_; }
    TSS::ComboBox& getPatchNameDisplayModeCombo() { return patchNameDisplayModeCombo_; }

private:
    void updateModeSpecificVisibility();
    void layoutContent(juce::Rectangle<int> bounds);
    void layoutSectionHeader(juce::Rectangle<int>& bounds,
                             TSS::Label& title,
                             TSS::HorizontalSeparator& separator,
                             int controlHeight,
                             int separatorHeight,
                             int rowGap);

    inline constexpr static int kPadding_ = 16;
    inline constexpr static int kGap_ = 8;
    inline constexpr static int kRowGap_ = 12;
    inline constexpr static int kSectionTitleGap_ = 4;
    inline constexpr static int kControlHeight_ = 20;
    inline constexpr static int kSeparatorHeight_ = 8;
    inline constexpr static int kLabelWidth_ = 128;
    inline constexpr static int kSliderWidth_ = 72;
    inline constexpr static int kContentWidth_ = kDesignWidth - kPadding_ * 2;

    TSS::ISkin* skin_;
    float uiScale_ = 1.0f;
    bool isPluginMode_ = false;

    TSS::Label generalSectionLabel_;
    TSS::HorizontalSeparator generalSectionSeparator_;
    TSS::Label hardwareLatencyLabel_;
    TSS::Slider hardwareLatencySlider_;
    TSS::Label masterOpsLabel_;
    TSS::Label masterOpsPlaceholder_;
    TSS::Label policiesLabel_;
    TSS::ComboBox nameReconciliationPolicyCombo_;
    TSS::Label unsavedEditWarningLabel_;
    TSS::ComboBox unsavedEditWarningPolicyCombo_;
    TSS::Label mutatorDeleteWarningLabel_;
    TSS::ComboBox mutatorDeleteWarningPolicyCombo_;
    TSS::Label defragLabel_;
    TSS::Label defragPlaceholder_;
    TSS::Label loggingLabel_;
    TSS::Label loggingPlaceholder_;

    TSS::Label matrix1000SectionLabel_;
    TSS::HorizontalSeparator matrix1000SectionSeparator_;
    TSS::Label patchNameDisplayLabel_;
    TSS::ComboBox patchNameDisplayModeCombo_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsPanel)
};
