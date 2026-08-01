// Extracted from MidiManager.cpp for modular maintenance.
// Outbound queue consumer thread and SysEx pacing.

#include "MidiManager.h"

#include "Core/Loggers/MidiLogger.h"
#include "Core/MIDI/EditorOutboundGate.h"
#include "Core/Services/DeviceTypeRegistry.h"
#include "Shared/Definitions/MatrixDeviceTypes.h"

namespace
{
    Core::MidiActivityTracker::Path pathForOutboundMessage(
        const Core::MidiOutboundQueue::Message& msg) noexcept
    {
        if (msg.category == Core::MidiOutboundQueue::MessageCategory::kSysEx)
            return Core::MidiActivityTracker::Path::kEditor;

        if (msg.midiMessage.isProgramChange())
            return Core::MidiActivityTracker::Path::kEditor;

        return Core::MidiActivityTracker::Path::kInstrument;
    }
}

void MidiManager::wakeConsumer() noexcept
{
    notify();
}

bool MidiManager::canSendSysExNow() const noexcept
{
    const auto nowMs = static_cast<juce::int64>(juce::Time::getMillisecondCounterHiRes());
    return sysExDelay_.millisUntilNextAllowed(nowMs) == 0;
}

bool MidiManager::isOutboundQueueIdle() const noexcept
{
    return outboundQueue_.isEmpty() && ! hasPendingSysEx_.load(std::memory_order_acquire);
}

bool MidiManager::hasOutboundIdleTimedOut(const OutboundIdlePollArgs& args) const noexcept
{
    return juce::Time::getMillisecondCounter() - args.idleStartMs
           >= static_cast<juce::uint32>(args.outboundIdleTimeoutMs);
}

void MidiManager::sendSysExWithDelay(const juce::MemoryBlock& sysExMessage, const juce::String& description)
{
    sysExDelay_.waitUntilReady();
    midiSender->sendSysEx(sysExMessage);
    MidiLogger::getInstance().logSysExSent(sysExMessage, description);
    sysExDelay_.recordSysExSent(static_cast<juce::int64>(juce::Time::getMillisecondCounterHiRes()));
}

void MidiManager::sendQueuedSysEx(const juce::MemoryBlock& sysExMessage, const juce::String& description)
{
    midiSender->sendSysEx(sysExMessage);
    MidiLogger::getInstance().logSysExSent(sysExMessage, description);
    sysExDelay_.recordSysExSent(static_cast<juce::int64>(juce::Time::getMillisecondCounterHiRes()));
}

void MidiManager::dispatchRealtimeMessage(const Core::MidiOutboundQueue::Message& msg)
{
    midiSender->sendMidiMessage(msg.midiMessage);
    activityTracker_.notifyActivity(pathForOutboundMessage(msg));
    activityTracker_.notifyActivity(Core::MidiActivityTracker::Path::kOutbound);
}

void MidiManager::notifyOutboundActivity(const Core::MidiOutboundQueue::Message& msg)
{
    activityTracker_.notifyActivity(pathForOutboundMessage(msg));
    activityTracker_.notifyActivity(Core::MidiActivityTracker::Path::kOutbound);
}

bool MidiManager::isEditorSysExAllowed(const juce::MemoryBlock& sysEx) const
{
    const bool deviceDetected = static_cast<bool>(apvts.state.getProperty("deviceDetected", false));
    const auto deviceType = Core::DeviceTypeRegistry::fromApvtsProperty(
        apvts.state.getProperty(MatrixDeviceTypes::kApvtsPropertyName));
    return Core::maySendEditorSysEx(deviceDetected, deviceType, sysEx);
}

bool MidiManager::handleOutboundMessage(Core::MidiOutboundQueue::Message& msg)
{
    if (msg.category == Core::MidiOutboundQueue::MessageCategory::kRealtime)
    {
        dispatchRealtimeMessage(msg);
        return true;
    }

    if (msg.sysExData.getSize() == 0)
        return true;

    if (! isEditorSysExAllowed(msg.sysExData))
        return true;

    if (pendingSysEx_.has_value())
    {
        outboundQueue_.enqueueSysEx(std::move(msg.sysExData));
        return false;
    }

    if (canSendSysExNow())
    {
        sendQueuedSysEx(msg.sysExData, "QUEUED");
        notifyOutboundActivity(msg);
        return true;
    }

    pendingSysEx_ = std::move(msg);
    return false;
}

bool MidiManager::tryDispatchPendingSysEx()
{
    if (! pendingSysEx_.has_value() || ! canSendSysExNow())
        return false;

    try
    {
        if (! isEditorSysExAllowed(pendingSysEx_->sysExData))
        {
            pendingSysEx_.reset();
            return false;
        }

        sendQueuedSysEx(pendingSysEx_->sysExData, "QUEUED");
        notifyOutboundActivity(*pendingSysEx_);
        pendingSysEx_.reset();
        return true;
    }
    catch (const MidiConnectionException& e)
    {
        updateErrorState(e.getMessage(), "Connection");
    }
    catch (const std::exception& e)
    {
        updateErrorState(e.what(), "MIDI");
    }

    return false;
}

bool MidiManager::processOutboundQueue()
{
    bool didWork = false;

    while (true)
    {
        auto msg = outboundQueue_.dequeue();
        if (! msg.has_value())
            break;

        didWork = true;

        try
        {
            if (! handleOutboundMessage(*msg))
                break;
        }
        catch (const MidiConnectionException& e)
        {
            updateErrorState(e.getMessage(), "Connection");
            break;
        }
        catch (const std::exception& e)
        {
            updateErrorState(e.what(), "MIDI");
            break;
        }
    }

    if (tryDispatchPendingSysEx())
        didWork = true;

    hasPendingSysEx_.store(pendingSysEx_.has_value(), std::memory_order_release);
    return didWork;
}

void MidiManager::run()
{
    while (! threadShouldExit())
    {
        if (! midiSender->isOutputAvailable())
        {
            wait(1);
            continue;
        }

        if (processOutboundQueue())
            continue;

        int sleepMs = 1;
        if (pendingSysEx_.has_value())
        {
            const auto nowMs = static_cast<juce::int64>(juce::Time::getMillisecondCounterHiRes());
            sleepMs = juce::jmax(1, sysExDelay_.millisUntilNextAllowed(nowMs));
        }

        wait(sleepMs);
    }
}
