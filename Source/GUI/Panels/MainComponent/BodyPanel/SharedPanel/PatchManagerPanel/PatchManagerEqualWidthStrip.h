#pragma once

// Shared integer equal-width strip for Patch Manager modules (bank buttons, memory buttons, toggles).
// Successive scaledInt widths + gaps — no float step × index accumulation.
// Fixed-width policy (no remainder absorption) preserves design X at 100 %.

#include "GUI/Layout/ScaledLayout.h"

#include <juce_gui_basics/juce_gui_basics.h>

namespace TSS
{
struct EqualWidthStripArgs
{
    int startX = 0;
    int y = 0;
    float uiScale = 1.0f;
    int designWidth = 0;
    int designHeight = 0;
    int designGap = 0;
    juce::Component* const* controls = nullptr;
    int count = 0;
};

// Places equal-width controls left-to-right starting at startX.
// Returns X immediately after the last control (no trailing gap).
inline int placeEqualWidthStrip(const EqualWidthStripArgs& args)
{
    const int w = ScaledLayout::scaledInt(static_cast<float>(args.designWidth), args.uiScale);
    const int h = ScaledLayout::scaledInt(static_cast<float>(args.designHeight), args.uiScale);
    const int gap = ScaledLayout::scaledInt(static_cast<float>(args.designGap), args.uiScale);

    int x = args.startX;
    for (int i = 0; i < args.count; ++i)
    {
        if (args.controls[i] != nullptr)
            args.controls[i]->setBounds(x, args.y, w, h);

        x += w;
        if (i + 1 < args.count)
            x += gap;
    }

    return x;
}
}
