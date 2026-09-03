#pragma once

#include <juce_core/juce_core.h>

namespace PatchTestFixtures
{
    inline juce::File patchFixturesRoot()
    {
        return juce::File(MATRIX_TEST_FIXTURES_DIR).getChildFile("Patches");
    }

    // Resolves user patch fixtures under Patches/User/ (after Short List flatten).
    inline juce::File resolvePatchFixtureFile(const juce::String& fileName)
    {
        const auto root = patchFixturesRoot();
        const auto userFile = root.getChildFile("User").getChildFile(fileName);
        if (userFile.existsAsFile())
            return userFile;

        const auto legacy = root.getChildFile(fileName);
        if (legacy.existsAsFile())
            return legacy;

        return userFile;
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
