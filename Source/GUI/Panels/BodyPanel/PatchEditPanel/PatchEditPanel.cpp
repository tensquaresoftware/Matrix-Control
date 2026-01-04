#include "PatchEditPanel.h"

#include "Modules/Dco1Panel.h"
#include "Modules/Dco2Panel.h"
#include "Modules/VcfVcaPanel.h"
#include "Modules/FmTrackPanel.h"
#include "Modules/RampPortamentoPanel.h"

#include "../../../Themes/Theme.h"
#include "../../../Widgets/SectionName.h"
#include "../BodyPanel.h"
#include "../../../../Shared/SynthDescriptors.h"
#include "../../../../GUI/Factories/WidgetFactory.h"

using tss::Theme;

PatchEditPanel::~PatchEditPanel() = default;

PatchEditPanel::PatchEditPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme(&inTheme)
    , sectionName(std::make_unique<tss::SectionName>(
        inTheme, 
        getWidth(), 
        SynthDescriptors::getSectionDisplayName(SynthDescriptors::SectionIds::kPatchEdit)))
    , dco1Panel(std::make_unique<Dco1Panel>(inTheme, widgetFactory, apvts))
    , dco2Panel(std::make_unique<Dco2Panel>(inTheme, widgetFactory, apvts))
    , vcfVcaPanel(std::make_unique<VcfVcaPanel>(inTheme, widgetFactory, apvts))
    , fmTrackPanel(std::make_unique<FmTrackPanel>(inTheme, widgetFactory, apvts))
    , rampPortamentoPanel(std::make_unique<RampPortamentoPanel>(inTheme, widgetFactory, apvts))
{
    addAndMakeVisible(*sectionName);
    addAndMakeVisible(*dco1Panel);
    addAndMakeVisible(*dco2Panel);
    addAndMakeVisible(*vcfVcaPanel);
    addAndMakeVisible(*fmTrackPanel);
    addAndMakeVisible(*rampPortamentoPanel);

    setSize(getWidth(), getHeight());
}

void PatchEditPanel::paint(juce::Graphics& g)
{
    if (theme == nullptr)
    {
        return;
    }

    g.fillAll(theme->getPatchEditPanelBackgroundColour());
}

void PatchEditPanel::resized()
{
    const auto sectionNameHeight = tss::SectionName::getHeight();
    const auto panelWidth = Dco1Panel::getWidth();
    const auto panelHeight = Dco1Panel::getHeight();
    const auto spacing = BodyPanel::getSpacing();

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

void PatchEditPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (sectionName != nullptr)
    {
        sectionName->setTheme(inTheme);
    }

    if (dco1Panel != nullptr)
    {
        dco1Panel->setTheme(inTheme);
    }

    if (dco2Panel != nullptr)
    {
        dco2Panel->setTheme(inTheme);
    }

    if (vcfVcaPanel != nullptr)
    {
        vcfVcaPanel->setTheme(inTheme);
    }

    if (fmTrackPanel != nullptr)
    {
        fmTrackPanel->setTheme(inTheme);
    }

    if (rampPortamentoPanel != nullptr)
    {
        rampPortamentoPanel->setTheme(inTheme);
    }

    repaint();
}

