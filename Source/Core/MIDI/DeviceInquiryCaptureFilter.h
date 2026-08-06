#pragma once

#include <juce_core/juce_core.h>

#include "Core/MIDI/SysEx/SysExConstants.h"

namespace Core
{
// True for a Universal Device Inquiry *identity reply* long enough to validate as a
// Matrix-family candidate (rejects inquiry request echo F0 7E .. 06 01 .. and other SysEx).
inline bool isDeviceInquiryIdentityReply(const juce::MemoryBlock& sysex) noexcept
{
    if (sysex.getSize() < 15)
        return false;

    const auto* data = static_cast<const juce::uint8*>(sysex.getData());
    const auto last = static_cast<size_t>(sysex.getSize() - 1);

    return data[0] == SysExConstants::kSysExStart
           && data[last] == SysExConstants::kSysExEnd
           && data[1] == SysExConstants::DeviceInquiry::kUniversalNonRealtimeId
           && data[3] == SysExConstants::DeviceInquiry::kSubIdGeneralInfo
           && data[4] == SysExConstants::DeviceInquiry::kSubIdDeviceIdReply
           && data[5] == SysExConstants::DeviceInquiry::kExpectedManufacturer
           && data[6] == SysExConstants::DeviceInquiry::kExpectedFamily
           && data[7] == SysExConstants::DeviceInquiry::kExpectedFamilyHigh;
}
} // namespace Core
