#include <juce_core/juce_core.h>

#include "Core/Services/BankImportPlanner.h"

namespace
{
    Core::PatchFolderScanResult makeScan(int validCount, int invalidCount, int sortedValidFileNameCount)
    {
        Core::PatchFolderScanResult scan;
        scan.folderUsable = true;
        scan.validCount = validCount;
        scan.invalidCount = invalidCount;

        for (int i = 0; i < sortedValidFileNameCount; ++i)
            scan.sortedValidFileNames.add("P" + juce::String(i).paddedLeft('0', 2) + ".syx");

        return scan;
    }
}

class BankImportPlannerTests : public juce::UnitTest
{
public:
    BankImportPlannerTests() : juce::UnitTest("BankImportPlanner") {}

    void runTest() override
    {
        resolve_tallies_foundAsValidPlusInvalid();
        resolve_usesAllValidFiles_whenUnderCap();
        resolve_capsAtOneHundred_whenOverCap();
        resolve_zeroValidFiles_yieldsZeroCappedCount();
    }

private:
    void resolve_tallies_foundAsValidPlusInvalid()
    {
        beginTest("resolve_tallies_foundAsValidPlusInvalid");

        const auto scan = makeScan(7, 3, 7);
        const auto plan = Core::BankImportPlanner::resolve(scan);

        expectEquals(plan.foundCount, 10);
    }

    void resolve_usesAllValidFiles_whenUnderCap()
    {
        beginTest("resolve_usesAllValidFiles_whenUnderCap");

        const auto scan = makeScan(42, 1, 42);
        const auto plan = Core::BankImportPlanner::resolve(scan);

        expectEquals(plan.cappedFileCount, 42);
    }

    void resolve_capsAtOneHundred_whenOverCap()
    {
        beginTest("resolve_capsAtOneHundred_whenOverCap");

        const auto scan = makeScan(150, 0, 150);
        const auto plan = Core::BankImportPlanner::resolve(scan);

        expectEquals(plan.foundCount, 150);
        expectEquals(plan.cappedFileCount, Core::BankImportPlanner::kMaxImportSlots);
    }

    void resolve_zeroValidFiles_yieldsZeroCappedCount()
    {
        beginTest("resolve_zeroValidFiles_yieldsZeroCappedCount");

        const auto scan = makeScan(0, 5, 0);
        const auto plan = Core::BankImportPlanner::resolve(scan);

        expectEquals(plan.foundCount, 5);
        expectEquals(plan.cappedFileCount, 0);
    }
};

static BankImportPlannerTests bankImportPlannerTests;
