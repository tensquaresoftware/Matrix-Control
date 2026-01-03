#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;
    class SectionName;
}

class WidgetFactory;

using tss::Theme;

class Dco1Panel;
class Dco2Panel;
class VcfVcaPanel;
class FmTrackPanel;
class RampPortamentoPanel;

class PatchEditPanel : public juce::Component
{
public:
    PatchEditPanel(Theme& newTheme, WidgetFactory& widgetFactory);
    ~PatchEditPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(Theme& newTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kWidth = 810;
    inline constexpr static int kHeight = 315;
    Theme* theme;

    std::unique_ptr<tss::SectionName> sectionName;
    std::unique_ptr<Dco1Panel> dco1Panel;
    std::unique_ptr<Dco2Panel> dco2Panel;
    std::unique_ptr<VcfVcaPanel> vcfVcaPanel;
    std::unique_ptr<FmTrackPanel> fmTrackPanel;
    std::unique_ptr<RampPortamentoPanel> rampPortamentoPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchEditPanel)
};

