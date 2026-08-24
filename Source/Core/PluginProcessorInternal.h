#pragma once

#include <functional>

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "Shared/ProjectPaths.h"

// Free helpers shared by PluginProcessor.cpp and its PluginProcessor*.cpp companions.
// Extracted from the PluginProcessor.cpp anonymous namespace for modular maintenance.
namespace PluginProcessorInternal
{
    inline bool isStandaloneWrapper()
    {
        return juce::PluginHostType::getPluginLoadedAs() == juce::AudioProcessor::wrapperType_Standalone;
    }

    inline bool isVst3Wrapper()
    {
        return juce::PluginHostType::getPluginLoadedAs() == juce::AudioProcessor::wrapperType_VST3;
    }

    inline void runSyncOnMessageThread(std::function<void()> task)
    {
        if (juce::MessageManager::existsAndIsCurrentThread())
        {
            task();
            return;
        }

        juce::MessageManager::callAsync(std::move(task));
    }

    inline bool isMidiInputDeviceAvailable(const juce::String& deviceId)
    {
        if (deviceId.isEmpty())
            return false;

        for (const auto& device : juce::MidiInput::getAvailableDevices())
        {
            if (device.identifier == deviceId)
                return true;
        }

        return false;
    }

    inline bool isMidiOutputDeviceAvailable(const juce::String& deviceId)
    {
        if (deviceId.isEmpty())
            return false;

        for (const auto& device : juce::MidiOutput::getAvailableDevices())
        {
            if (device.identifier == deviceId)
                return true;
        }

        return false;
    }

    inline juce::String sanitizePersistedMidiInputPortId(const juce::String& deviceId)
    {
        return isMidiInputDeviceAvailable(deviceId) ? deviceId : juce::String();
    }

    inline juce::String sanitizePersistedMidiOutputPortId(const juce::String& deviceId)
    {
        return isMidiOutputDeviceAvailable(deviceId) ? deviceId : juce::String();
    }

    inline bool shouldUseDevelopmentLogging()
    {
#if JUCE_DEBUG
        if (isStandaloneWrapper())
            return true;

        return !ProjectPaths::isUsingFallbackRoot();
#else
        return false;
#endif
    }

    inline constexpr int kEditorialUndoMaxStoredUnits = 100;
}
