#include <juce_core/juce_core.h>

#include "GUI/Helpers/DeviceVersionDisplayFormat.h"

class DeviceVersionDisplayFormatTests : public juce::UnitTest
{
public:
    DeviceVersionDisplayFormatTests() : juce::UnitTest("DeviceVersionDisplayFormat") {}

    void runTest() override
    {
        formatsFourDigitPacksWithDot();
        stripsLeadingSpacesBeforeSplit();
        preservesExistingDot();
        leavesShortStringsUnbroken();
        returnsEmptyForWhitespaceOnly();
        uppercasesLetters();
    }

private:
    void formatsFourDigitPacksWithDot()
    {
        beginTest("formatsFourDigitPacksWithDot");
        expectEquals(TSS::formatDeviceVersionForDisplay("1234"), juce::String("12.34"));
    }

    void stripsLeadingSpacesBeforeSplit()
    {
        beginTest("stripsLeadingSpacesBeforeSplit");
        expectEquals(TSS::formatDeviceVersionForDisplay(" 116"), juce::String("1.16"));
        expectEquals(TSS::formatDeviceVersionForDisplay("116"), juce::String("1.16"));
    }

    void preservesExistingDot()
    {
        beginTest("preservesExistingDot");
        expectEquals(TSS::formatDeviceVersionForDisplay("1.11"), juce::String("1.11"));
        expectEquals(TSS::formatDeviceVersionForDisplay("1.20"), juce::String("1.20"));
    }

    void leavesShortStringsUnbroken()
    {
        beginTest("leavesShortStringsUnbroken");
        expectEquals(TSS::formatDeviceVersionForDisplay("16"), juce::String("16"));
        expectEquals(TSS::formatDeviceVersionForDisplay("1"), juce::String("1"));
        expectEquals(TSS::formatDeviceVersionForDisplay(""), juce::String());
    }

    void returnsEmptyForWhitespaceOnly()
    {
        beginTest("returnsEmptyForWhitespaceOnly");
        expectEquals(TSS::formatDeviceVersionForDisplay("   "), juce::String());
        expectEquals(TSS::formatDeviceVersionForDisplay("\t"), juce::String());
    }

    void uppercasesLetters()
    {
        beginTest("uppercasesLetters");
        expectEquals(TSS::formatDeviceVersionForDisplay("ab12"), juce::String("AB.12"));
        expectEquals(TSS::formatDeviceVersionForDisplay("abcd"), juce::String("AB.CD"));
    }
};

static DeviceVersionDisplayFormatTests deviceVersionDisplayFormatTests;
