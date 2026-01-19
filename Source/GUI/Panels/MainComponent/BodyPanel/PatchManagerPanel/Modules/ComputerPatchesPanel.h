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
    class ComboBox;
}

class WidgetFactory;

class ComputerPatchesPanel : public juce::Component
{
public:
    ComputerPatchesPanel(tss::Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~ComputerPatchesPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& theme);

    static int getWidth() { return PluginDimensions::Panels::PatchManagerPanel::ComputerPatchesPanel::kWidth; }
    static int getHeight() { return PluginDimensions::Panels::PatchManagerPanel::ComputerPatchesPanel::kHeight; }

private:
    inline constexpr static int kTopPadding_ = 5;
    inline constexpr static int kGroupLabelSpacing_ = 10;
    inline constexpr static int kSpacing_ = 5;
    inline constexpr static int kSelectPatchFileEmptyId_ = 1;

    tss::Theme* theme_;
    juce::AudioProcessorValueTreeState& apvts_;

    std::unique_ptr<tss::ModuleHeader> moduleHeader_;

    std::unique_ptr<tss::GroupLabel> browserGroupLabel;
    std::unique_ptr<tss::GroupLabel> utilityGroupLabel;

    std::unique_ptr<tss::Button> loadPreviousPatchFileButton_;
    std::unique_ptr<tss::Button> loadNextPatchFileButton_;
    std::unique_ptr<tss::ComboBox> selectPatchFileComboBox_;

    std::unique_ptr<tss::Button> openPatchFolderButton_;
    std::unique_ptr<tss::Button> savePatchFileAsButton_;
    std::unique_ptr<tss::Button> savePatchFileButton_;

    void setupModuleHeader(tss::Theme& theme, WidgetFactory& widgetFactory, const juce::String& moduleId);
    void setupBrowserGroupLabel(tss::Theme& theme);
    void setupLoadPreviousPatchFileButton(tss::Theme& theme, WidgetFactory& widgetFactory);
    void setupLoadNextPatchFileButton(tss::Theme& theme, WidgetFactory& widgetFactory);
    void setupSelectPatchFileComboBox(tss::Theme& theme);
    void setupUtilityGroupLabel(tss::Theme& theme);
    void setupOpenPatchFolderButton(tss::Theme& theme, WidgetFactory& widgetFactory);
    void setupSavePatchFileAsButton(tss::Theme& theme, WidgetFactory& widgetFactory);
    void setupSavePatchFileButton(tss::Theme& theme, WidgetFactory& widgetFactory);

    void layoutModuleHeader(int x, int y);
    void layoutBrowserGroupLabel(int x, int y);
    void layoutLoadPreviousPatchFileButton(int x, int y);
    void layoutLoadNextPatchFileButton(int x, int y);
    void layoutSelectPatchFileComboBox(int x, int y);
    void layoutUtilityGroupLabel(int x, int y);
    void layoutOpenPatchFolderButton(int x, int y);
    void layoutSavePatchFileAsButton(int x, int y);
    void layoutSavePatchFileButton(int x, int y);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComputerPatchesPanel)
};
