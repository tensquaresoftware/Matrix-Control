#pragma once

#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "Shared/Definitions/MatrixDeviceTypes.h"

namespace TSS
{
    // Composes FR-2 device lock with Mutator Compare lock for footer sync and focus steal.
    // Visual/hit dimming is owned by MainComponent's LockDimmingFilm — binders must not
    // setAlpha or setInterceptsMouseClicks on sections (avoids double dimming / fights).
    // Use lockOnCompare=false for Patch Mutator (COMPARE stays live while Compare is active;
    // film hole policy decides whether COMPARE is uncovered).
    class CompareLockBinder : public juce::ValueTree::Listener
    {
    public:
        CompareLockBinder(juce::AudioProcessorValueTreeState& apvts,
                          std::vector<juce::Component*> lockedComponents,
                          bool lockOnCompare = true);
        ~CompareLockBinder() override;

        void valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& property) override;
        void valueTreeRedirected(juce::ValueTree& tree) override;

        void valueTreeChildAdded(juce::ValueTree&, juce::ValueTree&) override {}
        void valueTreeChildRemoved(juce::ValueTree&, juce::ValueTree&, int) override {}
        void valueTreeChildOrderChanged(juce::ValueTree&, int, int) override {}
        void valueTreeParentChanged(juce::ValueTree&) override {}

    private:
        void apply();
        void syncDeviceLockFooter(bool deviceDetected,
                                  MatrixDeviceTypes::Type deviceType,
                                  bool compareActive,
                                  bool deviceMidiUnresponsive);

        juce::AudioProcessorValueTreeState& apvts_;
        std::vector<juce::Component::SafePointer<juce::Component>> locked_;
        bool lockOnCompare_ = true;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompareLockBinder)
    };
}
