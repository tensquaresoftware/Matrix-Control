#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>


namespace tss
{
    class Theme;
    class VerticalSeparator;
}

class PatchEditPanel;
class MatrixModulationPanel;
class PatchManagerPanel;
class MasterEditPanel;
class WidgetFactory;

class BodyPanel : public juce::Component
{
public:
    BodyPanel(tss::Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~BodyPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& theme);

    static int getHeight() { return kHeight_; }
    static int getPadding() { return kPadding_; }

private:
    inline constexpr static int kHeight_ = 730;
    inline constexpr static int kPadding_ = 15;

    tss::Theme* theme_;

    std::unique_ptr<PatchEditPanel> patchEditPanel_;
    std::unique_ptr<tss::VerticalSeparator> verticalSeparator1_;
    std::unique_ptr<MatrixModulationPanel> matrixModulationPanel_;
    std::unique_ptr<PatchManagerPanel> patchManagerPanel_;
    std::unique_ptr<tss::VerticalSeparator> verticalSeparator2_;
    std::unique_ptr<MasterEditPanel> masterEditPanel_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BodyPanel)
};

