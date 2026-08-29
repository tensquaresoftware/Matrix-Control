#pragma once

#include <juce_core/juce_core.h>

#include "Core/Init/InitDefaults.h"
#include "Core/Services/PatchMutator/MutationAlgorithm.h"
#include "Core/Services/PatchMutator/MutationMatrixModPolicy.h"
#include "Core/Services/PatchMutator/MutationPostApply.h"
#include "Shared/Definitions/Matrix1000Limits.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"

// Shared fixtures for the playable-calibration suites: patch reads and writes by
// parameter id, Matrix Modulation bus setup, and one-shot mutation runs.
namespace MutationCalibrationTestSupport
{
    namespace Dco1Ids = PluginIDs::PatchEditSection::Dco1Module::ParameterWidgets;
    namespace Dco2Ids = PluginIDs::PatchEditSection::Dco2Module::ParameterWidgets;
    namespace VcfVcaIds = PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets;
    namespace WaveSelectNames = PluginDisplayNames::ChoiceLists::WaveSelect;
    namespace SourceNames = PluginDisplayNames::ChoiceLists::ModulationBus::Source;
    namespace DestinationNames = PluginDisplayNames::ChoiceLists::ModulationBus::Destination;

    constexpr int kBusCount = ::Matrix1000Limits::kModulationBusCount;

    // Enough rolls that a policy violation cannot hide behind one lucky draw.
    constexpr int kTrialCount = 32;

    inline int readInt(const Core::PatchModel& patch, const char* parameterId)
    {
        const auto* descriptor = Core::findMutationIntDescriptor(parameterId);
        jassert(descriptor != nullptr);
        return patch.getValue(*descriptor);
    }

    inline void writeInt(Core::PatchModel& patch, const char* parameterId, int value)
    {
        const auto* descriptor = Core::findMutationIntDescriptor(parameterId);
        jassert(descriptor != nullptr);
        patch.setValue(*descriptor, juce::jlimit(descriptor->minValue, descriptor->maxValue, value));
    }

    inline void writeChoiceByName(Core::PatchModel& patch,
                                  const char* parameterId,
                                  const char* choiceName)
    {
        const auto* descriptor = Core::findMutationChoiceDescriptor(parameterId);
        jassert(descriptor != nullptr);
        const int index = descriptor->choices.indexOf(juce::String(choiceName));
        jassert(index >= 0);
        patch.setChoiceIndex(*descriptor, index);
    }

    inline bool isWaveSelectOff(const Core::PatchModel& patch, const char* parameterId)
    {
        const auto* descriptor = Core::findMutationChoiceDescriptor(parameterId);
        jassert(descriptor != nullptr);
        return patch.getChoiceIndex(*descriptor)
               == descriptor->choices.indexOf(juce::String(WaveSelectNames::kOff));
    }

    inline const PluginDescriptors::ChoiceParameterDescriptor& busSource(int busIndex)
    {
        return PluginDescriptors::MatrixModulationSection::kModulationBusChoiceParameters[
            static_cast<size_t>(busIndex)][0];
    }

    inline const PluginDescriptors::ChoiceParameterDescriptor& busDestination(int busIndex)
    {
        return PluginDescriptors::MatrixModulationSection::kModulationBusChoiceParameters[
            static_cast<size_t>(busIndex)][1];
    }

    inline const PluginDescriptors::IntParameterDescriptor& busAmount(int busIndex)
    {
        return PluginDescriptors::MatrixModulationSection::kModulationBusIntParameters[
            static_cast<size_t>(busIndex)][0];
    }

    struct BusRoute
    {
        int busIndex = 0;
        const char* sourceName = SourceNames::kNone;
        const char* destinationName = DestinationNames::kNone;
        int amount = 0;
    };

    inline void writeMatrixModBus(Core::PatchModel& patch, const BusRoute& route)
    {
        const auto& source = busSource(route.busIndex);
        const auto& destination = busDestination(route.busIndex);
        const int sourceIndex = source.choices.indexOf(juce::String(route.sourceName));
        const int destinationIndex = destination.choices.indexOf(juce::String(route.destinationName));
        jassert(sourceIndex >= 0);
        jassert(destinationIndex >= 0);
        if (sourceIndex < 0 || destinationIndex < 0)
            return;

        patch.setChoiceIndex(source, sourceIndex);
        patch.setChoiceIndex(destination, destinationIndex);
        patch.setValue(busAmount(route.busIndex), route.amount);
    }

    inline void clearAllMatrixModBuses(Core::PatchModel& patch)
    {
        for (int busIndex = 0; busIndex < kBusCount; ++busIndex)
            writeMatrixModBus(patch, BusRoute { busIndex });
    }

    inline Core::MutationRecipe makeRecipe(Core::MutationMode mode, Core::MutationPitchMode pitchMode)
    {
        Core::MutationRecipe recipe;
        recipe.mode = mode;
        recipe.pitchMode = pitchMode;
        recipe.applyModeCalibration();
        return recipe;
    }

    inline Core::PatchModel makeInitPatchModel()
    {
        Core::PatchModel model;
        model.loadFrom(Core::InitDefaults::patchData());
        return model;
    }

    inline Core::PatchModel mutated(const Core::PatchModel& seed,
                                    const Core::MutationRecipe& recipe,
                                    juce::uint32 rngSeed)
    {
        Core::PatchModel working;
        working.loadFrom(seed.data());

        Core::SeededRandom rng(rngSeed);
        Core::MutationAlgorithm algorithm;
        algorithm.apply(working, recipe, rng);
        return working;
    }
} // namespace MutationCalibrationTestSupport
