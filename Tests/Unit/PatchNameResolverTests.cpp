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

        const auto result = Core::PatchNameResolver::resolve(
            "BNK2: 05", 2, 5, true, "VOICES 1", "OVERLAY",
            Core::PatchNameResolver::Mode::kMusical);
        expectEquals(result, juce::String("VOICES 1"));
    }

    void musical_overlayWinsOnBnkWhenNoFactory()
    {
        beginTest("musical_overlayWinsOnBnkWhenNoFactory");

        const auto result = Core::PatchNameResolver::resolve(
            "BNK0: 12", 0, 12, false, {}, "PAPANO 4",
            Core::PatchNameResolver::Mode::kMusical);
        expectEquals(result, juce::String("PAPANO 4"));
    }

    void musical_keepsUsableNonBnkDump()
    {
        beginTest("musical_keepsUsableNonBnkDump");

        const auto result = Core::PatchNameResolver::resolve(
            "BRASS 1", 0, 3, false, {}, "OVERLAY",
            Core::PatchNameResolver::Mode::kMusical);
        expectEquals(result, juce::String("BRASS 1"));
    }

    void musical_fallsBackToBxPyyWhenEmpty()
    {
        beginTest("musical_fallsBackToBxPyyWhenEmpty");

        const auto result = Core::PatchNameResolver::resolve(
            "   ", 1, 7, false, {}, {},
            Core::PatchNameResolver::Mode::kMusical);
        expectEquals(result, juce::String("B1-P07"));
    }

    void hardware_keepsBnkWithoutOverlayOrFactory()
    {
        beginTest("hardware_keepsBnkWithoutOverlayOrFactory");

        const auto result = Core::PatchNameResolver::resolve(
            "BNK0: 12", 0, 12, true, "VOICES 1", "PAPANO 4",
            Core::PatchNameResolver::Mode::kHardware);
        expectEquals(result, juce::String("BNK0: 12"));
    }

    void hardware_fallsBackToBxPyyWhenEmpty()
    {
        beginTest("hardware_fallsBackToBxPyyWhenEmpty");

        const auto result = Core::PatchNameResolver::resolve(
            "", 0, 4, false, {}, "PAPANO 4",
            Core::PatchNameResolver::Mode::kHardware);
        expectEquals(result, juce::String("B0-P04"));
    }

    void hardware_keepsUsableMusicalDump()
    {
        beginTest("hardware_keepsUsableMusicalDump");

        const auto result = Core::PatchNameResolver::resolve(
            "BRASS 1", 0, 3, false, {}, {},
            Core::PatchNameResolver::Mode::kHardware);
        expectEquals(result, juce::String("BRASS 1"));
    }
};

static PatchNameResolverTests patchNameResolverTests;
