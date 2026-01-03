#include "ApvtsFactory.h"

#include <set>
#include <functional>

juce::AudioProcessorValueTreeState::ParameterLayout ApvtsFactory::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout parameterLayout;
    
    const auto& allGroups = SynthDescriptors::kAllGroups;
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
        juce::ParameterID(desc.parameterId, 1),
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
        juce::ParameterID(desc.parameterId, 1),
        desc.displayName,
        desc.choices,
        desc.defaultIndex
    );
    group.addChild(std::move(param));
}

std::vector<SynthDescriptors::IntParameterDescriptor> ApvtsFactory::getAllIntParameters()
{
    std::vector<SynthDescriptors::IntParameterDescriptor> allParams;

    auto addParams = [&allParams](const std::vector<SynthDescriptors::IntParameterDescriptor>& params)
    {
        allParams.insert(allParams.end(), params.begin(), params.end());
    };

    addParams(SynthDescriptors::kDco1IntParameters);
    addParams(SynthDescriptors::kDco2IntParameters);
    addParams(SynthDescriptors::kVcfVcaIntParameters);
    addParams(SynthDescriptors::kFmTrackIntParameters);
    addParams(SynthDescriptors::kRampPortamentoIntParameters);
    addParams(SynthDescriptors::kEnv1IntParameters);
    addParams(SynthDescriptors::kEnv2IntParameters);
    addParams(SynthDescriptors::kEnv3IntParameters);
    addParams(SynthDescriptors::kLfo1IntParameters);
    addParams(SynthDescriptors::kLfo2IntParameters);

    addMatrixModulationBusIntParameters(allParams);

    addParams(SynthDescriptors::kMasterEditIntParameters);

    return allParams;
}

std::vector<SynthDescriptors::ChoiceParameterDescriptor> ApvtsFactory::getAllChoiceParameters()
{
    std::vector<SynthDescriptors::ChoiceParameterDescriptor> allParams;

    auto addParams = [&allParams](const std::vector<SynthDescriptors::ChoiceParameterDescriptor>& params)
    {
        allParams.insert(allParams.end(), params.begin(), params.end());
    };

    addParams(SynthDescriptors::kDco1ChoiceParameters);
    addParams(SynthDescriptors::kDco2ChoiceParameters);
    addParams(SynthDescriptors::kVcfVcaChoiceParameters);
    addParams(SynthDescriptors::kFmTrackChoiceParameters);
    addParams(SynthDescriptors::kRampPortamentoChoiceParameters);
    addParams(SynthDescriptors::kEnv1ChoiceParameters);
    addParams(SynthDescriptors::kEnv2ChoiceParameters);
    addParams(SynthDescriptors::kEnv3ChoiceParameters);
    addParams(SynthDescriptors::kLfo1ChoiceParameters);
    addParams(SynthDescriptors::kLfo2ChoiceParameters);

    addMatrixModulationBusChoiceParameters(allParams);

    addParams(SynthDescriptors::kMasterEditChoiceParameters);

    return allParams;
}

std::vector<SynthDescriptors::StandaloneWidgetDescriptor> ApvtsFactory::getAllStandaloneWidgets()
{
    std::vector<SynthDescriptors::StandaloneWidgetDescriptor> allWidgets;

    auto addWidgets = [&allWidgets](const std::vector<SynthDescriptors::StandaloneWidgetDescriptor>& widgets)
    {
        allWidgets.insert(allWidgets.end(), widgets.begin(), widgets.end());
    };

    addWidgets(SynthDescriptors::kDcoStandaloneWidgets);
    addWidgets(SynthDescriptors::kEnvStandaloneWidgets);
    addWidgets(SynthDescriptors::kLfoStandaloneWidgets);
    addWidgets(SynthDescriptors::kBankUtilityWidgets);
    addWidgets(SynthDescriptors::kInternalPatchesWidgets);
    addWidgets(SynthDescriptors::kComputerPatchesWidgets);
    addWidgets(SynthDescriptors::kMasterEditStandaloneWidgets);

    return allWidgets;
}

const char* ApvtsFactory::getBusId(int busNumber)
{
    static constexpr const char* busIds[] =
    {
        SynthDescriptors::MatrixModBusIds::kMatrixModBus0,
        SynthDescriptors::MatrixModBusIds::kMatrixModBus1,
        SynthDescriptors::MatrixModBusIds::kMatrixModBus2,
        SynthDescriptors::MatrixModBusIds::kMatrixModBus3,
        SynthDescriptors::MatrixModBusIds::kMatrixModBus4,
        SynthDescriptors::MatrixModBusIds::kMatrixModBus5,
        SynthDescriptors::MatrixModBusIds::kMatrixModBus6,
        SynthDescriptors::MatrixModBusIds::kMatrixModBus7,
        SynthDescriptors::MatrixModBusIds::kMatrixModBus8,
        SynthDescriptors::MatrixModBusIds::kMatrixModBus9
    };
    
    jassert(busNumber >= 0 && busNumber < SynthDescriptors::kMatrixModBusCount);
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
        SynthDescriptors::getGroupDisplayName(SynthDescriptors::ModuleIds::kDco1).toRawUTF8(),
        SynthDescriptors::kDco1IntParameters,
        SynthDescriptors::kDco1ChoiceParameters
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kDco2,
        SynthDescriptors::getGroupDisplayName(SynthDescriptors::ModuleIds::kDco2).toRawUTF8(),
        SynthDescriptors::kDco2IntParameters,
        SynthDescriptors::kDco2ChoiceParameters
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kVcfVca,
        SynthDescriptors::getGroupDisplayName(SynthDescriptors::ModuleIds::kVcfVca).toRawUTF8(),
        SynthDescriptors::kVcfVcaIntParameters,
        SynthDescriptors::kVcfVcaChoiceParameters
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kFmTrack,
        SynthDescriptors::getGroupDisplayName(SynthDescriptors::ModuleIds::kFmTrack).toRawUTF8(),
        SynthDescriptors::kFmTrackIntParameters,
        SynthDescriptors::kFmTrackChoiceParameters
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kRampPortamento,
        SynthDescriptors::getGroupDisplayName(SynthDescriptors::ModuleIds::kRampPortamento).toRawUTF8(),
        SynthDescriptors::kRampPortamentoIntParameters,
        SynthDescriptors::kRampPortamentoChoiceParameters
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kEnvelope1,
        SynthDescriptors::getGroupDisplayName(SynthDescriptors::ModuleIds::kEnvelope1).toRawUTF8(),
        SynthDescriptors::kEnv1IntParameters,
        SynthDescriptors::kEnv1ChoiceParameters
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kEnvelope2,
        SynthDescriptors::getGroupDisplayName(SynthDescriptors::ModuleIds::kEnvelope2).toRawUTF8(),
        SynthDescriptors::kEnv2IntParameters,
        SynthDescriptors::kEnv2ChoiceParameters
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kEnvelope3,
        SynthDescriptors::getGroupDisplayName(SynthDescriptors::ModuleIds::kEnvelope3).toRawUTF8(),
        SynthDescriptors::kEnv3IntParameters,
        SynthDescriptors::kEnv3ChoiceParameters
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kLfo1,
        SynthDescriptors::getGroupDisplayName(SynthDescriptors::ModuleIds::kLfo1).toRawUTF8(),
        SynthDescriptors::kLfo1IntParameters,
        SynthDescriptors::kLfo1ChoiceParameters
    );
    
    addModuleParameters(
        patchEditGroup,
        SynthDescriptors::ModuleIds::kLfo2,
        SynthDescriptors::getGroupDisplayName(SynthDescriptors::ModuleIds::kLfo2).toRawUTF8(),
        SynthDescriptors::kLfo2IntParameters,
        SynthDescriptors::kLfo2ChoiceParameters
    );
}

void ApvtsFactory::addMatrixModulationParameters(juce::AudioProcessorParameterGroup& matrixModulationGroup)
{
    for (int bus = 0; bus < SynthDescriptors::kMatrixModBusCount; ++bus)
    {
        const juce::String busId = getBusId(bus);
        const juce::String busDisplayName = SynthDescriptors::getGroupDisplayName(busId);
        
        auto busGroup = std::make_unique<juce::AudioProcessorParameterGroup>(
            busId, busDisplayName, kSubgroupSeparator
        );
        
        for (const auto& paramDesc : SynthDescriptors::kMatrixModBusIntParameters[static_cast<size_t>(bus)])
        {
            ApvtsFactory::addIntParameter(*busGroup, paramDesc);
        }
        
        for (const auto& paramDesc : SynthDescriptors::kMatrixModBusChoiceParameters[static_cast<size_t>(bus)])
        {
            ApvtsFactory::addChoiceParameter(*busGroup, paramDesc);
        }
        
        matrixModulationGroup.addChild(std::move(busGroup));
    }
}

void ApvtsFactory::addMatrixModulationBusIntParameters(
    std::vector<SynthDescriptors::IntParameterDescriptor>& allParams)
{
    for (int bus = 0; bus < SynthDescriptors::kMatrixModBusCount; ++bus)
    {
        allParams.insert(allParams.end(),
                        SynthDescriptors::kMatrixModBusIntParameters[static_cast<size_t>(bus)].begin(),
                        SynthDescriptors::kMatrixModBusIntParameters[static_cast<size_t>(bus)].end());
    }
}

void ApvtsFactory::addMatrixModulationBusChoiceParameters(
    std::vector<SynthDescriptors::ChoiceParameterDescriptor>& allParams)
{
    for (int bus = 0; bus < SynthDescriptors::kMatrixModBusCount; ++bus)
    {
        allParams.insert(allParams.end(),
                        SynthDescriptors::kMatrixModBusChoiceParameters[static_cast<size_t>(bus)].begin(),
                        SynthDescriptors::kMatrixModBusChoiceParameters[static_cast<size_t>(bus)].end());
    }
}

void ApvtsFactory::addValidationErrorsToResult(
    const juce::StringArray& errors,
    ValidationResult& result)
{
    if (!errors.isEmpty())
    {
        result.isValid = false;
        result.errors.addArray(errors);
    }
}

// ============================================================================
// Validation Implementation
// ============================================================================

ApvtsFactory::ValidationResult ApvtsFactory::validateSynthDescriptors()
{
    ValidationResult result;
    
    addValidationErrorsToResult(validateGroups(), result);
    addValidationErrorsToResult(validateParameters(), result);
    addValidationErrorsToResult(validateStandaloneWidgets(), result);
    addValidationErrorsToResult(checkForDuplicateIds(), result);
    addValidationErrorsToResult(checkForOrphanedReferences(), result);
    addValidationErrorsToResult(checkForCircularReferences(), result);
    
    return result;
}

juce::StringArray ApvtsFactory::validateGroups()
{
    juce::StringArray errors;
    const auto& allGroups = SynthDescriptors::kAllGroups;
    std::set<juce::String> seenIds;
    
    for (const auto& group : allGroups)
    {
        checkGroupIdUniqueness(group, seenIds, errors);
        checkGroupParentExists(group, allGroups, errors);
        checkGroupDisplayNameNotEmpty(group, errors);
    }
    
    return errors;
}

juce::StringArray ApvtsFactory::validateParameters()
{
    juce::StringArray errors;
    auto allIntParams = getAllIntParameters();
    auto allChoiceParams = getAllChoiceParameters();
    std::set<juce::String> seenParamIds;
    
    for (const auto& param : allIntParams)
    {
        checkParameterIdUniqueness(param.parameterId, seenParamIds, errors);
        checkIntParameterParentGroupExists(param, errors);
        checkIntParameterDisplayNameNotEmpty(param, errors);
        checkIntParameterValueRange(param, errors);
    }
    
    for (const auto& param : allChoiceParams)
    {
        checkParameterIdUniqueness(param.parameterId, seenParamIds, errors);
        checkChoiceParameterParentGroupExists(param, errors);
        checkChoiceParameterDisplayNameNotEmpty(param, errors);
        checkChoiceParameterChoicesNotEmpty(param, errors);
        checkChoiceParameterDefaultIndexValid(param, errors);
    }
    
    return errors;
}

juce::StringArray ApvtsFactory::validateStandaloneWidgets()
{
    juce::StringArray errors;
    auto allWidgets = getAllStandaloneWidgets();
    std::set<juce::String> seenWidgetIds;
    
    for (const auto& widget : allWidgets)
    {
        checkWidgetIdUniqueness(widget, seenWidgetIds, errors);
        checkWidgetParentGroupExists(widget, errors);
    }
    
    return errors;
}

juce::StringArray ApvtsFactory::checkForDuplicateIds()
{
    juce::StringArray errors;
    auto groupIds = collectAllGroupIds();
    auto paramIds = collectAllParameterIds();
    auto widgetIds = collectAllWidgetIds();
    std::set<juce::String> allIds;
    
    checkGroupIdCollisions(groupIds, allIds, errors);
    checkParameterIdCollisions(paramIds, allIds, errors);
    checkWidgetIdCollisions(widgetIds, allIds, errors);
    
    return errors;
}

juce::StringArray ApvtsFactory::checkForOrphanedReferences()
{
    juce::StringArray errors;
    const auto& allGroups = SynthDescriptors::kAllGroups;
    std::set<juce::String> definedGroupIds = buildDefinedGroupIdsSet(allGroups);
    
    checkParentIdReferencesPointToDefinedGroups(allGroups, definedGroupIds, errors);
    
    return errors;
}

juce::StringArray ApvtsFactory::checkForCircularReferences()
{
    juce::StringArray errors;
    const auto& allGroups = SynthDescriptors::kAllGroups;
    std::map<juce::String, juce::String> parentMap = buildGroupParentMap(allGroups);
    
    detectCyclesInGroupHierarchy(allGroups, parentMap, errors);
    
    return errors;
}

void ApvtsFactory::checkWidgetIdUniqueness(
    const SynthDescriptors::StandaloneWidgetDescriptor& widget,
    std::set<juce::String>& seenWidgetIds,
    juce::StringArray& errors)
{
    if (seenWidgetIds.find(widget.widgetId) != seenWidgetIds.end())
    {
        errors.add("Duplicate widget ID: " + widget.widgetId);
    }
    seenWidgetIds.insert(widget.widgetId);
}

void ApvtsFactory::checkWidgetParentGroupExists(
    const SynthDescriptors::StandaloneWidgetDescriptor& widget,
    juce::StringArray& errors)
{
    if (!groupExists(widget.parentGroupId))
    {
        errors.add("Standalone widget '" + widget.widgetId + "' references non-existent group: " + widget.parentGroupId);
    }
}

void ApvtsFactory::checkGroupIdUniqueness(
    const SynthDescriptors::GroupDescriptor& group,
    std::set<juce::String>& seenIds,
    juce::StringArray& errors)
{
    if (seenIds.find(group.groupId) != seenIds.end())
    {
        errors.add("Duplicate group ID: " + group.groupId);
    }
    seenIds.insert(group.groupId);
}

void ApvtsFactory::checkGroupParentExists(
    const SynthDescriptors::GroupDescriptor& group,
    const std::vector<SynthDescriptors::GroupDescriptor>& allGroups,
    juce::StringArray& errors)
{
    if (group.parentId.isEmpty())
        return;
    
    for (const auto& otherGroup : allGroups)
    {
        if (otherGroup.groupId == group.parentId)
            return;
    }
    
    errors.add("Group '" + group.groupId + "' references non-existent parent: " + group.parentId);
}

void ApvtsFactory::checkGroupDisplayNameNotEmpty(
    const SynthDescriptors::GroupDescriptor& group,
    juce::StringArray& errors)
{
    if (group.displayName.isEmpty())
    {
        errors.add("Group '" + group.groupId + "' has empty display name");
    }
}

void ApvtsFactory::checkParameterIdUniqueness(
    const juce::String& parameterId,
    std::set<juce::String>& seenParamIds,
    juce::StringArray& errors)
{
    if (seenParamIds.find(parameterId) != seenParamIds.end())
    {
        errors.add("Duplicate parameter ID: " + parameterId);
    }
    seenParamIds.insert(parameterId);
}

void ApvtsFactory::checkIntParameterParentGroupExists(
    const SynthDescriptors::IntParameterDescriptor& param,
    juce::StringArray& errors)
{
    if (!groupExists(param.parentGroupId))
    {
        errors.add("Int parameter '" + param.parameterId + "' references non-existent group: " + param.parentGroupId);
    }
}

void ApvtsFactory::checkIntParameterDisplayNameNotEmpty(
    const SynthDescriptors::IntParameterDescriptor& param,
    juce::StringArray& errors)
{
    if (param.displayName.isEmpty())
    {
        errors.add("Int parameter '" + param.parameterId + "' has empty display name");
    }
}

void ApvtsFactory::checkIntParameterValueRange(
    const SynthDescriptors::IntParameterDescriptor& param,
    juce::StringArray& errors)
{
    if (param.minValue > param.maxValue)
    {
        errors.add("Int parameter '" + param.parameterId + "' has minValue > maxValue");
    }
    
    if (param.defaultValue < param.minValue || param.defaultValue > param.maxValue)
    {
        errors.add("Int parameter '" + param.parameterId + "' has defaultValue outside [minValue, maxValue]");
    }
}

void ApvtsFactory::checkChoiceParameterParentGroupExists(
    const SynthDescriptors::ChoiceParameterDescriptor& param,
    juce::StringArray& errors)
{
    if (!groupExists(param.parentGroupId))
    {
        errors.add("Choice parameter '" + param.parameterId + "' references non-existent group: " + param.parentGroupId);
    }
}

void ApvtsFactory::checkChoiceParameterDisplayNameNotEmpty(
    const SynthDescriptors::ChoiceParameterDescriptor& param,
    juce::StringArray& errors)
{
    if (param.displayName.isEmpty())
    {
        errors.add("Choice parameter '" + param.parameterId + "' has empty display name");
    }
}

void ApvtsFactory::checkChoiceParameterChoicesNotEmpty(
    const SynthDescriptors::ChoiceParameterDescriptor& param,
    juce::StringArray& errors)
{
    if (param.choices.isEmpty())
    {
        errors.add("Choice parameter '" + param.parameterId + "' has empty choices list");
    }
}

void ApvtsFactory::checkChoiceParameterDefaultIndexValid(
    const SynthDescriptors::ChoiceParameterDescriptor& param,
    juce::StringArray& errors)
{
    if (param.defaultIndex < 0 || param.defaultIndex >= param.choices.size())
    {
        errors.add("Choice parameter '" + param.parameterId + "' has invalid defaultIndex: " + 
                  juce::String(param.defaultIndex) + " (choices size: " + juce::String(param.choices.size()) + ")");
    }
}

void ApvtsFactory::checkGroupIdCollisions(
    const juce::StringArray& groupIds,
    std::set<juce::String>& allIds,
    juce::StringArray& errors)
{
    for (const auto& id : groupIds)
    {
        if (allIds.find(id) != allIds.end())
        {
            errors.add("ID collision: '" + id + "' is used as both group ID and another type");
        }
        allIds.insert(id);
    }
}

void ApvtsFactory::checkParameterIdCollisions(
    const juce::StringArray& paramIds,
    std::set<juce::String>& allIds,
    juce::StringArray& errors)
{
    for (const auto& id : paramIds)
    {
        if (allIds.find(id) != allIds.end())
        {
            errors.add("ID collision: '" + id + "' is used as both parameter ID and another type");
        }
        allIds.insert(id);
    }
}

void ApvtsFactory::checkWidgetIdCollisions(
    const juce::StringArray& widgetIds,
    std::set<juce::String>& allIds,
    juce::StringArray& errors)
{
    for (const auto& id : widgetIds)
    {
        if (allIds.find(id) != allIds.end())
        {
            errors.add("ID collision: '" + id + "' is used as both widget ID and another type");
        }
        allIds.insert(id);
    }
}

std::set<juce::String> ApvtsFactory::buildDefinedGroupIdsSet(
    const std::vector<SynthDescriptors::GroupDescriptor>& allGroups)
{
    std::set<juce::String> definedGroupIds;
    for (const auto& group : allGroups)
    {
        definedGroupIds.insert(group.groupId);
    }
    return definedGroupIds;
}

void ApvtsFactory::checkParentIdReferencesPointToDefinedGroups(
    const std::vector<SynthDescriptors::GroupDescriptor>& allGroups,
    const std::set<juce::String>& definedGroupIds,
    juce::StringArray& errors)
{
    for (const auto& group : allGroups)
    {
        if (!group.parentId.isEmpty() && definedGroupIds.find(group.parentId) == definedGroupIds.end())
        {
            errors.add("Orphaned group reference: '" + group.groupId + "' references undefined parent: " + group.parentId);
        }
    }
}

std::map<juce::String, juce::String> ApvtsFactory::buildGroupParentMap(
    const std::vector<SynthDescriptors::GroupDescriptor>& allGroups)
{
    std::map<juce::String, juce::String> parentMap;
    for (const auto& group : allGroups)
    {
        if (!group.parentId.isEmpty())
        {
            parentMap[group.groupId] = group.parentId;
        }
    }
    return parentMap;
}

void ApvtsFactory::detectCyclesInGroupHierarchy(
    const std::vector<SynthDescriptors::GroupDescriptor>& allGroups,
    const std::map<juce::String, juce::String>& parentMap,
    juce::StringArray& errors)
{
    std::set<juce::String> visited;
    std::set<juce::String> recursionStack;
    
    std::function<bool(const juce::String&)> hasCycle = [&](const juce::String& groupId) -> bool
    {
        if (recursionStack.find(groupId) != recursionStack.end())
            return true;
        
        if (visited.find(groupId) != visited.end())
            return false;
        
        visited.insert(groupId);
        recursionStack.insert(groupId);
        
        auto it = parentMap.find(groupId);
        if (it != parentMap.end())
        {
            if (hasCycle(it->second))
                return true;
        }
        
        recursionStack.erase(groupId);
        return false;
    };
    
    for (const auto& group : allGroups)
    {
        if (visited.find(group.groupId) == visited.end())
        {
            if (hasCycle(group.groupId))
            {
                errors.add("Circular reference detected in group hierarchy involving: " + group.groupId);
            }
        }
    }
}

// Helper methods for validation
juce::StringArray ApvtsFactory::collectAllGroupIds()
{
    juce::StringArray ids;
    for (const auto& group : SynthDescriptors::kAllGroups)
    {
        ids.add(group.groupId);
    }
    return ids;
}

juce::StringArray ApvtsFactory::collectAllParameterIds()
{
    juce::StringArray ids;
    
    auto allIntParams = getAllIntParameters();
    auto allChoiceParams = getAllChoiceParameters();
    
    for (const auto& param : allIntParams)
    {
        ids.add(param.parameterId);
    }
    
    for (const auto& param : allChoiceParams)
    {
        ids.add(param.parameterId);
    }
    
    return ids;
}

juce::StringArray ApvtsFactory::collectAllWidgetIds()
{
    juce::StringArray ids;
    
    auto allWidgets = getAllStandaloneWidgets();
    
    for (const auto& widget : allWidgets)
    {
        ids.add(widget.widgetId);
    }
    
    return ids;
}

bool ApvtsFactory::groupExists(const juce::String& groupId)
{
    for (const auto& group : SynthDescriptors::kAllGroups)
    {
        if (group.groupId == groupId)
        {
            return true;
        }
    }
    return false;
}

bool ApvtsFactory::parameterIdExists(const juce::String& parameterId)
{
    auto allIntParams = getAllIntParameters();
    auto allChoiceParams = getAllChoiceParameters();
    
    for (const auto& param : allIntParams)
    {
        if (param.parameterId == parameterId)
        {
            return true;
        }
    }
    
    for (const auto& param : allChoiceParams)
    {
        if (param.parameterId == parameterId)
        {
            return true;
        }
    }
    
    return false;
}

bool ApvtsFactory::widgetIdExists(const juce::String& widgetId)
{
    auto allWidgets = getAllStandaloneWidgets();
    
    for (const auto& widget : allWidgets)
    {
        if (widget.widgetId == widgetId)
        {
            return true;
        }
    }
    
    return false;
}

