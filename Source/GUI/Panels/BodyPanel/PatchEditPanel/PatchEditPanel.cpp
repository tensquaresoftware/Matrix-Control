#include "PatchEditPanel.h"

#include "Modules/Dco1Panel.h"
#include "Modules/Dco2Panel.h"
#include "Modules/VcfVcaPanel.h"
#include "Modules/FmTrackPanel.h"
#include "Modules/RampPortamentoPanel.h"

#include "../../../Skin/Skin.h"
#include "../../../Skin/SkinDimensions.h"
#include "../../../Widgets/SectionName.h"
#include "../../../../Shared/SynthDescriptors.h"
#include "../../../../Shared/ApvtsFactory.h"
#include "../../../../Shared/WidgetFactory.h"

using tss::Skin;

PatchEditPanel::~PatchEditPanel() = default;

PatchEditPanel::PatchEditPanel(Skin& newSkin, WidgetFactory& widgetFactory)
    : skin(&newSkin)
    , sectionName(std::make_unique<tss::SectionName>(
        newSkin, 
        tss::SkinDimensions::PatchEditPanel::kWidth, 
        ApvtsFactory::getSectionDisplayName(SynthDescriptors::SectionIds::kPatchEdit)))
    , dco1Panel(std::make_unique<Dco1Panel>(newSkin, widgetFactory))
    , dco2Panel(std::make_unique<Dco2Panel>(newSkin))
    , vcfVcaPanel(std::make_unique<VcfVcaPanel>(newSkin))
    , fmTrackPanel(std::make_unique<FmTrackPanel>(newSkin))
    , rampPortamentoPanel(std::make_unique<RampPortamentoPanel>(newSkin))
{
    addAndMakeVisible(*sectionName);
    addAndMakeVisible(*dco1Panel);
    addAndMakeVisible(*dco2Panel);
    addAndMakeVisible(*vcfVcaPanel);
    addAndMakeVisible(*fmTrackPanel);
    addAndMakeVisible(*rampPortamentoPanel);

    setSize(tss::SkinDimensions::PatchEditPanel::kWidth, tss::SkinDimensions::PatchEditPanel::kHeight);
}

void PatchEditPanel::paint(juce::Graphics& g)
{
    if (skin == nullptr)
    {
        return;
    }

    g.fillAll(skin->getPatchEditPanelBackgroundColour());
}

void PatchEditPanel::resized()
{
    const auto sectionNameHeight = tss::SkinDimensions::Widget::SectionName::kHeight;
    const auto panelWidth = tss::SkinDimensions::PatchEditModulePanel::kWidth;
    const auto panelHeight = tss::SkinDimensions::PatchEditModulePanel::kHeight;
    const auto spacing = tss::SkinDimensions::BodyPanel::kSpacing;

    if (sectionName != nullptr)
    {
        sectionName->setBounds(0, 0, getWidth(), sectionNameHeight);
    }

    int x = 0;
    int y = sectionNameHeight;

    if (dco1Panel != nullptr)
    {
        dco1Panel->setBounds(x, y, panelWidth, panelHeight);
        x += panelWidth + spacing;
    }

    if (dco2Panel != nullptr)
    {
        dco2Panel->setBounds(x, y, panelWidth, panelHeight);
        x += panelWidth + spacing;
    }

    if (vcfVcaPanel != nullptr)
    {
        vcfVcaPanel->setBounds(x, y, panelWidth, panelHeight);
        x += panelWidth + spacing;
    }

    if (fmTrackPanel != nullptr)
    {
        fmTrackPanel->setBounds(x, y, panelWidth, panelHeight);
        x += panelWidth + spacing;
    }

    if (rampPortamentoPanel != nullptr)
    {
        rampPortamentoPanel->setBounds(x, y, panelWidth, panelHeight);
    }
}

void PatchEditPanel::setSkin(Skin& newSkin)
{
    skin = &newSkin;

    if (sectionName != nullptr)
    {
        sectionName->setSkin(newSkin);
    }

    if (dco1Panel != nullptr)
    {
        dco1Panel->setSkin(newSkin);
    }

    if (dco2Panel != nullptr)
    {
        dco2Panel->setSkin(newSkin);
    }

    if (vcfVcaPanel != nullptr)
    {
        vcfVcaPanel->setSkin(newSkin);
    }

    if (fmTrackPanel != nullptr)
    {
        fmTrackPanel->setSkin(newSkin);
    }

    if (rampPortamentoPanel != nullptr)
    {
        rampPortamentoPanel->setSkin(newSkin);
    }

    repaint();
}

