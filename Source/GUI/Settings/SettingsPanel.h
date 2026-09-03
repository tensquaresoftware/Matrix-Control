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
    // Wide enough for MATRIX-1000 PATCHES + DISPLAY MUSICAL NAMES without clipping.
    static constexpr int kDesignWidth = 400;
    static constexpr int kDesignHeight = 420;

    SettingsPanel(TSS::ISkin& skin, bool isPluginMode);
    ~SettingsPanel() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setSkin(TSS::ISkin& skin);
    void setUiScale(float uiScale);
    void setPluginMode(bool isPluginMode);

    TSS::Slider& getHardwareLatencySlider() { return *hardwareLatencySlider_; }
    TSS::ComboBox& getMatrix1000PatchesCombo() { return *matrix1000PatchesCombo_; }
    TSS::ComboBox& getComputerPatchesCombo() { return *computerPatchesCombo_; }
    TSS::ComboBox& getUnsavedStateCombo() { return *unsavedStateCombo_; }
    TSS::ComboBox& getDeleteWarningCombo() { return *deleteWarningCombo_; }

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

    void setupPatchSection(TSS::ISkin& skin);
    void setupPatchMutatorSection(TSS::ISkin& skin);
    void setupMasterSection(TSS::ISkin& skin);
    void populateComboItems();
    void applyComboPopupLooks(TSS::ISkin& skin);
    void applyChildLooks(TSS::ISkin& skin);

    std::unique_ptr<TSS::Label> makeLabel(TSS::ISkin& skin, int width, const juce::String& text);
    std::unique_ptr<TSS::ComboBox> makeCombo(TSS::ISkin& skin, int width);
    std::unique_ptr<TSS::HorizontalSeparator> makeSeparator(TSS::ISkin& skin);

    void updateModeSpecificVisibility();
    void layoutContent(juce::Rectangle<int> bounds);
    void layoutPatchSection(juce::Rectangle<int>& bounds, const RowLayoutMetrics& metrics);
    void layoutPatchMutatorSection(juce::Rectangle<int>& bounds, const RowLayoutMetrics& metrics);
    void layoutMasterSection(juce::Rectangle<int>& bounds, const RowLayoutMetrics& metrics);
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
    inline constexpr static int kLabelWidth_ = 160;
    inline constexpr static int kSliderWidth_ = 72;
    inline constexpr static int kContentWidth_ = kDesignWidth - kPadding_ * 2;

    TSS::ISkin* skin_;
    float uiScale_ = 1.0f;
    bool isPluginMode_ = false;

    std::unique_ptr<TSS::Label> patchSectionLabel_;
    std::unique_ptr<TSS::HorizontalSeparator> patchSectionSeparator_;
    std::unique_ptr<TSS::Label> matrix1000PatchesLabel_;
    std::unique_ptr<TSS::ComboBox> matrix1000PatchesCombo_;
    std::unique_ptr<TSS::Label> computerPatchesLabel_;
    std::unique_ptr<TSS::ComboBox> computerPatchesCombo_;
    std::unique_ptr<TSS::Label> unsavedStateLabel_;
    std::unique_ptr<TSS::ComboBox> unsavedStateCombo_;

    std::unique_ptr<TSS::Label> patchMutatorSectionLabel_;
    std::unique_ptr<TSS::HorizontalSeparator> patchMutatorSectionSeparator_;
    std::unique_ptr<TSS::Label> deleteWarningLabel_;
    std::unique_ptr<TSS::ComboBox> deleteWarningCombo_;
    std::unique_ptr<TSS::Label> defragHistoryLabel_;
    std::unique_ptr<TSS::Label> defragHistoryPlaceholder_;

    std::unique_ptr<TSS::Label> masterSectionLabel_;
    std::unique_ptr<TSS::HorizontalSeparator> masterSectionSeparator_;
    std::unique_ptr<TSS::Label> hardwareLatencyLabel_;
    std::unique_ptr<TSS::Slider> hardwareLatencySlider_;
    std::unique_ptr<TSS::Label> masterOperationsLabel_;
    std::unique_ptr<TSS::Label> masterOperationsPlaceholder_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsPanel)
};
