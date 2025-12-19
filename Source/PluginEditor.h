#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "PluginProcessor.h"
#include "UI/Themes/McTheme.h"
#include "UI/Widgets/McButton.h"
#include "UI/Widgets/McComboBox.h"
#include "UI/Widgets/McParameterLabel.h"
#include "UI/Widgets/McSlider.h"

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
    
    std::unique_ptr<McTheme> theme;
    
    McButton buttonBlack;
    McButton buttonCream;
    McButton buttonDebug;
    McButton buttonDisabled;
    
    McParameterLabel labelPortamentoRate;
    McSlider sliderPortamentoRate;
    
    McParameterLabel labelPortaVelocity;
    McSlider sliderPortaVelocity;
    
    McParameterLabel labelFrequency;
    McSlider sliderFrequency;
    
    McParameterLabel labelTrackInput;
    McComboBox comboBoxTrackInput;
    
    McParameterLabel labelLegatoPorta;
    McComboBox comboBoxLegatoPorta;
    
    McComboBox comboBoxTopLeft;
    McComboBox comboBoxTopRight;
    McComboBox comboBoxBottomLeft;
    McComboBox comboBoxBottomRight;
    
    void updateTheme();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};