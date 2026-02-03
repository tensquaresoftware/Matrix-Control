#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "GUI/Widgets/Label.h"
#include "GUI/Widgets/ComboBox.h"

namespace tss
{
    class Skin;
    class Label;
    class ComboBox;
}

class HeaderPanel : public juce::Component
{
public:
    explicit HeaderPanel(tss::Skin& skin);
    ~HeaderPanel() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setSkin(tss::Skin& skin);

    tss::ComboBox& getSkinComboBox() { return skinComboBox_; }
    tss::ComboBox& getZoomComboBox() { return zoomComboBox_; }

    static int getHeight() { return kHeight_; }
    static int getSpacing() { return kSpacing_; }

private:
    inline constexpr static int kHeight_ = 30;
    inline constexpr static int kSpacing_ = 5;
    inline constexpr static int kLabelWidth_ = 35;
    inline constexpr static int kComboBoxWidth_ = 50;
    inline constexpr static int kControlHeight_ = 20;

    tss::Skin* skin_;

    tss::Label skinLabel_;
    tss::ComboBox skinComboBox_;
    tss::Label zoomLabel_;
    tss::ComboBox zoomComboBox_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderPanel)
};

