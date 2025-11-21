#pragma once

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_core/juce_core.h>

#include "../Exceptions/Exceptions.h"

class MidiSender
{
public:
    MidiSender();
    ~MidiSender() = default;

    void setMidiOutput(juce::MidiOutput* output) noexcept;

    void sendSysEx(const juce::MemoryBlock& sysExData);
    void sendProgramChange(uint8_t programNumber, int channel = 1);
    void sendNoteOn(uint8_t noteNumber, uint8_t velocity, int channel = 1);
    void sendNoteOff(uint8_t noteNumber, uint8_t velocity = 64, int channel = 1);
    void sendControlChange(uint8_t controllerNumber, uint8_t value, int channel = 1);
    
    bool isOutputAvailable() const noexcept;

private:
    juce::MidiOutput* midiOutput;

    void ensureOutputAvailable() const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiSender)
};

