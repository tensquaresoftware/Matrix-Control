#include "WidgetFactory.h"

#include "../Widgets/Slider.h"
#include "../Widgets/ComboBox.h"
#include "../Widgets/Button.h"
#include "../../Shared/SynthDescriptors.h"

WidgetFactory::WidgetFactory(juce::AudioProcessorValueTreeState& inApvts)
    : validator(inApvts)
{
    buildSearchMaps();
}

std::unique_ptr<tss::Slider> WidgetFactory::createSliderFromDescriptor(
    const SynthDescriptors::IntParameterDescriptor* desc,
    tss::Theme& theme)
{
    auto slider = std::make_unique<tss::Slider>(theme, static_cast<double>(desc->defaultValue));
    slider->setRange(static_cast<double>(desc->minValue), static_cast<double>(desc->maxValue), 1.0);
    slider->setValue(static_cast<double>(desc->defaultValue));
    return slider;
}

std::unique_ptr<tss::ComboBox> WidgetFactory::createComboBoxFromDescriptor(
    const SynthDescriptors::ChoiceParameterDescriptor* desc,
    tss::Theme& theme)
{
    auto comboBox = std::make_unique<tss::ComboBox>(theme);
    
    for (const auto& choice : desc->choices)
        comboBox->addItem(choice, comboBox->getNumItems() + 1);
    
    comboBox->setSelectedItemIndex(desc->defaultIndex);
    return comboBox;
}

std::unique_ptr<tss::Slider> WidgetFactory::createIntParameterSlider(
    const juce::String& parameterId,
    tss::Theme& theme)
{
    validator.throwIfParameterIdEmpty(parameterId);
    const auto* desc = findIntParameter(parameterId);
    validator.getIntParameterDescriptorOrThrow(desc, parameterId);
    validator.validateIntParameterValues(desc, parameterId);
    return createSliderFromDescriptor(desc, theme);
}

std::unique_ptr<tss::ComboBox> WidgetFactory::createChoiceParameterComboBox(
    const juce::String& parameterId,
    tss::Theme& theme)
{
    validator.throwIfParameterIdEmpty(parameterId);
    const auto* desc = findChoiceParameter(parameterId);
    validator.getChoiceParameterDescriptorOrThrow(desc, parameterId);
    validator.validateChoiceParameterValues(desc, parameterId);
    return createComboBoxFromDescriptor(desc, theme);
}

std::unique_ptr<tss::Button> WidgetFactory::createStandaloneButton(
    const juce::String& widgetId,
    tss::Theme& theme)
{
    validator.throwIfWidgetIdEmpty(widgetId);
    const auto* desc = findStandaloneWidget(widgetId);
    validator.getStandaloneWidgetDescriptorOrThrow(desc, widgetId);
    validator.validateWidgetType(desc, widgetId);
    
    return std::make_unique<tss::Button>(
        theme, 
        tss::Button::getDefaultWidth(), 
        desc->displayName
    );
}

juce::String WidgetFactory::getParameterDisplayName(const juce::String& parameterId) const
{
    const auto* intParam = findIntParameter(parameterId);
    if (intParam != nullptr)
        return intParam->displayName;
    
    const auto* choiceParam = findChoiceParameter(parameterId);
    if (choiceParam != nullptr)
        return choiceParam->displayName;
    
    return juce::String();
}

juce::String WidgetFactory::getGroupDisplayName(const juce::String& groupId) const
{
    return SynthDescriptors::getGroupDisplayName(groupId);
}

juce::String WidgetFactory::getStandaloneWidgetDisplayName(const juce::String& widgetId) const
{
    const auto* desc = findStandaloneWidget(widgetId);
    if (desc != nullptr)
        return desc->displayName;
    
    return juce::String();
}

const SynthDescriptors::IntParameterDescriptor* WidgetFactory::findIntParameter(const juce::String& parameterId) const
{
    auto it = intParameterMap.find(parameterId);
    return (it != intParameterMap.end()) ? it->second : nullptr;
}

const SynthDescriptors::ChoiceParameterDescriptor* WidgetFactory::findChoiceParameter(const juce::String& parameterId) const
{
    auto it = choiceParameterMap.find(parameterId);
    return (it != choiceParameterMap.end()) ? it->second : nullptr;
}

const SynthDescriptors::StandaloneWidgetDescriptor* WidgetFactory::findStandaloneWidget(const juce::String& widgetId) const
{
    auto it = standaloneWidgetMap.find(widgetId);
    return (it != standaloneWidgetMap.end()) ? it->second : nullptr;
}

void WidgetFactory::buildGroupMap()
{
    for (const auto& group : SynthDescriptors::kAllApvtsGroups)
        groupMap[group.groupId] = &group;
}

const SynthDescriptors::ApvtsGroupDescriptor* WidgetFactory::findGroup(const juce::String& groupId) const
{
    auto it = groupMap.find(groupId);
    return (it != groupMap.end()) ? it->second : nullptr;
}

void WidgetFactory::buildSearchMaps()
{
    buildIntParameterMap();
    buildChoiceParameterMap();
    buildStandaloneWidgetMap();
    buildGroupMap();
}

void WidgetFactory::buildIntParameterMap()
{
    addPatchEditModuleIntParametersToMap();
    addMatrixModulationBusIntParametersToMap();
    addMasterEditIntParametersToMap();
}

void WidgetFactory::buildChoiceParameterMap()
{
    addPatchEditModuleChoiceParametersToMap();
    addMatrixModulationBusChoiceParametersToMap();
    addMasterEditChoiceParametersToMap();
}

void WidgetFactory::buildStandaloneWidgetMap()
{
    addPatchEditStandaloneWidgetsToMap();
    addPatchManagerStandaloneWidgetsToMap();
    addMasterEditStandaloneWidgetsToMap();
}

void WidgetFactory::addIntParametersToMap(const std::vector<SynthDescriptors::IntParameterDescriptor>& parameters)
{
    for (const auto& param : parameters)
        intParameterMap[param.parameterId] = &param;
}

void WidgetFactory::addPatchEditModuleIntParametersToMap()
{
    addIntParametersToMap(SynthDescriptors::kDco1IntParameters);
    addIntParametersToMap(SynthDescriptors::kDco2IntParameters);
    addIntParametersToMap(SynthDescriptors::kVcfVcaIntParameters);
    addIntParametersToMap(SynthDescriptors::kFmTrackIntParameters);
    addIntParametersToMap(SynthDescriptors::kRampPortamentoIntParameters);
    addIntParametersToMap(SynthDescriptors::kEnv1IntParameters);
    addIntParametersToMap(SynthDescriptors::kEnv2IntParameters);
    addIntParametersToMap(SynthDescriptors::kEnv3IntParameters);
    addIntParametersToMap(SynthDescriptors::kLfo1IntParameters);
    addIntParametersToMap(SynthDescriptors::kLfo2IntParameters);
}

void WidgetFactory::addMatrixModulationBusIntParametersToMap()
{
    for (int bus = 0; bus < SynthDescriptors::kModulationBusCount; ++bus)
        addIntParametersToMap(SynthDescriptors::kModulationBusIntParameters[static_cast<size_t>(bus)]);
}

void WidgetFactory::addMasterEditIntParametersToMap()
{
    addIntParametersToMap(SynthDescriptors::kMasterEditIntParameters);
}

void WidgetFactory::addChoiceParametersToMap(const std::vector<SynthDescriptors::ChoiceParameterDescriptor>& parameters)
{
    for (const auto& param : parameters)
        choiceParameterMap[param.parameterId] = &param;
}

void WidgetFactory::addPatchEditModuleChoiceParametersToMap()
{
    addChoiceParametersToMap(SynthDescriptors::kDco1ChoiceParameters);
    addChoiceParametersToMap(SynthDescriptors::kDco2ChoiceParameters);
    addChoiceParametersToMap(SynthDescriptors::kVcfVcaChoiceParameters);
    addChoiceParametersToMap(SynthDescriptors::kFmTrackChoiceParameters);
    addChoiceParametersToMap(SynthDescriptors::kRampPortamentoChoiceParameters);
    addChoiceParametersToMap(SynthDescriptors::kEnv1ChoiceParameters);
    addChoiceParametersToMap(SynthDescriptors::kEnv2ChoiceParameters);
    addChoiceParametersToMap(SynthDescriptors::kEnv3ChoiceParameters);
    addChoiceParametersToMap(SynthDescriptors::kLfo1ChoiceParameters);
    addChoiceParametersToMap(SynthDescriptors::kLfo2ChoiceParameters);
}

void WidgetFactory::addMatrixModulationBusChoiceParametersToMap()
{
    for (int bus = 0; bus < SynthDescriptors::kModulationBusCount; ++bus)
        addChoiceParametersToMap(SynthDescriptors::kModulationBusChoiceParameters[static_cast<size_t>(bus)]);
}

void WidgetFactory::addMasterEditChoiceParametersToMap()
{
    addChoiceParametersToMap(SynthDescriptors::kMasterEditChoiceParameters);
}

void WidgetFactory::addStandaloneWidgetsToMap(const std::vector<SynthDescriptors::StandaloneWidgetDescriptor>& widgets)
{
    for (const auto& widget : widgets)
        standaloneWidgetMap[widget.widgetId] = &widget;
}

void WidgetFactory::addPatchEditStandaloneWidgetsToMap()
{
    addStandaloneWidgetsToMap(SynthDescriptors::kDco1StandaloneWidgets);
    addStandaloneWidgetsToMap(SynthDescriptors::kDco2StandaloneWidgets);
    addStandaloneWidgetsToMap(SynthDescriptors::kVcfVcaStandaloneWidgets);
    addStandaloneWidgetsToMap(SynthDescriptors::kFmTrackStandaloneWidgets);
    addStandaloneWidgetsToMap(SynthDescriptors::kRampPortamentoStandaloneWidgets);
    addStandaloneWidgetsToMap(SynthDescriptors::kEnv1StandaloneWidgets);
    addStandaloneWidgetsToMap(SynthDescriptors::kEnv2StandaloneWidgets);
    addStandaloneWidgetsToMap(SynthDescriptors::kEnv3StandaloneWidgets);
    addStandaloneWidgetsToMap(SynthDescriptors::kLfo1StandaloneWidgets);
    addStandaloneWidgetsToMap(SynthDescriptors::kLfo2StandaloneWidgets);
}

void WidgetFactory::addPatchManagerStandaloneWidgetsToMap()
{
    addStandaloneWidgetsToMap(SynthDescriptors::kBankUtilityWidgets);
    addStandaloneWidgetsToMap(SynthDescriptors::kInternalPatchesWidgets);
    addStandaloneWidgetsToMap(SynthDescriptors::kComputerPatchesWidgets);
}

void WidgetFactory::addMasterEditStandaloneWidgetsToMap()
{
    addStandaloneWidgetsToMap(SynthDescriptors::kMasterEditStandaloneWidgets);
}

