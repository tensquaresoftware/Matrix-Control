#include "PluginProcessor.h"
#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p)
    , pluginProcessor(p)
    , midiInputLabel("MidiInputLabel", "MIDI Input:")
    , midiOutputLabel("MidiOutputLabel", "MIDI Output:")
{
    // Setup MIDI Input label and combo box
    midiInputLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    midiInputLabel.attachToComponent(&midiInputComboBox, true);
    midiInputComboBox.addListener(this);
    addAndMakeVisible(midiInputComboBox);
    addAndMakeVisible(midiInputLabel);
    
    // Setup MIDI Output label and combo box
    midiOutputLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    midiOutputLabel.attachToComponent(&midiOutputComboBox, true);
    midiOutputComboBox.addListener(this);
    addAndMakeVisible(midiOutputComboBox);
    addAndMakeVisible(midiOutputLabel);
    
    // Populate combo boxes with available MIDI ports
    populateMidiPorts();
    
    // Restore selected ports from APVTS
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
        int selectedId = midiInputComboBox.getSelectedId();
        if (selectedId == 1)
        {
            // "None" selected
            pluginProcessor.getApvts().state.setProperty("midiInputPortId", juce::String(), nullptr);
        }
        else if (selectedId > 1)
        {
            juce::String deviceName = midiInputComboBox.getItemText(selectedId - 1);
            // Find the actual device ID from the name
            auto devices = juce::MidiInput::getAvailableDevices();
            int deviceIndex = selectedId - 2; // selectedId 2 = device index 0
            if (deviceIndex >= 0 && deviceIndex < devices.size())
            {
                pluginProcessor.setMidiInputPort(devices[deviceIndex].identifier);
            }
        }
    }
    else if (comboBoxThatHasChanged == &midiOutputComboBox)
    {
        int selectedId = midiOutputComboBox.getSelectedId();
        if (selectedId == 1)
        {
            // "None" selected
            pluginProcessor.getApvts().state.setProperty("midiOutputPortId", juce::String(), nullptr);
        }
        else if (selectedId > 1)
        {
            juce::String deviceName = midiOutputComboBox.getItemText(selectedId - 1);
            // Find the actual device ID from the name
            auto devices = juce::MidiOutput::getAvailableDevices();
            int deviceIndex = selectedId - 2; // selectedId 2 = device index 0
            if (deviceIndex >= 0 && deviceIndex < devices.size())
            {
                pluginProcessor.setMidiOutputPort(devices[deviceIndex].identifier);
            }
        }
    }
}

void PluginEditor::populateMidiPorts()
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
    
    midiOutputComboBox.clear();
    midiOutputComboBox.addItem("None", 1);
    midiOutputComboBox.setSelectedId(1, juce::dontSendNotification);
    
    itemId = 2;
    auto outputDevices = juce::MidiOutput::getAvailableDevices();
    for (const auto& device : outputDevices)
    {
        midiOutputComboBox.addItem(device.name, itemId++);
    }
}

void PluginEditor::updateMidiPorts()
{
    // Restore MIDI Input selection
    auto inputPortId = pluginProcessor.getApvts().state.getProperty("midiInputPortId").toString();
    bool inputFound = false;
    if (inputPortId.isNotEmpty())
    {
        auto devices = juce::MidiInput::getAvailableDevices();
        for (int i = 0; i < devices.size(); ++i)
        {
            if (devices[i].identifier == inputPortId)
            {
                midiInputComboBox.setSelectedId(i + 2, juce::dontSendNotification);
                inputFound = true;
                break;
            }
        }
    }
    if (!inputFound)
    {
        midiInputComboBox.setSelectedId(1, juce::dontSendNotification); // "None"
    }
    
    // Restore MIDI Output selection
    auto outputPortId = pluginProcessor.getApvts().state.getProperty("midiOutputPortId").toString();
    bool outputFound = false;
    if (outputPortId.isNotEmpty())
    {
        auto devices = juce::MidiOutput::getAvailableDevices();
        for (int i = 0; i < devices.size(); ++i)
        {
            if (devices[i].identifier == outputPortId)
            {
                midiOutputComboBox.setSelectedId(i + 2, juce::dontSendNotification);
                outputFound = true;
                break;
            }
        }
    }
    if (!outputFound)
    {
        midiOutputComboBox.setSelectedId(1, juce::dontSendNotification); // "None"
    }
}