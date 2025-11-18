#include "PluginProcessor.h"
#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    setSize(400, 300);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawFittedText("Ten Square Software | Matrix-Control", getLocalBounds(), juce::Justification::centred, 1);
}

void PluginEditor::resized()
{
    // Layout your GUI components here
}