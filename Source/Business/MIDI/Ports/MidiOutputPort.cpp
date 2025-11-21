#include "MidiOutputPort.h"

MidiOutputPort::MidiOutputPort()
    : portIsOpen(false)
{
}

MidiOutputPort::~MidiOutputPort()
{
    closePort();
}

bool MidiOutputPort::openPort(const juce::String& deviceId)
{
    closePort();

    if (deviceId.isEmpty())
    {
        return false;
    }

    auto devices = juce::MidiOutput::getAvailableDevices();
    for (const auto& device : devices)
    {
        if (device.identifier == deviceId)
        {
            midiOutput = juce::MidiOutput::openDevice(device.identifier);
            if (midiOutput != nullptr)
            {
                portIsOpen = true;
                return true;
            }
            break;
        }
    }

    return false;
}

void MidiOutputPort::closePort()
{
    if (midiOutput != nullptr)
    {
        midiOutput.reset();
    }
    portIsOpen = false;
}

bool MidiOutputPort::isOpen() const noexcept
{
    return portIsOpen && midiOutput != nullptr;
}

juce::MidiOutput* MidiOutputPort::getMidiOutput() const noexcept
{
    return midiOutput.get();
}

