#include <array>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

#include "Core/MIDI/MidiActivityTracker.h"
#include "Core/MIDI/MidiManager.h"
#include "Core/MIDI/Queue/MidiOutboundQueue.h"
#include "Core/MIDI/SysEx/SysExConstants.h"
#include "Shared/Definitions/MatrixDeviceTypes.h"

namespace
{
    class MinimalAudioProcessor : public juce::AudioProcessor
    {
    public:
        MinimalAudioProcessor()
            : juce::AudioProcessor(BusesProperties())
            , apvts(*this, nullptr, "P", {})
        {
        }

        juce::AudioProcessorValueTreeState apvts;

        const juce::String getName() const override { return "Test"; }
        void prepareToPlay(double, int) override {}
        void releaseResources() override {}
        void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override {}
        juce::AudioProcessorEditor* createEditor() override { return nullptr; }
        bool hasEditor() const override { return false; }
        bool acceptsMidi() const override { return false; }
        bool producesMidi() const override { return false; }
        bool isMidiEffect() const override { return false; }
        double getTailLengthSeconds() const override { return 0.0; }
        int getNumPrograms() override { return 1; }
        int getCurrentProgram() override { return 0; }
        void setCurrentProgram(int) override {}
        const juce::String getProgramName(int) override { return {}; }
        void changeProgramName(int, const juce::String&) override {}
        void getStateInformation(juce::MemoryBlock&) override {}
        void setStateInformation(const void*, int) override {}
    };

    bool waitForQueueEmpty(Core::MidiOutboundQueue& queue, int timeoutMs)
    {
        const auto deadline = juce::Time::getMillisecondCounter()
                              + static_cast<juce::uint32>(timeoutMs);

        while (juce::Time::getMillisecondCounter() < deadline)
        {
            if (queue.isEmpty())
                return true;

            juce::Thread::sleep(1);
        }

        return queue.isEmpty();
    }

    juce::String firstAvailableOutputDeviceId()
    {
        const auto devices = juce::MidiOutput::getAvailableDevices();
        return devices.isEmpty() ? juce::String() : devices.getReference(0).identifier;
    }

    bool openFirstAvailableOutputOrSkip(MidiManager& manager, juce::UnitTest& test)
    {
        const auto outputId = firstAvailableOutputDeviceId();
        if (outputId.isEmpty())
        {
            test.logMessage("Skipped — no MIDI output device available");
            return false;
        }

        if (!manager.setMidiOutputPort(outputId))
        {
            test.logMessage("Skipped — MIDI output port could not be opened");
            return false;
        }

        return true;
    }
}

class MidiManagerTests : public juce::UnitTest
{
public:
    MidiManagerTests() : juce::UnitTest("MidiManager Tests") {}

    void runTest() override
    {
        testRealtimeRetainedWithoutOutput();
        testSysExRetainedWithoutOutput();
        testQueuedSysExGateSharingTwoMessagesDrain();
        testNoOutputPortDoesNotThrow();
        testRealtimeDispatchesAfterOutputPortOpened();
        testEmptySysExPayloadSkipped();
        testRealtimeNotStarvedDuringSysExGate();
        testDeviceDumpUnavailableWithoutDevice();
        testWaitUntilOutboundQueueIdleReturnsTrueWhenEmpty();
        testRefreshInquiryClearsDetectionWithoutPorts();
        testEditorOutboundGateBlocksProgramChangeWhenUndetected();
        testEditorOutboundGateBlocksSysExWhenUndetected();
        testEditorOutboundGateAllowsSendWhenDetected();
        testSendMasterFailClosedForNonMatrix1000();
        testWeakReferenceClearsAfterMidiManagerDestroy();
        testRequestSinglePatchAsyncIdleTimeoutFailsVisibly();
    }

private:
    void testDeviceDumpUnavailableWithoutDevice()
    {
        beginTest("Device dump unavailable when no output/device detected");

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        // No MIDI ports open at construction.
        expect(!manager.isDeviceDumpAvailable(),
               "Device dump must be unavailable without open MIDI input and output ports");
    }

    void testRefreshInquiryClearsDetectionWithoutPorts()
    {
        beginTest("refreshDeviceInquiryAfterPortSync clears stale detection without ports");

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        proc.apvts.state.setProperty("deviceDetected", true, nullptr);
        proc.apvts.state.setProperty("deviceVersion", "1.20", nullptr);
        proc.apvts.state.setProperty("deviceType", "Matrix-1000", nullptr);

        manager.refreshDeviceInquiryAfterPortSync();

        expect(! static_cast<bool>(proc.apvts.state.getProperty("deviceDetected")),
               "Detection must clear when MIDI From/To are not both available");
        expectEquals(proc.apvts.state.getProperty("deviceType").toString(), juce::String("Unknown"));
        expect(proc.apvts.state.getProperty("deviceVersion").toString().isEmpty(),
               "deviceVersion must clear with detection");
    }

    void testWaitUntilOutboundQueueIdleReturnsTrueWhenEmpty()
    {
        beginTest("waitUntilOutboundQueueIdle returns true when queue already empty");

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        const auto startMs = juce::Time::getMillisecondCounter();
        const bool idle = manager.waitUntilOutboundQueueIdle(200);
        const auto elapsedMs = juce::Time::getMillisecondCounter() - startMs;

        expect(idle, "Empty queue should report idle immediately");
        expect(elapsedMs < 100, "Idle wait should return promptly on an empty queue");
    }

    void testEditorOutboundGateBlocksProgramChangeWhenUndetected()
    {
        beginTest("FR-2 — Program Change not enqueued when deviceDetected=false");

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        proc.apvts.state.setProperty("deviceDetected", false, nullptr);
        expect(! manager.isEditorOutboundAllowed());

        manager.startThread();
        manager.sendProgramChange(42, 1);
        juce::Thread::sleep(50);

        expect(queue.isEmpty(), "Program Change must not enqueue while locked");
        manager.stopThread(2000);
    }

    void testEditorOutboundGateBlocksSysExWhenUndetected()
    {
        beginTest("FR-2 — editor SysEx not enqueued when deviceDetected=false");

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        proc.apvts.state.setProperty("deviceDetected", false, nullptr);

        manager.startThread();
        manager.enqueueRemoteParameterEdit(10, 64);
        manager.sendSetBank(1);
        juce::Thread::sleep(50);

        expect(queue.isEmpty(), "Editor SysEx must not enqueue while locked");
        manager.stopThread(2000);
    }

    void testEditorOutboundGateAllowsSendWhenDetected()
    {
        beginTest("FR-2 — Program Change and SysEx enqueue when deviceDetected=true");

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        proc.apvts.state.setProperty("deviceDetected", true, nullptr);
        proc.apvts.state.setProperty("deviceType", "Matrix-1000", nullptr);
        expect(manager.isEditorOutboundAllowed());

        manager.startThread();
        manager.sendProgramChange(7, 1);
        manager.enqueueRemoteParameterEdit(5, 32);
        juce::Thread::sleep(50);

        expect(! queue.isEmpty(), "Editor outbound must enqueue once unlocked");
        manager.stopThread(2000);
    }

    void testSendMasterFailClosedForNonMatrix1000()
    {
        beginTest("FR-46 — sendMaster is a no-op for Matrix-6 / Matrix-6R / Unknown / undetected");

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        std::array<juce::uint8, SysExConstants::kMasterPackedDataSize> packed{};
        packed.fill(0);

        manager.startThread();

        // Undetected — editor outbound already false.
        proc.apvts.state.setProperty("deviceDetected", false, nullptr);
        proc.apvts.state.setProperty(MatrixDeviceTypes::kApvtsPropertyName,
                                      MatrixDeviceTypes::kMatrix1000Id,
                                      nullptr);
        manager.sendMaster(0x03, packed.data());
        juce::Thread::sleep(50);
        expect(queue.isEmpty(), "sendMaster must not enqueue while undetected");

        // Detected Matrix-6 — editor outbound allowed, MASTER still blocked.
        proc.apvts.state.setProperty("deviceDetected", true, nullptr);
        proc.apvts.state.setProperty(MatrixDeviceTypes::kApvtsPropertyName,
                                      MatrixDeviceTypes::kMatrix6Id,
                                      nullptr);
        expect(manager.isEditorOutboundAllowed());
        manager.sendMaster(0x03, packed.data());
        juce::Thread::sleep(50);
        expect(queue.isEmpty(), "sendMaster must not enqueue for Matrix-6");

        // Detected Matrix-6R — same FR-46 fail-closed peer as Matrix-6.
        proc.apvts.state.setProperty(MatrixDeviceTypes::kApvtsPropertyName,
                                      MatrixDeviceTypes::kMatrix6RId,
                                      nullptr);
        expect(manager.isEditorOutboundAllowed());
        manager.sendMaster(0x03, packed.data());
        juce::Thread::sleep(50);
        expect(queue.isEmpty(), "sendMaster must not enqueue for Matrix-6R");

        // Detected Unknown — locked for editor and MASTER.
        proc.apvts.state.setProperty(MatrixDeviceTypes::kApvtsPropertyName,
                                      MatrixDeviceTypes::toApvtsString(MatrixDeviceTypes::Type::kUnknown),
                                      nullptr);
        manager.sendMaster(0x03, packed.data());
        juce::Thread::sleep(50);
        expect(queue.isEmpty(), "sendMaster must not enqueue for Unknown");

        // Matrix-1000 — MASTER SysEx may enqueue (defense-in-depth allows).
        proc.apvts.state.setProperty(MatrixDeviceTypes::kApvtsPropertyName,
                                      MatrixDeviceTypes::kMatrix1000Id,
                                      nullptr);
        expect(manager.isEditorOutboundAllowed());
        manager.sendMaster(0x03, packed.data());
        juce::Thread::sleep(50);
        expect(! queue.isEmpty(), "sendMaster must enqueue for detected Matrix-1000");

        manager.stopThread(2000);
    }

    void testWeakReferenceClearsAfterMidiManagerDestroy()
    {
        beginTest("WeakReference — deferred MidiManager capture clears after destroy");

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        juce::WeakReference<MidiManager> weak;

        {
            MidiManager manager(proc.apvts, queue, tracker);
            weak = &manager;
            expect(weak.get() != nullptr, "WeakReference must resolve while MidiManager is alive");
        }

        expect(weak.get() == nullptr,
               "WeakReference must clear after MidiManager destruction (UAF guard primitive)");
    }

    void testRealtimeRetainedWithoutOutput()
    {
        beginTest("Enqueue realtime — queue retained when no output port");

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        proc.apvts.state.setProperty("deviceDetected", true, nullptr);
        proc.apvts.state.setProperty("deviceType", "Matrix-1000", nullptr);

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
        proc.apvts.state.setProperty("deviceType", "Matrix-1000", nullptr);

        manager.startThread();
        manager.enqueueRemoteParameterEdit(10, 64);

        juce::Thread::sleep(50);
        expect(!queue.isEmpty(), "SysEx message should remain queued without output port");
        manager.stopThread(2000);
        expect(!manager.isThreadRunning(), "MIDI thread should stop cleanly");
    }

    void testQueuedSysExGateSharingTwoMessagesDrain()
    {
        beginTest("Queued SysEx gate sharing (sendSysExWithDelay) — two SysEx drain without hang");

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        proc.apvts.state.setProperty("deviceDetected", true, nullptr);
        proc.apvts.state.setProperty("deviceType", "Matrix-1000", nullptr);

        if (!openFirstAvailableOutputOrSkip(manager, *this))
            return;

        manager.startThread();

        manager.enqueueRemoteParameterEdit(1, 10);
        manager.enqueueRemoteParameterEdit(2, 20);

        expect(waitForQueueEmpty(queue, 5000),
               "Two queued SysEx messages should drain via sendSysExWithDelay gate sharing");

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
        proc.apvts.state.setProperty("deviceType", "Matrix-1000", nullptr);

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
        beginTest("Empty SysEx payload — dequeued and skipped without sendSysExWithDelay");

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
        proc.apvts.state.setProperty("deviceType", "Matrix-1000", nullptr);

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

    void testRequestSinglePatchAsyncIdleTimeoutFailsVisibly()
    {
        beginTest("requestSinglePatchAsync — injectable idle timeout fails with empty callback");

        Core::MidiOutboundQueue queue;
        Core::MidiActivityTracker tracker;
        MinimalAudioProcessor proc;
        MidiManager manager(proc.apvts, queue, tracker);

        proc.apvts.state.setProperty("deviceDetected", true, nullptr);
        proc.apvts.state.setProperty(MatrixDeviceTypes::kApvtsPropertyName,
                                      MatrixDeviceTypes::kMatrix1000Id,
                                      nullptr);

        // Keep the outbound queue non-idle without starting the consumer thread.
        const juce::uint8 stuckBytes[] = { 0xF0, 0x10, 0x06, 0x06, 0x00, 0x40, 0xF7 };
        juce::MemoryBlock stuckSysEx(stuckBytes, sizeof(stuckBytes));
        queue.enqueueSysEx(stuckSysEx);

        bool callbackRan = false;
        bool callbackEmpty = false;
        manager.requestSinglePatchAsync(
            0,
            [&](std::vector<juce::uint8> packed)
            {
                callbackRan = true;
                callbackEmpty = packed.empty();
            },
            0,
            0);

        expect(callbackRan, "Idle timeout must invoke the dump callback");
        expect(callbackEmpty, "Timed-out dump must fail with an empty packed buffer");
        expect(proc.apvts.state.getProperty("lastError").toString().contains("Timeout"),
               "Idle timeout must surface a visible Timeout error state");
        expectEquals(proc.apvts.state.getProperty("errorType").toString(), juce::String("Timeout"));
    }
};

static MidiManagerTests midiManagerTests;
