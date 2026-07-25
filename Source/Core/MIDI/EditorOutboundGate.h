#pragma once

#include <juce_core/juce_core.h>

#include "Core/MIDI/SysEx/SysExConstants.h"
#include "Shared/Definitions/MatrixDeviceTypes.h"

namespace Core
{
    /** FR-2 / V1.2: editor SysEx / Program Change only when a supported Matrix is detected.
        Unknown Matrix-family members are connected-but-unsupported (locked).
        Device Inquiry is the unlock path and must bypass this gate (see maySendEditorSysEx). */
    inline bool isEditorOutboundAllowed(bool deviceDetected,
                                        MatrixDeviceTypes::Type deviceType) noexcept
    {
        return deviceDetected && MatrixDeviceTypes::isSupportedMatrixDevice(deviceType);
    }

    /** Named allowlist: Universal Device Inquiry request (MMA non-realtime). */
    inline bool isDeviceInquirySysEx(const juce::MemoryBlock& sysEx) noexcept
    {
        constexpr auto kLen = SysExConstants::DeviceInquiry::kRequestMessageLength;
        if (sysEx.getSize() != kLen)
            return false;

        const auto* data = static_cast<const juce::uint8*>(sysEx.getData());
        for (size_t i = 0; i < kLen; ++i)
        {
            if (data[i] != SysExConstants::DeviceInquiry::kRequestMessage[i])
                return false;
        }

        return true;
    }

    inline bool maySendEditorProgramChange(bool deviceDetected,
                                           MatrixDeviceTypes::Type deviceType) noexcept
    {
        return isEditorOutboundAllowed(deviceDetected, deviceType);
    }

    inline bool maySendEditorSysEx(bool deviceDetected,
                                   MatrixDeviceTypes::Type deviceType,
                                   const juce::MemoryBlock& sysEx) noexcept
    {
        return isEditorOutboundAllowed(deviceDetected, deviceType) || isDeviceInquirySysEx(sysEx);
    }

    /** Panels stay locked while undetected, unsupported (Unknown), or while Mutator Compare is active. */
    inline bool isSectionLocked(bool deviceDetected,
                                MatrixDeviceTypes::Type deviceType,
                                bool compareActive) noexcept
    {
        return ! isEditorOutboundAllowed(deviceDetected, deviceType) || compareActive;
    }
}
