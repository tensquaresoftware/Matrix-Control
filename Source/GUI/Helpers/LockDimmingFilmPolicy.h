#pragma once

#include <vector>

#include <juce_gui_basics/juce_gui_basics.h>

#include "Core/MIDI/EditorOutboundGate.h"
#include "Shared/Definitions/MatrixDeviceTypes.h"

namespace TSS
{
    /** Hole set for the MainComponent lock dimming film (spec I/O matrix). */
    enum class LockDimmingFilmMode
    {
        kOff,                 // Device OK, Compare off — film hidden
        kHeaderFooterOnly,    // Device lock (and equivalents) — no COMPARE hole
        kHeaderFooterCompare  // Compare + supported device — Header + Footer + COMPARE
    };

    /**
     * Resolves film visibility and hole policy from APVTS-derived lock inputs.
     * Device lock wins over Compare (no COMPARE hole when both are true).
     */
    inline LockDimmingFilmMode resolveLockDimmingFilmMode(bool deviceDetected,
                                                          MatrixDeviceTypes::Type deviceType,
                                                          bool compareActive,
                                                          bool deviceMidiUnresponsive = false) noexcept
    {
        const bool deviceLocked = Core::isSectionLocked(
            deviceDetected, deviceType, false, deviceMidiUnresponsive);

        if (deviceLocked)
            return LockDimmingFilmMode::kHeaderFooterOnly;

        if (compareActive)
            return LockDimmingFilmMode::kHeaderFooterCompare;

        return LockDimmingFilmMode::kOff;
    }

    inline bool lockDimmingFilmIncludesCompareHole(LockDimmingFilmMode mode) noexcept
    {
        return mode == LockDimmingFilmMode::kHeaderFooterCompare;
    }

    inline bool lockDimmingFilmIsActive(LockDimmingFilmMode mode) noexcept
    {
        return mode != LockDimmingFilmMode::kOff;
    }

    /** True when the film should capture the point (outside all holes). */
    inline bool lockDimmingFilmBlocksPoint(const std::vector<juce::Rectangle<int>>& holes,
                                           int x,
                                           int y) noexcept
    {
        for (const auto& hole : holes)
        {
            if (hole.contains(x, y))
                return false;
        }

        return true;
    }
}
