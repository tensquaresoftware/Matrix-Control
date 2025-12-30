#include "ApvtsFactory.h"

juce::AudioProcessorValueTreeState::ParameterLayout ApvtsFactory::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout parameterLayout;
    
    const auto allGroups = SynthDescriptors::getAllGroups();
    std::map<juce::String, std::unique_ptr<juce::AudioProcessorParameterGroup>> groupMap;
    
    createRootGroups(allGroups, groupMap);
    createChildGroups(allGroups, groupMap);
    addRootGroupsToLayout(groupMap, parameterLayout);
    
    return parameterLayout;
}

void ApvtsFactory::createRootGroups(
    const std::vector<SynthDescriptors::GroupDescriptor>& allGroups,
    std::map<juce::String, std::unique_ptr<juce::AudioProcessorParameterGroup>>& groupMap)
{
    for (const auto& group : allGroups)
    {
        if (group.parentId.isEmpty())
        {
            groupMap[group.groupId] = std::make_unique<juce::AudioProcessorParameterGroup>(
                group.groupId, group.displayName, kSubgroupSeparator
            );
        }
    }
}

void ApvtsFactory::createChildGroups(
    const std::vector<SynthDescriptors::GroupDescriptor>& allGroups,
    std::map<juce::String, std::unique_ptr<juce::AudioProcessorParameterGroup>>& groupMap)
{
    for (const auto& group : allGroups)
    {
        if (group.parentId.isEmpty() || groupMap.find(group.parentId) == groupMap.end())
            continue;
        
        auto childGroup = std::make_unique<juce::AudioProcessorParameterGroup>(
            group.groupId, group.displayName, kSubgroupSeparator
        );
        
        addParametersToChildGroup(group.groupId, *childGroup);
        
        groupMap[group.parentId]->addChild(std::move(childGroup));
    }
}

void ApvtsFactory::addParametersToChildGroup(
    const juce::String& childGroupId,
    juce::AudioProcessorParameterGroup& childGroup)
{
    if (childGroupId == SynthDescriptors::SectionIds::kPatchEdit)
    {
        addPatchEditParameters(childGroup);
    }
    else if (childGroupId == SynthDescriptors::SectionIds::kMatrixModulation)
    {
        addMatrixModulationParameters(childGroup);
    }
    else if (childGroupId == SynthDescriptors::SectionIds::kPatchManager)
    {
        // Patch Manager uses ValueTree properties, not APVTS parameters
    }
    // Master Edit skipped for now
}

void ApvtsFactory::addRootGroupsToLayout(
    std::map<juce::String, std::unique_ptr<juce::AudioProcessorParameterGroup>>& groupMap,
    juce::AudioProcessorValueTreeState::ParameterLayout& parameterLayout)
{
    for (auto& [id, group] : groupMap)
    {
        if (group->getParent() == nullptr)
        {
            parameterLayout.add(std::move(group));
        }
    }
}

void ApvtsFactory::addIntParameter(
    juce::AudioProcessorParameterGroup& group,
    const SynthDescriptors::IntParameterDescriptor& desc)
{
    auto param = std::make_unique<juce::AudioParameterInt>(
        desc.parameterId,
        desc.displayName,
        desc.minValue,
        desc.maxValue,
        desc.defaultValue
    );
    group.addChild(std::move(param));
}

void ApvtsFactory::addChoiceParameter(
    juce::AudioProcessorParameterGroup& group,
    const SynthDescriptors::ChoiceParameterDescriptor& desc)
{
    auto param = std::make_unique<juce::AudioParameterChoice>(
        desc.parameterId,
        desc.displayName,
        desc.choices,
        desc.defaultIndex
    );
    group.addChild(std::move(param));
}

juce::String ApvtsFactory::getGroupDisplayName(const juce::String& groupId)
{
    const auto allGroups = SynthDescriptors::getAllGroups();
    for (const auto& group : allGroups)
    {
        if (group.groupId == groupId)
        {
            return group.displayName;
        }
    }
    return groupId;  // Fallback to ID if not found
}

const char* ApvtsFactory::getBusId(int busNumber)
{
    static constexpr const char* busIds[] =
    {
        SynthDescriptors::BusIds::kMatrixModBus0,
        SynthDescriptors::BusIds::kMatrixModBus1,
        SynthDescriptors::BusIds::kMatrixModBus2,
        SynthDescriptors::BusIds::kMatrixModBus3,
        SynthDescriptors::BusIds::kMatrixModBus4,
        SynthDescriptors::BusIds::kMatrixModBus5,
        SynthDescriptors::BusIds::kMatrixModBus6,
        SynthDescriptors::BusIds::kMatrixModBus7,
        SynthDescriptors::BusIds::kMatrixModBus8,
        SynthDescriptors::BusIds::kMatrixModBus9
    };
    
    jassert(busNumber >= 0 && busNumber < 10);
    return busIds[busNumber];
}

void ApvtsFactory::addModuleParameters(
    juce::AudioProcessorParameterGroup& parentGroup,
    const char* moduleId,
    const char* moduleDisplayName,
    const std::vector<SynthDescriptors::IntParameterDescriptor>& intParams,
    const std::vector<SynthDescriptors::ChoiceParameterDescriptor>& choiceParams)
{
    auto moduleGroup = std::make_unique<juce::AudioProcessorParameterGroup>(
        moduleId, moduleDisplayName, kSubgroupSeparator
    );
    
    for (const auto& paramDesc : intParams)
    {
        ApvtsFactory::addIntParameter(*moduleGroup, paramDesc);
    }
    
    for (const auto& paramDesc : choiceParams)
    {
        ApvtsFactory::addChoiceParameter(*moduleGroup, paramDesc);
    }
    
    parentGroup.addChild(std::move(moduleGroup));
}

void ApvtsFactory::addPatchEditParameters(juce::AudioProcessorParameterGroup& patchEditGroup)
{
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kDco1,
        getGroupDisplayName(SynthDescriptors::ModuleIds::kDco1).toRawUTF8(),
        SynthDescriptors::getDco1IntParameters(),
        SynthDescriptors::getDco1ChoiceParameters()
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kDco2,
        getGroupDisplayName(SynthDescriptors::ModuleIds::kDco2).toRawUTF8(),
        SynthDescriptors::getDco2IntParameters(),
        SynthDescriptors::getDco2ChoiceParameters()
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kVcfVca,
        getGroupDisplayName(SynthDescriptors::ModuleIds::kVcfVca).toRawUTF8(),
        SynthDescriptors::getVcfVcaIntParameters(),
        SynthDescriptors::getVcfVcaChoiceParameters()
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kFmTrack,
        getGroupDisplayName(SynthDescriptors::ModuleIds::kFmTrack).toRawUTF8(),
        SynthDescriptors::getFmTrackIntParameters(),
        SynthDescriptors::getFmTrackChoiceParameters()
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kRampPortamento,
        getGroupDisplayName(SynthDescriptors::ModuleIds::kRampPortamento).toRawUTF8(),
        SynthDescriptors::getRampPortamentoIntParameters(),
        SynthDescriptors::getRampPortamentoChoiceParameters()
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kEnvelope1,
        getGroupDisplayName(SynthDescriptors::ModuleIds::kEnvelope1).toRawUTF8(),
        SynthDescriptors::getEnv1IntParameters(),
        SynthDescriptors::getEnv1ChoiceParameters()
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kEnvelope2,
        getGroupDisplayName(SynthDescriptors::ModuleIds::kEnvelope2).toRawUTF8(),
        SynthDescriptors::getEnv2IntParameters(),
        SynthDescriptors::getEnv2ChoiceParameters()
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kEnvelope3,
        getGroupDisplayName(SynthDescriptors::ModuleIds::kEnvelope3).toRawUTF8(),
        SynthDescriptors::getEnv3IntParameters(),
        SynthDescriptors::getEnv3ChoiceParameters()
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kLfo1,
        getGroupDisplayName(SynthDescriptors::ModuleIds::kLfo1).toRawUTF8(),
        SynthDescriptors::getLfo1IntParameters(),
        SynthDescriptors::getLfo1ChoiceParameters()
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kLfo2,
        getGroupDisplayName(SynthDescriptors::ModuleIds::kLfo2).toRawUTF8(),
        SynthDescriptors::getLfo2IntParameters(),
        SynthDescriptors::getLfo2ChoiceParameters()
    );
}

void ApvtsFactory::addMatrixModulationParameters(juce::AudioProcessorParameterGroup& matrixModulationGroup)
{
    for (int bus = 0; bus < 10; ++bus)
    {
        const juce::String busId = getBusId(bus);
        const juce::String busDisplayName = getGroupDisplayName(busId);
        
        auto busGroup = std::make_unique<juce::AudioProcessorParameterGroup>(
            busId, busDisplayName, kSubgroupSeparator
        );
        
        for (const auto& paramDesc : SynthDescriptors::getMatrixModBusIntParameters(bus))
        {
            ApvtsFactory::addIntParameter(*busGroup, paramDesc);
        }
        
        for (const auto& paramDesc : SynthDescriptors::getMatrixModBusChoiceParameters(bus))
        {
            ApvtsFactory::addChoiceParameter(*busGroup, paramDesc);
        }
        
        matrixModulationGroup.addChild(std::move(busGroup));
    }
}

