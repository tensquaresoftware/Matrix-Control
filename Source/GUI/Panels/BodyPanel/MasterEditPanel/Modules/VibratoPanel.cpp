#include "VibratoPanel.h"

#include "../../../../Themes/Theme.h"
#include "../../../../Widgets/ModuleHeader.h"
#include "../../../../Widgets/Label.h"
#include "../../../../Widgets/Slider.h"
#include "../../../../Widgets/ComboBox.h"
#include "../../../../Widgets/HorizontalSeparator.h"
#include "../../../../../Shared/PluginDescriptors.h"
#include "../../../../Factories/WidgetFactory.h"

using tss::Theme;

VibratoPanel::VibratoPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    setupModuleHeader(inTheme, widgetFactory, PluginDescriptors::ModuleIds::kVibrato);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kVibratoSpeed,
                                vibratoSpeedLabel, vibratoSpeedSlider, vibratoSpeedAttachment, horizontalSeparator1);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kVibratoWaveform,
                                     vibratoWaveformLabel, vibratoWaveformComboBox, vibratoWaveformAttachment, horizontalSeparator2);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kVibratoAmplitude,
                                vibratoAmplitudeLabel, vibratoAmplitudeSlider, vibratoAmplitudeAttachment, horizontalSeparator3);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kVibratoSpeedModSource,
                                     vibratoSpeedModSourceLabel, vibratoSpeedModSourceComboBox, vibratoSpeedModSourceAttachment, horizontalSeparator4);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kVibratoSpeedModAmount,
                                vibratoSpeedModAmountLabel, vibratoSpeedModAmountSlider, vibratoSpeedModAmountAttachment, horizontalSeparator5);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kVibratoAmpModSource,
                                     vibratoAmpModSourceLabel, vibratoAmpModSourceComboBox, vibratoAmpModSourceAttachment, horizontalSeparator6);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kVibratoAmpModAmount,
                                vibratoAmpModAmountLabel, vibratoAmpModAmountSlider, vibratoAmpModAmountAttachment, horizontalSeparator7);

    setSize(getWidth(), getHeight());
}

VibratoPanel::~VibratoPanel() = default;

void VibratoPanel::paint(juce::Graphics& g)
{
    g.fillAll(theme->getMasterEditPanelBackgroundColour());
}

void VibratoPanel::resized()
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

    if (auto* header = vibratoModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    y += moduleHeaderHeight;

    if (auto* label = vibratoSpeedLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vibratoSpeedSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vibratoWaveformLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = vibratoWaveformComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vibratoAmplitudeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vibratoAmplitudeSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vibratoSpeedModSourceLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = vibratoSpeedModSourceComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vibratoSpeedModAmountLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vibratoSpeedModAmountSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vibratoAmpModSourceLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = vibratoAmpModSourceComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vibratoAmpModAmountLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vibratoAmpModAmountSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void VibratoPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = vibratoModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* label = vibratoSpeedLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = vibratoSpeedSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = vibratoWaveformLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = vibratoWaveformComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = vibratoAmplitudeLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = vibratoAmplitudeSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = vibratoSpeedModSourceLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = vibratoSpeedModSourceComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = vibratoSpeedModAmountLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = vibratoSpeedModAmountSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = vibratoAmpModSourceLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = vibratoAmpModSourceComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = vibratoAmpModAmountLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = vibratoAmpModAmountSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator7.get())
        separator->setTheme(inTheme);

    repaint();
}

void VibratoPanel::setupModuleHeader(Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    vibratoModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(moduleId),
        tss::ModuleHeader::ModuleWidth::MasterEdit,
        tss::ModuleHeader::ColourVariant::Orange);
    addAndMakeVisible(*vibratoModuleHeader);
}

void VibratoPanel::setupIntParameterWithSlider(Theme& inTheme, WidgetFactory& widgetFactory,
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

void VibratoPanel::setupChoiceParameterWithComboBox(Theme& inTheme, WidgetFactory& widgetFactory,
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
