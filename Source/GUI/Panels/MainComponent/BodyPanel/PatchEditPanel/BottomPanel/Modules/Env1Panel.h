#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "Shared/PluginDimensions.h"
#include "GUI/Panels/Reusable/BaseModulePanel.h"

namespace tss
{
    class Theme;
}

class WidgetFactory;

class Env1Panel : public BaseModulePanel
{
public:
    Env1Panel(tss::Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~Env1Panel() override = default;

    static int getWidth() { return PluginDimensions::Panels::PatchEditPanel::BottomPanel::ChildPanels::kWidth; }
    static int getHeight() { return PluginDimensions::Panels::PatchEditPanel::BottomPanel::ChildPanels::kHeight; }

private:
    static ModulePanelConfig createConfig();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Env1Panel)
};
