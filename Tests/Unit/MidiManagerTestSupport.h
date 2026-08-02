#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

#include "Core/MIDI/MidiManager.h"
#include "Core/MIDI/Queue/MidiOutboundQueue.h"

namespace MidiManagerTestSupport
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

    inline bool waitForQueueEmpty(Core::MidiOutboundQueue& queue, int timeoutMs)
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

    inline juce::String firstAvailableOutputDeviceId()
    {
        const auto devices = juce::MidiOutput::getAvailableDevices();
        return devices.isEmpty() ? juce::String() : devices.getReference(0).identifier;
    }

    inline bool openFirstAvailableOutputOrSkip(MidiManager& manager, juce::UnitTest& test)
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
