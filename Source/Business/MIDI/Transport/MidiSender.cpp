#include "MidiSender.h"
#include "../Exceptions/Exceptions.h"

MidiSender::MidiSender()
    : midiOutput(nullptr)
{
}

void MidiSender::setMidiOutput(juce::MidiOutput* output) noexcept
{
    midiOutput = output;
}

void MidiSender::sendSysEx(const juce::MemoryBlock& sysExData)
{
    ensureOutputAvailable();

    juce::MidiMessage message = juce::MidiMessage::createSysExMessage(
        sysExData.getData(), static_cast<int>(sysExData.getSize()));
    
    midiOutput->sendMessageNow(message);
}

void MidiSender::sendProgramChange(uint8_t programNumber, int channel)
{
    ensureOutputAvailable();

    juce::MidiMessage message = juce::MidiMessage::programChange(channel, programNumber);
    midiOutput->sendMessageNow(message);
}

void MidiSender::sendNoteOn(uint8_t noteNumber, uint8_t velocity, int channel)
{
    ensureOutputAvailable();

    juce::MidiMessage message = juce::MidiMessage::noteOn(channel, noteNumber, velocity);
    midiOutput->sendMessageNow(message);
}

void MidiSender::sendNoteOff(uint8_t noteNumber, uint8_t velocity, int channel)
{
    ensureOutputAvailable();

    juce::MidiMessage message = juce::MidiMessage::noteOff(channel, noteNumber, velocity);
    midiOutput->sendMessageNow(message);
}

void MidiSender::sendControlChange(uint8_t controllerNumber, uint8_t value, int channel)
{
    ensureOutputAvailable();

    juce::MidiMessage message = juce::MidiMessage::controllerEvent(channel, controllerNumber, value);
    midiOutput->sendMessageNow(message);
}

bool MidiSender::isOutputAvailable() const noexcept
{
    return midiOutput != nullptr;
}

void MidiSender::ensureOutputAvailable() const
{
    if (midiOutput == nullptr)
    {
        throw MidiConnectionException("MIDI output port not available");
    }
}

