#include "PatchEditPanel.h"

#include "TopPanel/TopPanel.h"
#include "MiddlePanel/MiddlePanel.h"
#include "BottomPanel/BottomPanel.h"

#include "../../../Themes/Theme.h"
#include "../../../Widgets/SectionHeader.h"
#include "../../../../Shared/SynthDescriptors.h"
#include "../../../../GUI/Factories/WidgetFactory.h"

using tss::Theme;

PatchEditPanel::~PatchEditPanel() = default;

PatchEditPanel::PatchEditPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme(&inTheme)
    , sectionHeader(std::make_unique<tss::SectionHeader>(
        inTheme, 
        tss::SectionHeader::SectionWidth::PatchEdit, 
        SynthDescriptors::getSectionDisplayName(SynthDescriptors::SectionIds::kPatchEdit)))
    , topPanel(std::make_unique<TopPanel>(inTheme, widgetFactory, apvts))
    , middlePanel(std::make_unique<MiddlePanel>(inTheme))
    , bottomPanel(std::make_unique<BottomPanel>(inTheme, widgetFactory, apvts))
{
    addAndMakeVisible(*sectionHeader);
    addAndMakeVisible(*topPanel);
    addAndMakeVisible(*middlePanel);
    addAndMakeVisible(*bottomPanel);

    setSize(getWidth(), getHeight());
}

void PatchEditPanel::paint(juce::Graphics& g)
{
    if (auto* currentTheme = theme)
        g.fillAll(currentTheme->getPatchEditPanelBackgroundColour());
}

void PatchEditPanel::resized()
{
    const auto bounds = getLocalBounds();
    
    const auto sectionHeaderY = 0;
    sectionHeader->setBounds(
        bounds.getX() + 0,
        bounds.getY() + sectionHeaderY,
        bounds.getWidth(),
        tss::SectionHeader::getHeight()
    );
    
    const auto topPanelY = sectionHeaderY + tss::SectionHeader::getHeight();
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

    if (auto* header = sectionHeader.get())
        header->setTheme(inTheme);

    if (auto* panel = topPanel.get())
        panel->setTheme(inTheme);

    if (auto* panel = middlePanel.get())
        panel->setTheme(inTheme);

    if (auto* panel = bottomPanel.get())
        panel->setTheme(inTheme);

    repaint();
}

