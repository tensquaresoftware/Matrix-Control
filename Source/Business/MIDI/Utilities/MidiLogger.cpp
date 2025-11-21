#include "MidiLogger.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

MidiLogger& MidiLogger::getInstance()
{
    static MidiLogger instance;
    return instance;
}

void MidiLogger::setLogLevel(LogLevel level)
{
    std::lock_guard<std::mutex> lock(logMutex);
    currentLogLevel = level;
}

juce::File MidiLogger::getDefaultLogDirectory() const
{
    // Use absolute path: /Volumes/Guillaume/Dev/Projects/MAO/Plugins/Matrix-Control/Tools/Logs/MIDI
    // This is more reliable than searching from current working directory
    return juce::File("/Volumes/Guillaume/Dev/Projects/MAO/Plugins/Matrix-Control/Tools/Logs/MIDI");
}

juce::String MidiLogger::generateTimestampedFilename() const
{
    // Simple fixed filename for now
    return juce::String("midi-log.txt");
}

void MidiLogger::setLogToFile(bool enabled, const juce::File& filePath)
{
    std::lock_guard<std::mutex> lock(logMutex);
    
    logToFile = enabled;
    if (enabled)
    {
        if (filePath.getFullPathName().isNotEmpty())
        {
            logFile = filePath;
        }
        else
        {
            // Use simple path: Tools/Logs/MIDI/midi-log.txt relative to current working directory
            juce::File logDir = getDefaultLogDirectory();
            juce::String filename = generateTimestampedFilename();
            logFile = logDir.getChildFile(filename);
        }
        
        // Create parent directory if it doesn't exist
        juce::File parentDir = logFile.getParentDirectory();
        parentDir.createDirectory();
        
        fileStream = std::make_unique<std::ofstream>(logFile.getFullPathName().toRawUTF8(), 
                                                       std::ios::app);
        
        if (fileStream && fileStream->is_open())
        {
            *fileStream << "\n=== MIDI Log Session Started ===\n";
            fileStream->flush();
        }
    }
    else
    {
        if (fileStream && fileStream->is_open())
        {
            *fileStream << "=== MIDI Log Session Ended ===\n\n";
            fileStream->flush();
            fileStream->close();
        }
        fileStream.reset();
    }
}

void MidiLogger::setLogToConsole(bool enabled)
{
    std::lock_guard<std::mutex> lock(logMutex);
    logToConsole = enabled;
}

void MidiLogger::logMessage(LogLevel level, const juce::String& message)
{
    if (level > currentLogLevel)
        return;
    
    juce::String formattedMessage = juce::String("[") + kLogLevelNames[static_cast<int>(level)] + "] "
                                    + getTimestamp() + " - " + message;
    
    writeLog(formattedMessage);
}

void MidiLogger::logSysExSent(const juce::MemoryBlock& sysEx, const juce::String& description)
{
    if (LogLevel::kDebug > currentLogLevel)
        return;
    
    juce::String message = "SENT SysEx";
    if (description.isNotEmpty())
        message += " (" + description + ")";
    message += ": " + formatSysExMessage(sysEx);
    
    logMessage(LogLevel::kDebug, message);
}

void MidiLogger::logSysExReceived(const juce::MemoryBlock& sysEx, const juce::String& description)
{
    if (LogLevel::kDebug > currentLogLevel)
        return;
    
    juce::String message = "RECEIVED SysEx";
    if (description.isNotEmpty())
        message += " (" + description + ")";
    message += ": " + formatSysExMessage(sysEx);
    
    logMessage(LogLevel::kDebug, message);
}

void MidiLogger::logProgramChange(uint8_t programNumber, const juce::String& direction)
{
    if (LogLevel::kInfo > currentLogLevel)
        return;
    
    juce::String message = "Program Change";
    if (direction.isNotEmpty())
        message += " " + direction;
    message += ": " + juce::String(programNumber);
    
    logMessage(LogLevel::kInfo, message);
}

void MidiLogger::logError(const juce::String& errorMessage)
{
    logMessage(LogLevel::kError, "ERROR: " + errorMessage);
}

void MidiLogger::logWarning(const juce::String& warningMessage)
{
    logMessage(LogLevel::kWarning, "WARNING: " + warningMessage);
}

void MidiLogger::logInfo(const juce::String& infoMessage)
{
    logMessage(LogLevel::kInfo, infoMessage);
}

juce::String MidiLogger::formatSysExMessage(const juce::MemoryBlock& sysEx) const
{
    if (sysEx.getSize() == 0)
        return "empty";
    
    juce::String hexString;
    const auto* data = static_cast<const juce::uint8*>(sysEx.getData());
    
    for (size_t i = 0; i < sysEx.getSize() && i < 256; ++i) // Limit display
    {
        hexString += juce::String::toHexString(data[i]).paddedLeft('0', 2).toUpperCase();
        if (i < sysEx.getSize() - 1)
            hexString += " ";
    }
    
    if (sysEx.getSize() > 256)
        hexString += " ... (truncated)";
    
    return hexString + " (" + juce::String(sysEx.getSize()) + " bytes)";
}

void MidiLogger::writeLog(const juce::String& formattedMessage)
{
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (logToConsole)
    {
        std::cout << formattedMessage.toRawUTF8() << std::endl;
    }
    
    if (logToFile && fileStream && fileStream->is_open())
    {
        *fileStream << formattedMessage.toRawUTF8() << std::endl;
        fileStream->flush();
    }
}

juce::String MidiLogger::getTimestamp() const
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::tm timeInfo = {};
    
    // Use thread-safe localtime_r on macOS/Linux (POSIX)
    #if defined(__APPLE__) || defined(__linux__) || defined(__unix__)
        localtime_r(&time, &timeInfo);
    #elif defined(_WIN32)
        localtime_s(&timeInfo, &time);
    #else
        // Fallback: use localtime (protected by mutex in writeLog)
        std::tm* tmPtr = std::localtime(&time);
        if (tmPtr != nullptr)
        {
            timeInfo = *tmPtr;
        }
    #endif
    
    std::stringstream ss;
    ss << std::put_time(&timeInfo, "%H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();
    
    return juce::String(ss.str());
}

