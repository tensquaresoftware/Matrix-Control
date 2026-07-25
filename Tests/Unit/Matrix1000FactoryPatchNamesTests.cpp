#include <juce_core/juce_core.h>

#include "Core/Services/Matrix1000FactoryPatchNames.h"

class Matrix1000FactoryPatchNamesTests : public juce::UnitTest
{
public:
    Matrix1000FactoryPatchNamesTests() : juce::UnitTest("Matrix1000FactoryPatchNames") {}

    void runTest() override
    {
        romLookup_returnsFactorySpelling();
        ramBanks_returnEmpty();
        outOfRange_returnsEmpty();
    }

private:
    void romLookup_returnsFactorySpelling()
    {
        beginTest("romLookup_returnsFactorySpelling");

        expectEquals(Core::Matrix1000FactoryPatchNames::nameFor(2, 52), juce::String("BS ETAK*"));
        expectEquals(Core::Matrix1000FactoryPatchNames::nameFor(6, 9), juce::String("*'CANOPY"));
        expectEquals(Core::Matrix1000FactoryPatchNames::nameFor(2, 0), juce::String("OBXA-11"));
        expectEquals(Core::Matrix1000FactoryPatchNames::nameFor(2, 88), juce::String("*FUNK AR")); // table 9 chars → 8
        expectEquals(Core::Matrix1000FactoryPatchNames::nameFor(9, 99), juce::String("GRNDR 6*"));
    }

    void ramBanks_returnEmpty()
    {
        beginTest("ramBanks_returnEmpty");

        expect(Core::Matrix1000FactoryPatchNames::nameFor(0, 0).isEmpty());
        expect(Core::Matrix1000FactoryPatchNames::nameFor(1, 50).isEmpty());
    }

    void outOfRange_returnsEmpty()
    {
        beginTest("outOfRange_returnsEmpty");

        expect(Core::Matrix1000FactoryPatchNames::nameFor(10, 0).isEmpty());
        expect(Core::Matrix1000FactoryPatchNames::nameFor(2, -1).isEmpty());
        expect(Core::Matrix1000FactoryPatchNames::nameFor(2, 100).isEmpty());
    }
};

static Matrix1000FactoryPatchNamesTests matrix1000FactoryPatchNamesTests;
