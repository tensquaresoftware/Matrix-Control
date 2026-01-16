#include "MidiPanel.h"

#include "../../../../Themes/Theme.h"
#include "../../../../Widgets/ModuleHeader.h"
#include "../../../../Widgets/Label.h"
#include "../../../../Widgets/Slider.h"
#include "../../../../Widgets/ComboBox.h"
#include "../../../../Widgets/HorizontalSeparator.h"
#include "../../../../../Shared/PluginDescriptors.h"
#include "../../../../../Shared/PluginDimensions.h"
#include "../../../../Factories/WidgetFactory.h"

using tss::Theme;

MidiPanel::MidiPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
    , apvts_(apvts)
{
    setupModuleHeader(theme, widgetFactory, PluginDescriptors::ModuleIds::kMidi);

    setupChoiceParameterWithComboBox(theme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kMidiChannel,
                                     midiChannelLabel_, midiChannelComboBox_, midiChannelAttachment_, horizontalSeparator1_);

    setupChoiceParameterWithComboBox(theme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kMidiEcho,
                                     midiEchoLabel_, midiEchoComboBox_, midiEchoAttachment_, horizontalSeparator2_);

    setupChoiceParameterWithComboBox(theme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kMidiControllers,
                                     midiControllersLabel_, midiControllersComboBox_, midiControllersAttachment_, horizontalSeparator3_);

    setupChoiceParameterWithComboBox(theme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kMidiPatchChanges,
                                     midiPatchChangesLabel_, midiPatchChangesComboBox_, midiPatchChangesAttachment_, horizontalSeparator4_);

    setupIntParameterWithSlider(theme, widgetFactory,
                                PluginDescriptors::ParameterIds::kMidiPedal1Select,
                                midiPedal1SelectLabel_, midiPedal1SelectSlider_, midiPedal1SelectAttachment_, horizontalSeparator5_);

    setupIntParameterWithSlider(theme, widgetFactory,
                                PluginDescriptors::ParameterIds::kMidiPedal2Select,
                                midiPedal2SelectLabel_, midiPedal2SelectSlider_, midiPedal2SelectAttachment_, horizontalSeparator6_);

    setupIntParameterWithSlider(theme, widgetFactory,
                                PluginDescriptors::ParameterIds::kMidiLever2Select,
                                midiLever2SelectLabel_, midiLever2SelectSlider_, midiLever2SelectAttachment_, horizontalSeparator7_);

    setupIntParameterWithSlider(theme, widgetFactory,
                                PluginDescriptors::ParameterIds::kMidiLever3Select,
                                midiLever3SelectLabel_, midiLever3SelectSlider_, midiLever3SelectAttachment_, horizontalSeparator8_);

    setSize(getWidth(), getHeight());
}

MidiPanel::~MidiPanel() = default;

void MidiPanel::paint(juce::Graphics& g)
{
    g.fillAll(theme_->getMasterEditPanelBackgroundColour());
}

void MidiPanel::resized()
{
    const auto moduleHeaderHeight = PluginDimensions::Widgets::Heights::kModuleHeader;
    const auto moduleHeaderWidth = PluginDimensions::Widgets::Widths::ModuleHeader::kMasterEditModule;
    const auto labelWidth = PluginDimensions::Widgets::Widths::Label::kMasterEditModule;
    const auto labelHeight = PluginDimensions::Widgets::Heights::kLabel;
    const auto sliderWidth = PluginDimensions::Widgets::Widths::Slider::kStandard;
    const auto sliderHeight = tss::Slider::getHeight();
    const auto comboBoxWidth = PluginDimensions::Widgets::Widths::ComboBox::kMasterEditModule;
    const auto comboBoxHeight = PluginDimensions::Widgets::Heights::kComboBox;
    const auto separatorWidth = PluginDimensions::Widgets::Widths::HorizontalSeparator::kMasterEditModule;
    const auto separatorHeight = PluginDimensions::Widgets::Heights::kHorizontalSeparator;

    int y = 0;

    if (auto* header = midiModuleHeader_.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    y += moduleHeaderHeight;

    if (auto* label = midiChannelLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = midiChannelComboBox_.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator1_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = midiEchoLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = midiEchoComboBox_.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = midiControllersLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = midiControllersComboBox_.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = midiPatchChangesLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = midiPatchChangesComboBox_.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = midiPedal1SelectLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = midiPedal1SelectSlider_.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = midiPedal2SelectLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = midiPedal2SelectSlider_.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = midiLever2SelectLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = midiLever2SelectSlider_.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = midiLever3SelectLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = midiLever3SelectSlider_.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator8_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void MidiPanel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = midiModuleHeader_.get())
        header->setTheme(theme);

    if (auto* label = midiChannelLabel_.get())
        label->setTheme(theme);

    if (auto* comboBox = midiChannelComboBox_.get())
        comboBox->setTheme(theme);

    if (auto* separator = horizontalSeparator1_.get())
        separator->setTheme(theme);

    if (auto* label = midiEchoLabel_.get())
        label->setTheme(theme);

    if (auto* comboBox = midiEchoComboBox_.get())
        comboBox->setTheme(theme);

    if (auto* separator = horizontalSeparator2_.get())
        separator->setTheme(theme);

    if (auto* label = midiControllersLabel_.get())
        label->setTheme(theme);

    if (auto* comboBox = midiControllersComboBox_.get())
        comboBox->setTheme(theme);

    if (auto* separator = horizontalSeparator3_.get())
        separator->setTheme(theme);

    if (auto* label = midiPatchChangesLabel_.get())
        label->setTheme(theme);

    if (auto* comboBox = midiPatchChangesComboBox_.get())
        comboBox->setTheme(theme);

    if (auto* separator = horizontalSeparator4_.get())
        separator->setTheme(theme);

    if (auto* label = midiPedal1SelectLabel_.get())
        label->setTheme(theme);

    if (auto* slider = midiPedal1SelectSlider_.get())
        slider->setTheme(theme);

    if (auto* separator = horizontalSeparator5_.get())
        separator->setTheme(theme);

    if (auto* label = midiPedal2SelectLabel_.get())
        label->setTheme(theme);

    if (auto* slider = midiPedal2SelectSlider_.get())
        slider->setTheme(theme);

    if (auto* separator = horizontalSeparator6_.get())
        separator->setTheme(theme);

    if (auto* label = midiLever2SelectLabel_.get())
        label->setTheme(theme);

    if (auto* slider = midiLever2SelectSlider_.get())
        slider->setTheme(theme);

    if (auto* separator = horizontalSeparator7_.get())
        separator->setTheme(theme);

    if (auto* label = midiLever3SelectLabel_.get())
        label->setTheme(theme);

    if (auto* slider = midiLever3SelectSlider_.get())
        slider->setTheme(theme);

    if (auto* separator = horizontalSeparator8_.get())
        separator->setTheme(theme);

    repaint();
}

void MidiPanel::setupModuleHeader(Theme& theme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    midiModuleHeader_ = std::make_unique<tss::ModuleHeader>(
        theme, 
        widgetFactory.getGroupDisplayName(moduleId),
        PluginDimensions::Widgets::Widths::ModuleHeader::kMasterEditModule,
        PluginDimensions::Widgets::Heights::kModuleHeader,
        tss::ModuleHeader::ColourVariant::Orange);
    addAndMakeVisible(*midiModuleHeader_);
}

void MidiPanel::setupIntParameterWithSlider(Theme& theme, WidgetFactory& widgetFactory,
                                            const juce::String& parameterId,
                                            std::unique_ptr<tss::Label>& label,
                                            std::unique_ptr<tss::Slider>& slider,
                                            std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
                                            std::unique_ptr<tss::HorizontalSeparator>& separator)
{
    label = std::make_unique<tss::Label>(
        theme, 
        PluginDimensions::Widgets::Widths::Label::kMasterEditModule,
        PluginDimensions::Widgets::Heights::kLabel,
        widgetFactory.getParameterDisplayName(parameterId));
    addAndMakeVisible(*label);

    slider = widgetFactory.createIntParameterSlider(parameterId, theme);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts_,
        parameterId,
        *slider);
    addAndMakeVisible(*slider);

    separator = std::make_unique<tss::HorizontalSeparator>(
        theme, 
        PluginDimensions::Widgets::Widths::HorizontalSeparator::kMasterEditModule,
        PluginDimensions::Widgets::Heights::kHorizontalSeparator);
    addAndMakeVisible(*separator);
}

void MidiPanel::setupChoiceParameterWithComboBox(Theme& theme, WidgetFactory& widgetFactory,
                                                 const juce::String& parameterId,
                                                 std::unique_ptr<tss::Label>& label,
                                                 std::unique_ptr<tss::ComboBox>& comboBox,
                                                 std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>& attachment,
                                                 std::unique_ptr<tss::HorizontalSeparator>& separator)
{
    label = std::make_unique<tss::Label>(
        theme, 
        PluginDimensions::Widgets::Widths::Label::kMasterEditModule,
        PluginDimensions::Widgets::Heights::kLabel,
        widgetFactory.getParameterDisplayName(parameterId));
    addAndMakeVisible(*label);

    comboBox = widgetFactory.createChoiceParameterComboBox(
        parameterId, 
        theme,
        PluginDimensions::Widgets::Widths::ComboBox::kMasterEditModule,
        PluginDimensions::Widgets::Heights::kComboBox);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts_,
        parameterId,
        *comboBox);
    addAndMakeVisible(*comboBox);

    separator = std::make_unique<tss::HorizontalSeparator>(
        theme, 
        PluginDimensions::Widgets::Widths::HorizontalSeparator::kMasterEditModule,
        PluginDimensions::Widgets::Heights::kHorizontalSeparator);
    addAndMakeVisible(*separator);
}
