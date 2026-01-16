#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "Core/PluginProcessor.h"
#include "Themes/Theme.h"
#include "Panels/MainComponent/HeaderPanel/HeaderPanel.h"
#include "Panels/MainComponent/BodyPanel/BodyPanel.h"
#include "Panels/MainComponent/FooterPanel/FooterPanel.h"
#include "MainComponent.h"

class WidgetFactory;

class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& e) override;

    static int getWidth() { return kWidth_; }
    static int getHeight() { return kHeight_; }

private:
    inline constexpr static int kWidth_ = 1335;
    inline constexpr static int kHeight_ = 790;

    PluginProcessor& pluginProcessor;
    
    std::unique_ptr<tss::Theme> theme;
    std::unique_ptr<WidgetFactory> widgetFactory;
    std::unique_ptr<MainComponent> mainComponent;
    
    void updateTheme();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};