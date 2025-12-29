#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "../../Skin/SkinDimensions.h"

namespace tss
{
    class Skin;
}

using tss::Skin;

class FooterPanel : public juce::Component
{
public:
    explicit FooterPanel(Skin& newSkin);
    ~FooterPanel() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setSkin(Skin& skin);

    static int getHeight() { return tss::SkinDimensions::FooterPanel::kHeight; }

private:

    Skin* skin;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FooterPanel)
};

