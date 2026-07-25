#include <juce_core/juce_core.h>

#include "Core/Services/PatchNameEditRules.h"

class PatchNameEditRulesTests : public juce::UnitTest
{
public:
    PatchNameEditRulesTests() : juce::UnitTest("PatchNameEditRules") {}

    void runTest() override
    {
        resolve_keepsEditedNameWhenNonEmpty();
        resolve_keepsPreviousWhenAllSpaces();
        resolve_keepsPreviousWhenCompletelyEmpty();
        resolve_keepsEditedNameWithInternalSpaces();
    }

private:
    void resolve_keepsEditedNameWhenNonEmpty()
    {
        beginTest("resolve_keepsEditedNameWhenNonEmpty");

        const auto result = Core::PatchNameEditRules::resolveCommittedPatchName("COLDPAD ", "WARMPAD ");
        expectEquals(result, juce::String("COLDPAD "));
    }

    void resolve_keepsPreviousWhenAllSpaces()
    {
        beginTest("resolve_keepsPreviousWhenAllSpaces");

        const auto result = Core::PatchNameEditRules::resolveCommittedPatchName("        ", "WARMPAD ");
        expectEquals(result, juce::String("WARMPAD "));
    }

    void resolve_keepsPreviousWhenCompletelyEmpty()
    {
        beginTest("resolve_keepsPreviousWhenCompletelyEmpty");

        const auto result = Core::PatchNameEditRules::resolveCommittedPatchName("", "WARMPAD ");
        expectEquals(result, juce::String("WARMPAD "));
    }

    void resolve_keepsEditedNameWithInternalSpaces()
    {
        beginTest("resolve_keepsEditedNameWithInternalSpaces");

        const auto result = Core::PatchNameEditRules::resolveCommittedPatchName("BS 1    ", "WARMPAD ");
        expectEquals(result, juce::String("BS 1    "));
    }
};

static PatchNameEditRulesTests patchNameEditRulesTests;
