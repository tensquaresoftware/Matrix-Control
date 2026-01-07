#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace tss
{
    class Theme;
}

class WidgetFactory;
class Dco1Panel;
class Dco2Panel;
class VcfVcaPanel;
class FmTrackPanel;
class RampPortamentoPanel;

class TopPanel : public juce::Component
{
public:
    TopPanel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~TopPanel() override;

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

    std::unique_ptr<Dco1Panel> dco1Panel;
    std::unique_ptr<Dco2Panel> dco2Panel;
    std::unique_ptr<VcfVcaPanel> vcfVcaPanel;
    std::unique_ptr<FmTrackPanel> fmTrackPanel;
    std::unique_ptr<RampPortamentoPanel> rampPortamentoPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopPanel)
};

