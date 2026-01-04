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
    auto headerHeight = HeaderPanel::getHeight();
    auto bodyPanelHeight = BodyPanel::getHeight();
    auto footerHeight = FooterPanel::getHeight();

    headerPanel.setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), headerHeight);
    bodyPanel.setBounds(bounds.getX(), bounds.getY() + headerHeight, bounds.getWidth(), bodyPanelHeight);
    footerPanel.setBounds(bounds.getX(), bounds.getY() + headerHeight + bodyPanelHeight, bounds.getWidth(), footerHeight);
}

void MainComponent::setTheme(Theme& theme)
{
    headerPanel.setTheme(theme);
    bodyPanel.setTheme(theme);
    footerPanel.setTheme(theme);
}

