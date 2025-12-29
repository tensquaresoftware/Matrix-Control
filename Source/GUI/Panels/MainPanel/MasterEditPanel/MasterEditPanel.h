#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Skin;
}

using tss::Skin;

class MasterEditPanel : public juce::Component
{
public:
    explicit MasterEditPanel(Skin& skin);
    ~MasterEditPanel() override = default;

    void paint(juce::Graphics&) override;
    void setSkin(Skin& skin);

private:
    Skin* skin;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MasterEditPanel)
};

