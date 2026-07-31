#pragma once

#include <functional>

#include <juce_core/juce_core.h>

namespace Core
{
    // Bank Utility EXPORT/IMPORT progress modal contract, shared between PatchManagerActionHandler
    // (which drives it) and PluginProcessor/PluginEditor (which wire it to BankTransferProgressDialog).
    struct BankTransferProgressPresenter
    {
        // totalSteps is the number of steps (1-100). detail is typically an absolute folder path.
        // Leave the presenter default-constructed to run without any UI (e.g. unit tests).
        std::function<void(const juce::String& title,
                           const juce::String& message,
                           const juce::String& detail,
                           int totalSteps,
                           std::function<void()> onCancelRequested)>
            show;
        // Updates the active progress lane.
        std::function<void(int completedSteps)> update;
        // Freezes the primary lane at 100% and starts a second lane (import write / restore).
        std::function<void(const juce::String& message, int totalSteps)> beginSecondaryPhase;
        std::function<void(const juce::String& message)> setMessage;
        std::function<void(const juce::String& detail)> setDetail;
        std::function<void(bool enabled)> setCancelEnabled;
        std::function<void()> hide;
    };

} // namespace Core
