#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Business/MIDI/MidiManager.h"
#include <juce_audio_devices/juce_audio_devices.h>

PluginProcessor::PluginProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
    , apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
    , midiManager(std::make_unique<MidiManager>(apvts))
{
    // Initialize APVTS properties for MIDI port selection
    if (!apvts.state.hasProperty("midiInputPortId"))
    {
        apvts.state.setProperty("midiInputPortId", juce::String(), nullptr);
    }
    if (!apvts.state.hasProperty("midiOutputPortId"))
    {
        apvts.state.setProperty("midiOutputPortId", juce::String(), nullptr);
    }
}

PluginProcessor::~PluginProcessor()
{
}

const juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String PluginProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void PluginProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  juce::ignoreUnused(sampleRate, samplesPerBlock);
  startMidiThread();
}

void PluginProcessor::startMidiThread()
{
  if (midiManager != nullptr && !midiManager->isThreadRunning()) {
    midiManager->startThread();
  }
}

void PluginProcessor::releaseResources()
{
    stopMidiThread();    
}

void PluginProcessor::stopMidiThread()
{
    if (midiManager != nullptr && midiManager->isThreadRunning())
    {
        midiManager->stopThread(5000);  // Wait up to 5 seconds
    }
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& audioBuffer,
                                   juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(audioBuffer);
    juce::ignoreUnused(midiMessages);
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor(*this);
}

void PluginProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName(apvts.state.getType()))
        {
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

void PluginProcessor::setMidiInputPort(const juce::String& deviceId)
{
    if (midiManager != nullptr)
    {
        if (midiManager->setMidiInputPort(deviceId))
        {
            apvts.state.setProperty("midiInputPortId", deviceId, nullptr);
        }
    }
}

void PluginProcessor::setMidiOutputPort(const juce::String& deviceId)
{
    if (midiManager != nullptr)
    {
        if (midiManager->setMidiOutputPort(deviceId))
        {
            apvts.state.setProperty("midiOutputPortId", deviceId, nullptr);
        }
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout parameterLayout;
    
    // For now, we only add MIDI status properties
    // Patch and master parameters will be added later via PluginParameterFactory
    
    // Note: APVTS properties for status are set directly via state.setProperty()
    // They don't need to be in the parameter layout
    
    return parameterLayout;
}