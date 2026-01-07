#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>


namespace tss
{
    class Theme;
    class PanelSeparator;
}

class PatchEditPanel;
class MatrixModulationPanel;
class PatchManagerPanel;
class MasterEditPanel;
class WidgetFactory;

class BodyPanel : public juce::Component
{
public:
    BodyPanel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~BodyPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& inTheme);

    static int getHeight() { return kHeight; }
    static int getPadding() { return kPadding; }

private:
    inline constexpr static int kHeight = 730;
    inline constexpr static int kPadding = 15;

    tss::Theme* theme;

    std::unique_ptr<PatchEditPanel> patchEditPanel;
    std::unique_ptr<tss::PanelSeparator> panelSeparator1;
    std::unique_ptr<MatrixModulationPanel> matrixModulationPanel;
    std::unique_ptr<PatchManagerPanel> patchManagerPanel;
    std::unique_ptr<tss::PanelSeparator> panelSeparator2;
    std::unique_ptr<MasterEditPanel> masterEditPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BodyPanel)
};

