#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Skin;
    class SectionName;
}

class WidgetFactory;

using tss::Skin;

class Dco1Panel;
class Dco2Panel;
class VcfVcaPanel;
class FmTrackPanel;
class RampPortamentoPanel;

class PatchEditPanel : public juce::Component
{
public:
    PatchEditPanel(Skin& skin, WidgetFactory& widgetFactory);
    ~PatchEditPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setSkin(Skin& skin);

private:
    Skin* skin;

    std::unique_ptr<tss::SectionName> sectionName;
    std::unique_ptr<Dco1Panel> dco1Panel;
    std::unique_ptr<Dco2Panel> dco2Panel;
    std::unique_ptr<VcfVcaPanel> vcfVcaPanel;
    std::unique_ptr<FmTrackPanel> fmTrackPanel;
    std::unique_ptr<RampPortamentoPanel> rampPortamentoPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchEditPanel)
};

