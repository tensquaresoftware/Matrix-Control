#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "../../Widgets/Button.h"

namespace tss
{
    class Theme;
    class Button;
}

class HeaderPanel : public juce::Component
{
public:
    explicit HeaderPanel(tss::Theme& newTheme);
    ~HeaderPanel() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& newTheme);

    tss::Button& getButtonBlack() { return buttonBlack; }
    tss::Button& getButtonCream() { return buttonCream; }
    tss::Button& getButtonDebug() { return buttonDebug; }
    tss::Button& getButtonDisabled() { return buttonDisabled; }

    static int getHeight() { return kHeight; }
    static int getButtonWidth() { return kButtonWidth; }
    static int getButtonHeight() { return kButtonHeight; }
    static int getButtonSpacing() { return kButtonSpacing; }

private:
    inline constexpr static int kHeight = 30;
    inline constexpr static int kButtonWidth = 70;
    inline constexpr static int kButtonHeight = 20;
    inline constexpr static int kButtonSpacing = 5;

    tss::Theme* theme;

    tss::Button buttonBlack;
    tss::Button buttonCream;
    tss::Button buttonDebug;
    tss::Button buttonDisabled;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderPanel)
};

