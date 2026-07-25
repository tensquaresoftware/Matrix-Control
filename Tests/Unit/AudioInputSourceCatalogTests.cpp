#include <juce_core/juce_core.h>

#include "Core/Audio/AudioInputSourceCatalog.h"

class AudioInputSourceCatalogTests : public juce::UnitTest
{
public:
    AudioInputSourceCatalogTests() : juce::UnitTest("AudioInputSourceCatalog") {}

    void runTest() override
    {
        testActiveChannelsUseCompactedIds();
        testActiveStereoPairsStepByTwo();
        testLowPairKeepsHardwareDisplayWithCompactedIds();
        testNonAdjacentActiveChannelsSkipBogusStereo();
    }

private:
    void testActiveChannelsUseCompactedIds()
    {
        beginTest("Non-zero-based active pair yields compacted mono/stereo ids");

        juce::BigInteger active;
        active.setBit(2);
        active.setBit(3);

        const auto entries = Core::AudioInputSourceCatalog::buildEntriesForActiveChannels(
            "Scarlett", active, 6);

        expectEquals(static_cast<int>(entries.size()), 3);
        expectEquals(entries[0].sourceId, juce::String("mono:0"));
        expectEquals(entries[1].sourceId, juce::String("mono:1"));
        expectEquals(entries[2].sourceId, juce::String("stereo:0"));
        expect(entries[0].displayName.contains("3"));
        expect(entries[1].displayName.contains("4"));
        expect(entries[2].displayName.contains("3/4"));
    }

    void testActiveStereoPairsStepByTwo()
    {
        beginTest("Active stereo entries step by two compacted channels");

        juce::BigInteger active;
        active.setBit(0);
        active.setBit(1);
        active.setBit(2);
        active.setBit(3);

        const auto entries = Core::AudioInputSourceCatalog::buildEntriesForActiveChannels(
            "Interface", active, 4);

        juce::StringArray stereoIds;
        for (const auto& entry : entries)
        {
            if (entry.sourceId.startsWith("stereo:"))
                stereoIds.add(entry.sourceId);
        }

        expectEquals(stereoIds.size(), 2);
        expectEquals(stereoIds[0], juce::String("stereo:0"));
        expectEquals(stereoIds[1], juce::String("stereo:2"));
        expect(! stereoIds.contains("stereo:1"));
    }

    void testLowPairKeepsHardwareDisplayWithCompactedIds()
    {
        beginTest("Channels 1/2 active still map to mono:0/1 and stereo:0");

        juce::BigInteger active;
        active.setBit(0);
        active.setBit(1);

        const auto entries = Core::AudioInputSourceCatalog::buildEntriesForActiveChannels(
            "Scarlett", active, 6);

        expectEquals(static_cast<int>(entries.size()), 3);
        expectEquals(entries[0].sourceId, juce::String("mono:0"));
        expectEquals(entries[1].sourceId, juce::String("mono:1"));
        expectEquals(entries[2].sourceId, juce::String("stereo:0"));
        expect(entries[0].displayName.contains("(1)"));
        expect(entries[1].displayName.contains("(2)"));
    }

    void testNonAdjacentActiveChannelsSkipBogusStereo()
    {
        beginTest("Non-adjacent active channels do not invent a stereo pair");

        juce::BigInteger active;
        active.setBit(0);
        active.setBit(2);

        const auto entries = Core::AudioInputSourceCatalog::buildEntriesForActiveChannels(
            "Interface", active, 4);

        expectEquals(static_cast<int>(entries.size()), 2);
        expectEquals(entries[0].sourceId, juce::String("mono:0"));
        expectEquals(entries[1].sourceId, juce::String("mono:1"));

        for (const auto& entry : entries)
            expect(! entry.sourceId.startsWith("stereo:"));
    }
};

static AudioInputSourceCatalogTests audioInputSourceCatalogTests;
