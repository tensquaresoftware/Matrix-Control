#pragma once

#include <juce_core/juce_core.h>

namespace Core
{
    // Matrix-1000 factory patch names for ROM banks 2–9 (800 entries).
    // Source: _bmad-output/reference-docs/oberheim/oberheim-matrix-1000-factory-patch-names.md
    struct Matrix1000FactoryPatchNames
    {
        // Returns empty if bank is not ROM (0–1) or out of range.
        static juce::String nameFor(int bank, int patch) noexcept;
    };
}
