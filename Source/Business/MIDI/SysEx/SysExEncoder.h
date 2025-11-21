#pragma once

#include <vector>

#include <juce_core/juce_core.h>

#include "SysExConstants.h"

class SysExEncoder
{
public:
    SysExEncoder() = default;
    ~SysExEncoder() = default;

    juce::MemoryBlock encodePatchSysEx(uint8_t patchNumber, const uint8_t* packedData) const;
    juce::MemoryBlock encodeMasterSysEx(uint8_t version, const uint8_t* packedData) const;
    static juce::MemoryBlock encodeDeviceInquiry();
    juce::MemoryBlock encodeRequestMessage(uint8_t requestType, uint8_t patchNumber = 0) const;
    static size_t unpackBytes(const uint8_t* bytes, size_t numBytes, uint8_t* output);
    static uint8_t calculateChecksum(const uint8_t* data, size_t length);

private:
    std::vector<uint8_t> buildHeader(uint8_t opcode, uint8_t headerData = 0) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SysExEncoder)
};

