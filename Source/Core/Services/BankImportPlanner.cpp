#include "Core/Services/BankImportPlanner.h"

namespace Core
{
    BankImportPlan BankImportPlanner::resolve(const PatchFolderScanResult& scan)
    {
        BankImportPlan plan;
        plan.foundCount = scan.validCount + scan.invalidCount;
        plan.cappedFileCount = juce::jmin(kMaxImportSlots, scan.sortedValidFileNames.size());
        return plan;
    }

} // namespace Core
