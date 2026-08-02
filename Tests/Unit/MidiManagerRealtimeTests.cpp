#include <juce_core/juce_core.h>

#include "Core/MIDI/MidiActivityTracker.h"
#include "Core/MIDI/MidiManager.h"
#include "Core/MIDI/Queue/MidiOutboundQueue.h"
#include "MidiManagerTestSupport.h"
#include "Shared/Definitions/MatrixDeviceTypes.h"

using MidiManagerTestSupport::MinimalAudioProcessor;
using MidiManagerTestSupport::firstAvailableOutputDeviceId;
using MidiManagerTestSupport::openFirstAvailableOutputOrSkip;
using MidiManagerTestSupport::waitForQueueEmpty;

class MidiManagerRealtimeTests : public juce::UnitTest
{
public:
    MidiManagerRealtimeTests() : juce::UnitTest("MidiManager Realtime") {}

    void runTest() override
    {
        testRealtimeRetainedWithoutOutput();
        testSysExRetainedWithoutOutput();
        testQueuedSysExGateSharingTwoMessagesDrain();
        testNoOutputPortDoesNotThrow();
        testRealtimeDispatchesAfterOutputPortOpened();
        testEmptySysExPayloadSkipped();
        testRealtimeNotStarvedDuringSysExGate();
        testSendPanicEnqueuesAllNotesOffThenResetControllers();
        testDrainRealtimeDoesNotReorderSysEx();
    }

private:
    void testRealtimeRetainedWithoutOutput()
    {
        beginTest("Enqueue realtime — queue retained when no output port");

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        proc.apvts.state.setProperty("deviceDetected", true, nullptr);
        proc.apvts.state.setProperty(MatrixDeviceTypes::kApvtsPropertyName,
                                      MatrixDeviceTypes::kMatrix1000Id,
                                      nullptr);

        manager.startThread();
        manager.sendProgramChange(42, 1);

        juce::Thread::sleep(50);
        expect(!queue.isEmpty(), "Realtime message should remain queued without output port");
        manager.stopThread(2000);
        expect(!manager.isThreadRunning(), "MIDI thread should stop cleanly");
    }

    void testSysExRetainedWithoutOutput()
    {
        beginTest("Enqueue SysEx — queue retained when no output port");

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        proc.apvts.state.setProperty("deviceDetected", true, nullptr);
        proc.apvts.state.setProperty(MatrixDeviceTypes::kApvtsPropertyName,
                                      MatrixDeviceTypes::kMatrix1000Id,
                                      nullptr);

        manager.startThread();
        manager.enqueueRemoteParameterEdit(10, 64);

        juce::Thread::sleep(50);
        expect(!queue.isEmpty(), "SysEx message should remain queued without output port");
        manager.stopThread(2000);
        expect(!manager.isThreadRunning(), "MIDI thread should stop cleanly");
    }

    void testQueuedSysExGateSharingTwoMessagesDrain()
    {
        beginTest("Queued SysEx gate sharing — two SysEx drain without hang");

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        proc.apvts.state.setProperty("deviceDetected", true, nullptr);
        proc.apvts.state.setProperty(MatrixDeviceTypes::kApvtsPropertyName,
                                      MatrixDeviceTypes::kMatrix1000Id,
                                      nullptr);

        if (!openFirstAvailableOutputOrSkip(manager, *this))
            return;

        manager.startThread();
        manager.enqueueRemoteParameterEdit(1, 10);
        manager.enqueueRemoteParameterEdit(2, 20);

        expect(waitForQueueEmpty(queue, 5000),
               "Two queued SysEx messages should drain via gate sharing");
        manager.stopThread(2000);
        expect(!manager.isThreadRunning(), "MIDI thread should stop cleanly after SysEx drain");
    }

    void testNoOutputPortDoesNotThrow()
    {
        beginTest("No output port — enqueue does not throw and retains messages");

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        proc.apvts.state.setProperty("deviceDetected", true, nullptr);
        proc.apvts.state.setProperty(MatrixDeviceTypes::kApvtsPropertyName,
                                      MatrixDeviceTypes::kMatrix1000Id,
                                      nullptr);

        manager.startThread();

        bool threw = false;
        try
        {
            manager.sendProgramChange(7, 1);
            manager.enqueueRemoteParameterEdit(5, 32);
            juce::Thread::sleep(50);
            expect(!queue.isEmpty(), "Queue should retain messages when output unavailable");
        }
        catch (...)
        {
            threw = true;
        }

        expect(!threw, "Enqueue with no output port must not throw");
        manager.stopThread(2000);
    }

    void testRealtimeDispatchesAfterOutputPortOpened()
    {
        beginTest("Realtime message dispatches after output port becomes available");

        const auto outputId = firstAvailableOutputDeviceId();
        if (outputId.isEmpty())
        {
            logMessage("Skipped — no MIDI output device available");
            return;
        }

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        manager.startThread();
        queue.enqueueRealtime(juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)));

        juce::Thread::sleep(50);
        expect(!queue.isEmpty(), "Message should wait until output port is opened");

        if (!manager.setMidiOutputPort(outputId))
        {
            logMessage("Skipped — MIDI output port could not be opened");
            manager.stopThread(2000);
            return;
        }

        expect(waitForQueueEmpty(queue, 2000), "Message should dispatch after output port opens");
        expect(tracker.getActivityLevel(Core::MidiActivityTracker::Path::kOutbound) > 0.0f,
               "Outbound activity should be recorded after successful send");
        manager.stopThread(2000);
    }

    void testEmptySysExPayloadSkipped()
    {
        beginTest("Empty SysEx payload — dequeued and skipped without blocking delay");

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        if (!openFirstAvailableOutputOrSkip(manager, *this))
            return;

        manager.startThread();
        queue.enqueueSysEx(juce::MemoryBlock());

        expect(waitForQueueEmpty(queue, 2000), "Empty SysEx should still be dequeued when output is available");
        manager.stopThread(2000);
    }

    void testRealtimeNotStarvedDuringSysExGate()
    {
        beginTest("Realtime MIDI drains while SysEx inter-message gate is active");

        const auto outputId = firstAvailableOutputDeviceId();
        if (outputId.isEmpty())
        {
            logMessage("Skipped — no MIDI output device available");
            return;
        }

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        proc.apvts.state.setProperty("deviceDetected", true, nullptr);
        proc.apvts.state.setProperty(MatrixDeviceTypes::kApvtsPropertyName,
                                      MatrixDeviceTypes::kMatrix1000Id,
                                      nullptr);

        if (!manager.setMidiOutputPort(outputId))
        {
            logMessage("Skipped — MIDI output port could not be opened");
            return;
        }

        manager.startThread();

        constexpr int kRealtimeBurstCount = 50;
        manager.enqueueRemoteParameterEdit(1, 10);

        for (int i = 0; i < kRealtimeBurstCount; ++i)
            queue.enqueueRealtime(juce::MidiMessage::noteOff(1, static_cast<int>(i % 128)));

        manager.enqueueRemoteParameterEdit(2, 20);

        const auto startMs = juce::Time::getMillisecondCounter();
        expect(waitForQueueEmpty(queue, 3000),
               "Realtime burst should not be blocked by SysEx inter-message gate");
        const auto elapsedMs = juce::Time::getMillisecondCounter() - startMs;

        expect(elapsedMs < 500,
               "Draining realtime during SysEx gate should complete well under gate*N blocking");
        manager.stopThread(2000);
    }

    void testSendPanicEnqueuesAllNotesOffThenResetControllers()
    {
        beginTest("sendPanic without midiChannel param clears all 16 channels (120, 123, 121)");

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        manager.sendPanic();

        expectEquals(static_cast<int>(queue.realtimeDepth()), 48);

        for (int channel = 1; channel <= 16; ++channel)
        {
            auto allSoundOff = queue.dequeue();
            expect(allSoundOff.has_value());
            expect(allSoundOff->midiMessage.isController());
            expectEquals(allSoundOff->midiMessage.getControllerNumber(), 120);
            expectEquals(allSoundOff->midiMessage.getControllerValue(), 0);
            expectEquals(allSoundOff->midiMessage.getChannel(), channel);

            auto notesOff = queue.dequeue();
            expect(notesOff.has_value());
            expect(notesOff->midiMessage.isController());
            expectEquals(notesOff->midiMessage.getControllerNumber(), 123);
            expectEquals(notesOff->midiMessage.getControllerValue(), 0);
            expectEquals(notesOff->midiMessage.getChannel(), channel);

            auto reset = queue.dequeue();
            expect(reset.has_value());
            expect(reset->midiMessage.isController());
            expectEquals(reset->midiMessage.getControllerNumber(), 121);
            expectEquals(reset->midiMessage.getControllerValue(), 0);
            expectEquals(reset->midiMessage.getChannel(), channel);
        }

        expect(queue.isEmpty());
    }

    void testDrainRealtimeDoesNotReorderSysEx()
    {
        beginTest("tryDequeueRealtime drains realtime without touching SysEx FIFO");

        Core::MidiOutboundQueue queue;
        queue.enqueueSysEx(juce::MemoryBlock { "\xf0\x01\xf7", 3 });
        queue.enqueueSysEx(juce::MemoryBlock { "\xf0\x02\xf7", 3 });
        queue.enqueueRealtime(juce::MidiMessage::noteOff(1, 60));
        queue.enqueueRealtime(juce::MidiMessage::noteOff(1, 61));

        auto firstRt = queue.tryDequeueRealtime();
        expect(firstRt.has_value());
        expect(firstRt->isNoteOff());
        expectEquals(firstRt->getNoteNumber(), 60);

        auto secondRt = queue.tryDequeueRealtime();
        expect(secondRt.has_value());
        expectEquals(secondRt->getNoteNumber(), 61);

        expect(! queue.tryDequeueRealtime().has_value());
        expectEquals(static_cast<int>(queue.sysExDepth()), 2);

        auto firstSx = queue.dequeue();
        expect(firstSx.has_value());
        expectEquals(static_cast<int>(static_cast<juce::uint8>(firstSx->sysExData[1])), 0x01);

        auto secondSx = queue.dequeue();
        expect(secondSx.has_value());
        expectEquals(static_cast<int>(static_cast<juce::uint8>(secondSx->sysExData[1])), 0x02);
    }
};

static MidiManagerRealtimeTests midiManagerRealtimeTests;
