#include "MainComponent.h"

using tss::Skin;

MainComponent::MainComponent(Skin& skin)
    : headerPanel(skin)
    , mainPanel(skin)
    , footerPanel(skin)
{
    addAndMakeVisible(headerPanel);
    addAndMakeVisible(mainPanel);
    addAndMakeVisible(footerPanel);
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    auto headerHeight = HeaderPanel::getHeight();
    auto mainPanelHeight = MainPanel::getHeight();
    auto footerHeight = FooterPanel::getHeight();

    headerPanel.setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), headerHeight);
    mainPanel.setBounds(bounds.getX(), bounds.getY() + headerHeight, bounds.getWidth(), mainPanelHeight);
    footerPanel.setBounds(bounds.getX(), bounds.getY() + headerHeight + mainPanelHeight, bounds.getWidth(), footerHeight);
}

void MainComponent::setSkin(Skin& skin)
{
    headerPanel.setSkin(skin);
    mainPanel.setSkin(skin);
    footerPanel.setSkin(skin);
}

