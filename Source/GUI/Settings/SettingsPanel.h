#pragma once

#include <memory>

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
    static constexpr int kDesignWidth = 300;
    static constexpr int kDesignHeight = 390;

    SettingsPanel(TSS::ISkin& skin, bool isPluginMode);
    ~SettingsPanel() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setSkin(TSS::ISkin& skin);
    void setUiScale(float uiScale);
    void setPluginMode(bool isPluginMode);

    TSS::Slider& getHardwareLatencySlider() { return *hardwareLatencySlider_; }
    TSS::ComboBox& getNameReconciliationPolicyCombo() { return *nameReconciliationPolicyCombo_; }
    TSS::ComboBox& getUnsavedEditWarningPolicyCombo() { return *unsavedEditWarningPolicyCombo_; }
    TSS::ComboBox& getMutatorDeleteWarningPolicyCombo() { return *mutatorDeleteWarningPolicyCombo_; }
    TSS::ComboBox& getPatchNameDisplayModeCombo() { return *patchNameDisplayModeCombo_; }

private:
    struct SectionHeaderLayoutArgs
    {
        TSS::Label* title = nullptr;
        TSS::HorizontalSeparator* separator = nullptr;
        int controlHeight = 0;
        int separatorHeight = 0;
        int rowGap = 0;
    };

    struct RowLayoutMetrics
    {
        int rowGap = 0;
        int gap = 0;
        int labelWidth = 0;
        int sliderWidth = 0;
        int controlHeight = 0;
        int separatorHeight = 0;
        int comboWidth = 0;
    };

    struct LabeledControlRowArgs
    {
        TSS::Label* label = nullptr;
        juce::Component* control = nullptr;
        int controlWidth = 0;
    };

    void setupGeneralSection(TSS::ISkin& skin);
    void setupPolicyControls(TSS::ISkin& skin);
    void setupPlaceholderRows(TSS::ISkin& skin);
    void setupMatrix1000Section(TSS::ISkin& skin);
    void populatePolicyComboItems();
    void applyComboPopupLooks(TSS::ISkin& skin);
    void applyChildLooks(TSS::ISkin& skin);

    std::unique_ptr<TSS::Label> makeLabel(TSS::ISkin& skin, int width, const juce::String& text);
    std::unique_ptr<TSS::ComboBox> makeCombo(TSS::ISkin& skin, int width);
    std::unique_ptr<TSS::HorizontalSeparator> makeSeparator(TSS::ISkin& skin);

    void updateModeSpecificVisibility();
    void layoutContent(juce::Rectangle<int> bounds);
    void layoutGeneralSection(juce::Rectangle<int>& bounds, const RowLayoutMetrics& metrics);
    void layoutMatrix1000Section(juce::Rectangle<int>& bounds, const RowLayoutMetrics& metrics);
    void layoutSectionHeader(juce::Rectangle<int>& bounds, const SectionHeaderLayoutArgs& args);
    void layoutLabeledControlRow(juce::Rectangle<int>& bounds,
                                 const RowLayoutMetrics& metrics,
                                 const LabeledControlRowArgs& args);
    void layoutPlaceholderRow(juce::Rectangle<int>& bounds,
                              const RowLayoutMetrics& metrics,
                              TSS::Label& label,
                              TSS::Label& placeholder);

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

    std::unique_ptr<TSS::Label> generalSectionLabel_;
    std::unique_ptr<TSS::HorizontalSeparator> generalSectionSeparator_;
    std::unique_ptr<TSS::Label> hardwareLatencyLabel_;
    std::unique_ptr<TSS::Slider> hardwareLatencySlider_;
    std::unique_ptr<TSS::Label> masterOpsLabel_;
    std::unique_ptr<TSS::Label> masterOpsPlaceholder_;
    std::unique_ptr<TSS::Label> policiesLabel_;
    std::unique_ptr<TSS::ComboBox> nameReconciliationPolicyCombo_;
    std::unique_ptr<TSS::Label> unsavedEditWarningLabel_;
    std::unique_ptr<TSS::ComboBox> unsavedEditWarningPolicyCombo_;
    std::unique_ptr<TSS::Label> mutatorDeleteWarningLabel_;
    std::unique_ptr<TSS::ComboBox> mutatorDeleteWarningPolicyCombo_;
    std::unique_ptr<TSS::Label> defragLabel_;
    std::unique_ptr<TSS::Label> defragPlaceholder_;
    std::unique_ptr<TSS::Label> loggingLabel_;
    std::unique_ptr<TSS::Label> loggingPlaceholder_;

    std::unique_ptr<TSS::Label> matrix1000SectionLabel_;
    std::unique_ptr<TSS::HorizontalSeparator> matrix1000SectionSeparator_;
    std::unique_ptr<TSS::Label> patchNameDisplayLabel_;
    std::unique_ptr<TSS::ComboBox> patchNameDisplayModeCombo_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsPanel)
};
