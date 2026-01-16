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
class TopPanel;
class MiddlePanel;
class BottomPanel;

class PatchEditPanel : public juce::Component
{
public:
    PatchEditPanel(tss::Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~PatchEditPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& theme);

    static int getWidth() { return PluginDimensions::Panels::PatchEditPanel::kWidth; }
    static int getHeight() { return PluginDimensions::Panels::PatchEditPanel::kHeight; }

private:
    tss::Theme* theme_;

    std::unique_ptr<tss::SectionHeader> sectionHeader_;
    std::unique_ptr<TopPanel> topPanel_;
    std::unique_ptr<MiddlePanel> middlePanel_;
    std::unique_ptr<BottomPanel> bottomPanel_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchEditPanel)
};

