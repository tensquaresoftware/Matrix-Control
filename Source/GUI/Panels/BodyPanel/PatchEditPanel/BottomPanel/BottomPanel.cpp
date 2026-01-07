#include "BottomPanel.h"

#include "Modules/Env1Panel.h"
#include "Modules/Env2Panel.h"
#include "Modules/Env3Panel.h"
#include "Modules/Lfo1Panel.h"
#include "Modules/Lfo2Panel.h"

#include "../../../../Themes/Theme.h"
#include "../../../../../GUI/Factories/WidgetFactory.h"

using tss::Theme;

BottomPanel::~BottomPanel() = default;

BottomPanel::BottomPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme(&inTheme)
    , env1Panel(std::make_unique<Env1Panel>(inTheme, widgetFactory, apvts))
    , env2Panel(std::make_unique<Env2Panel>(inTheme, widgetFactory, apvts))
    , env3Panel(std::make_unique<Env3Panel>(inTheme, widgetFactory, apvts))
    , lfo1Panel(std::make_unique<Lfo1Panel>(inTheme, widgetFactory, apvts))
    , lfo2Panel(std::make_unique<Lfo2Panel>(inTheme, widgetFactory, apvts))
{
    addAndMakeVisible(*env1Panel);
    addAndMakeVisible(*env2Panel);
    addAndMakeVisible(*env3Panel);
    addAndMakeVisible(*lfo1Panel);
    addAndMakeVisible(*lfo2Panel);

    setSize(getWidth(), getHeight());
}

void BottomPanel::paint(juce::Graphics& g)
{
    if (theme == nullptr)
    {
        return;
    }

    g.fillAll(theme->getPatchEditPanelBackgroundColour());
}

void BottomPanel::resized()
{
    const auto bounds = getLocalBounds();
    const auto spacing = getSpacing();
    
    const auto env1PanelX = 0;
    const auto env1PanelY = 0;
    env1Panel->setBounds(
        bounds.getX() + env1PanelX,
        bounds.getY() + env1PanelY,
        Env1Panel::getWidth(),
        Env1Panel::getHeight()
    );
    
    const auto env2PanelX = env1PanelX + Env1Panel::getWidth() + spacing;
    const auto env2PanelY = 0;
    env2Panel->setBounds(
        bounds.getX() + env2PanelX,
        bounds.getY() + env2PanelY,
        Env2Panel::getWidth(),
        Env2Panel::getHeight()
    );
    
    const auto env3PanelX = env2PanelX + Env2Panel::getWidth() + spacing;
    const auto env3PanelY = 0;
    env3Panel->setBounds(
        bounds.getX() + env3PanelX,
        bounds.getY() + env3PanelY,
        Env3Panel::getWidth(),
        Env3Panel::getHeight()
    );
    
    const auto lfo1PanelX = env3PanelX + Env3Panel::getWidth() + spacing;
    const auto lfo1PanelY = 0;
    lfo1Panel->setBounds(
        bounds.getX() + lfo1PanelX,
        bounds.getY() + lfo1PanelY,
        Lfo1Panel::getWidth(),
        Lfo1Panel::getHeight()
    );
    
    const auto lfo2PanelX = lfo1PanelX + Lfo1Panel::getWidth() + spacing;
    const auto lfo2PanelY = 0;
    lfo2Panel->setBounds(
        bounds.getX() + lfo2PanelX,
        bounds.getY() + lfo2PanelY,
        Lfo2Panel::getWidth(),
        Lfo2Panel::getHeight()
    );
}

void BottomPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (env1Panel != nullptr)
    {
        env1Panel->setTheme(inTheme);
    }

    if (env2Panel != nullptr)
    {
        env2Panel->setTheme(inTheme);
    }

    if (env3Panel != nullptr)
    {
        env3Panel->setTheme(inTheme);
    }

    if (lfo1Panel != nullptr)
    {
        lfo1Panel->setTheme(inTheme);
    }

    if (lfo2Panel != nullptr)
    {
        lfo2Panel->setTheme(inTheme);
    }

    repaint();
}

