#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <vector>
#include <chrono>

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_core/juce_core.h>

#include "Core/MIDI/Exceptions/Exceptions.h"
#include "Core/MIDI/MidiActivityTracker.h"
#include "Core/MIDI/SysEx/SysExConstants.h"

class MidiReceiver : public juce::MidiInputCallback
{
public:
    using SysExCaptureCallback = std::function<void(const juce::MemoryBlock&)>;
    // Return true to deliver and clear the one-shot; false to keep listening (MIDI input thread).
    using SysExCaptureFilter = std::function<bool(const juce::MemoryBlock&)>;

    MidiReceiver();
    ~MidiReceiver() override;

    void setActivityTracker(Core::MidiActivityTracker* tracker) noexcept;
    void setMidiInput(juce::MidiInput* midiInput);
    void handleIncomingMidiMessage(juce::MidiInput* source,
                                   const juce::MidiMessage& message) override;

    // Legacy blocking wait — prefer armOneShotSysExCapture (does not block the message thread).
    juce::MemoryBlock waitForSysExResponse(int timeoutMs = SysExConstants::kDefaultTimeoutMs);

    // Arm a one-shot capture. Without a filter, the next complete SysEx invokes callback on the
    // MIDI input thread. With a filter, non-matching SysEx leave the capture armed until a match,
    // cancel, or a new arm. Caller should marshal to the message thread if needed.
    void armOneShotSysExCapture(SysExCaptureCallback callback, SysExCaptureFilter filter = {});
    void cancelOneShotSysExCapture() noexcept;

    // Feed a completed SysEx into store + one-shot delivery (unit tests / simulators).
    void feedCompleteSysEx(const juce::MemoryBlock& completeSysEx);

    void reset();
    bool isInputAvailable() const noexcept;

private:
    Core::MidiActivityTracker* activityTracker_ { nullptr };
    juce::MidiInput* midiInput;
    std::atomic<bool> isDestroying;

    std::atomic<bool> isReceivingSysEx;
    std::vector<juce::uint8> sysExBuffer;
    std::mutex bufferMutex;

    std::atomic<bool> responseReceived;
    juce::MemoryBlock receivedSysEx;
    std::mutex responseMutex;

    SysExCaptureCallback oneShotCapture_;
    SysExCaptureFilter oneShotFilter_;
    std::mutex oneShotMutex_;

    void processCompleteSysEx(const juce::MemoryBlock& completeSysEx);
    void handleIncomingSysEx(const juce::MidiMessage& message);
    void logIncomingNonSysExMessage(const juce::MidiMessage& message);
    bool checkIfResponseReceived();
    juce::MemoryBlock getReceivedSysEx();
    bool hasTimeoutElapsed(const std::chrono::steady_clock::time_point& startTime, int timeoutMs) const;
    void logTimeoutAndReset(int timeoutMs);
    void sleepToAvoidBusyWaiting();
    void storeReceivedSysExAndNotify(const juce::MemoryBlock& completeSysEx);
    void deliverOneShotCapture(const juce::MemoryBlock& completeSysEx);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiReceiver)
};
