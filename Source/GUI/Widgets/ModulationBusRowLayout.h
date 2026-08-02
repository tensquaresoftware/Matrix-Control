#pragma once

// Shared integer column strip for Matrix Mod bus header labels and bus cell controls.
// Successive scaledInt widths + gaps — no float step accumulation across columns.
// Destination design width differs: header text 68 vs cell combo 104 (same destination X origin
// when preceding columns match).

#include "GUI/Layout/ScaledLayout.h"

namespace TSS
{
struct ModulationBusColumnStrip
{
    int busNumberX = 0;
    int busNumberW = 0;
    int sourceX = 0;
    int sourceW = 0;
    int amountX = 0;
    int amountW = 0;
    int destinationX = 0;
    int destinationW = 0;
    int initX = 0;
    int initW = 0;
};

struct ModulationBusColumnDesignWidths
{
    int busNumberW = 0;
    int sourceW = 0;
    int amountW = 0;
    int destinationW = 0;
    int initW = 0;
    int gap = 0;
};

inline ModulationBusColumnStrip computeModulationBusColumnStrip(
    float uiScale,
    const ModulationBusColumnDesignWidths& design)
{
    ModulationBusColumnStrip strip;
    const int gap = ScaledLayout::scaledInt(static_cast<float>(design.gap), uiScale);

    strip.busNumberW = ScaledLayout::scaledInt(static_cast<float>(design.busNumberW), uiScale);
    strip.sourceW = ScaledLayout::scaledInt(static_cast<float>(design.sourceW), uiScale);
    strip.amountW = ScaledLayout::scaledInt(static_cast<float>(design.amountW), uiScale);
    strip.destinationW = ScaledLayout::scaledInt(static_cast<float>(design.destinationW), uiScale);
    strip.initW = ScaledLayout::scaledInt(static_cast<float>(design.initW), uiScale);

    int x = 0;
    strip.busNumberX = x;
    x += strip.busNumberW + gap;
    strip.sourceX = x;
    x += strip.sourceW + gap;
    strip.amountX = x;
    x += strip.amountW + gap;
    strip.destinationX = x;
    x += strip.destinationW + gap;
    strip.initX = x;

    return strip;
}
}
