#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "Shared/PluginDimensions.h"

namespace tss
{
    class Theme;
    class SectionHeader;
}

class WidgetFactory;
class MidiPanel;
class VibratoPanel;
class MiscPanel;

class MasterEditPanel : public juce::Component
{
public:
    MasterEditPanel(tss::Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~MasterEditPanel() override;

    void resized() override;
    void setTheme(tss::Theme& theme);

    static int getWidth() { return PluginDimensions::Panels::MasterEditPanel::kWidth; }
    static int getHeight() { return PluginDimensions::Panels::MasterEditPanel::kHeight; }

private:
    tss::Theme* theme_;

    std::unique_ptr<tss::SectionHeader> sectionHeader_;
    std::unique_ptr<MidiPanel> midiPanel_;
    std::unique_ptr<VibratoPanel> vibratoPanel_;
    std::unique_ptr<MiscPanel> miscPanel_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MasterEditPanel)
};

