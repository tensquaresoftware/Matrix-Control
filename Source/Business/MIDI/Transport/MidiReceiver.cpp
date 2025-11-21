#include "MidiReceiver.h"
#include <thread>

MidiReceiver::MidiReceiver()
    : midiInput(nullptr)
    , isDestroying(false)
    , isReceivingSysEx(false)
    , responseReceived(false)
{
}

MidiReceiver::~MidiReceiver()
{
    // Mark as destroying to prevent callbacks from accessing members
    isDestroying = true;
    
    // Stop MIDI input to prevent new callbacks
    if (midiInput != nullptr)
    {
        midiInput->stop();
    }
    
    reset();
}

void MidiReceiver::setMidiInput(juce::MidiInput* input)
{
    if (midiInput != nullptr)
    {
        midiInput->stop();
    }

    midiInput = input;

    // Note: The callback must be set when opening the port via MidiInputPort::openPort()
    // The MidiInput is already started if callback was provided during opening
}

void MidiReceiver::handleIncomingMidiMessage(juce::MidiInput* source,
                                             const juce::MidiMessage& message)
{
    juce::ignoreUnused(source);

    // Safety check: ensure we're not being destroyed and still valid
    if (isDestroying.load() || midiInput == nullptr)
    {
        return;
    }

    if (message.isSysEx())
    {
        const uint8_t* sysExData = message.getSysExData();
        int sysExSize = message.getSysExDataSize();

        if (sysExSize > 0 && sysExData != nullptr)
        {
            juce::MemoryBlock completeSysEx(sysExData, static_cast<size_t>(sysExSize));
            processCompleteSysEx(completeSysEx);
        }
    }
}

juce::MemoryBlock MidiReceiver::waitForSysExResponse(int timeoutMs)
{
    reset();

    auto startTime = std::chrono::steady_clock::now();
    
    while (true)
    {
        // Check if response received
        {
            std::lock_guard<std::mutex> lock(responseMutex);
            if (responseReceived.load())
            {
                responseReceived = false;
                return receivedSysEx;
            }
        }

        // Check timeout
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - startTime).count();

        if (elapsed >= timeoutMs)
        {
            reset();
            return {};  // Timeout
        }

        // Sleep to avoid busy-waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void MidiReceiver::reset()
{
    std::lock_guard<std::mutex> bufferLock(bufferMutex);
    std::lock_guard<std::mutex> responseLock(responseMutex);
    
    isReceivingSysEx = false;
    sysExBuffer.clear();
    responseReceived = false;
    receivedSysEx.reset();
}

bool MidiReceiver::isInputAvailable() const noexcept
{
    return midiInput != nullptr;
}

void MidiReceiver::processCompleteSysEx(const juce::MemoryBlock& completeSysEx)
{
    // Safety check: ensure we're not being destroyed
    if (isDestroying.load())
    {
        return;
    }
    
    // Store the received SysEx and notify waiting thread
    {
        std::lock_guard<std::mutex> lock(responseMutex);
        if (!isDestroying.load())
        {
            receivedSysEx = completeSysEx;
            responseReceived = true;
        }
    }
}

