#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;
    class ModuleHeader;
}

class WidgetFactory;

class ComputerPatchesPanel : public juce::Component
{
public:
    ComputerPatchesPanel(tss::Theme& inTheme, WidgetFactory& widgetFactory);
    ~ComputerPatchesPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& inTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kWidth = 275;
    inline constexpr static int kHeight = 115;
    inline constexpr static int kTopPadding = 20;

    tss::Theme* theme;

    std::unique_ptr<tss::ModuleHeader> computerPatchesModuleHeader;

    void setupModuleHeader(tss::Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComputerPatchesPanel)
};
