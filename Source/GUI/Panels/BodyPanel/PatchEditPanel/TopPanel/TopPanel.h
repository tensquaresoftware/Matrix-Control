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
    TopPanel(tss::Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~TopPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& theme);

    static int getWidth() { return kWidth_; }
    static int getHeight() { return kHeight_; }
    static int getSpacing() { return kSpacing_; }

private:
    inline constexpr static int kWidth_ = 810;
    inline constexpr static int kHeight_ = 280;
    inline constexpr static int kSpacing_ = 15;

    tss::Theme* theme_;

    std::unique_ptr<Dco1Panel> dco1Panel_;
    std::unique_ptr<Dco2Panel> dco2Panel_;
    std::unique_ptr<VcfVcaPanel> vcfVcaPanel_;
    std::unique_ptr<FmTrackPanel> fmTrackPanel_;
    std::unique_ptr<RampPortamentoPanel> rampPortamentoPanel_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopPanel)
};

