#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "Shared/PluginDimensions.h"

namespace tss
{
    class Theme;
}

class MiddlePanel : public juce::Component
{
public:
    MiddlePanel(tss::Theme& theme);
    ~MiddlePanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& theme);

    static int getWidth() { return PluginDimensions::Panels::PatchEditPanel::MiddlePanel::kWidth; }
    static int getHeight() { return PluginDimensions::Panels::PatchEditPanel::MiddlePanel::kHeight; }

private:

    tss::Theme* theme_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiddlePanel)
};

