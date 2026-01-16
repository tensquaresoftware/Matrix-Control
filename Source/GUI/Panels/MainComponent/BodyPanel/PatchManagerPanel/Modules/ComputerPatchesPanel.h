#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

#include "Shared/PluginDimensions.h"

namespace tss
{
    class Theme;
    class ModuleHeader;
}

class WidgetFactory;

class ComputerPatchesPanel : public juce::Component
{
public:
    ComputerPatchesPanel(tss::Theme& theme, WidgetFactory& widgetFactory);
    ~ComputerPatchesPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& theme);

    static int getWidth() { return PluginDimensions::Panels::PatchManagerPanel::ComputerPatchesPanel::kWidth; }
    static int getHeight() { return PluginDimensions::Panels::PatchManagerPanel::ComputerPatchesPanel::kHeight; }

private:
    inline constexpr static int kTopPadding_ = 20;

    tss::Theme* theme_;

    std::unique_ptr<tss::ModuleHeader> moduleHeader_;

    void setupModuleHeader(tss::Theme& theme, WidgetFactory& widgetFactory, const juce::String& moduleId);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComputerPatchesPanel)
};
