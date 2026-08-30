#pragma once

#include "Shared/Definitions/Matrix1000Limits.h"
#include "Shared/Definitions/PluginDescriptors.h"

// Shared Matrix Modulation bus accessors for MutationMatrixModPolicy*.cpp units.
namespace Core::MatrixModPolicyDetail
{
    constexpr int kBusCount = ::Matrix1000Limits::kModulationBusCount;

    inline const PluginDescriptors::ChoiceParameterDescriptor& busSourceDescriptor(int busIndex)
    {
        return PluginDescriptors::MatrixModulationSection::kModulationBusChoiceParameters[
            static_cast<size_t>(busIndex)][0];
    }

    inline const PluginDescriptors::ChoiceParameterDescriptor& busDestinationDescriptor(int busIndex)
    {
        return PluginDescriptors::MatrixModulationSection::kModulationBusChoiceParameters[
            static_cast<size_t>(busIndex)][1];
    }

    inline const PluginDescriptors::IntParameterDescriptor& busAmountDescriptor(int busIndex)
    {
        return PluginDescriptors::MatrixModulationSection::kModulationBusIntParameters[
            static_cast<size_t>(busIndex)][0];
    }
} // namespace Core::MatrixModPolicyDetail
