#include "PluginProcessor.h"
#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p)
    , pluginProcessor(p)
    , pluginVersionLabel("VersionLabel", "v0.0.5")
{
    pluginVersionLabel.setFont(juce::Font(juce::FontOptions().withHeight(8.0f)));
    pluginVersionLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    pluginVersionLabel.setJustificationType(juce::Justification::bottomLeft);
    addAndMakeVisible(pluginVersionLabel);
    
    setSize(1335, 730);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void PluginEditor::resized()
{
    pluginVersionLabel.setBounds(0, getHeight() - 20, 100, 20);
}
