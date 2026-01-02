#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "PluginProcessor.h"
#include "GUI/Skin/Skin.h"
#include "GUI/Skin/SkinDimensions.h"
#include "GUI/MainComponent.h"

class WidgetFactory;

class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& e) override;

private:

    PluginProcessor& pluginProcessor;
    
    std::unique_ptr<tss::Skin> skin;
    std::unique_ptr<WidgetFactory> widgetFactory;
    std::unique_ptr<MainComponent> mainComponent;
    
    void updateSkin();
    void validateSynthDescriptorsAtStartup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};