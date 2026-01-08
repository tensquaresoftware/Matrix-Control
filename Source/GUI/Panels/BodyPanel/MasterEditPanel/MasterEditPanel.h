#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

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
    MasterEditPanel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~MasterEditPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& inTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kWidth = 160;
    inline constexpr static int kHeight = 700; // SectionHeader (30) + MidiPanel (245) + VibratoPanel (220) + MiscPanel (205)
    tss::Theme* theme;

    std::unique_ptr<tss::SectionHeader> sectionHeader;
    std::unique_ptr<MidiPanel> midiPanel;
    std::unique_ptr<VibratoPanel> vibratoPanel;
    std::unique_ptr<MiscPanel> miscPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MasterEditPanel)
};

