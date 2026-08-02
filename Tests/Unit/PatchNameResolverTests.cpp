#include <juce_core/juce_core.h>

#include "Core/Services/PatchNameResolver.h"

class PatchNameResolverTests : public juce::UnitTest
{
public:
    PatchNameResolverTests() : juce::UnitTest("PatchNameResolver") {}

    void runTest() override
    {
        musical_romFactoryWinsOverBnk();
        musical_overlayWinsOnBnkWhenNoFactory();
        musical_keepsUsableNonBnkDump();
        musical_fallsBackToBxPyyWhenEmpty();
        hardware_keepsBnkWithoutOverlayOrFactory();
        hardware_fallsBackToBxPyyWhenEmpty();
        hardware_keepsUsableMusicalDump();
    }

private:
    void musical_romFactoryWinsOverBnk()
    {
        beginTest("musical_romFactoryWinsOverBnk");

        const auto result = Core::PatchNameResolver::resolve({
            .deviceName = "BNK2: 05",
            .bank = 2,
            .patchNumber = 5,
            .isRomBank = true,
            .factoryName = "VOICES 1",
            .overlayName = "OVERLAY",
            .mode = Core::PatchNameResolver::Mode::kMusical,
        });
        expectEquals(result, juce::String("VOICES 1"));
    }

    void musical_overlayWinsOnBnkWhenNoFactory()
    {
        beginTest("musical_overlayWinsOnBnkWhenNoFactory");

        const auto result = Core::PatchNameResolver::resolve({
            .deviceName = "BNK0: 12",
            .bank = 0,
            .patchNumber = 12,
            .isRomBank = false,
            .factoryName = {},
            .overlayName = "PAPANO 4",
            .mode = Core::PatchNameResolver::Mode::kMusical,
        });
        expectEquals(result, juce::String("PAPANO 4"));
    }

    void musical_keepsUsableNonBnkDump()
    {
        beginTest("musical_keepsUsableNonBnkDump");

        const auto result = Core::PatchNameResolver::resolve({
            .deviceName = "BRASS 1",
            .bank = 0,
            .patchNumber = 3,
            .isRomBank = false,
            .factoryName = {},
            .overlayName = "OVERLAY",
            .mode = Core::PatchNameResolver::Mode::kMusical,
        });
        expectEquals(result, juce::String("BRASS 1"));
    }

    void musical_fallsBackToBxPyyWhenEmpty()
    {
        beginTest("musical_fallsBackToBxPyyWhenEmpty");

        const auto result = Core::PatchNameResolver::resolve({
            .deviceName = "   ",
            .bank = 1,
            .patchNumber = 7,
            .isRomBank = false,
            .factoryName = {},
            .overlayName = {},
            .mode = Core::PatchNameResolver::Mode::kMusical,
        });
        expectEquals(result, juce::String("B1-P07"));
    }

    void hardware_keepsBnkWithoutOverlayOrFactory()
    {
        beginTest("hardware_keepsBnkWithoutOverlayOrFactory");

        const auto result = Core::PatchNameResolver::resolve({
            .deviceName = "BNK0: 12",
            .bank = 0,
            .patchNumber = 12,
            .isRomBank = true,
            .factoryName = "VOICES 1",
            .overlayName = "PAPANO 4",
            .mode = Core::PatchNameResolver::Mode::kHardware,
        });
        expectEquals(result, juce::String("BNK0: 12"));
    }

    void hardware_fallsBackToBxPyyWhenEmpty()
    {
        beginTest("hardware_fallsBackToBxPyyWhenEmpty");

        const auto result = Core::PatchNameResolver::resolve({
            .deviceName = "",
            .bank = 0,
            .patchNumber = 4,
            .isRomBank = false,
            .factoryName = {},
            .overlayName = "PAPANO 4",
            .mode = Core::PatchNameResolver::Mode::kHardware,
        });
        expectEquals(result, juce::String("B0-P04"));
    }

    void hardware_keepsUsableMusicalDump()
    {
        beginTest("hardware_keepsUsableMusicalDump");

        const auto result = Core::PatchNameResolver::resolve({
            .deviceName = "BRASS 1",
            .bank = 0,
            .patchNumber = 3,
            .isRomBank = false,
            .factoryName = {},
            .overlayName = {},
            .mode = Core::PatchNameResolver::Mode::kHardware,
        });
        expectEquals(result, juce::String("BRASS 1"));
    }
};

static PatchNameResolverTests patchNameResolverTests;
