#include "MasterEditPanel.h"

#include "Modules/MidiPanel.h"
#include "Modules/VibratoPanel.h"
#include "Modules/MiscPanel.h"

#include "../../../Themes/Theme.h"
#include "../../../Widgets/SectionHeader.h"
#include "../../../../Shared/SynthDescriptors.h"
#include "../../../../GUI/Factories/WidgetFactory.h"

using tss::Theme;

MasterEditPanel::MasterEditPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme(&inTheme)
    , sectionHeader(std::make_unique<tss::SectionHeader>(
        inTheme, 
        tss::SectionHeader::SectionWidth::MasterEdit, 
        SynthDescriptors::getSectionDisplayName(SynthDescriptors::SectionIds::kMasterEdit),
        tss::SectionHeader::ColourVariant::Orange))
    , midiPanel(std::make_unique<MidiPanel>(inTheme, widgetFactory, apvts))
    , vibratoPanel(std::make_unique<VibratoPanel>(inTheme, widgetFactory, apvts))
    , miscPanel(std::make_unique<MiscPanel>(inTheme, widgetFactory, apvts))
{
    addAndMakeVisible(*sectionHeader);
    addAndMakeVisible(*midiPanel);
    addAndMakeVisible(*vibratoPanel);
    addAndMakeVisible(*miscPanel);

    setSize(getWidth(), getHeight());
}

MasterEditPanel::~MasterEditPanel() = default;

void MasterEditPanel::paint(juce::Graphics& g)
{
    if (auto* currentTheme = theme)
        g.fillAll(currentTheme->getMasterEditPanelBackgroundColour());
}

void MasterEditPanel::resized()
{
    const auto bounds = getLocalBounds();
    
    const auto sectionHeaderY = 0;
    sectionHeader->setBounds(
        bounds.getX() + 0,
        bounds.getY() + sectionHeaderY,
        bounds.getWidth(),
        tss::SectionHeader::getHeight()
    );
    
    const auto midiPanelY = sectionHeaderY + tss::SectionHeader::getHeight();
    midiPanel->setBounds(
        bounds.getX() + 0,
        bounds.getY() + midiPanelY,
        MidiPanel::getWidth(),
        MidiPanel::getHeight()
    );
    
    const auto vibratoPanelY = midiPanelY + MidiPanel::getHeight();
    vibratoPanel->setBounds(
        bounds.getX() + 0,
        bounds.getY() + vibratoPanelY,
        VibratoPanel::getWidth(),
        VibratoPanel::getHeight()
    );
    
    const auto miscPanelY = vibratoPanelY + VibratoPanel::getHeight();
    miscPanel->setBounds(
        bounds.getX() + 0,
        bounds.getY() + miscPanelY,
        MiscPanel::getWidth(),
        MiscPanel::getHeight()
    );
}

void MasterEditPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = sectionHeader.get())
        header->setTheme(inTheme);

    if (auto* panel = midiPanel.get())
        panel->setTheme(inTheme);

    if (auto* panel = vibratoPanel.get())
        panel->setTheme(inTheme);

    if (auto* panel = miscPanel.get())
        panel->setTheme(inTheme);

    repaint();
}

