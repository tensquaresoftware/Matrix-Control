#pragma once

#include <functional>

#include <juce_core/juce_core.h>

namespace Core
{
    // Bank Utility EXPORT/IMPORT progress modal contract, shared between PatchManagerActionHandler
    // (which drives it) and PluginProcessor/PluginEditor (which wire it to BankTransferProgressDialog).
    // Standalone header (rather than nested in PatchManagerActionHandler) so PluginProcessor.h can
    // reference the type without pulling in the full handler header.
    struct BankTransferProgressPresenter
    {
        // totalSteps is the number of steps (1-100). onCancelRequested is invoked at most once;
        // leave the whole presenter default-constructed (all fields empty) to run without any UI
        // (e.g. unit tests).
        std::function<void(const juce::String& title,
                           const juce::String& message,
                           int totalSteps,
                           std::function<void()> onCancelRequested)>
            show;
        std::function<void(int completedSteps)> update;
        // Message-only update, used to change copy without moving the bar (e.g. "Restoring…").
        std::function<void(const juce::String& message)> setMessage;
        std::function<void(bool enabled)> setCancelEnabled;
        std::function<void()> hide;
    };

} // namespace Core
