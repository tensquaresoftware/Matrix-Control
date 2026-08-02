#pragma once

#include <cstddef>
#include <optional>

#include <juce_core/juce_core.h>

namespace Core
{
    // Hysteresis for realtime outbound-queue pressure alerts (PANIC look + centre footer).
    // Defaults match spec-bug-midi-01-residual-panic-alert Design Notes.
    class RealtimeQueuePressureMonitor
    {
    public:
        static constexpr std::size_t kEngageDepth = 32;
        static constexpr std::size_t kClearDepth = 4;
        static constexpr int kSettleMs = 250;
        static constexpr int kEngageTicks = 2; // ~2× HeaderRefreshTimer @ 30 Hz ≈ sustained

        bool isAlertActive() const noexcept { return alertActive_; }

        // Returns true when alertActive_ changed.
        bool update(std::size_t realtimeDepth, juce::uint32 nowMs) noexcept
        {
            if (alertActive_)
            {
                engageStreak_ = 0;

                if (realtimeDepth > kClearDepth)
                {
                    belowClearSinceMs_.reset();
                    return false;
                }

                if (! belowClearSinceMs_.has_value())
                    belowClearSinceMs_ = nowMs;

                if (nowMs - *belowClearSinceMs_ < static_cast<juce::uint32>(kSettleMs))
                    return false;

                alertActive_ = false;
                belowClearSinceMs_.reset();
                return true;
            }

            belowClearSinceMs_.reset();

            if (realtimeDepth < kEngageDepth)
            {
                engageStreak_ = 0;
                return false;
            }

            ++engageStreak_;
            if (engageStreak_ < kEngageTicks)
                return false;

            alertActive_ = true;
            engageStreak_ = 0;
            return true;
        }

    private:
        bool alertActive_ = false;
        int engageStreak_ = 0;
        std::optional<juce::uint32> belowClearSinceMs_;
    };
}
