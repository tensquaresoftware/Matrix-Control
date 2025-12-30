#pragma once

#include <map>
#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>

#include "SynthDescriptors.h"

class ApvtsFactory
{
public:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    static constexpr const char* kSubgroupSeparator = "|";

    static juce::String getGroupDisplayName(const juce::String& groupId);
    
    static const char* getBusId(int busNumber);

    static void createRootGroups(
        const std::vector<SynthDescriptors::GroupDescriptor>& allGroups,
        std::map<juce::String, std::unique_ptr<juce::AudioProcessorParameterGroup>>& groupMap);
    
    static void createChildGroups(
        const std::vector<SynthDescriptors::GroupDescriptor>& allGroups,
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
};

