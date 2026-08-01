#pragma once

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

#include "Core/Exceptions/ExceptionPropagator.h"
#include "Core/MIDI/EditorOutboundGate.h"
#include "Core/MIDI/MidiPortOpenFeedback.h"
#include "Core/Services/DeviceTypeRegistry.h"
#include "Shared/Definitions/MatrixDeviceTypes.h"
#include "Shared/Definitions/PluginDisplayNames.h"

// Free helpers shared by MidiManager.cpp and its MidiManager*.cpp companions.
namespace MidiManagerInternal
{
    struct PortOpenFailureArgs
    {
        bool isInput = false;
        Core::MidiPortOpenResult result;
        juce::String deviceId;
        bool reportOpenFailure = true;
    };

    // Port open/clear paths wipe the left-zone footer; re-assert FR-2 / Unknown guidance while locked.
    inline void clearFooterThenReassertDeviceLockGuidance(juce::AudioProcessorValueTreeState& apvts)
    {
        ExceptionPropagator::clearMessage(apvts);

        const bool deviceDetected = static_cast<bool>(apvts.state.getProperty("deviceDetected", false));
        const auto deviceType = Core::DeviceTypeRegistry::fromApvtsProperty(
            apvts.state.getProperty(MatrixDeviceTypes::kApvtsPropertyName));

        if (Core::isEditorOutboundAllowed(deviceDetected, deviceType))
            return;

        const auto* message = deviceDetected
                                  ? PluginDisplayNames::FooterPanel::kUnsupportedMatrixDeviceFooter
                                  : PluginDisplayNames::FooterPanel::kDeviceLockGuidance;
        apvts.state.setProperty("uiMessageText", juce::String(message), nullptr);
        apvts.state.setProperty("uiMessageSeverity",
                               deviceDetected ? "info" : "error",
                               nullptr);
    }

    inline void reportPortOpenFailure(juce::AudioProcessorValueTreeState& apvts,
                                      const PortOpenFailureArgs& args)
    {
        Core::MidiPortOpenFeedback::logOpenFailure(args.isInput,
                                                   args.result.portDisplayName,
                                                   args.deviceId,
                                                   args.result.failureReason);
        if (args.reportOpenFailure)
        {
            Core::MidiPortOpenFeedback::propagateOpenFailure(apvts,
                                                            args.isInput,
                                                            args.result.portDisplayName,
                                                            args.result.failureReason);
        }
    }

    inline juce::String midiInputDeviceNameForId(const juce::String& deviceId)
    {
        for (const auto& device : juce::MidiInput::getAvailableDevices())
        {
            if (device.identifier == deviceId)
                return device.name;
        }

        return deviceId;
    }

    inline juce::String midiOutputDeviceNameForId(const juce::String& deviceId)
    {
        for (const auto& device : juce::MidiOutput::getAvailableDevices())
        {
            if (device.identifier == deviceId)
                return device.name;
        }

        return deviceId;
    }
}
