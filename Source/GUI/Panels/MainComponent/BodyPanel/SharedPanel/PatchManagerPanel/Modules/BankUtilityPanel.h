#pragma once

#include <array>
#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "GUI/Layout/PanelDimensions.h"
#include "GUI/Looks/WidgetLooks.h"

namespace TSS
{
    class ISkin;
    class ModuleHeader;
    class Label;
    class Button;
}

class WidgetFactory;

class BankUtilityPanel : public juce::Component,
                         public juce::ValueTree::Listener
{
public:
    BankUtilityPanel(TSS::ISkin& skin, const BankUtilityPanelDimensions& dims, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~BankUtilityPanel() override;

    void resized() override;
    void setSkin(TSS::ISkin& skin);
    void setUiScale(float uiScale);

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged,
                                 const juce::Identifier& property) override;
    void valueTreeChildAdded(juce::ValueTree&, juce::ValueTree&) override {}
    void valueTreeChildRemoved(juce::ValueTree&, juce::ValueTree&, int) override {}
    void valueTreeChildOrderChanged(juce::ValueTree&, int, int) override {}
    void valueTreeParentChanged(juce::ValueTree&) override {}
    void valueTreeRedirected(juce::ValueTree&) override;

    static constexpr int kBankCount = 10;

private:
    BankUtilityPanelDimensions dims_;
    TSS::ISkin* skin_;
    float uiScale_ = 1.0f;
    juce::AudioProcessorValueTreeState& apvts_;
    bool bankUtilityGrayed_ = false;
    TSS::ButtonLook normalBankLook_;

    std::unique_ptr<TSS::ModuleHeader> bankUtilityModuleHeader_;
    std::unique_ptr<TSS::Label> bankSelectorLabel_;
    std::unique_ptr<TSS::Button> unlockBankButton_;
    std::array<std::unique_ptr<TSS::Button>, kBankCount> selectBankButtons_;
    std::unique_ptr<TSS::Button> importBankButton_;
    std::unique_ptr<TSS::Button> exportBankButton_;

    void setupModuleHeader(TSS::ISkin& skin, WidgetFactory& widgetFactory, const juce::String& moduleId);
    void setupBankSelectorLabel(TSS::ISkin& skin);
    void setupSelectBankButtons(TSS::ISkin& skin, WidgetFactory& widgetFactory);
    void setupImportExportButtons(TSS::ISkin& skin, WidgetFactory& widgetFactory);
    void setupUnlockBankButton(TSS::ISkin& skin, WidgetFactory& widgetFactory);

    void layoutContentRows(float sf);
    void applyChildUiScales(float sf);
    void applyNormalLookToActionButtons();

    void refreshDeviceGating();
    void refreshSelectedBankHighlight();
    void refreshImportExportEnabled();
    void setBankUtilityGrayed(bool grayed);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BankUtilityPanel)
};
