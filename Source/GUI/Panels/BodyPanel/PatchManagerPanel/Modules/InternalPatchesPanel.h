#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;
    class ModuleHeader;
    class GroupLabel;
    class Button;
}

class WidgetFactory;

class InternalPatchesPanel : public juce::Component
{
public:
    InternalPatchesPanel(tss::Theme& inTheme, WidgetFactory& widgetFactory);
    ~InternalPatchesPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& inTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kWidth = 275;
    inline constexpr static int kHeight = 115;
    inline constexpr static int kTopPadding = 10;
    inline constexpr static int kSpacing = 5;

    tss::Theme* theme;

    std::unique_ptr<tss::ModuleHeader> internalPatchesModuleHeader;

    std::unique_ptr<tss::GroupLabel> browserGroupLabel;
    std::unique_ptr<tss::GroupLabel> utilityGroupLabel;

    std::unique_ptr<tss::Button> loadPreviousInternalPatchButton;
    std::unique_ptr<tss::Button> loadNextInternalPatchButton;

    std::unique_ptr<tss::Button> initInternalPatchButton;
    std::unique_ptr<tss::Button> copyInternalPatchButton;
    std::unique_ptr<tss::Button> pasteInternalPatchButton;
    std::unique_ptr<tss::Button> storeInternalPatchButton;

    void setupModuleHeader(tss::Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InternalPatchesPanel)
};
