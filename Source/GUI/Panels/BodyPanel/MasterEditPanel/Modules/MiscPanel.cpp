#include "MiscPanel.h"

#include "../../../../Themes/Theme.h"
#include "../../../../Widgets/ModuleHeader.h"
#include "../../../../Widgets/Label.h"
#include "../../../../Widgets/Slider.h"
#include "../../../../Widgets/ComboBox.h"
#include "../../../../Widgets/HorizontalSeparator.h"
#include "../../../../../Shared/PluginDescriptors.h"
#include "../../../../Factories/WidgetFactory.h"

using tss::Theme;

MiscPanel::MiscPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    setupModuleHeader(inTheme, widgetFactory, PluginDescriptors::ModuleIds::kMisc);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kMasterTune,
                                masterTuneLabel, masterTuneSlider, masterTuneAttachment, horizontalSeparator1);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kMasterTranspose,
                                masterTransposeLabel, masterTransposeSlider, masterTransposeAttachment, horizontalSeparator2);
    masterTransposeSlider->setUnit("st");

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kBendRange,
                                bendRangeLabel, bendRangeSlider, bendRangeAttachment, horizontalSeparator3);
    bendRangeSlider->setUnit("st");

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kUnisonEnable,
                                     unisonEnableLabel, unisonEnableComboBox, unisonEnableAttachment, horizontalSeparator4);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kVolumeInvertEnable,
                                     volumeInvertEnableLabel, volumeInvertEnableComboBox, volumeInvertEnableAttachment, horizontalSeparator5);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kBankLockEnable,
                                     bankLockEnableLabel, bankLockEnableComboBox, bankLockEnableAttachment, horizontalSeparator6);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kMemoryProtectEnable,
                                     memoryProtectEnableLabel, memoryProtectEnableComboBox, memoryProtectEnableAttachment, horizontalSeparator7);

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

void MiscPanel::setupModuleHeader(Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    miscModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(moduleId),
        tss::ModuleHeader::ModuleWidth::MasterEdit,
        tss::ModuleHeader::ColourVariant::Orange);
    addAndMakeVisible(*miscModuleHeader);
}

void MiscPanel::setupIntParameterWithSlider(Theme& inTheme, WidgetFactory& widgetFactory,
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

void MiscPanel::setupChoiceParameterWithComboBox(Theme& inTheme, WidgetFactory& widgetFactory,
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
