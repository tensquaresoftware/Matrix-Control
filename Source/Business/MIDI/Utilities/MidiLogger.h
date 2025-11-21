#pragma once

#include <juce_core/juce_core.h>
#include <fstream>
#include <memory>
#include <mutex>

class MidiLogger
{
public:
    enum class LogLevel
    {
        kNone = 0,
        kError = 1,
        kWarning = 2,
        kInfo = 3,
        kDebug = 4,
        kVerbose = 5
    };

    static MidiLogger& getInstance();
    
    void setLogLevel(LogLevel level);
    void setLogToFile(bool enabled, const juce::File& logFile = juce::File());
    void setLogToConsole(bool enabled);
    
    void logMessage(LogLevel level, const juce::String& message);
    void logSysExSent(const juce::MemoryBlock& sysEx, const juce::String& description = "");
    void logSysExReceived(const juce::MemoryBlock& sysEx, const juce::String& description = "");
    void logProgramChange(uint8_t programNumber, const juce::String& direction = "");
    void logError(const juce::String& errorMessage);
    void logWarning(const juce::String& warningMessage);
    void logInfo(const juce::String& infoMessage);
    
    juce::String formatSysExMessage(const juce::MemoryBlock& sysEx) const;
    
private:
    MidiLogger() = default;
    ~MidiLogger() = default;
    MidiLogger(const MidiLogger&) = delete;
    MidiLogger& operator=(const MidiLogger&) = delete;
    
    void writeLog(const juce::String& formattedMessage);
    juce::String getTimestamp() const;
    juce::File getDefaultLogDirectory() const;
    juce::String generateTimestampedFilename() const;  // Returns "midi-log.txt" for now
    
    LogLevel currentLogLevel = LogLevel::kInfo;
    bool logToFile = false;
    bool logToConsole = true;
    juce::File logFile;
    std::unique_ptr<std::ofstream> fileStream;
    std::mutex logMutex;
    
    static constexpr const char* kLogLevelNames[] = {
        "NONE", "ERROR", "WARNING", "INFO", "DEBUG", "VERBOSE"
    };
};

