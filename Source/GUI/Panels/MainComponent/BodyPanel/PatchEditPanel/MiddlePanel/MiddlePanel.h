#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "GUI/Widgets/EnvelopeDisplay.h"
#include "GUI/Widgets/PatchNameDisplay.h"
#include "GUI/Widgets/TrackGeneratorDisplay.h"
#include "Shared/PluginDimensions.h"

namespace tss
{
    class Skin;
}

class MiddlePanel : public juce::Component
{
public:
    MiddlePanel(tss::Skin& skin);
    ~MiddlePanel() override;

    void resized() override;
    void setSkin(tss::Skin& skin);

    static int getWidth() { return PluginDimensions::Panels::PatchEditPanel::MiddlePanel::kWidth; }
    static int getHeight() { return PluginDimensions::Panels::PatchEditPanel::MiddlePanel::kHeight; }

private:
    inline constexpr static int kSpacing = 15;

    tss::Skin* skin_;

    tss::EnvelopeDisplay envelope1Display_;
    tss::EnvelopeDisplay envelope2Display_;
    tss::EnvelopeDisplay envelope3Display_;
    tss::TrackGeneratorDisplay trackGeneratorDisplay_;
    tss::PatchNameDisplay patchNameDisplay_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiddlePanel)
};

