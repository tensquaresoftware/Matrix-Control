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

class Dco2Panel : public BaseModulePanel
{
public:
    Dco2Panel(tss::Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~Dco2Panel() override = default;

    static int getWidth() { return PluginDimensions::Panels::PatchEditPanel::TopPanel::ChildPanels::kWidth; }
    static int getHeight() { return PluginDimensions::Panels::PatchEditPanel::TopPanel::ChildPanels::kHeight; }

private:
    static ModulePanelConfig createConfig();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dco2Panel)
};
