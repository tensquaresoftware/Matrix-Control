#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "PluginProcessor.h"
#include "UI/LookAndFeel/McLookAndFeel.h"
#include "UI/Widgets/McSlider.h"
#include "UI/Widgets/McButton.h"
#include "UI/Widgets/McComboBox.h"
#include "UI/Widgets/McParameterLabel.h"

class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    PluginProcessor& pluginProcessor;
    
    std::unique_ptr<McLookAndFeel> lookAndFeel;
    
    juce::Label pluginVersionLabel;
    McSlider testSlider1;
    McSlider testSlider2;
    McSlider testSlider3;
    McSlider testSlider4;
    
    McButton buttonI;
    McButton buttonC;
    McButton buttonP;
    McButton buttonUnlock;
    McButton buttonInit;
    McButton buttonStore;
    
    McComboBox comboBox1;
    McComboBox comboBox2;
    
    McParameterLabel frequencyLabel;
    McSlider frequencySlider;
    
    McParameterLabel syncLabel;
    McComboBox syncComboBox;
    
    McParameterLabel themeLabel;
    McComboBox themeComboBox;

    void updateTheme();
    void updateAllWidgetsLookAndFeel();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};