#include "MiscPanel.h"

#include "../../../../Themes/Theme.h"
#include "../../../../Widgets/ModuleHeader.h"
#include "../../../../Widgets/Label.h"
#include "../../../../Widgets/Slider.h"
#include "../../../../Widgets/ComboBox.h"
#include "../../../../Widgets/HorizontalSeparator.h"
#include "../../../../../Shared/SynthDescriptors.h"
#include "../../../../Factories/WidgetFactory.h"

using tss::Theme;

MiscPanel::MiscPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    miscModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kMisc),
        tss::ModuleHeader::ModuleWidth::MasterEdit,
        tss::ModuleHeader::ColourVariant::Orange);
    addAndMakeVisible(*miscModuleHeader);

    // Master Tune
    masterTuneLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::MasterEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kMasterTune));
    addAndMakeVisible(*masterTuneLabel);

    masterTuneSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kMasterTune, inTheme);
    masterTuneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kMasterTune,
        *masterTuneSlider);
    addAndMakeVisible(*masterTuneSlider);

    horizontalSeparator1 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::MasterEditModule);
    addAndMakeVisible(*horizontalSeparator1);

    // Master Transpose
    masterTransposeLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::MasterEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kMasterTranspose));
    addAndMakeVisible(*masterTransposeLabel);

    masterTransposeSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kMasterTranspose, inTheme);
    masterTransposeSlider->setUnit("st");
    masterTransposeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kMasterTranspose,
        *masterTransposeSlider);
    addAndMakeVisible(*masterTransposeSlider);

    horizontalSeparator2 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::MasterEditModule);
    addAndMakeVisible(*horizontalSeparator2);

    // Bend Range
    bendRangeLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::MasterEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kBendRange));
    addAndMakeVisible(*bendRangeLabel);

    bendRangeSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kBendRange, inTheme);
    bendRangeSlider->setUnit("st");
    bendRangeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kBendRange,
        *bendRangeSlider);
    addAndMakeVisible(*bendRangeSlider);

    horizontalSeparator3 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::MasterEditModule);
    addAndMakeVisible(*horizontalSeparator3);

    // Unison
    unisonEnableLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::MasterEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kUnisonEnable));
    addAndMakeVisible(*unisonEnableLabel);

    unisonEnableComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kUnisonEnable, inTheme);
    unisonEnableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kUnisonEnable,
        *unisonEnableComboBox);
    addAndMakeVisible(*unisonEnableComboBox);

    horizontalSeparator4 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::MasterEditModule);
    addAndMakeVisible(*horizontalSeparator4);

    // Volume Invert
    volumeInvertEnableLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::MasterEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVolumeInvertEnable));
    addAndMakeVisible(*volumeInvertEnableLabel);

    volumeInvertEnableComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kVolumeInvertEnable, inTheme);
    volumeInvertEnableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVolumeInvertEnable,
        *volumeInvertEnableComboBox);
    addAndMakeVisible(*volumeInvertEnableComboBox);

    horizontalSeparator5 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::MasterEditModule);
    addAndMakeVisible(*horizontalSeparator5);

    // Bank Lock
    bankLockEnableLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::MasterEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kBankLockEnable));
    addAndMakeVisible(*bankLockEnableLabel);

    bankLockEnableComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kBankLockEnable, inTheme);
    bankLockEnableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kBankLockEnable,
        *bankLockEnableComboBox);
    addAndMakeVisible(*bankLockEnableComboBox);

    horizontalSeparator6 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::MasterEditModule);
    addAndMakeVisible(*horizontalSeparator6);

    // Memory Protect
    memoryProtectEnableLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::MasterEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kMemoryProtectEnable));
    addAndMakeVisible(*memoryProtectEnableLabel);

    memoryProtectEnableComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kMemoryProtectEnable, inTheme);
    memoryProtectEnableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kMemoryProtectEnable,
        *memoryProtectEnableComboBox);
    addAndMakeVisible(*memoryProtectEnableComboBox);

    horizontalSeparator7 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::MasterEditModule);
    addAndMakeVisible(*horizontalSeparator7);

    setSize(getWidth(), getHeight());
}

MiscPanel::~MiscPanel() = default;

void MiscPanel::paint(juce::Graphics& g)
{
    g.fillAll(theme->getMasterEditPanelBackgroundColour());
}

void MiscPanel::resized()
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

    if (auto* header = miscModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    y += moduleHeaderHeight;

    if (auto* label = masterTuneLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = masterTuneSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = masterTransposeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = masterTransposeSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = bendRangeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = bendRangeSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = unisonEnableLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = unisonEnableComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = volumeInvertEnableLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = volumeInvertEnableComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = bankLockEnableLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = bankLockEnableComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = memoryProtectEnableLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = memoryProtectEnableComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void MiscPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = miscModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* label = masterTuneLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = masterTuneSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = masterTransposeLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = masterTransposeSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = bendRangeLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = bendRangeSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = unisonEnableLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = unisonEnableComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = volumeInvertEnableLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = volumeInvertEnableComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = bankLockEnableLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = bankLockEnableComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = memoryProtectEnableLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = memoryProtectEnableComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator7.get())
        separator->setTheme(inTheme);

    repaint();
}
