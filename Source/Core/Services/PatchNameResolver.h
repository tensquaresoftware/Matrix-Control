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

        struct ResolveArgs
        {
            juce::String deviceName;
            int bank = 0;
            int patchNumber = 0;
            bool isRomBank = false;
            juce::String factoryName;
            juce::String overlayName;
            Mode mode = Mode::kMusical;
        };

        static juce::String resolve(const ResolveArgs& args);
    };

} // namespace Core
