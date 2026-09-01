#pragma once

#include <juce_core/juce_core.h>

namespace PatchTestFixtures
{
    inline juce::File patchFixturesRoot()
    {
        return juce::File(MATRIX_TEST_FIXTURES_DIR).getChildFile("Patches");
    }

    // Resolves user patch fixtures after the Patches/ tree reshuffle (User/Short List/).
    inline juce::File resolvePatchFixtureFile(const juce::String& fileName)
    {
        const auto root = patchFixturesRoot();
        const auto userShort = root.getChildFile("User").getChildFile("Short List").getChildFile(fileName);
        if (userShort.existsAsFile())
            return userShort;

        const auto legacy = root.getChildFile(fileName);
        if (legacy.existsAsFile())
            return legacy;

        return userShort;
    }

    // Resolves a path under Tests/Fixtures/, preferring Factory/ over deleted ROM/ copies.
    inline juce::File resolveFixtureFile(const juce::String& relativePath)
    {
        const auto direct = juce::File(MATRIX_TEST_FIXTURES_DIR).getChildFile(relativePath);
        if (direct.existsAsFile())
            return direct;

        if (relativePath.contains("Patches/ROM/"))
        {
            const auto factoryPath = relativePath.replace("Patches/ROM/", "Patches/Factory/");
            const auto factoryFile = juce::File(MATRIX_TEST_FIXTURES_DIR).getChildFile(factoryPath);
            if (factoryFile.existsAsFile())
                return factoryFile;
        }

        return direct;
    }
}
