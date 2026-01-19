#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "GUI/Widgets/EnvelopeDisplay.h"
#include "GUI/Widgets/PatchNameDisplay.h"
#include "GUI/Widgets/TrackGeneratorDisplay.h"
#include "Shared/PluginDimensions.h"

namespace tss
{
    class Theme;
}

class MiddlePanel : public juce::Component
{
public:
    MiddlePanel(tss::Theme& theme);
    ~MiddlePanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& theme);

    static int getWidth() { return PluginDimensions::Panels::PatchEditPanel::MiddlePanel::kWidth; }
    static int getHeight() { return PluginDimensions::Panels::PatchEditPanel::MiddlePanel::kHeight; }

private:
    inline constexpr static int kSpacing = 15;

    tss::Theme* theme_;

    tss::EnvelopeDisplay envelope1Display_;
    tss::EnvelopeDisplay envelope2Display_;
    tss::EnvelopeDisplay envelope3Display_;
    tss::TrackGeneratorDisplay trackGeneratorDisplay_;
    tss::PatchNameDisplay patchNameDisplay_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiddlePanel)
};

