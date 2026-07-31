#include "Core/Services/PatchNameOverlayStore.h"

namespace Core
{

    int PatchNameOverlayStore::makeKey(int bank, int patch) noexcept
    {
        return juce::jlimit(0, 9, bank) * 100 + juce::jlimit(0, 99, patch);
    }

    void PatchNameOverlayStore::remember(int bank, int patch, juce::String name)
    {
        name = name.trimEnd();
        if (name.isEmpty())
        {
            forget(bank, patch);
            return;
        }

        entries_[makeKey(bank, patch)] = std::move(name);
    }

    void PatchNameOverlayStore::forget(int bank, int patch)
    {
        entries_.erase(makeKey(bank, patch));
    }

    void PatchNameOverlayStore::clearBank(int bank)
    {
        const int clampedBank = juce::jlimit(0, 9, bank);
        for (auto it = entries_.begin(); it != entries_.end();)
        {
            if (it->first / 100 == clampedBank)
                it = entries_.erase(it);
            else
                ++it;
        }
    }

    juce::String PatchNameOverlayStore::lookup(int bank, int patch) const
    {
        const auto it = entries_.find(makeKey(bank, patch));
        if (it == entries_.end())
            return {};

        return it->second;
    }

    juce::ValueTree PatchNameOverlayStore::toValueTree() const
    {
        juce::ValueTree tree { kValueTreeType };
        for (const auto& entry : entries_)
        {
            juce::ValueTree child { kEntryType };
            child.setProperty(kBankProp, entry.first / 100, nullptr);
            child.setProperty(kPatchProp, entry.first % 100, nullptr);
            child.setProperty(kNameProp, entry.second, nullptr);
            tree.appendChild(child, nullptr);
        }
        return tree;
    }

    void PatchNameOverlayStore::replaceFromValueTree(const juce::ValueTree& tree)
    {
        entries_.clear();

        for (int i = 0; i < tree.getNumChildren(); ++i)
        {
            const auto child = tree.getChild(i);
            if (! child.hasType(kEntryType))
                continue;

            const int bank = static_cast<int>(child.getProperty(kBankProp, -1));
            const int patch = static_cast<int>(child.getProperty(kPatchProp, -1));
            const auto name = child.getProperty(kNameProp, {}).toString().trimEnd();
            if (bank < 0 || bank > 9 || patch < 0 || patch > 99 || name.isEmpty())
                continue;

            entries_[makeKey(bank, patch)] = name;
        }
    }

} // namespace Core
