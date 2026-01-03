#pragma once

#include <juce_gui_basics/juce_gui_basics.h>


namespace tss
{
    class Theme;
}

using tss::Theme;

class FooterPanel : public juce::Component
{
public:
    explicit FooterPanel(Theme& newTheme);
    ~FooterPanel() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(Theme& newTheme);

    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kHeight = 30;

    Theme* theme;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FooterPanel)
};

