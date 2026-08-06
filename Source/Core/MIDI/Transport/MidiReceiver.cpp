#include <thread>

#include "MidiReceiver.h"

#include "Core/Loggers/MidiLogger.h"

namespace
{
juce::String midiMessageTypeLabel(const juce::MidiMessage& message)
{
    if (message.isNoteOn())
        return "NoteOn";
    if (message.isNoteOff())
        return "NoteOff";
    if (message.isController())
        return "CC";
    if (message.isPitchWheel())
        return "PitchWheel";
    if (message.isAftertouch())
        return "Aftertouch";
    return "Unknown";
}
} // namespace

MidiReceiver::MidiReceiver()
    : midiInput(nullptr)
    , isDestroying(false)
    , isReceivingSysEx(false)
    , responseReceived(false)
{
}

void MidiReceiver::setActivityTracker(Core::MidiActivityTracker* tracker) noexcept
{
    activityTracker_ = tracker;
}

MidiReceiver::~MidiReceiver()
{
    // Mark as destroying to prevent callbacks from accessing members
    isDestroying = true;

    cancelOneShotSysExCapture();

    // Stop MIDI input to prevent new callbacks
    if (midiInput != nullptr)
        midiInput->stop();

    reset();
}

void MidiReceiver::setMidiInput(juce::MidiInput* input)
{
    // Re-binding the same device must not stop it: MidiManager calls this on every
    // syncMidiPortsFromState / restoreMidiPortsForHost, including when the port is
    // already open. Stopping without start() silently kills inbound MIDI for the session.
    if (midiInput == input)
        return;

    if (midiInput != nullptr)
        midiInput->stop();

    midiInput = input;

    if (midiInput != nullptr)
        midiInput->start();
}

void MidiReceiver::handleIncomingMidiMessage(juce::MidiInput* source,
                                             const juce::MidiMessage& message)
{
    juce::ignoreUnused(source);

    if (isDestroying.load() || midiInput == nullptr)
        return;

    if (activityTracker_ != nullptr)
        activityTracker_->notifyActivity(Core::MidiActivityTracker::Path::kMidiFromInbound);

    MidiLogger::getInstance().logInfo("MIDI message received: "
                                      + juce::String(message.getRawDataSize())
                                      + " bytes");

    if (message.isSysEx())
        handleIncomingSysEx(message);
    else
        logIncomingNonSysExMessage(message);
}

void MidiReceiver::handleIncomingSysEx(const juce::MidiMessage& message)
{
    // JUCE's getSysExData() excludes F0/F7. Our parser/decoder require the full envelope
    // (same format as on-disk .syx and outbound encode). Use getRawData() which keeps both.
    const juce::uint8* rawData = message.getRawData();
    const int rawSize = message.getRawDataSize();

    MidiLogger::getInstance().logInfo("SysEx message detected: "
                                      + juce::String(rawSize) + " bytes");

    if (rawSize <= 0 || rawData == nullptr)
    {
        MidiLogger::getInstance().logWarning("SysEx message has null data or zero size");
        return;
    }

    juce::MemoryBlock completeSysEx(rawData, static_cast<size_t>(rawSize));
    MidiLogger::getInstance().logSysExReceived(completeSysEx);
    processCompleteSysEx(completeSysEx);
}

void MidiReceiver::logIncomingNonSysExMessage(const juce::MidiMessage& message)
{
    if (message.isProgramChange())
    {
        MidiLogger::getInstance().logProgramChange(
            static_cast<juce::uint8>(message.getProgramChangeNumber()), "RECEIVED");
        return;
    }

    MidiLogger::getInstance().logInfo("MIDI message type: "
                                      + midiMessageTypeLabel(message)
                                      + ", channel: "
                                      + juce::String(message.getChannel()));
}

juce::MemoryBlock MidiReceiver::waitForSysExResponse(int timeoutMs)
{
    reset();

    auto startTime = std::chrono::steady_clock::now();
    
    while (true)
    {
        if (checkIfResponseReceived())
        {
            return getReceivedSysEx();
        }

        if (hasTimeoutElapsed(startTime, timeoutMs))
        {
            logTimeoutAndReset(timeoutMs);
            return {};
        }

        sleepToAvoidBusyWaiting();
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

void MidiReceiver::armOneShotSysExCapture(SysExCaptureCallback callback, SysExCaptureFilter filter)
{
    std::lock_guard<std::mutex> lock(oneShotMutex_);
    oneShotCapture_ = std::move(callback);
    oneShotFilter_ = std::move(filter);
}

void MidiReceiver::cancelOneShotSysExCapture() noexcept
{
    std::lock_guard<std::mutex> lock(oneShotMutex_);
    oneShotCapture_ = nullptr;
    oneShotFilter_ = nullptr;
}

void MidiReceiver::feedCompleteSysEx(const juce::MemoryBlock& completeSysEx)
{
    processCompleteSysEx(completeSysEx);
}

void MidiReceiver::processCompleteSysEx(const juce::MemoryBlock& completeSysEx)
{
    if (isDestroying.load())
        return;

    storeReceivedSysExAndNotify(completeSysEx);
    deliverOneShotCapture(completeSysEx);
}

void MidiReceiver::deliverOneShotCapture(const juce::MemoryBlock& completeSysEx)
{
    SysExCaptureCallback callback;
    SysExCaptureFilter filter;
    {
        std::lock_guard<std::mutex> lock(oneShotMutex_);
        if (! oneShotCapture_)
            return;

        filter = oneShotFilter_;
        if (! filter)
        {
            callback = std::move(oneShotCapture_);
            oneShotCapture_ = nullptr;
            oneShotFilter_ = nullptr;
        }
    }

    if (filter)
    {
        bool accepted = false;
        try
        {
            accepted = filter(completeSysEx);
        }
        catch (...)
        {
            return; // keep armed
        }

        if (! accepted)
            return;

        std::lock_guard<std::mutex> lock(oneShotMutex_);
        if (! oneShotCapture_)
            return; // cancelled while filtering

        callback = std::move(oneShotCapture_);
        oneShotCapture_ = nullptr;
        oneShotFilter_ = nullptr;
    }

    if (callback)
        callback(completeSysEx);
}

bool MidiReceiver::checkIfResponseReceived()
{
    std::lock_guard<std::mutex> lock(responseMutex);
    if (responseReceived.load())
    {
        responseReceived = false;
        return true;
    }
    return false;
}

juce::MemoryBlock MidiReceiver::getReceivedSysEx()
{
    std::lock_guard<std::mutex> lock(responseMutex);
    return receivedSysEx;
}

bool MidiReceiver::hasTimeoutElapsed(
    const std::chrono::steady_clock::time_point& startTime,
    int timeoutMs) const
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        currentTime - startTime).count();
    return elapsed >= timeoutMs;
}

void MidiReceiver::logTimeoutAndReset(int timeoutMs)
{
    MidiLogger::getInstance().logWarning("Timeout waiting for SysEx response (" + 
                                          juce::String(timeoutMs) + "ms)");
    reset();
}

void MidiReceiver::sleepToAvoidBusyWaiting()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void MidiReceiver::storeReceivedSysExAndNotify(const juce::MemoryBlock& completeSysEx)
{
    std::lock_guard<std::mutex> lock(responseMutex);
    if (!isDestroying.load())
    {
        receivedSysEx = completeSysEx;
        responseReceived = true;
    }
}

