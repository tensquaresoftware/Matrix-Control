#include "TopPanel.h"

#include "Modules/Dco1Panel.h"
#include "Modules/Dco2Panel.h"
#include "Modules/VcfVcaPanel.h"
#include "Modules/FmTrackPanel.h"
#include "Modules/RampPortamentoPanel.h"

#include "../../../../Themes/Theme.h"
#include "../../../../../GUI/Factories/WidgetFactory.h"

using tss::Theme;

TopPanel::~TopPanel() = default;

TopPanel::TopPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme(&inTheme)
    , dco1Panel(std::make_unique<Dco1Panel>(inTheme, widgetFactory, apvts))
    , dco2Panel(std::make_unique<Dco2Panel>(inTheme, widgetFactory, apvts))
    , vcfVcaPanel(std::make_unique<VcfVcaPanel>(inTheme, widgetFactory, apvts))
    , fmTrackPanel(std::make_unique<FmTrackPanel>(inTheme, widgetFactory, apvts))
    , rampPortamentoPanel(std::make_unique<RampPortamentoPanel>(inTheme, widgetFactory, apvts))
{
    addAndMakeVisible(*dco1Panel);
    addAndMakeVisible(*dco2Panel);
    addAndMakeVisible(*vcfVcaPanel);
    addAndMakeVisible(*fmTrackPanel);
    addAndMakeVisible(*rampPortamentoPanel);

    setSize(getWidth(), getHeight());
}

void TopPanel::paint(juce::Graphics& g)
{
    if (auto* currentTheme = theme)
        g.fillAll(currentTheme->getPatchEditPanelBackgroundColour());
}

void TopPanel::resized()
{
    const auto bounds = getLocalBounds();
    const auto spacing = getSpacing();
    
    const auto dco1PanelX = 0;
    const auto dco1PanelY = 0;
    dco1Panel->setBounds(
        bounds.getX() + dco1PanelX,
        bounds.getY() + dco1PanelY,
        Dco1Panel::getWidth(),
        Dco1Panel::getHeight()
    );
    
    const auto dco2PanelX = dco1PanelX + Dco1Panel::getWidth() + spacing;
    const auto dco2PanelY = 0;
    dco2Panel->setBounds(
        bounds.getX() + dco2PanelX,
        bounds.getY() + dco2PanelY,
        Dco2Panel::getWidth(),
        Dco2Panel::getHeight()
    );
    
    const auto vcfVcaPanelX = dco2PanelX + Dco2Panel::getWidth() + spacing;
    const auto vcfVcaPanelY = 0;
    vcfVcaPanel->setBounds(
        bounds.getX() + vcfVcaPanelX,
        bounds.getY() + vcfVcaPanelY,
        VcfVcaPanel::getWidth(),
        VcfVcaPanel::getHeight()
    );
    
    const auto fmTrackPanelX = vcfVcaPanelX + VcfVcaPanel::getWidth() + spacing;
    const auto fmTrackPanelY = 0;
    fmTrackPanel->setBounds(
        bounds.getX() + fmTrackPanelX,
        bounds.getY() + fmTrackPanelY,
        FmTrackPanel::getWidth(),
        FmTrackPanel::getHeight()
    );
    
    const auto rampPortamentoPanelX = fmTrackPanelX + FmTrackPanel::getWidth() + spacing;
    const auto rampPortamentoPanelY = 0;
    rampPortamentoPanel->setBounds(
        bounds.getX() + rampPortamentoPanelX,
        bounds.getY() + rampPortamentoPanelY,
        RampPortamentoPanel::getWidth(),
        RampPortamentoPanel::getHeight()
    );
}

void TopPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* panel = dco1Panel.get())
        panel->setTheme(inTheme);

    if (auto* panel = dco2Panel.get())
        panel->setTheme(inTheme);

    if (auto* panel = vcfVcaPanel.get())
        panel->setTheme(inTheme);

    if (auto* panel = fmTrackPanel.get())
        panel->setTheme(inTheme);

    if (auto* panel = rampPortamentoPanel.get())
        panel->setTheme(inTheme);

    repaint();
}

