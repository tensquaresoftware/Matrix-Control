#include "MidiManager.h"
#include "Utilities/MidiLogger.h"
#include <thread>

MidiManager::MidiManager(juce::AudioProcessorValueTreeState& apvtsRef)
    : juce::Thread("MidiManager")
    , apvts(apvtsRef)
    , inputMidiPort(std::make_unique<MidiInputPort>())
    , outputMidiPort(std::make_unique<MidiOutputPort>())
    , midiSender(std::make_unique<MidiSender>())
    , midiReceiver(std::make_unique<MidiReceiver>())
    , sysExParser(std::make_unique<SysExParser>())
    , sysExDecoder(std::make_unique<SysExDecoder>(*sysExParser))
    , sysExEncoder(std::make_unique<SysExEncoder>())
{
    // Initialize APVTS properties for MIDI status
    this->apvts.state.setProperty("deviceDetected", false, nullptr);
    this->apvts.state.setProperty("deviceVersion", juce::String(), nullptr);
    this->apvts.state.setProperty("lastError", juce::String(), nullptr);
    this->apvts.state.setProperty("errorType", juce::String(), nullptr);
    this->apvts.state.setProperty("lastPatchLoaded", juce::String(), nullptr);
    
    MidiLogger::getInstance().logInfo("MidiManager initialized");
}

MidiManager::~MidiManager()
{
    stopThread(5000);  // Wait up to 5 seconds for thread to stop
    
    // Stop MIDI input callbacks before closing ports
    // This ensures no callbacks are executing when we destroy midiReceiver
    if (inputMidiPort != nullptr && midiReceiver != nullptr)
    {
        auto* midiInput = inputMidiPort->getMidiInput();
        if (midiInput != nullptr)
        {
            midiInput->stop();
            // Clear callback reference before closing port
            midiReceiver->setMidiInput(nullptr);
            // Give time for any in-flight callbacks to complete
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        inputMidiPort->closePort();
    }
    
    if (outputMidiPort != nullptr)
    {
        outputMidiPort->closePort();
    }
}

bool MidiManager::setMidiInputPort(const juce::String& deviceId)
{
    MidiLogger::getInstance().logInfo("Setting MIDI input port: " + deviceId);
    if (inputMidiPort->openPort(deviceId, midiReceiver.get()))
    {
        midiReceiver->setMidiInput(inputMidiPort->getMidiInput());
        MidiLogger::getInstance().logInfo("MIDI input port successfully set");
        return true;
    }
    MidiLogger::getInstance().logError("Failed to set MIDI input port");
    return false;
}

bool MidiManager::setMidiOutputPort(const juce::String& deviceId)
{
    MidiLogger::getInstance().logInfo("Setting MIDI output port: " + deviceId);
    if (outputMidiPort->openPort(deviceId))
    {
        midiSender->setMidiOutput(outputMidiPort->getMidiOutput());
        MidiLogger::getInstance().logInfo("MIDI output port successfully set");
        return true;
    }
    MidiLogger::getInstance().logError("Failed to set MIDI output port");
    return false;
}

void MidiManager::sendPatch(uint8_t patchNumber, const uint8_t* packedData)
{
    if (packedData == nullptr)
    {
        updateErrorState("Invalid patch data", "SysEx");
        return;
    }

    try
    {
        auto sysExMessage = sysExEncoder->encodePatchSysEx(patchNumber, packedData);
        midiSender->sendSysEx(sysExMessage);
        
        MidiLogger::getInstance().logSysExSent(sysExMessage, 
            "Patch #" + juce::String(patchNumber));
        
        // Minimum delay between SysEx messages
        std::this_thread::sleep_for(std::chrono::milliseconds(SysExConstants::kMinSysExDelayMs));
    }
    catch (const MidiConnectionException& e)
    {
        updateErrorState(e.getMessage(), "Connection");
    }
    catch (const std::exception& e)
    {
        updateErrorState(e.what(), "SysEx");
    }
}

void MidiManager::sendMaster(uint8_t version, const uint8_t* packedData)
{
    if (packedData == nullptr)
    {
        updateErrorState("Invalid master data", "SysEx");
        return;
    }

    try
    {
        auto sysExMessage = sysExEncoder->encodeMasterSysEx(version, packedData);
        midiSender->sendSysEx(sysExMessage);
        
        MidiLogger::getInstance().logSysExSent(sysExMessage, "Master parameters");
        
        // Minimum delay between SysEx messages
        std::this_thread::sleep_for(std::chrono::milliseconds(SysExConstants::kMinSysExDelayMs));
    }
    catch (const MidiConnectionException& e)
    {
        updateErrorState(e.getMessage(), "Connection");
    }
    catch (const std::exception& e)
    {
        updateErrorState(e.what(), "SysEx");
    }
}

void MidiManager::sendProgramChange(uint8_t programNumber, int channel)
{
    try
    {
        midiSender->sendProgramChange(programNumber, channel);
        MidiLogger::getInstance().logProgramChange(programNumber, "SENT");
    }
    catch (const MidiConnectionException& e)
    {
        updateErrorState(e.getMessage(), "Connection");
    }
}

std::vector<uint8_t> MidiManager::requestCurrentPatch()
{
    try
    {
        // Send request message
        auto requestMessage = sysExEncoder->encodeRequestMessage(
            SysExConstants::RequestType::kRequestEditBuffer, 0);
        midiSender->sendSysEx(requestMessage);

        // Wait for response with timeout
        auto response = midiReceiver->waitForSysExResponse(SysExConstants::kDefaultTimeoutMs);
        
        if (response.getSize() == 0)
        {
            updateErrorState("Timeout waiting for patch response", "Timeout");
            return {};
        }

        MidiLogger::getInstance().logSysExReceived(response, "Patch response");

        // Decode response
        std::vector<uint8_t> packedData(SysExConstants::kPatchPackedDataSize);
        if (sysExDecoder->decodePatchSysEx(response, packedData.data()))
        {
            return packedData;
        }
        else
        {
            updateErrorState("Failed to decode patch response", "SysEx");
            return {};
        }
    }
    catch (const MidiConnectionException& e)
    {
        updateErrorState(e.getMessage(), "Connection");
        return {};
    }
    catch (const std::exception& e)
    {
        updateErrorState(e.what(), "SysEx");
        return {};
    }
}

std::vector<uint8_t> MidiManager::requestMasterData()
{
    try
    {
        // Send request message
        auto requestMessage = sysExEncoder->encodeRequestMessage(
            SysExConstants::RequestType::kRequestMasterParameters, 0);
        midiSender->sendSysEx(requestMessage);

        // Wait for response with timeout
        auto response = midiReceiver->waitForSysExResponse(SysExConstants::kDefaultTimeoutMs);
        
        if (response.getSize() == 0)
        {
            updateErrorState("Timeout waiting for master response", "Timeout");
            return {};
        }

        MidiLogger::getInstance().logSysExReceived(response, "Master response");

        // Decode response
        std::vector<uint8_t> packedData(SysExConstants::kMasterPackedDataSize);
        if (sysExDecoder->decodeMasterSysEx(response, packedData.data()))
        {
            return packedData;
        }
        else
        {
            updateErrorState("Failed to decode master response", "SysEx");
            return {};
        }
    }
    catch (const MidiConnectionException& e)
    {
        updateErrorState(e.getMessage(), "Connection");
        return {};
    }
    catch (const std::exception& e)
    {
        updateErrorState(e.what(), "SysEx");
        return {};
    }
}

bool MidiManager::performDeviceInquiry()
{
    try
    {
        // Send Device Inquiry request
        auto inquiryMessage = SysExEncoder::encodeDeviceInquiry();
        midiSender->sendSysEx(inquiryMessage);

        // Wait for Device ID response with timeout
        auto response = midiReceiver->waitForSysExResponse(SysExConstants::kDefaultTimeoutMs);
        
        if (response.getSize() == 0)
        {
            updateDeviceStatus(false);
            updateErrorState("Timeout waiting for Device ID response", "Timeout");
            return false;
        }

        MidiLogger::getInstance().logSysExReceived(response, "Device ID response");

        // Decode Device ID
        DeviceIdInfo deviceInfo = sysExDecoder->decodeDeviceId(response);
        
        if (deviceInfo.isValid)
        {
            updateDeviceStatus(true, deviceInfo.version);
            return true;
        }
        else
        {
            updateDeviceStatus(false);
            updateErrorState("Connected device is not a Matrix-1000", "Device");
            return false;
        }
    }
    catch (const MidiConnectionException& e)
    {
        updateDeviceStatus(false);
        updateErrorState(e.getMessage(), "Connection");
        return false;
    }
    catch (const std::exception& e)
    {
        updateDeviceStatus(false);
        updateErrorState(e.what(), "SysEx");
        return false;
    }
}

void MidiManager::run()
{
    while (!threadShouldExit())
    {
        // Check for incoming SysEx messages (non-blocking)
        // MidiReceiver handles this via callback, but we can check for completed messages
        // For now, the main work is done in request methods which call waitForSysExResponse
        
        // Sleep to avoid busy-waiting
        wait(SysExConstants::kMinSysExDelayMs);
    }
}

void MidiManager::updateErrorState(const juce::String& errorMessage, const juce::String& errorType)
{
    apvts.state.setProperty("lastError", errorMessage, nullptr);
    apvts.state.setProperty("errorType", errorType, nullptr);
    
    MidiLogger::getInstance().logError(errorMessage);
}

void MidiManager::updateDeviceStatus(bool detected, const juce::String& version)
{
    apvts.state.setProperty("deviceDetected", detected, nullptr);
    apvts.state.setProperty("deviceVersion", version, nullptr);
    
    if (detected)
    {
        MidiLogger::getInstance().logInfo("Matrix-1000 detected. Version: " + version);
    }
    else
    {
        MidiLogger::getInstance().logWarning("Matrix-1000 not detected");
    }
}

void MidiManager::handleIncomingSysEx(const juce::MemoryBlock& sysEx)
{
    // Validate message
    auto validation = sysExParser->validateSysEx(sysEx);
    
    if (!validation.isValid)
    {
        updateErrorState(validation.errorMessage, "SysEx");
        return;
    }

    // Handle different message types
    switch (validation.messageType)
    {
        case SysExParser::MessageType::kPatch:
            // Patch received - could update APVTS here if needed
            break;
        case SysExParser::MessageType::kMaster:
            // Master received - could update APVTS here if needed
            break;
        case SysExParser::MessageType::kDeviceId:
            // Device ID - handled in performDeviceInquiry
            break;
        case SysExParser::MessageType::kUnknown:
            // Unknown message type - already handled by validation
            break;
        case SysExParser::MessageType::kSplitPatch:
            // Matrix-6 compatibility split patch - ignore
            break;
        default:
            break;
    }
}

