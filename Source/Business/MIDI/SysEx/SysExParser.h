#pragma once

#include <juce_core/juce_core.h>

#include "../Exceptions/Exceptions.h"
#include "SysExConstants.h"

class SysExParser
{
public:
    enum class MessageType
    {
        kUnknown,
        kPatch,
        kMaster,
        kDeviceId,
        kSplitPatch
    };

    struct ValidationResult
    {
        bool isValid;
        MessageType messageType;
        juce::String errorMessage;

        static ValidationResult success(MessageType type)
        {
            return { true, type, {} };
        }

        static ValidationResult failure(const juce::String& error)
        {
            return { false, MessageType::kUnknown, error };
        }
    };

    SysExParser() = default;
    ~SysExParser() = default;

    ValidationResult validateSysEx(const juce::MemoryBlock& sysEx) const;
    bool validateStructure(const juce::MemoryBlock& sysEx) const;
    bool validateChecksum(const juce::MemoryBlock& sysEx) const;
    ValidationResult validateMessageType(const juce::MemoryBlock& sysEx) const;
    static uint8_t calculateChecksum(const uint8_t* data, size_t length);
    static size_t packNibbles(const uint8_t* nibbles, size_t numNibbles, uint8_t* output);

private:
    bool validateManufacturerAndDevice(const juce::MemoryBlock& sysEx) const;
    static MessageType getMessageTypeFromOpcode(uint8_t opcode);
    static bool isDeviceInquiryMessage(const uint8_t* data);
    static std::vector<uint8_t> packNibblesToBytes(const uint8_t* nibbles, size_t numNibbles);
    static bool isDeviceIdResponse(const uint8_t* data);
    static bool isOberheimMatrix1000Message(const uint8_t* data);
    static size_t getExpectedMessageLength(MessageType messageType);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SysExParser)
};

