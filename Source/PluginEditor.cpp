#include "PluginProcessor.h"
#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p)
    , pluginProcessor(p)
    , midiInputLabel("MidiInputLabel", "MIDI Input:")
    , midiOutputLabel("MidiOutputLabel", "MIDI Output:")
{
    setupMidiInputComponents();
    setupMidiOutputComponents();
    populateMidiPorts();
    updateMidiPorts();
    setSize(400, 300);
}

PluginEditor::~PluginEditor()
{
    midiInputComboBox.removeListener(this);
    midiOutputComboBox.removeListener(this);
}

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawFittedText("Ten Square Software | Matrix-Control", 
                     0, 0, getWidth(), 40, 
                     juce::Justification::centred, 1);
}

void PluginEditor::resized()
{
    auto area = getLocalBounds();
    area.removeFromTop(50); // Space for title
    
    const int labelWidth = 100;
    const int comboBoxHeight = 25;
    const int rightMargin = 50;
    const int verticalSpacing = 5;
    
    // MIDI Input
    auto inputArea = area.removeFromTop(comboBoxHeight);
    inputArea.removeFromRight(rightMargin);
    midiInputComboBox.setBounds(inputArea.removeFromRight(inputArea.getWidth() - labelWidth));
    
    // Spacing between Input and Output
    area.removeFromTop(verticalSpacing);
    
    // MIDI Output
    auto outputArea = area.removeFromTop(comboBoxHeight);
    outputArea.removeFromRight(rightMargin);
    midiOutputComboBox.setBounds(outputArea.removeFromRight(outputArea.getWidth() - labelWidth));
}

void PluginEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &midiInputComboBox)
    {
        handleMidiInputSelection(midiInputComboBox.getSelectedId());
    }
    else if (comboBoxThatHasChanged == &midiOutputComboBox)
    {
        handleMidiOutputSelection(midiOutputComboBox.getSelectedId());
    }
}

void PluginEditor::populateMidiPorts()
{
    populateMidiInputPorts();
    populateMidiOutputPorts();
}

void PluginEditor::updateMidiPorts()
{
    restoreMidiInputPort();
    restoreMidiOutputPort();
}

void PluginEditor::setupMidiInputComponents()
{
    midiInputLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    midiInputLabel.attachToComponent(&midiInputComboBox, true);
    midiInputComboBox.addListener(this);
    addAndMakeVisible(midiInputComboBox);
    addAndMakeVisible(midiInputLabel);
}

void PluginEditor::setupMidiOutputComponents()
{
    midiOutputLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    midiOutputLabel.attachToComponent(&midiOutputComboBox, true);
    midiOutputComboBox.addListener(this);
    addAndMakeVisible(midiOutputComboBox);
    addAndMakeVisible(midiOutputLabel);
}

void PluginEditor::handleMidiInputSelection(int selectedId)
{
    if (selectedId == 1)
    {
        pluginProcessor.getApvts().state.setProperty("midiInputPortId", juce::String(), nullptr);
        return;
    }
    
    if (selectedId > 1)
    {
        auto devices = juce::MidiInput::getAvailableDevices();
        int deviceIndex = selectedId - 2;
        if (deviceIndex >= 0 && deviceIndex < devices.size())
        {
            pluginProcessor.setMidiInputPort(devices[deviceIndex].identifier);
        }
    }
}

void PluginEditor::handleMidiOutputSelection(int selectedId)
{
    if (selectedId == 1)
    {
        pluginProcessor.getApvts().state.setProperty("midiOutputPortId", juce::String(), nullptr);
        return;
    }
    
    if (selectedId > 1)
    {
        auto devices = juce::MidiOutput::getAvailableDevices();
        int deviceIndex = selectedId - 2;
        if (deviceIndex >= 0 && deviceIndex < devices.size())
        {
            pluginProcessor.setMidiOutputPort(devices[deviceIndex].identifier);
        }
    }
}

void PluginEditor::restoreMidiInputPort()
{
    auto inputPortId = pluginProcessor.getApvts().state.getProperty("midiInputPortId").toString();
    if (inputPortId.isEmpty())
    {
        midiInputComboBox.setSelectedId(1, juce::dontSendNotification);
        return;
    }
    
    auto devices = juce::MidiInput::getAvailableDevices();
    for (int i = 0; i < devices.size(); ++i)
    {
        if (devices[i].identifier == inputPortId)
        {
            midiInputComboBox.setSelectedId(i + 2, juce::dontSendNotification);
            pluginProcessor.setMidiInputPort(inputPortId);
            return;
        }
    }
    
    midiInputComboBox.setSelectedId(1, juce::dontSendNotification);
}

void PluginEditor::restoreMidiOutputPort()
{
    auto outputPortId = pluginProcessor.getApvts().state.getProperty("midiOutputPortId").toString();
    if (outputPortId.isEmpty())
    {
        midiOutputComboBox.setSelectedId(1, juce::dontSendNotification);
        return;
    }
    
    auto devices = juce::MidiOutput::getAvailableDevices();
    for (int i = 0; i < devices.size(); ++i)
    {
        if (devices[i].identifier == outputPortId)
        {
            midiOutputComboBox.setSelectedId(i + 2, juce::dontSendNotification);
            pluginProcessor.setMidiOutputPort(outputPortId);
            return;
        }
    }
    
    midiOutputComboBox.setSelectedId(1, juce::dontSendNotification);
}

void PluginEditor::populateMidiInputPorts()
{
    midiInputComboBox.clear();
    midiInputComboBox.addItem("None", 1);
    midiInputComboBox.setSelectedId(1, juce::dontSendNotification);
    
    int itemId = 2;
    auto inputDevices = juce::MidiInput::getAvailableDevices();
    for (const auto& device : inputDevices)
    {
        midiInputComboBox.addItem(device.name, itemId++);
    }
}

void PluginEditor::populateMidiOutputPorts()
{
    midiOutputComboBox.clear();
    midiOutputComboBox.addItem("None", 1);
    midiOutputComboBox.setSelectedId(1, juce::dontSendNotification);
    
    int itemId = 2;
    auto outputDevices = juce::MidiOutput::getAvailableDevices();
    for (const auto& device : outputDevices)
    {
        midiOutputComboBox.addItem(device.name, itemId++);
    }
}