#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "Shared/PluginDimensions.h"

namespace tss
{
    class Skin;
}

class WidgetFactory;
class Env1Panel;
class Env2Panel;
class Env3Panel;
class Lfo1Panel;
class Lfo2Panel;

class BottomPanel : public juce::Component
{
public:
    BottomPanel(tss::Skin& skin, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~BottomPanel() override;

    void resized() override;
    void setSkin(tss::Skin& skin);

    static int getWidth() { return PluginDimensions::Panels::PatchEditPanel::BottomPanel::kWidth; }
    static int getHeight() { return PluginDimensions::Panels::PatchEditPanel::BottomPanel::kHeight; }
    static int getSpacing() { return PluginDimensions::Panels::BodyPanel::kPadding; }

private:

    tss::Skin* skin_;

    std::unique_ptr<Env1Panel> env1Panel_;
    std::unique_ptr<Env2Panel> env2Panel_;
    std::unique_ptr<Env3Panel> env3Panel_;
    std::unique_ptr<Lfo1Panel> lfo1Panel_;
    std::unique_ptr<Lfo2Panel> lfo2Panel_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BottomPanel)
};

