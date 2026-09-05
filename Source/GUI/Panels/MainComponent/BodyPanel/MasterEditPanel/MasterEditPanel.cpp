#include "MasterEditPanel.h"

#include "Modules/MidiPanel.h"
#include "Modules/VibratoPanel.h"
#include "Modules/MiscPanel.h"

#include "Core/MIDI/EditorOutboundGate.h"
#include "Core/MIDI/MasterEditGate.h"
#include "Core/Services/DeviceTypeRegistry.h"
#include "GUI/Layout/ScaledLayout.h"
#include "GUI/Looks/LookBuilders.h"
#include "GUI/Skins/ISkin.h"
#include "GUI/Skins/SkinHelpers.h"
#include "GUI/Skins/SkinValues.h"
#include "GUI/Widgets/SectionHeader.h"
#include "GUI/Widgets/ModuleHeader.h"
#include "Shared/Definitions/MatrixDeviceTypes.h"
#include "Shared/Definitions/PluginDescriptors.h"
#include "Shared/Definitions/PluginHelpers.h"
#include "Shared/Definitions/PluginIDs.h"
#include "GUI/Factories/WidgetFactory.h"

MasterEditPanel::MasterEditPanel(TSS::ISkin& skin, const MasterEditPanelDimensions& dims, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : dims_(dims)
    , skin_(&skin)
    , apvts_(apvts)
    , sectionHeader_(std::make_unique<TSS::SectionHeader>(
        dims_.sectionHeaderWidth,
        dims_.sectionHeaderHeight,
        TSS::sectionHeaderLookFromSkin(skin),
        PluginHelpers::getSectionDisplayName(PluginIDs::MasterEditSection::kGroupId),
        TSS::SectionHeader::ColourVariant::Orange))
    , midiPanel_(std::make_unique<MidiPanel>(MidiPanel::Config{
          .skin = skin,
          .width = dims_.childModuleWidth,
          .height = dims_.midiPanelHeight,
          .widgetFactory = widgetFactory,
          .apvts = apvts,
          .moduleHeaderDims = dims_.moduleHeader,
          .parameterCellDims = dims_.parameterCell}))
    , vibratoPanel_(std::make_unique<VibratoPanel>(VibratoPanel::Config{
          .skin = skin,
          .width = dims_.childModuleWidth,
          .height = dims_.vibratoPanelHeight,
          .widgetFactory = widgetFactory,
          .apvts = apvts,
          .moduleHeaderDims = dims_.moduleHeader,
          .parameterCellDims = dims_.parameterCell}))
    , miscPanel_(std::make_unique<MiscPanel>(MiscPanel::Config{
          .skin = skin,
          .width = dims_.childModuleWidth,
          .height = dims_.miscPanelHeight,
          .widgetFactory = widgetFactory,
          .apvts = apvts,
          .moduleHeaderDims = dims_.moduleHeader,
          .parameterCellDims = dims_.parameterCell}))
{
    setOpaque(true);
    addAndMakeVisible(*sectionHeader_);
    addAndMakeVisible(*midiPanel_);
    addAndMakeVisible(*vibratoPanel_);
    addAndMakeVisible(*miscPanel_);

    apvts_.state.addListener(this);
    refreshDeviceGating();

    setSize(dims_.width, dims_.height);
}

MasterEditPanel::~MasterEditPanel()
{
    apvts_.state.removeListener(this);
}

void MasterEditPanel::valueTreePropertyChanged(juce::ValueTree&,
                                               const juce::Identifier& property)
{
    const auto propertyName = property.toString();
    namespace MutatorState = PluginIDs::PatchManagerSection::PatchMutatorModule::StateProperties;

    if (propertyName == MatrixDeviceTypes::kApvtsPropertyName
        || propertyName == "deviceDetected"
        || propertyName == Core::kDeviceMidiUnresponsiveProperty
        || propertyName == MutatorState::kCompareActive)
    {
        refreshDeviceGating();
    }
}

void MasterEditPanel::valueTreeRedirected(juce::ValueTree&)
{
    refreshDeviceGating();
}

void MasterEditPanel::refreshDeviceGating()
{
    const bool deviceDetected = static_cast<bool>(apvts_.state.getProperty("deviceDetected", false));
    const bool deviceMidiUnresponsive = static_cast<bool>(
        apvts_.state.getProperty(Core::kDeviceMidiUnresponsiveProperty, false));
    const auto deviceType = Core::DeviceTypeRegistry::fromApvtsProperty(
        apvts_.state.getProperty(MatrixDeviceTypes::kApvtsPropertyName));
    const bool compareActive = static_cast<bool>(apvts_.state.getProperty(
        PluginIDs::PatchManagerSection::PatchMutatorModule::StateProperties::kCompareActive,
        false));

    // Root Compare/device lock owns dimming — skip hide so children stay under that lock.
    const bool rootLocked = Core::isSectionLocked(
        deviceDetected, deviceType, compareActive, deviceMidiUnresponsive);
    const bool shouldHide = Core::shouldHideMasterEditContent(
        rootLocked, deviceDetected, deviceType);
    applyMasterEditContentVisibility(shouldHide);
}

void MasterEditPanel::applyMasterEditContentVisibility(bool hidden)
{
    const bool stateChanged = hidden != masterEditContentHidden_;
    masterEditContentHidden_ = hidden;

    if (hidden && stateChanged)
    {
        juce::PopupMenu::dismissAllActiveMenus();
        giveAwayKeyboardFocus();
    }

    const bool contentVisible = ! hidden;
    sectionHeader_->setVisible(contentVisible);
    midiPanel_->setVisible(contentVisible);
    vibratoPanel_->setVisible(contentVisible);
    miscPanel_->setVisible(contentVisible);

    if (stateChanged)
        repaint();
}

void MasterEditPanel::paint(juce::Graphics& g)
{
    if (skin_ != nullptr)
        g.fillAll(skin_->getColour(TSS::SkinColourId::kBodyPanelBackground));
}

void MasterEditPanel::resized()
{
    auto area = getLocalBounds();
    const float sf = uiScale_;

    const int padding = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.padding), sf);
    area.reduce(padding, padding);

    const int sectionHeaderHeight = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dims_.sectionHeaderHeight), sf);
    const int childWidth = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.childModuleWidth), sf);
    const int midiH = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.midiPanelHeight), sf);
    const int vibratoH = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.vibratoPanelHeight), sf);
    const int miscH = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.miscPanelHeight), sf);
    const int interModuleGap = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.interModuleGap), sf);

    sectionHeader_->setBounds(area.removeFromTop(sectionHeaderHeight));

    midiPanel_->setBounds(area.removeFromTop(midiH).withWidth(childWidth));
    area.removeFromTop(interModuleGap);
    vibratoPanel_->setBounds(area.removeFromTop(vibratoH).withWidth(childWidth));
    area.removeFromTop(interModuleGap);
    miscPanel_->setBounds(area.removeFromTop(miscH).withWidth(childWidth));
}

void MasterEditPanel::setSkin(TSS::ISkin& skin)
{
    skin_ = &skin;
    sectionHeader_->setLook(TSS::sectionHeaderLookFromSkin(skin));
    TSS::propagateSkin(skin,
        midiPanel_.get(),
        vibratoPanel_.get(),
        miscPanel_.get());

    applyMasterEditContentVisibility(masterEditContentHidden_);

    repaint();
}

void MasterEditPanel::setInitConfirmationGate(TSS::ModuleHeader::InitConfirmationGate gate)
{
    if (midiPanel_)
        midiPanel_->setInitConfirmationGate(gate);
    if (vibratoPanel_)
        vibratoPanel_->setInitConfirmationGate(gate);
    if (miscPanel_)
        miscPanel_->setInitConfirmationGate(gate);
}

void MasterEditPanel::setUiScale(float uiScale)
{
    if (juce::approximatelyEqual(uiScale_, uiScale))
        return;
    
    uiScale_ = uiScale;
    
    if (sectionHeader_)
        sectionHeader_->setUiScale(uiScale_);
    if (midiPanel_)
        midiPanel_->setUiScale(uiScale_);
    if (vibratoPanel_)
        vibratoPanel_->setUiScale(uiScale_);
    if (miscPanel_)
        miscPanel_->setUiScale(uiScale_);
    
    resized();
    repaint();
}
