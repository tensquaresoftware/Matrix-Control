#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace tss
{
    class Theme;
    class SectionName;
}

class WidgetFactory;
class TopPanel;
class MiddlePanel;
class BottomPanel;

class PatchEditPanel : public juce::Component
{
public:
    PatchEditPanel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~PatchEditPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& inTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kWidth = 810;
    inline constexpr static int kHeight = 700;
    tss::Theme* theme;

    std::unique_ptr<tss::SectionName> sectionName;
    std::unique_ptr<TopPanel> topPanel;
    std::unique_ptr<MiddlePanel> middlePanel;
    std::unique_ptr<BottomPanel> bottomPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchEditPanel)
};

