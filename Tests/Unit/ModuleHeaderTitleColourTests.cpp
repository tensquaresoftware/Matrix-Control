#include <juce_graphics/juce_graphics.h>

#include "GUI/Widgets/ModuleHeaderTitleColour.h"

class ModuleHeaderTitleColourTests : public juce::UnitTest
{
public:
    ModuleHeaderTitleColourTests()
        : juce::UnitTest("ModuleHeaderTitleColour")
    {
    }

    void runTest() override
    {
        beginTest("hover_usesFocusColour_onlyWhenClickEnabled");

        const auto text = juce::Colours::white;
        const auto focus = juce::Colours::red;

        expect(TSS::resolveModuleHeaderTitleTextColour(true, true, text, focus) == focus);
        expect(TSS::resolveModuleHeaderTitleTextColour(true, false, text, focus) == text);
        expect(TSS::resolveModuleHeaderTitleTextColour(false, true, text, focus) == text);
        expect(TSS::resolveModuleHeaderTitleTextColour(false, false, text, focus) == text);
    }
};

static ModuleHeaderTitleColourTests moduleHeaderTitleColourTests;
