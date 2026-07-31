#pragma once

#include <juce_graphics/juce_graphics.h>

namespace TSS::TextFitHelpers
{
    // ASCII-only ellipsis so bitmap/UI fonts never emit U+2026 (avoids mojibake like "â€¦").
    inline constexpr const char* kAsciiEllipsis = "...";

    inline float measureWidth(const juce::Font& font, const juce::String& text)
    {
        return juce::GlyphArrangement::getStringWidth(font, text);
    }

    // Truncates at juce::String character boundaries (UTF-16 code units / safe for BMP paths).
    // middle=true keeps start and end (paths); middle=false keeps the start (status messages).
    inline juce::String fitWithAsciiEllipsis(const juce::String& text,
                                            const juce::Font& font,
                                            float maxWidth,
                                            bool middle = false)
    {
        if (text.isEmpty() || maxWidth <= 0.0f)
            return {};

        if (measureWidth(font, text) <= maxWidth)
            return text;

        const juce::String ellipsis(kAsciiEllipsis);
        const float ellipsisWidth = measureWidth(font, ellipsis);

        if (ellipsisWidth >= maxWidth)
            return ellipsis;

        const int length = text.length();

        if (! middle)
        {
            int lo = 0;
            int hi = length;

            while (lo < hi)
            {
                const int mid = (lo + hi + 1) / 2;
                if (measureWidth(font, text.substring(0, mid) + ellipsis) <= maxWidth)
                    lo = mid;
                else
                    hi = mid - 1;
            }

            return lo > 0 ? text.substring(0, lo) + ellipsis : ellipsis;
        }

        // Path-style: keep roughly 1/3 head + 2/3 tail so the leaf folder stays readable.
        int bestHead = 0;
        int bestTail = 0;
        int lo = 0;
        int hi = length - 1;

        while (lo <= hi)
        {
            const int keep = (lo + hi) / 2;
            const int head = keep / 3;
            const int tail = keep - head;
            const auto candidate = text.substring(0, head) + ellipsis + text.substring(length - tail);

            if (measureWidth(font, candidate) <= maxWidth)
            {
                bestHead = head;
                bestTail = tail;
                lo = keep + 1;
            }
            else
            {
                hi = keep - 1;
            }
        }

        if (bestHead + bestTail <= 0)
            return ellipsis;

        return text.substring(0, bestHead) + ellipsis + text.substring(length - bestTail);
    }
}
