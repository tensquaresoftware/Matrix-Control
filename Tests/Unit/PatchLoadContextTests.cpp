#include <juce_core/juce_core.h>

#include "Core/Services/PatchMutator/PatchLoadContext.h"

class PatchLoadContextTests : public juce::UnitTest
{
public:
    PatchLoadContextTests() : juce::UnitTest("PatchLoadContext") {}

    void runTest() override
    {
        deviceNamed_buildsNameAtLocation();
        deviceBlankName_locationOnly();
        deviceOsStrippedName_keepsSpaces();
        deviceUnsanitizableName_locationOnly();
        computerFile_stemAtSyxFile();
    }

private:
    void deviceNamed_buildsNameAtLocation()
    {
        beginTest("deviceNamed_buildsNameAtLocation");

        const auto context = Core::PatchLoadContext::deviceMemory(8, 25);
        expectEquals(context.computeExportBasename("OB-VOX"), juce::String("OB-VOX @ B8P25"));
    }

    void deviceBlankName_locationOnly()
    {
        beginTest("deviceBlankName_locationOnly");

        const auto context = Core::PatchLoadContext::deviceMemory(0, 0);
        expectEquals(context.computeExportBasename("   "), juce::String("B0P00"));
        expectEquals(context.computeExportBasename(""), juce::String("B0P00"));
    }

    void deviceOsStrippedName_keepsSpaces()
    {
        beginTest("deviceOsStrippedName_keepsSpaces");

        const auto context = Core::PatchLoadContext::deviceMemory(2, 52);
        expectEquals(context.computeExportBasename("BS ETAK*"), juce::String("BS ETAK @ B2P52"));
    }

    void deviceUnsanitizableName_locationOnly()
    {
        beginTest("deviceUnsanitizableName_locationOnly");

        const auto context = Core::PatchLoadContext::deviceMemory(1, 77);
        // '*' '?' '"' ':' are OS-forbidden and strip to empty.
        expectEquals(context.computeExportBasename("*?:"), juce::String("B1P77"));
    }

    void computerFile_stemAtSyxFile()
    {
        beginTest("computerFile_stemAtSyxFile");

        const auto context = Core::PatchLoadContext::computerFile("WARM-PAD");
        expectEquals(context.computeExportBasename("IGNORED"), juce::String("WARM-PAD @ SyxFile"));

        const auto fromExtension = Core::PatchLoadContext::computerFile("WARM-PAD.syx");
        expectEquals(fromExtension.computeExportBasename("IGNORED"), juce::String("WARM-PAD @ SyxFile"));

        const auto blank = Core::PatchLoadContext::computerFile("***");
        expectEquals(blank.computeExportBasename("IGNORED"), juce::String("PATCH @ SyxFile"));
    }
};

static PatchLoadContextTests patchLoadContextTests;
