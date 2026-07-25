#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace TSS
{
    class Button;

    // Drives Compare-style red blink on one Copy or Paste button from APVTS feedback mirrors.
    // Source role: bright when clipboardFeedbackCopyLit is true.
    // Target role: bright on the opposite phase. Disabled Paste targets never blink.
    class ClipboardFeedbackButtonBinding : private juce::ValueTree::Listener
    {
    public:
        ClipboardFeedbackButtonBinding(juce::AudioProcessorValueTreeState& apvts,
                                       Button& button,
                                       const juce::String& rolePropertyId,
                                       bool isSourceRole);
        ~ClipboardFeedbackButtonBinding() override;

        void refresh() { apply(); }

        ClipboardFeedbackButtonBinding(const ClipboardFeedbackButtonBinding&) = delete;
        ClipboardFeedbackButtonBinding& operator=(const ClipboardFeedbackButtonBinding&) = delete;

    private:
        void valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier& property) override;
        void valueTreeChildAdded(juce::ValueTree&, juce::ValueTree&) override {}
        void valueTreeChildRemoved(juce::ValueTree&, juce::ValueTree&, int) override {}
        void valueTreeChildOrderChanged(juce::ValueTree&, int, int) override {}
        void valueTreeParentChanged(juce::ValueTree&) override {}
        void valueTreeRedirected(juce::ValueTree&) override;

        void apply();

        juce::ValueTree state_;
        Button& button_;
        juce::String rolePropertyId_;
        bool isSourceRole_ = true;
    };

} // namespace TSS
