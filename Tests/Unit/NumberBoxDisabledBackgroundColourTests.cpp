#include <juce_core/juce_core.h>

#include "GUI/Skins/ColourChart.h"
#include "GUI/Skins/SkinColoursWidgetsControls.h"
#include "GUI/Skins/SkinColoursWidgetsDisplays.h"
#include "GUI/Skins/SkinColoursWidgetsSelection.h"

class NumberBoxDisabledBackgroundColourTests : public juce::UnitTest
{
public:
    NumberBoxDisabledBackgroundColourTests()
        : juce::UnitTest("NumberBoxDisabledBackgroundColour")
    {
    }

    void runTest() override
    {
        using namespace TSS::SkinColours::Widgets;

        beginTest("disabledBackground_matchesButtonLikeAndButtonGrey1");

        expect(NumberBox::kBackgroundDisabled.blackVariant == ColourChart::kDarkGrey1);
        expect(NumberBox::kBackgroundDisabled.creamVariant == ColourChart::kDarkGrey1);
        expect(NumberBox::kBackgroundDisabled.blackVariant
               == Button::kBackgroundDisabled.blackVariant);
        expect(NumberBox::kBackgroundDisabled.creamVariant
               == Button::kBackgroundDisabled.creamVariant);
        expect(NumberBox::kBackgroundDisabled.blackVariant
               == ComboBox::ButtonLike::kBackgroundDisabled.blackVariant);
        expect(NumberBox::kBackgroundDisabled.creamVariant
               == ComboBox::ButtonLike::kBackgroundDisabled.creamVariant);

        beginTest("enabledBackground_isBlack");

        expect(NumberBox::kBackground.blackVariant == ColourChart::kBlack);
        expect(NumberBox::kBackground.creamVariant == ColourChart::kBlack);
    }
};

static NumberBoxDisabledBackgroundColourTests numberBoxDisabledBackgroundColourTests;
