#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "Core/Actions/ActionExecutionHooks.h"
#include "Core/Models/ApvtsPatchMapper.h"
#include "Core/Models/PatchNameSyncer.h"
#include "Core/Services/PatchFileService.h"

// Free helpers shared by PatchManagerActionHandler.cpp and its companion .cpp files.
namespace PatchManagerActionHandlerInternal
{
    inline void setPatchLoadSuppressHooks(Core::ActionExecutionHooks& hooks, bool suppress)
    {
        if (hooks.setSuppressPatchSysEx)
            hooks.setSuppressPatchSysEx(suppress);

        if (hooks.setSuppressMatrixModSysEx)
            hooks.setSuppressMatrixModSysEx(suppress);
    }

    inline void flushDeferredApvtsParameterSync(juce::AudioProcessorValueTreeState& apvts)
    {
        // setValueNotifyingHost queues ValueTree sync on APVTS's internal timer; copyState() forces flush (public API).
        (void) apvts.copyState();
    }

    inline void pushPatchModelToApvtsWithSuppress(juce::AudioProcessorValueTreeState& apvts,
                                                  Core::ActionExecutionHooks& hooks,
                                                  Core::ApvtsPatchMapper& mapper,
                                                  Core::PatchNameSyncer* patchNameSyncer)
    {
        setPatchLoadSuppressHooks(hooks, true);
        mapper.bufferToApvts();

        if (patchNameSyncer != nullptr)
            patchNameSyncer->bufferToApvts();

        flushDeferredApvtsParameterSync(apvts);
        setPatchLoadSuppressHooks(hooks, false);
    }

    inline int indexOfFileNameIgnoreCase(const juce::StringArray& names, const juce::String& fileName)
    {
        for (int i = 0; i < names.size(); ++i)
        {
            if (names[i].equalsIgnoreCase(fileName))
                return i;
        }

        return -1;
    }

    inline juce::String savedSyxFileName(const juce::File& targetFile)
    {
        return targetFile.hasFileExtension(Core::PatchFileService::kSyxExtension)
            ? targetFile.getFileName()
            : targetFile.withFileExtension(Core::PatchFileService::kSyxExtension).getFileName();
    }
}
