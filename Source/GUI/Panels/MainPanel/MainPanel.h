#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

#include "../../Skin/SkinDimensions.h"

namespace tss
{
    class Skin;
    class PanelSeparator;
}

using tss::Skin;
using tss::PanelSeparator;

class PatchEditPanel;
class MatrixModulationPanel;
class PatchManagerPanel;
class MasterEditPanel;

class MainPanel : public juce::Component
{
public:
    explicit MainPanel(Skin& newSkin);
    ~MainPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setSkin(Skin& newSkin);

    static int getHeight() { return tss::SkinDimensions::MainPanel::kHeight; }

private:

    Skin* skin;

    std::unique_ptr<PatchEditPanel> patchEditPanel;
    std::unique_ptr<PanelSeparator> panelSeparator1;
    std::unique_ptr<MatrixModulationPanel> matrixModulationPanel;
    std::unique_ptr<PatchManagerPanel> patchManagerPanel;
    std::unique_ptr<PanelSeparator> panelSeparator2;
    std::unique_ptr<MasterEditPanel> masterEditPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainPanel)
};

