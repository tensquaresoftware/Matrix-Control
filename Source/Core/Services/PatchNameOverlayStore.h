#pragma once

#include <map>

#include <juce_data_structures/juce_data_structures.h>

namespace Core
{

    // Matrix-1000 firmware stores sound params from SysEx but rewrites name bytes 0–7 to
    // "BNKx: yy" placeholders (confirmed on 01H and on 0x0D+0x0E). This store keeps the
    // musical names the editor should show for RAM slots after IMPORT / STORE.
    class PatchNameOverlayStore
    {
    public:
        static constexpr const char* kValueTreeType = "PatchNameOverlay";
        static constexpr const char* kEntryType = "Entry";
        static constexpr const char* kBankProp = "bank";
        static constexpr const char* kPatchProp = "patch";
        static constexpr const char* kNameProp = "name";

        void remember(int bank, int patch, juce::String name);
        void forget(int bank, int patch);
        void clearBank(int bank);

        // Empty when no overlay is remembered for that slot.
        juce::String lookup(int bank, int patch) const;

        juce::ValueTree toValueTree() const;
        void replaceFromValueTree(const juce::ValueTree& tree);

    private:
        static int makeKey(int bank, int patch) noexcept;

        std::map<int, juce::String> entries_;
    };

} // namespace Core
