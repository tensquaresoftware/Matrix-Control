#pragma once

#include <juce_graphics/juce_graphics.h>

namespace TSS
{
    // Shared by ModuleHeader paint and unit tests (hover focus-red token).
    inline juce::Colour resolveModuleHeaderTitleTextColour(bool titleClickEnabled,
                                                           bool titleHovered,
                                                           juce::Colour text,
                                                           juce::Colour textFocus)
    {
        return titleClickEnabled && titleHovered ? textFocus : text;
    }
}
