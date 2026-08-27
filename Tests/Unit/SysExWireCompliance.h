#pragma once

#include <juce_core/juce_core.h>

#include "Core/MIDI/SysEx/SysExConstants.h"

namespace SysExWireCompliance
{

inline bool assertValidSysExDataBytes(const juce::MemoryBlock& block) noexcept
{
    if (block.getSize() < SysExConstants::kMinSysExEnvelopeLength)
        return false;

    const auto* data = static_cast<const juce::uint8*>(block.getData());
    if (data[0] != SysExConstants::kSysExStart)
        return false;
    if (data[block.getSize() - 1] != SysExConstants::kSysExEnd)
        return false;

    for (size_t i = 1; i < block.getSize() - 1; ++i)
    {
        if ((data[i] & 0x80) != 0)
            return false;
    }

    return true;
}

inline bool assertRemoteEditMatches(const juce::MemoryBlock& block,
                                    juce::uint8 expectedParam,
                                    juce::uint8 expectedWireValue) noexcept
{
    if (block.getSize() != 7)
        return false;

    const auto* data = static_cast<const juce::uint8*>(block.getData());
    return data[0] == SysExConstants::kSysExStart
        && data[1] == SysExConstants::kManufacturerIdOberheim
        && data[2] == SysExConstants::kDeviceIdMatrix1000
        && data[3] == SysExConstants::Opcode::kRemoteParameterEdit
        && data[4] == static_cast<juce::uint8>(expectedParam & 0x7F)
        && data[5] == static_cast<juce::uint8>(expectedWireValue & 0x7F)
        && data[6] == SysExConstants::kSysExEnd;
}

struct MatrixModBusWireExpectation
{
    juce::uint8 bus;
    juce::uint8 source;
    juce::uint8 amount;
    juce::uint8 destination;
};

inline bool assertMatrixModBusEditMatches(const juce::MemoryBlock& block,
                                          const MatrixModBusWireExpectation& expected) noexcept
{
    if (block.getSize() != SysExConstants::kMatrixModBusMessageLength)
        return false;

    const auto* data = static_cast<const juce::uint8*>(block.getData());
    return data[0] == SysExConstants::kSysExStart
        && data[1] == SysExConstants::kManufacturerIdOberheim
        && data[2] == SysExConstants::kDeviceIdMatrix1000
        && data[3] == SysExConstants::Opcode::kRemoteParameterEditMatrix
        && data[4] == static_cast<juce::uint8>(expected.bus & 0x7F)
        && data[5] == static_cast<juce::uint8>(expected.source & 0x7F)
        && data[6] == static_cast<juce::uint8>(expected.amount & 0x7F)
        && data[7] == static_cast<juce::uint8>(expected.destination & 0x7F)
        && data[8] == SysExConstants::kSysExEnd;
}

inline bool assertMasterMessageMatches(const juce::MemoryBlock& block,
                                       juce::uint8 expectedVersion) noexcept
{
    if (block.getSize() != SysExConstants::kMasterMessageLength)
        return false;

    const auto* data = static_cast<const juce::uint8*>(block.getData());
    return data[0] == SysExConstants::kSysExStart
        && data[1] == SysExConstants::kManufacturerIdOberheim
        && data[2] == SysExConstants::kDeviceIdMatrix1000
        && data[3] == SysExConstants::Opcode::kMasterParameterData
        && data[4] == static_cast<juce::uint8>(expectedVersion & 0x7F)
        && data[block.getSize() - 1] == SysExConstants::kSysExEnd;
}

} // namespace SysExWireCompliance
