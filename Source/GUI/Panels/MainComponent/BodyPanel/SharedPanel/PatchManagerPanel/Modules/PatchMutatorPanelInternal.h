#pragma once

#include "GUI/Widgets/Button.h"
#include "GUI/Widgets/Slider.h"
#include "GUI/Widgets/Toggle.h"
#include "Core/Services/PatchMutator/MutationPolicy.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"

#include <initializer_list>
#include <memory>
#include <utility>

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_data_structures/juce_data_structures.h>

// Free helpers shared by PatchMutatorPanel.cpp and its companion .cpp files.
namespace PatchMutatorPanelInternal
{
    namespace MutatorDisplayNames = PluginDisplayNames::PatchManagerSection::PatchMutatorModule::StandaloneWidgets;
    namespace MutatorChoiceLists = PluginDisplayNames::PatchManagerSection::PatchMutatorModule::ChoiceLists;
    namespace MutatorIds = PluginIDs::PatchManagerSection::PatchMutatorModule;
    namespace MutatorState = MutatorIds::StateProperties;
    namespace MutatorWidgets = MutatorIds::StandaloneWidgets;

    struct ActionEnabledBinding
    {
        const char* propertyId = nullptr;
        TSS::Button* button = nullptr;
    };

    // Reserved HISTORY level-1 id — negative so it never collides with the 1..N mutation ids.
    constexpr int kHistoryInitialPrimaryId = -1;

    // Same word as the Patch Name secondary line while the origin snapshot is auditioned.
    constexpr const char* kHistoryInitialLabel =
        PluginDisplayNames::PatchEditSection::PatchNameModule::StandaloneWidgets::kCompareSecondaryLabel;

    inline bool isHistoryInitialRowAvailable(const juce::ValueTree& state)
    {
        return static_cast<bool>(state.getProperty(MutatorState::kInitialSnapshotAvailable, false))
               && state.getProperty(MutatorState::kHistoryMutateList).toString().isNotEmpty();
    }

    inline bool isHistoryInitialSelected(const juce::ValueTree& state)
    {
        return static_cast<bool>(state.getProperty(MutatorState::kInitialSelected, false));
    }

    inline bool matchesAnyPropertyId(const juce::String& propertyName,
                                     std::initializer_list<const char*> propertyIds)
    {
        for (const auto* propertyId : propertyIds)
        {
            if (propertyName == propertyId)
                return true;
        }

        return false;
    }

    inline bool isHistoryComboRefreshProperty(const juce::String& propertyName)
    {
        return matchesAnyPropertyId(propertyName,
                                    { MutatorState::kHistoryMutateList,
                                      MutatorState::kHistoryRetryList,
                                      MutatorState::kHistoryRetryListsByRoot,
                                      MutatorState::kSelectedMutateRootIndex,
                                      MutatorState::kSelectedRetryIndex,
                                      MutatorState::kCompareActive,
                                      MutatorState::kInitialSelected,
                                      MutatorState::kInitialSnapshotAvailable });
    }

    inline bool isCompareUiRefreshProperty(const juce::String& propertyName)
    {
        return matchesAnyPropertyId(propertyName,
                                    { MutatorState::kHistoryMutateList,
                                      MutatorState::kHistoryRetryListsByRoot,
                                      MutatorState::kSelectedMutateRootIndex,
                                      MutatorState::kCompareActive,
                                      MutatorState::kInitialSelected,
                                      MutatorState::kInitialSnapshotAvailable });
    }

    // PITCH combo ids: level 1 carries the pitch mode, level 2 the octave window.
    // Child id packs primary * 10 + octaves, so the octave window must stay a single digit.
    static_assert(Core::MutationCalibration::kMaxPitchOctaves < 10,
                  "pitchChildIdFor encodes octaves in the ones place");
    inline int pitchPrimaryIdForMode(int pitchModeIndex) { return pitchModeIndex + 1; }
    inline int pitchModeIndexForPrimaryId(int primaryId) { return primaryId - 1; }
    inline int pitchChildIdFor(int primaryId, int octaves) { return primaryId * 10 + octaves; }
    inline int pitchOctavesForChildId(int childId) { return childId % 10; }

    inline juce::String pitchClosedPrimaryLabel(int pitchModeIndex)
    {
        if (pitchModeIndex == static_cast<int>(Core::MutationPitchMode::kConsonant))
            return MutatorChoiceLists::MutationPitch::kConsonantClosed;
        if (pitchModeIndex == static_cast<int>(Core::MutationPitchMode::kDissonant))
            return MutatorChoiceLists::MutationPitch::kDissonantClosed;
        return {};
    }

    inline void hydrateToggleBinding(TSS::Toggle* toggle,
                                     const juce::ValueTree& state,
                                     const char* propertyId)
    {
        if (toggle == nullptr)
            return;

        toggle->setToggleState(static_cast<bool>(state.getProperty(propertyId, false)),
                               juce::dontSendNotification);
    }

    inline bool isWaveSelectAudible(const juce::AudioProcessorValueTreeState& apvts,
                                    const char* waveSelectParameterId)
    {
        // Prefer the live AudioParameterChoice index — Wave Select SSOT puts OFF at 0.
        if (const auto* choice = dynamic_cast<const juce::AudioParameterChoice*>(
                apvts.getParameter(waveSelectParameterId)))
        {
            return choice->getIndex() > 0;
        }

        if (auto* raw = apvts.getRawParameterValue(waveSelectParameterId))
            return juce::roundToInt(raw->load()) > 0;

        return false;
    }

    inline bool isRecipePropertyId(const juce::String& propertyName)
    {
        static constexpr const char* kRecipePropertyIds[] = {
            MutatorWidgets::kMode,
            MutatorWidgets::kPitch,
            MutatorWidgets::kPitchOctaves,
            MutatorWidgets::kEnableDco1,
            MutatorWidgets::kEnableDco2,
            MutatorWidgets::kEnableVcfVca,
            MutatorWidgets::kEnableFmTrack,
            MutatorWidgets::kEnableRampPortamento,
            MutatorWidgets::kEnableEnvelope1,
            MutatorWidgets::kEnableEnvelope2,
            MutatorWidgets::kEnableEnvelope3,
            MutatorWidgets::kEnableLfo1,
            MutatorWidgets::kEnableLfo2,
            MutatorWidgets::kEnableMatrixMod,
        };

        for (const auto* id : kRecipePropertyIds)
        {
            if (propertyName == id)
                return true;
        }

        return false;
    }

    template <typename WidgetT>
    void applyUiScale(std::unique_ptr<WidgetT>& widget, float sf)
    {
        if (widget != nullptr)
            widget->setUiScale(sf);
    }

    template <typename WidgetT, typename LookT>
    void applyLook(std::unique_ptr<WidgetT>& widget, LookT&& look)
    {
        if (widget != nullptr)
            widget->setLook(std::forward<LookT>(look));
    }
}
