#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Skin;
}

using tss::Skin;

class PatchEditPanel : public juce::Component
{
public:
    explicit PatchEditPanel(Skin& skin);
    ~PatchEditPanel() override = default;

    void paint(juce::Graphics&) override;
    void setSkin(Skin& skin);

private:
    Skin* skin;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchEditPanel)
};

