#pragma once

#include <juce_core/juce_core.h>

namespace TSS
{
    // Footer display-only formatting for APVTS deviceVersion (Device Inquiry ASCII packs).
    // Does not mutate stored APVTS values.
    inline juce::String formatDeviceVersionForDisplay(const juce::String& rawVersion)
    {
        const auto cleaned = rawVersion.removeCharacters(" \t").toUpperCase();
        if (cleaned.isEmpty())
            return {};

        if (cleaned.containsChar('.') || cleaned.length() < 3)
            return cleaned;

        return cleaned.dropLastCharacters(2) + "." + cleaned.getLastCharacters(2);
    }
}
