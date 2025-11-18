/**
 * Factory function to create the plugin instance.
 * Required by JUCE plugin host system.
 */

#include "PluginProcessor.h"

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}