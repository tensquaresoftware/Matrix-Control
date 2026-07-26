#pragma once

#include "Core/Services/PatchFileService.h"

namespace Core
{
    struct BankImportPlan
    {
        int foundCount = 0;      // valid + invalid .syx files seen in the scan
        int cappedFileCount = 0; // number of (alpha-sorted, valid) files to actually import
    };

    // Bank Utility IMPORT: given an already-alpha-sorted PatchFolderScanResult, decide how many
    // of the valid files to import (hard cap at 100 device slots). Pure logic — no file I/O, no MIDI.
    struct BankImportPlanner
    {
        static constexpr int kMaxImportSlots = 100;

        static BankImportPlan resolve(const PatchFolderScanResult& scan);
    };

} // namespace Core
