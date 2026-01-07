#include "PatchEditPanel.h"

#include "TopPanel/TopPanel.h"
#include "MiddlePanel/MiddlePanel.h"
#include "BottomPanel/BottomPanel.h"

#include "../../../Themes/Theme.h"
#include "../../../Widgets/SectionName.h"
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
    , topPanel(std::make_unique<TopPanel>(inTheme, widgetFactory, apvts))
    , middlePanel(std::make_unique<MiddlePanel>(inTheme))
    , bottomPanel(std::make_unique<BottomPanel>(inTheme, widgetFactory, apvts))
{
    addAndMakeVisible(*sectionName);
    addAndMakeVisible(*topPanel);
    addAndMakeVisible(*middlePanel);
    addAndMakeVisible(*bottomPanel);

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
    const auto bounds = getLocalBounds();
    
    const auto sectionNameY = 0;
    sectionName->setBounds(
        bounds.getX() + 0,
        bounds.getY() + sectionNameY,
        bounds.getWidth(),
        tss::SectionName::getHeight()
    );
    
    const auto topPanelY = sectionNameY + tss::SectionName::getHeight();
    topPanel->setBounds(
        bounds.getX() + 0,
        bounds.getY() + topPanelY,
        bounds.getWidth(),
        TopPanel::getHeight()
    );
    
    const auto middlePanelY = topPanelY + TopPanel::getHeight();
    middlePanel->setBounds(
        bounds.getX() + 0,
        bounds.getY() + middlePanelY,
        bounds.getWidth(),
        MiddlePanel::getHeight()
    );
    
    const auto bottomPanelY = middlePanelY + MiddlePanel::getHeight();
    bottomPanel->setBounds(
        bounds.getX() + 0,
        bounds.getY() + bottomPanelY,
        bounds.getWidth(),
        BottomPanel::getHeight()
    );
}

void PatchEditPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (sectionName != nullptr)
    {
        sectionName->setTheme(inTheme);
    }

    if (topPanel != nullptr)
    {
        topPanel->setTheme(inTheme);
    }

    if (middlePanel != nullptr)
    {
        middlePanel->setTheme(inTheme);
    }

    if (bottomPanel != nullptr)
    {
        bottomPanel->setTheme(inTheme);
    }

    repaint();
}

