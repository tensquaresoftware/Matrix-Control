#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace tss
{
    class Theme;
}

class WidgetFactory;
class Env1Panel;
class Env2Panel;
class Env3Panel;
class Lfo1Panel;
class Lfo2Panel;

class BottomPanel : public juce::Component
{
public:
    BottomPanel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~BottomPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& inTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }
    static int getSpacing() { return kSpacing; }

private:
    inline constexpr static int kWidth = 810;
    inline constexpr static int kHeight = 280;
    inline constexpr static int kSpacing = 15;

    tss::Theme* theme;

    std::unique_ptr<Env1Panel> env1Panel;
    std::unique_ptr<Env2Panel> env2Panel;
    std::unique_ptr<Env3Panel> env3Panel;
    std::unique_ptr<Lfo1Panel> lfo1Panel;
    std::unique_ptr<Lfo2Panel> lfo2Panel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BottomPanel)
};

