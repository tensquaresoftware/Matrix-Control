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

class VibratoPanel : public BaseModulePanel
{
public:
    VibratoPanel(tss::Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~VibratoPanel() override = default;

    static int getWidth() { return PluginDimensions::Panels::MasterEditPanel::ChildPanels::kWidth; }
    static int getHeight() { return PluginDimensions::Panels::MasterEditPanel::VibratoPanel::kHeight; }

private:
    static ModulePanelConfig createConfig();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VibratoPanel)
};
