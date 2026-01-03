#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace tss
{
    class Theme;
}

class FooterPanel : public juce::Component,
                    public juce::ValueTree::Listener
{
public:
    FooterPanel(tss::Theme& newTheme, juce::AudioProcessorValueTreeState& apvtsRef);
    ~FooterPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& newTheme);

    static int getHeight() { return kHeight; }

    // ValueTree::Listener
    void valueTreePropertyChanged(juce::ValueTree& tree,
                                 const juce::Identifier& property) override;

private:
    enum class MessageSeverity
    {
        None,
        Info,
        Success,
        Warning,
        Error
    };
    
    inline constexpr static int kHeight = 30;
    inline constexpr static int kPadding = 8;
    inline constexpr static int kIconSize = 14;
    
    tss::Theme* theme;
    juce::AudioProcessorValueTreeState& apvts;
    
    juce::String currentMessage;
    MessageSeverity currentSeverity = MessageSeverity::None;
    
    static const juce::Identifier kMessageTextId;
    static const juce::Identifier kMessageSeverityId;
    
    MessageSeverity parseSeverity(const juce::String& severityStr) const;
    juce::Colour getSeverityColour(MessageSeverity severity) const;
    juce::String getSeverityIcon(MessageSeverity severity) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FooterPanel)
};

