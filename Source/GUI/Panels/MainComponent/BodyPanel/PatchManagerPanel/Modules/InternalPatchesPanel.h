#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "Shared/PluginDimensions.h"

namespace tss
{
    class Theme;
    class ModuleHeader;
    class GroupLabel;
    class Button;
    class NumberBox;
}

class WidgetFactory;

class InternalPatchesPanel : public juce::Component
{
public:
    InternalPatchesPanel(tss::Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~InternalPatchesPanel() override;

    void resized() override;
    void setTheme(tss::Theme& theme);

    static int getWidth() { return PluginDimensions::Panels::PatchManagerPanel::InternalPatchesPanel::kWidth; }
    static int getHeight() { return PluginDimensions::Panels::PatchManagerPanel::InternalPatchesPanel::kHeight; }

private:
    inline constexpr static int kTopPadding_ = 5;
    inline constexpr static int kGroupLabelSpacing_ = 10;
    inline constexpr static int kSpacing_ = 5;

    tss::Theme* theme_;
    juce::AudioProcessorValueTreeState& apvts_;

    std::unique_ptr<tss::ModuleHeader> moduleHeader;

    std::unique_ptr<tss::GroupLabel> browserGroupLabel;
    std::unique_ptr<tss::GroupLabel> utilityGroupLabel;

    std::unique_ptr<tss::Button> loadPreviousPatchButton_;
    std::unique_ptr<tss::Button> loadNextPatchButton_;

    std::unique_ptr<tss::NumberBox> currentBankNumber;
    std::unique_ptr<tss::NumberBox> currentPatchNumber;

    std::unique_ptr<tss::Button> initPatchButton_;
    std::unique_ptr<tss::Button> copyPatchButton_;
    std::unique_ptr<tss::Button> pastePatchButton_;
    std::unique_ptr<tss::Button> storePatchButton_;

    void setupModuleHeader(tss::Theme& theme, WidgetFactory& widgetFactory, const juce::String& moduleId);
    void setupBrowserGroupLabel(tss::Theme& theme);
    void setupLoadPreviousPatchButton(tss::Theme& theme, WidgetFactory& widgetFactory);
    void setupLoadNextPatchButton(tss::Theme& theme, WidgetFactory& widgetFactory);
    void setupCurrentBankNumberBox(tss::Theme& theme);
    void setupCurrentPatchNumberBox(tss::Theme& theme);
    void setupUtilityGroupLabel(tss::Theme& theme);
    void setupInitPatchButton(tss::Theme& theme, WidgetFactory& widgetFactory);
    void setupCopyPatchButton(tss::Theme& theme, WidgetFactory& widgetFactory);
    void setupPastePatchButton(tss::Theme& theme, WidgetFactory& widgetFactory);
    void setupStorePatchButton(tss::Theme& theme, WidgetFactory& widgetFactory);

    void layoutModuleHeader(int x, int y);
    void layoutBrowserGroupLabel(int x, int y);
    void layoutLoadPreviousPatchButton(int x, int y);
    void layoutLoadNextPatchButton(int x, int y);
    void layoutCurrentBankNumberBox(int x, int y);
    void layoutCurrentPatchNumberBox(int x, int y);
    void layoutUtilityGroupLabel(int x, int y);
    void layoutInitPatchButton(int x, int y);
    void layoutCopyPatchButton(int x, int y);
    void layoutPastePatchButton(int x, int y);
    void layoutStorePatchButton(int x, int y);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InternalPatchesPanel)
};
