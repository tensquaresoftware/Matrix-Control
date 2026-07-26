#include "Core/Services/PatchNameEditRules.h"

namespace Core
{

    juce::String PatchNameEditRules::resolveCommittedPatchName(const juce::String& editedName,
                                                                const juce::String& previousName)
    {
        if (editedName.trim().isEmpty())
            return previousName;

        return editedName;
    }

} // namespace Core
