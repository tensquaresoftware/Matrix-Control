#include "MidiPanel.h"

#include "../../../../Themes/Theme.h"
#include "../../../../Widgets/ModuleHeader.h"
#include "../../../../Widgets/Label.h"
#include "../../../../Widgets/Slider.h"
#include "../../../../Widgets/ComboBox.h"
#include "../../../../Widgets/HorizontalSeparator.h"
#include "../../../../../Shared/PluginDescriptors.h"
#include "../../../../Factories/WidgetFactory.h"

using tss::Theme;

MidiPanel::MidiPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    setupModuleHeader(inTheme, widgetFactory, PluginDescriptors::ModuleIds::kMidi);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kMidiChannel,
                                     midiChannelLabel, midiChannelComboBox, midiChannelAttachment, horizontalSeparator1);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kMidiEcho,
                                     midiEchoLabel, midiEchoComboBox, midiEchoAttachment, horizontalSeparator2);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kMidiControllers,
                                     midiControllersLabel, midiControllersComboBox, midiControllersAttachment, horizontalSeparator3);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kMidiPatchChanges,
                                     midiPatchChangesLabel, midiPatchChangesComboBox, midiPatchChangesAttachment, horizontalSeparator4);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kMidiPedal1Select,
                                midiPedal1SelectLabel, midiPedal1SelectSlider, midiPedal1SelectAttachment, horizontalSeparator5);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kMidiPedal2Select,
                                midiPedal2SelectLabel, midiPedal2SelectSlider, midiPedal2SelectAttachment, horizontalSeparator6);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kMidiLever2Select,
                                midiLever2SelectLabel, midiLever2SelectSlider, midiLever2SelectAttachment, horizontalSeparator7);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kMidiLever3Select,
                                midiLever3SelectLabel, midiLever3SelectSlider, midiLever3SelectAttachment, horizontalSeparator8);

    setSize(getWidth(), getHeight());
}

MidiPanel::~MidiPanel() = default;

void MidiPanel::paint(juce::Graphics& g)
{
    g.fillAll(theme->getMasterEditPanelBackgroundColour());
}

void MidiPanel::resized()
{
    const auto moduleHeaderHeight = tss::ModuleHeader::getHeight();
    const auto moduleHeaderWidth = tss::ModuleHeader::getWidth(tss::ModuleHeader::ModuleWidth::MasterEdit);
    const auto labelWidth = tss::Label::getWidth(tss::Label::LabelWidth::MasterEditModule);
    const auto labelHeight = tss::Label::getHeight();
    const auto sliderWidth = tss::Slider::getWidth();
    const auto sliderHeight = tss::Slider::getHeight();
    const auto comboBoxWidth = tss::ComboBox::getWidth(tss::ComboBox::ComboBoxWidth::MasterEditModule);
    const auto comboBoxHeight = tss::ComboBox::getHeight();
    const auto separatorWidth = tss::HorizontalSeparator::getWidth(tss::HorizontalSeparator::SeparatorWidth::MasterEditModule);
    const auto separatorHeight = tss::HorizontalSeparator::getHeight();

    int y = 0;

    if (auto* header = midiModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    y += moduleHeaderHeight;

    if (auto* label = midiChannelLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = midiChannelComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = midiEchoLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = midiEchoComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = midiControllersLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = midiControllersComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = midiPatchChangesLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = midiPatchChangesComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = midiPedal1SelectLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = midiPedal1SelectSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = midiPedal2SelectLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = midiPedal2SelectSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = midiLever2SelectLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = midiLever2SelectSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = midiLever3SelectLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = midiLever3SelectSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator8.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void MidiPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = midiModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* label = midiChannelLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = midiChannelComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = midiEchoLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = midiEchoComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = midiControllersLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = midiControllersComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = midiPatchChangesLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = midiPatchChangesComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = midiPedal1SelectLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = midiPedal1SelectSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = midiPedal2SelectLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = midiPedal2SelectSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = midiLever2SelectLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = midiLever2SelectSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator7.get())
        separator->setTheme(inTheme);

    if (auto* label = midiLever3SelectLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = midiLever3SelectSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator8.get())
        separator->setTheme(inTheme);

    repaint();
}

void MidiPanel::setupModuleHeader(Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    midiModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(moduleId),
        tss::ModuleHeader::ModuleWidth::MasterEdit,
        tss::ModuleHeader::ColourVariant::Orange);
    addAndMakeVisible(*midiModuleHeader);
}

void MidiPanel::setupIntParameterWithSlider(Theme& inTheme, WidgetFactory& widgetFactory,
                                            const juce::String& parameterId,
                                            std::unique_ptr<tss::Label>& label,
                                            std::unique_ptr<tss::Slider>& slider,
                                            std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
                                            std::unique_ptr<tss::HorizontalSeparator>& separator)
{
    label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::MasterEditModule, 
        widgetFactory.getParameterDisplayName(parameterId));
    addAndMakeVisible(*label);

    slider = widgetFactory.createIntParameterSlider(parameterId, inTheme);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        parameterId,
        *slider);
    addAndMakeVisible(*slider);

    separator = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::MasterEditModule);
    addAndMakeVisible(*separator);
}

void MidiPanel::setupChoiceParameterWithComboBox(Theme& inTheme, WidgetFactory& widgetFactory,
                                                 const juce::String& parameterId,
                                                 std::unique_ptr<tss::Label>& label,
                                                 std::unique_ptr<tss::ComboBox>& comboBox,
                                                 std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>& attachment,
                                                 std::unique_ptr<tss::HorizontalSeparator>& separator)
{
    label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::MasterEditModule, 
        widgetFactory.getParameterDisplayName(parameterId));
    addAndMakeVisible(*label);

    comboBox = widgetFactory.createChoiceParameterComboBox(parameterId, inTheme);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        parameterId,
        *comboBox);
    addAndMakeVisible(*comboBox);

    separator = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::MasterEditModule);
    addAndMakeVisible(*separator);
}
