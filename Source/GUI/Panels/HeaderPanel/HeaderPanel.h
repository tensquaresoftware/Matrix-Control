#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "../../Widgets/Button.h"

namespace tss
{
    class Theme;
    class Button;
}

using tss::Theme;
using tss::Button;

class HeaderPanel : public juce::Component
{
public:
    explicit HeaderPanel(Theme& newTheme);
    ~HeaderPanel() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(Theme& newTheme);

    Button& getButtonBlack() { return buttonBlack; }
    Button& getButtonCream() { return buttonCream; }
    Button& getButtonDebug() { return buttonDebug; }
    Button& getButtonDisabled() { return buttonDisabled; }

    static int getHeight() { return kHeight; }
    static int getButtonWidth() { return kButtonWidth; }
    static int getButtonHeight() { return kButtonHeight; }
    static int getButtonSpacing() { return kButtonSpacing; }

private:
    inline constexpr static int kHeight = 30;
    inline constexpr static int kButtonWidth = 70;
    inline constexpr static int kButtonHeight = 20;
    inline constexpr static int kButtonSpacing = 5;

    Theme* theme;

    Button buttonBlack;
    Button buttonCream;
    Button buttonDebug;
    Button buttonDisabled;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderPanel)
};

