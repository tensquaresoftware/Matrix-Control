#include "MainComponent.h"

#include "../GUI/Factories/WidgetFactory.h"

using tss::Theme;

MainComponent::MainComponent(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : headerPanel(theme)
    , bodyPanel(theme, widgetFactory, apvts)
    , footerPanel(theme, apvts)
{
    addAndMakeVisible(headerPanel);
    addAndMakeVisible(bodyPanel);
    addAndMakeVisible(footerPanel);
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    
    auto headerPanelY = 0;
    headerPanel.setBounds(
        bounds.getX(),
        bounds.getY() + headerPanelY,
        bounds.getWidth(),
        HeaderPanel::getHeight()
    );
    
    auto bodyPanelY = headerPanelY + HeaderPanel::getHeight();
    bodyPanel.setBounds(
        bounds.getX(),
        bounds.getY() + bodyPanelY,
        bounds.getWidth(),
        BodyPanel::getHeight()
    );
    
    auto footerPanelY = bodyPanelY + BodyPanel::getHeight();
    footerPanel.setBounds(
        bounds.getX(),
        bounds.getY() + footerPanelY,
        bounds.getWidth(),
        FooterPanel::getHeight()
    );
}

void MainComponent::setTheme(Theme& theme)
{
    headerPanel.setTheme(theme);
    bodyPanel.setTheme(theme);
    footerPanel.setTheme(theme);
}

