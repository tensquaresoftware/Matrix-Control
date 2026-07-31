#pragma once

#include <juce_core/juce_core.h>

namespace Core
{

    // Pure Patch Name resolution for display (and musical export). Overlay / factory
    // lookups are passed in so callers own store / table access.
    struct PatchNameResolver
    {
        enum class Mode
        {
            kMusical,
            kHardware
        };

        static juce::String resolve(const juce::String& deviceName,
                                    int bank,
                                    int patchNumber,
                                    bool isRomBank,
                                    const juce::String& factoryName,
                                    const juce::String& overlayName,
                                    Mode mode);
    };

} // namespace Core
