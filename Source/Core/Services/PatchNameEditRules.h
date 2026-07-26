#pragma once

#include <juce_core/juce_core.h>

namespace Core
{

    // Small, pure commit rule extracted from the Patch Name inline editor so it is
    // unit-testable without a Component: an empty or all-spaces edit keeps the previous
    // name instead of committing a blank patch name.
    struct PatchNameEditRules
    {
        static juce::String resolveCommittedPatchName(const juce::String& editedName,
                                                       const juce::String& previousName);
    };

} // namespace Core
