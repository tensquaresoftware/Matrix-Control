#include <juce_core/juce_core.h>

#include "Core/Models/PatchModel.h"
#include "Core/Services/ClipboardService.h"

class ClipboardBankServiceTests : public juce::UnitTest
{
public:
    ClipboardBankServiceTests() : juce::UnitTest("ClipboardBankService") {}

    void runTest() override
    {
        testBankCopyEligibilitySameRomCross();
        testBankModeReplacesFullPatch();
    }

private:
    void testBankCopyEligibilitySameRomCross()
    {
        beginTest("bank clipboard — same-bank and ROM targets rejected; cross-RAM allowed");

        Core::ClipboardService::BankPatchArray patches {};
        for (auto& slot : patches)
            slot[0] = 0x42;

        Core::ClipboardService clipboard;
        clipboard.copyBank(patches, 0);

        expect(clipboard.getMode() == Core::ClipboardMode::Bank);
        expect(clipboard.getBankSource().has_value());
        expectEquals(*clipboard.getBankSource(), 0);
        expect(! clipboard.canPasteBank(0));
        expect(clipboard.canPasteBank(1));
        expect(clipboard.canPasteBank(5)); // ROM gate is DeviceMemoryLimits, not ClipboardService

        Core::ClipboardService::BankPatchArray out {};
        expect(clipboard.pasteBank(out));
        expectEquals(static_cast<int>(out[0][0]), 0x42);
    }

    void testBankModeReplacesFullPatch()
    {
        beginTest("bank clipboard — replaces full-patch mode");

        Core::PatchModel patch;
        Core::ClipboardService clipboard;
        clipboard.copyFullPatch(patch, "BANK 0 / PATCH 0");
        expect(clipboard.canPasteFullPatch());

        Core::ClipboardService::BankPatchArray patches {};
        clipboard.copyBank(patches, 3);
        expect(clipboard.getMode() == Core::ClipboardMode::Bank);
        expect(! clipboard.canPasteFullPatch());
        expectEquals(*clipboard.getBankSource(), 3);
        expect(! clipboard.canPasteBank(3));
        expect(clipboard.canPasteBank(1));
    }

};

static ClipboardBankServiceTests clipboardBankServiceTests;
