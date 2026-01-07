#pragma once

#include <map>
#include <set>
#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>

#include "../../Shared/SynthDescriptors.h"

class ApvtsFactory
{
public:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // ============================================================================
    // Helper Methods
    // ============================================================================
    
    static std::vector<SynthDescriptors::IntParameterDescriptor> getAllIntParameters();
    static std::vector<SynthDescriptors::ChoiceParameterDescriptor> getAllChoiceParameters();
    static std::vector<SynthDescriptors::StandaloneWidgetDescriptor> getAllStandaloneWidgets();

    // ============================================================================
    // Validation Methods
    // ============================================================================
    
    struct ValidationResult
    {
        bool isValid = true;
        juce::StringArray errors;
        juce::StringArray warnings;
    };
    
    static ValidationResult validateSynthDescriptors();
    
    // Individual validation methods
    static juce::StringArray validateGroups();
    static juce::StringArray validateParameters();
    static juce::StringArray validateStandaloneWidgets();
    static juce::StringArray checkForDuplicateIds();
    static juce::StringArray checkForOrphanedReferences();
    static juce::StringArray checkForCircularReferences();

private:
    static constexpr const char* kSubgroupSeparator = "|";

    static const char* getBusId(int busNumber);

    static void createRootGroups(
        const std::vector<SynthDescriptors::ApvtsGroupDescriptor>& allGroups,
        std::map<juce::String, std::unique_ptr<juce::AudioProcessorParameterGroup>>& groupMap);
    
    static void createChildGroups(
        const std::vector<SynthDescriptors::ApvtsGroupDescriptor>& allGroups,
        std::map<juce::String, std::unique_ptr<juce::AudioProcessorParameterGroup>>& groupMap);
    
    static void addParametersToChildGroup(
        const juce::String& childGroupId,
        juce::AudioProcessorParameterGroup& childGroup);
    
    static void addRootGroupsToLayout(
        std::map<juce::String, std::unique_ptr<juce::AudioProcessorParameterGroup>>& groupMap,
        juce::AudioProcessorValueTreeState::ParameterLayout& parameterLayout);
    
    static void addIntParameter(
        juce::AudioProcessorParameterGroup& group,
        const SynthDescriptors::IntParameterDescriptor& desc);
    
    static void addChoiceParameter(
        juce::AudioProcessorParameterGroup& group,
        const SynthDescriptors::ChoiceParameterDescriptor& desc);
    
    static void addModuleParameters(
        juce::AudioProcessorParameterGroup& parentGroup,
        const char* moduleId,
        const char* moduleDisplayName,
        const std::vector<SynthDescriptors::IntParameterDescriptor>& intParams,
        const std::vector<SynthDescriptors::ChoiceParameterDescriptor>& choiceParams);
    
    static void addPatchEditParameters(juce::AudioProcessorParameterGroup& patchEditGroup);
    static void addMatrixModulationParameters(juce::AudioProcessorParameterGroup& matrixModulationGroup);
    
    static void addMatrixModulationBusIntParameters(
        std::vector<SynthDescriptors::IntParameterDescriptor>& allParams);
    
    static void addMatrixModulationBusChoiceParameters(
        std::vector<SynthDescriptors::ChoiceParameterDescriptor>& allParams);
    
    static void addValidationErrorsToResult(
        const juce::StringArray& errors,
        ValidationResult& result);

    // Helper methods for validation
    static juce::StringArray collectAllGroupIds();
    static juce::StringArray collectAllParameterIds();
    static juce::StringArray collectAllWidgetIds();
    static bool groupExists(const juce::String& groupId);
    static bool parameterIdExists(const juce::String& parameterId);
    static bool widgetIdExists(const juce::String& widgetId);
    
    // Validation helper methods
    static void checkWidgetIdUniqueness(
        const SynthDescriptors::StandaloneWidgetDescriptor& widget,
        std::set<juce::String>& seenWidgetIds,
        juce::StringArray& errors);
    
    static void checkWidgetParentGroupExists(
        const SynthDescriptors::StandaloneWidgetDescriptor& widget,
        juce::StringArray& errors);
    
    static void checkGroupIdUniqueness(
        const SynthDescriptors::ApvtsGroupDescriptor& group,
        std::set<juce::String>& seenIds,
        juce::StringArray& errors);
    
    static void checkGroupParentExists(
        const SynthDescriptors::ApvtsGroupDescriptor& group,
        const std::vector<SynthDescriptors::ApvtsGroupDescriptor>& allGroups,
        juce::StringArray& errors);
    
    static void checkGroupDisplayNameNotEmpty(
        const SynthDescriptors::ApvtsGroupDescriptor& group,
        juce::StringArray& errors);
    
    static void checkParameterIdUniqueness(
        const juce::String& parameterId,
        std::set<juce::String>& seenParamIds,
        juce::StringArray& errors);
    
    static void checkIntParameterParentGroupExists(
        const SynthDescriptors::IntParameterDescriptor& param,
        juce::StringArray& errors);
    
    static void checkIntParameterDisplayNameNotEmpty(
        const SynthDescriptors::IntParameterDescriptor& param,
        juce::StringArray& errors);
    
    static void checkIntParameterValueRange(
        const SynthDescriptors::IntParameterDescriptor& param,
        juce::StringArray& errors);
    
    static void checkChoiceParameterParentGroupExists(
        const SynthDescriptors::ChoiceParameterDescriptor& param,
        juce::StringArray& errors);
    
    static void checkChoiceParameterDisplayNameNotEmpty(
        const SynthDescriptors::ChoiceParameterDescriptor& param,
        juce::StringArray& errors);
    
    static void checkChoiceParameterChoicesNotEmpty(
        const SynthDescriptors::ChoiceParameterDescriptor& param,
        juce::StringArray& errors);
    
    static void checkChoiceParameterDefaultIndexValid(
        const SynthDescriptors::ChoiceParameterDescriptor& param,
        juce::StringArray& errors);
    
    static void checkGroupIdCollisions(
        const juce::StringArray& groupIds,
        std::set<juce::String>& allIds,
        juce::StringArray& errors);
    
    static void checkParameterIdCollisions(
        const juce::StringArray& paramIds,
        std::set<juce::String>& allIds,
        juce::StringArray& errors);
    
    static void checkWidgetIdCollisions(
        const juce::StringArray& widgetIds,
        std::set<juce::String>& allIds,
        juce::StringArray& errors);
    
    static std::set<juce::String> buildDefinedGroupIdsSet(
        const std::vector<SynthDescriptors::ApvtsGroupDescriptor>& allGroups);
    
    static void checkParentIdReferencesPointToDefinedGroups(
        const std::vector<SynthDescriptors::ApvtsGroupDescriptor>& allGroups,
        const std::set<juce::String>& definedGroupIds,
        juce::StringArray& errors);
    
    static std::map<juce::String, juce::String> buildGroupParentMap(
        const std::vector<SynthDescriptors::ApvtsGroupDescriptor>& allGroups);
    
    static void detectCyclesInGroupHierarchy(
        const std::vector<SynthDescriptors::ApvtsGroupDescriptor>& allGroups,
        const std::map<juce::String, juce::String>& parentMap,
        juce::StringArray& errors);
};

