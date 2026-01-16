#include <juce_audio_devices/juce_audio_devices.h>

#include "PluginProcessor.h"
#include "GUI/PluginEditor.h"
#include "MIDI/MidiManager.h"
#include "Loggers/MidiLogger.h"
#include "Loggers/ApvtsLogger.h"
#include "Factories/ApvtsFactory.h"

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
    validatePluginDescriptorsAtStartup();
    initializeMidiPortProperties();
    apvts.state.addListener(this);
    
    enableApvtsLogging();
}

PluginProcessor::~PluginProcessor()
{
    apvts.state.removeListener(this);
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

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
    
    enableFileLoggingForSession();
    enableApvtsLogging();
    startMidiThread();
}

void PluginProcessor::startMidiThread()
{
    if (midiManager != nullptr && !midiManager->isThreadRunning())
    {
        midiManager->startThread();
    }
}

void PluginProcessor::releaseResources()
{
    stopMidiThread();
    disableApvtsLogging();
    closeLogFileForSession();
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
            ApvtsLogger::getInstance().logStateLoaded("DAW state");
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
    return ApvtsFactory::createParameterLayout();
}

void PluginProcessor::validatePluginDescriptorsAtStartup()
{
    auto validationResult = ApvtsFactory::validatePluginDescriptors();
    if (!validationResult.isValid)
    {
        DBG("SynthDescriptors validation failed:");
        for (const auto& error : validationResult.errors)
        {
            DBG("  ERROR: " + error);
        }
        jassertfalse;
    }
}

void PluginProcessor::initializeMidiPortProperties()
{
    if (!apvts.state.hasProperty("midiInputPortId"))
    {
        apvts.state.setProperty("midiInputPortId", juce::String(), nullptr);
    }
    if (!apvts.state.hasProperty("midiOutputPortId"))
    {
        apvts.state.setProperty("midiOutputPortId", juce::String(), nullptr);
    }
}

void PluginProcessor::enableFileLoggingForSession()
{
    MidiLogger::getInstance().setLogLevel(MidiLogger::LogLevel::kDebug);
    MidiLogger::getInstance().setLogToFile(true);
}

void PluginProcessor::closeLogFileForSession()
{
    MidiLogger::getInstance().setLogToFile(false);
}

void PluginProcessor::enableApvtsLogging()
{
    ApvtsLogger::getInstance().setLogLevel(ApvtsLogger::LogLevel::kDebug);
    ApvtsLogger::getInstance().setLogToConsole(true);
    ApvtsLogger::getInstance().setLogToFile(true);
    ApvtsLogger::getInstance().logInfo("APVTS logging enabled");
}

void PluginProcessor::disableApvtsLogging()
{
    ApvtsLogger::getInstance().setLogToFile(false);
}

void PluginProcessor::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged,
                                              const juce::Identifier& property)
{
    juce::var newValue = treeWhosePropertyHasChanged.getProperty(property);
    
    juce::String threadName = juce::Thread::getCurrentThread() != nullptr 
        ? juce::Thread::getCurrentThread()->getThreadName() 
        : (juce::MessageManager::getInstance()->isThisTheMessageThread() ? "MessageThread" : "Unknown");
    
    juce::String propertyId = property.toString();
    juce::String parameterId = propertyId;
    
    if (propertyId == "value")
    {
        juce::Identifier treeType = treeWhosePropertyHasChanged.getType();
        juce::String treeTypeStr = treeType.toString();
        
        if (treeTypeStr == "PARAM")
        {
            juce::var idProperty = treeWhosePropertyHasChanged.getProperty("id");
            if (idProperty.isString() && idProperty.toString().isNotEmpty())
            {
                parameterId = idProperty.toString();
            }
            else
            {
                idProperty = treeWhosePropertyHasChanged.getProperty("parameterID");
                if (idProperty.isString() && idProperty.toString().isNotEmpty())
                {
                    parameterId = idProperty.toString();
                }
                else
                {
                    parameterId = treeTypeStr;
                }
            }
        }
        else
        {
            parameterId = treeTypeStr;
        }
    }
    
    auto* parameter = apvts.getParameter(parameterId);
    
    if (parameter != nullptr)
    {
        juce::String paramId = parameter->getParameterID();
        if (paramId.isNotEmpty())
        {
            parameterId = paramId;
        }
    }
    else if (propertyId == "value")
    {
        juce::ValueTree parentTree = treeWhosePropertyHasChanged.getParent();
        if (parentTree.isValid())
        {
            juce::Identifier parentType = parentTree.getType();
            juce::String parentTypeStr = parentType.toString();
            
            if (parentTypeStr != "PARAM" && parentTypeStr != "ROOT")
            {
                parameterId = parentTypeStr;
                parameter = apvts.getParameter(parameterId);
                if (parameter != nullptr)
                {
                    juce::String paramId = parameter->getParameterID();
                    if (paramId.isNotEmpty())
                    {
                        parameterId = paramId;
                    }
                }
            }
        }
        
        if (parameter == nullptr)
        {
            for (int i = 0; i < apvts.state.getNumChildren(); ++i)
            {
                juce::ValueTree child = apvts.state.getChild(i);
                if (child.isValid())
                {
                    juce::Identifier childType = child.getType();
                    juce::String childTypeStr = childType.toString();
                    
                    if (childTypeStr == "PARAM")
                    {
                        juce::var idProperty = child.getProperty("id");
                        if (idProperty.isString() && idProperty.toString().isNotEmpty())
                        {
                            juce::String childParamId = idProperty.toString();
                            if (child == treeWhosePropertyHasChanged || 
                                child.getChildWithProperty("value", newValue) == treeWhosePropertyHasChanged)
                            {
                                parameterId = childParamId;
                                break;
                            }
                        }
                    }
                    else if (child == treeWhosePropertyHasChanged)
                    {
                        parameterId = childTypeStr;
                        break;
                    }
                }
            }
        }
    }
    
    ApvtsLogger::getInstance().logValueTreePropertyChanged(
        juce::Identifier(parameterId), 
        juce::var(), 
        newValue, 
        threadName
    );
}

void PluginProcessor::valueTreeChildAdded(juce::ValueTree& parentTree,
                                         juce::ValueTree& childWhichHasBeenAdded)
{
    juce::ignoreUnused(parentTree, childWhichHasBeenAdded);
    ApvtsLogger::getInstance().logDebug("ValueTree child added: " + childWhichHasBeenAdded.getType().toString());
}

void PluginProcessor::valueTreeChildRemoved(juce::ValueTree& parentTree,
                                           juce::ValueTree& childWhichHasBeenRemoved,
                                           int indexFromWhichChildWasRemoved)
{
    juce::ignoreUnused(parentTree, childWhichHasBeenRemoved, indexFromWhichChildWasRemoved);
    ApvtsLogger::getInstance().logDebug("ValueTree child removed: " + childWhichHasBeenRemoved.getType().toString());
}

void PluginProcessor::valueTreeChildOrderChanged(juce::ValueTree& parentTreeWhoseChildrenHaveChanged,
                                                int oldIndex, int newIndex)
{
    juce::ignoreUnused(parentTreeWhoseChildrenHaveChanged, oldIndex, newIndex);
    ApvtsLogger::getInstance().logDebug("ValueTree child order changed");
}

void PluginProcessor::valueTreeParentChanged(juce::ValueTree& treeWhoseParentHasChanged)
{
    juce::ignoreUnused(treeWhoseParentHasChanged);
    ApvtsLogger::getInstance().logDebug("ValueTree parent changed");
}

void PluginProcessor::valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged)
{
    juce::ignoreUnused(treeWhichHasBeenChanged);
    ApvtsLogger::getInstance().logStateReplaced();
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}