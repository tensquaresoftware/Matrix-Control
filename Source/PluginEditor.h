#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>

#include "PluginProcessor.h"

class PluginEditor : public juce::AudioProcessorEditor,
                     public juce::ComboBox::Listener
{
public:
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

private:
    void populateMidiPorts();
    void updateMidiPorts();
    void setupMidiInputComponents();
    void setupMidiOutputComponents();
    void handleMidiInputSelection(int selectedId);
    void handleMidiOutputSelection(int selectedId);
    void restoreMidiInputPort();
    void restoreMidiOutputPort();
    void populateMidiInputPorts();
    void populateMidiOutputPorts();

    PluginProcessor& pluginProcessor;
    
    juce::Label midiInputLabel;
    juce::ComboBox midiInputComboBox;
    
    juce::Label midiOutputLabel;
    juce::ComboBox midiOutputComboBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};