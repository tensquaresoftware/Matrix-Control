#include "WidgetFactory.h"

#include "../Widgets/Slider.h"
#include "../Widgets/ComboBox.h"
#include "../Widgets/Button.h"
#include "../../Shared/SynthDescriptors.h"

WidgetFactory::WidgetFactory(juce::AudioProcessorValueTreeState& inApvts)
    : apvts(inApvts)
{
}

namespace
{
    template<typename DescriptorType>
    const DescriptorType* searchInVector(const std::vector<DescriptorType>& vec, const juce::String& id)
    {
        for (const auto& item : vec)
        {
            if constexpr (std::is_same_v<DescriptorType, SynthDescriptors::IntParameterDescriptor> || 
                         std::is_same_v<DescriptorType, SynthDescriptors::ChoiceParameterDescriptor>)
            {
                if (item.parameterId == id)
                {
                    return &item;
                }
            }
            else if constexpr (std::is_same_v<DescriptorType, SynthDescriptors::StandaloneWidgetDescriptor>)
            {
                if (item.widgetId == id)
                {
                    return &item;
                }
            }
        }
        return nullptr;
    }
}

std::unique_ptr<tss::Slider> WidgetFactory::createIntParameterSlider(
    const juce::String& parameterId,
    tss::Theme& theme)
{
    const auto* desc = findIntParameter(parameterId);
    jassert(desc != nullptr);
    
    if (desc == nullptr)
    {
        return nullptr;
    }
    
    auto slider = std::make_unique<tss::Slider>(theme, static_cast<double>(desc->defaultValue));
    slider->setRange(static_cast<double>(desc->minValue), static_cast<double>(desc->maxValue), 1.0);
    slider->setValue(static_cast<double>(desc->defaultValue));
    
    return slider;
}

std::unique_ptr<tss::ComboBox> WidgetFactory::createChoiceParameterComboBox(
    const juce::String& parameterId,
    tss::Theme& theme)
{
    const auto* desc = findChoiceParameter(parameterId);
    jassert(desc != nullptr);
    
    if (desc == nullptr)
    {
        return nullptr;
    }
    
    auto comboBox = std::make_unique<tss::ComboBox>(theme);
    
    for (const auto& choice : desc->choices)
    {
        comboBox->addItem(choice, comboBox->getNumItems() + 1);
    }
    
    comboBox->setSelectedItemIndex(desc->defaultIndex);
    
    return comboBox;
}

std::unique_ptr<tss::Button> WidgetFactory::createStandaloneButton(
    const juce::String& widgetId,
    tss::Theme& theme)
{
    const auto* desc = findStandaloneWidget(widgetId);
    jassert(desc != nullptr);
    jassert(desc->widgetType == SynthDescriptors::StandaloneWidgetType::kButton);
    
    if (desc == nullptr || desc->widgetType != SynthDescriptors::StandaloneWidgetType::kButton)
    {
        return nullptr;
    }
    
    auto button = std::make_unique<tss::Button>(
        theme, 
        tss::Button::kDefaultWidth, 
        desc->displayName
    );
    
    return button;
}

juce::String WidgetFactory::getParameterDisplayName(const juce::String& parameterId)
{
    const auto* intParam = findIntParameter(parameterId);
    if (intParam != nullptr)
    {
        return intParam->displayName;
    }
    
    const auto* choiceParam = findChoiceParameter(parameterId);
    if (choiceParam != nullptr)
    {
        return choiceParam->displayName;
    }
    
    return juce::String();
}

juce::String WidgetFactory::getGroupDisplayName(const juce::String& groupId)
{
    return SynthDescriptors::getGroupDisplayName(groupId);
}

juce::String WidgetFactory::getStandaloneWidgetDisplayName(const juce::String& widgetId)
{
    const auto* desc = findStandaloneWidget(widgetId);
    if (desc != nullptr)
    {
        return desc->displayName;
    }
    
    return juce::String();
}

const SynthDescriptors::IntParameterDescriptor* WidgetFactory::findIntParameter(const juce::String& parameterId)
{
    const auto* param = searchInPatchEditModuleIntParameters(parameterId);
    if (param != nullptr)
        return param;
    
    param = searchInMatrixModulationBusIntParameters(parameterId);
    if (param != nullptr)
        return param;
    
    param = searchInVector(SynthDescriptors::kMasterEditIntParameters, parameterId);
    if (param != nullptr)
        return param;
    
    return nullptr;
}

const SynthDescriptors::ChoiceParameterDescriptor* WidgetFactory::findChoiceParameter(const juce::String& parameterId)
{
    const auto* param = searchInPatchEditModuleChoiceParameters(parameterId);
    if (param != nullptr)
        return param;
    
    param = searchInMatrixModulationBusChoiceParameters(parameterId);
    if (param != nullptr)
        return param;
    
    param = searchInVector(SynthDescriptors::kMasterEditChoiceParameters, parameterId);
    if (param != nullptr)
        return param;
    
    return nullptr;
}

const SynthDescriptors::StandaloneWidgetDescriptor* WidgetFactory::findStandaloneWidget(const juce::String& widgetId)
{
    const auto* widget = searchInPatchEditStandaloneWidgets(widgetId);
    if (widget != nullptr)
        return widget;
    
    widget = searchInPatchManagerStandaloneWidgets(widgetId);
    if (widget != nullptr)
        return widget;
    
    widget = searchInVector(SynthDescriptors::kMasterEditStandaloneWidgets, widgetId);
    if (widget != nullptr)
        return widget;
    
    return nullptr;
}

const SynthDescriptors::GroupDescriptor* WidgetFactory::findGroup(const juce::String& groupId)
{
    for (const auto& group : SynthDescriptors::kAllGroups)
    {
        if (group.groupId == groupId)
        {
            return &group;
        }
    }
    
    return nullptr;
}

const SynthDescriptors::IntParameterDescriptor* WidgetFactory::searchInPatchEditModuleIntParameters(const juce::String& parameterId)
{
    const auto* param = searchInVector(SynthDescriptors::kDco1IntParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kDco2IntParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kVcfVcaIntParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kFmTrackIntParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kRampPortamentoIntParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kEnv1IntParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kEnv2IntParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kEnv3IntParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kLfo1IntParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kLfo2IntParameters, parameterId);
    if (param != nullptr) return param;
    
    return nullptr;
}

const SynthDescriptors::IntParameterDescriptor* WidgetFactory::searchInMatrixModulationBusIntParameters(const juce::String& parameterId)
{
    for (size_t bus = 0; bus < static_cast<size_t>(SynthDescriptors::kMatrixModBusCount); ++bus)
    {
        const auto* param = searchInVector(SynthDescriptors::kMatrixModBusIntParameters[bus], parameterId);
        if (param != nullptr)
            return param;
    }
    return nullptr;
}

const SynthDescriptors::ChoiceParameterDescriptor* WidgetFactory::searchInPatchEditModuleChoiceParameters(const juce::String& parameterId)
{
    const auto* param = searchInVector(SynthDescriptors::kDco1ChoiceParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kDco2ChoiceParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kVcfVcaChoiceParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kFmTrackChoiceParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kRampPortamentoChoiceParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kEnv1ChoiceParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kEnv2ChoiceParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kEnv3ChoiceParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kLfo1ChoiceParameters, parameterId);
    if (param != nullptr) return param;
    
    param = searchInVector(SynthDescriptors::kLfo2ChoiceParameters, parameterId);
    if (param != nullptr) return param;
    
    return nullptr;
}

const SynthDescriptors::ChoiceParameterDescriptor* WidgetFactory::searchInMatrixModulationBusChoiceParameters(const juce::String& parameterId)
{
    for (size_t bus = 0; bus < static_cast<size_t>(SynthDescriptors::kMatrixModBusCount); ++bus)
    {
        const auto* param = searchInVector(SynthDescriptors::kMatrixModBusChoiceParameters[bus], parameterId);
        if (param != nullptr)
            return param;
    }
    return nullptr;
}

const SynthDescriptors::StandaloneWidgetDescriptor* WidgetFactory::searchInPatchEditStandaloneWidgets(const juce::String& widgetId)
{
    const auto* widget = searchInVector(SynthDescriptors::kDcoStandaloneWidgets, widgetId);
    if (widget != nullptr) return widget;
    
    widget = searchInVector(SynthDescriptors::kEnvStandaloneWidgets, widgetId);
    if (widget != nullptr) return widget;
    
    widget = searchInVector(SynthDescriptors::kLfoStandaloneWidgets, widgetId);
    if (widget != nullptr) return widget;
    
    return nullptr;
}

const SynthDescriptors::StandaloneWidgetDescriptor* WidgetFactory::searchInPatchManagerStandaloneWidgets(const juce::String& widgetId)
{
    const auto* widget = searchInVector(SynthDescriptors::kBankUtilityWidgets, widgetId);
    if (widget != nullptr) return widget;
    
    widget = searchInVector(SynthDescriptors::kInternalPatchesWidgets, widgetId);
    if (widget != nullptr) return widget;
    
    widget = searchInVector(SynthDescriptors::kComputerPatchesWidgets, widgetId);
    if (widget != nullptr) return widget;
    
    return nullptr;
}

