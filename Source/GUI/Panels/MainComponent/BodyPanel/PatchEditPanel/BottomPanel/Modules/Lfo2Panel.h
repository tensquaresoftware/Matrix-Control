#pragma once

#include <memory>
#include <vector>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "Shared/PluginDimensions.h"

namespace tss
{
    class Theme;
}

class WidgetFactory;
class ModuleHeaderPanel;
class ParameterPanel;

class Lfo2Panel : public juce::Component
{
public:
    Lfo2Panel(tss::Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~Lfo2Panel() override;

    void resized() override;
    void setTheme(tss::Theme& theme);

    static int getWidth() { return PluginDimensions::Panels::PatchEditPanel::BottomPanel::ChildPanels::kWidth; }
    static int getHeight() { return PluginDimensions::Panels::PatchEditPanel::BottomPanel::ChildPanels::kHeight; }

private:
    
    tss::Theme* theme_;
    juce::AudioProcessorValueTreeState& apvts_;

    std::unique_ptr<ModuleHeaderPanel> moduleHeaderPanel_;
    std::vector<std::unique_ptr<ParameterPanel>> parameterPanels_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Lfo2Panel)
};
