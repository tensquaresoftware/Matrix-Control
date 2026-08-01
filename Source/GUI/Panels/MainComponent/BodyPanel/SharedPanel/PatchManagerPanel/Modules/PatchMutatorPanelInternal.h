#pragma once

#include "GUI/Widgets/Button.h"
#include "GUI/Widgets/Slider.h"
#include "GUI/Widgets/Toggle.h"
#include "Shared/Definitions/PluginDescriptors.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"

#include <memory>
#include <utility>

#include <juce_data_structures/juce_data_structures.h>

// Free helpers shared by PatchMutatorPanel.cpp and its companion .cpp files.
namespace PatchMutatorPanelInternal
{
    namespace MutatorDisplayNames = PluginDisplayNames::PatchManagerSection::PatchMutatorModule::StandaloneWidgets;
    namespace MutatorIds = PluginIDs::PatchManagerSection::PatchMutatorModule;
    namespace MutatorState = MutatorIds::StateProperties;
    namespace MutatorWidgets = MutatorIds::StandaloneWidgets;

    struct ActionEnabledBinding
    {
        const char* propertyId = nullptr;
        TSS::Button* button = nullptr;
    };

    inline const PluginDescriptors::IntParameterDescriptor* findMutatorIntDescriptor(const char* parameterId)
    {
        const auto& descriptors = PluginDescriptors::PatchManagerSection::PatchMutatorModule::kIntParameters;

        for (const auto& descriptor : descriptors)
        {
            if (descriptor.parameterId == parameterId)
                return &descriptor;
        }

        return nullptr;
    }

    inline void hydrateIntSlider(TSS::Slider* slider,
                                 const juce::ValueTree& state,
                                 const char* propertyId,
                                 int defaultValue)
    {
        if (slider == nullptr)
            return;

        slider->setValue(static_cast<double>(static_cast<int>(state.getProperty(propertyId, defaultValue))),
                         juce::dontSendNotification);
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

    inline bool isRecipePropertyId(const juce::String& propertyName)
    {
        static constexpr const char* kRecipePropertyIds[] = {
            MutatorWidgets::kAmount,
            MutatorWidgets::kRandom,
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
