#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;
}

using tss::Theme;

class PatchManagerPanel : public juce::Component
{
public:
    explicit PatchManagerPanel(Theme& newTheme);
    ~PatchManagerPanel() override = default;

    void paint(juce::Graphics&) override;
    void setTheme(Theme& newTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kWidth = 275;
    inline constexpr static int kHeight = 415;

    Theme* theme;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchManagerPanel)
};

