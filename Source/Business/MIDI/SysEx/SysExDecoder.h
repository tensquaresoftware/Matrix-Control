#pragma once

#include <vector>

#include <juce_core/juce_core.h>

#include "SysExParser.h"

struct DeviceIdInfo
{
    uint8_t manufacturerId;
    uint8_t familyLow;
    uint8_t familyHigh;
    uint8_t memberLow;
    uint8_t memberHigh;
    juce::String version;
    bool isValid;
};

class SysExDecoder
{
public:
    explicit SysExDecoder(SysExParser& parserRef);
    ~SysExDecoder() = default;

    bool decodePatchSysEx(const juce::MemoryBlock& sysEx, uint8_t* output) const;
    bool decodeMasterSysEx(const juce::MemoryBlock& sysEx, uint8_t* output) const;
    DeviceIdInfo decodeDeviceId(const juce::MemoryBlock& sysEx) const;
    static size_t packNibbles(const uint8_t* nibbles, size_t numNibbles, uint8_t* output);

private:
    SysExParser& parser;

    bool extractPackedData(const juce::MemoryBlock& sysEx,
                          size_t dataStartIndex,
                          size_t expectedPackedSize,
                          uint8_t* output) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SysExDecoder)
};

