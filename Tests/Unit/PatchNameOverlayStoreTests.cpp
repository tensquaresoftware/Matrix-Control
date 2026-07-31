#include <juce_core/juce_core.h>

#include "Core/Services/PatchNameOverlayStore.h"

class PatchNameOverlayStoreTests : public juce::UnitTest
{
public:
    PatchNameOverlayStoreTests()
        : juce::UnitTest("PatchNameOverlayStore")
    {
    }

    void runTest() override
    {
        beginTest("remember / lookup / forget");
        {
            Core::PatchNameOverlayStore store;
            store.remember(0, 3, "PAPANO 4");
            expectEquals(store.lookup(0, 3), juce::String("PAPANO 4"));
            expect(store.lookup(0, 4).isEmpty());
            store.forget(0, 3);
            expect(store.lookup(0, 3).isEmpty());
        }

        beginTest("clearBank removes only that bank");
        {
            Core::PatchNameOverlayStore store;
            store.remember(0, 1, "A");
            store.remember(1, 1, "B");
            store.clearBank(0);
            expect(store.lookup(0, 1).isEmpty());
            expectEquals(store.lookup(1, 1), juce::String("B"));
        }

        beginTest("ValueTree round-trip");
        {
            Core::PatchNameOverlayStore store;
            store.remember(0, 0, "PAPANO 4");
            store.remember(0, 1, "MIKPIANO");

            Core::PatchNameOverlayStore restored;
            restored.replaceFromValueTree(store.toValueTree());
            expectEquals(restored.lookup(0, 0), juce::String("PAPANO 4"));
            expectEquals(restored.lookup(0, 1), juce::String("MIKPIANO"));
        }

        beginTest("empty name forgets the slot");
        {
            Core::PatchNameOverlayStore store;
            store.remember(0, 2, "KEEP");
            store.remember(0, 2, "   ");
            expect(store.lookup(0, 2).isEmpty());
        }
    }
};

static PatchNameOverlayStoreTests patchNameOverlayStoreTests;
