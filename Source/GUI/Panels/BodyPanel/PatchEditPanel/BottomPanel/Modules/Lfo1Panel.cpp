#include "Lfo1Panel.h"

#include "../../../../../Themes/Theme.h"
#include "../../../../../Widgets/ModuleHeader.h"
#include "../../../../../Widgets/Label.h"
#include "../../../../../Widgets/Slider.h"
#include "../../../../../Widgets/ComboBox.h"
#include "../../../../../Widgets/Button.h"
#include "../../../../../Widgets/HorizontalSeparator.h"
#include "../../../../../../Shared/PluginDescriptors.h"
#include "../../../../../../GUI/Factories/WidgetFactory.h"

using tss::Theme;

Lfo1Panel::Lfo1Panel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    setupModuleHeader(inTheme, widgetFactory, PluginDescriptors::ModuleIds::kLfo1);
    setupInitCopyPasteButtons(inTheme, widgetFactory,
                              PluginDescriptors::StandaloneWidgetIds::kLfo1Init,
                              PluginDescriptors::StandaloneWidgetIds::kLfo1Copy,
                              PluginDescriptors::StandaloneWidgetIds::kLfo1Paste);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kLfo1Speed,
                                lfo1SpeedLabel, lfo1SpeedSlider, lfo1SpeedAttachment, horizontalSeparator1);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kLfo1SpeedModByPressure,
                                lfo1SpeedModByPressureLabel, lfo1SpeedModByPressureSlider, lfo1SpeedModByPressureAttachment, horizontalSeparator2);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kLfo1RetriggerPoint,
                                lfo1RetriggerPointLabel, lfo1RetriggerPointSlider, lfo1RetriggerPointAttachment, horizontalSeparator3);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kLfo1Amplitude,
                                lfo1AmplitudeLabel, lfo1AmplitudeSlider, lfo1AmplitudeAttachment, horizontalSeparator4);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kLfo1AmplitudeModByRamp1,
                                lfo1AmplitudeModByRamp1Label, lfo1AmplitudeModByRamp1Slider, lfo1AmplitudeModByRamp1Attachment, horizontalSeparator5);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kLfo1Waveform,
                                     lfo1WaveformLabel, lfo1WaveformComboBox, lfo1WaveformAttachment, horizontalSeparator6);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kLfo1TriggerMode,
                                     lfo1TriggerModeLabel, lfo1TriggerModeComboBox, lfo1TriggerModeAttachment, horizontalSeparator7);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kLfo1Lag,
                                     lfo1LagLabel, lfo1LagComboBox, lfo1LagAttachment, horizontalSeparator8);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kLfo1SampleInput,
                                     lfo1SampleInputLabel, lfo1SampleInputComboBox, lfo1SampleInputAttachment, horizontalSeparator9);

    horizontalSeparator10 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator10);

    setSize(getWidth(), getHeight());
}

Lfo1Panel::~Lfo1Panel() = default;

void Lfo1Panel::paint(juce::Graphics& g)
{
    if (auto* currentTheme = theme)
        g.fillAll(currentTheme->getPatchEditModulePanelBackgroundColour());
}

void Lfo1Panel::resized()
{
    const auto moduleHeaderHeight = tss::ModuleHeader::getHeight();
    const auto moduleHeaderWidth = tss::ModuleHeader::getWidth(tss::ModuleHeader::ModuleWidth::PatchEdit);
    const auto buttonWidth = tss::Button::getWidth(tss::Button::ButtonWidth::InitCopyPaste);
    const auto buttonHeight = tss::Button::getHeight();
    const auto labelWidth = tss::Label::getWidth(tss::Label::LabelWidth::PatchEditModule);
    const auto labelHeight = tss::Label::getHeight();
    const auto sliderWidth = tss::Slider::getWidth();
    const auto sliderHeight = tss::Slider::getHeight();
    const auto comboBoxWidth = tss::ComboBox::getWidth(tss::ComboBox::ComboBoxWidth::PatchEditModule);
    const auto comboBoxHeight = tss::ComboBox::getHeight();
    const auto separatorWidth = tss::HorizontalSeparator::getWidth(tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    const auto separatorHeight = tss::HorizontalSeparator::getHeight();
    const auto panelWidth = getWidth();

    int y = 0;

    if (auto* header = lfo1ModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    if (auto* button = lfo1PasteButton.get())
        button->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);

    if (auto* button = lfo1CopyButton.get())
        button->setBounds(panelWidth - buttonWidth * 2, y, buttonWidth, buttonHeight);

    if (auto* button = lfo1InitButton.get())
        button->setBounds(panelWidth - buttonWidth * 3, y, buttonWidth, buttonHeight);

    y += moduleHeaderHeight;

    if (auto* label = lfo1SpeedLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = lfo1SpeedSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo1SpeedModByPressureLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = lfo1SpeedModByPressureSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo1RetriggerPointLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = lfo1RetriggerPointSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo1AmplitudeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = lfo1AmplitudeSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo1AmplitudeModByRamp1Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = lfo1AmplitudeModByRamp1Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo1WaveformLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = lfo1WaveformComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo1TriggerModeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = lfo1TriggerModeComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo1LagLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = lfo1LagComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator8.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo1SampleInputLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = lfo1SampleInputComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator9.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    y += labelHeight;

    if (auto* separator = horizontalSeparator10.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void Lfo1Panel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = lfo1ModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* button = lfo1InitButton.get())
        button->setTheme(inTheme);

    if (auto* button = lfo1CopyButton.get())
        button->setTheme(inTheme);

    if (auto* button = lfo1PasteButton.get())
        button->setTheme(inTheme);

    if (auto* label = lfo1SpeedLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = lfo1SpeedSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo1SpeedModByPressureLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = lfo1SpeedModByPressureSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo1RetriggerPointLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = lfo1RetriggerPointSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo1AmplitudeLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = lfo1AmplitudeSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo1AmplitudeModByRamp1Label.get())
        label->setTheme(inTheme);

    if (auto* slider = lfo1AmplitudeModByRamp1Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo1WaveformLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = lfo1WaveformComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo1TriggerModeLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = lfo1TriggerModeComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator7.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo1LagLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = lfo1LagComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator8.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo1SampleInputLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = lfo1SampleInputComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator9.get())
        separator->setTheme(inTheme);

    if (auto* separator = horizontalSeparator10.get())
        separator->setTheme(inTheme);

    repaint();
}

void Lfo1Panel::setupModuleHeader(Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    lfo1ModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(moduleId));
    addAndMakeVisible(*lfo1ModuleHeader);
}

void Lfo1Panel::setupInitCopyPasteButtons(Theme& inTheme, WidgetFactory& widgetFactory,
                                         const juce::String& initWidgetId, const juce::String& copyWidgetId, const juce::String& pasteWidgetId)
{
    lfo1InitButton = widgetFactory.createStandaloneButton(initWidgetId, inTheme);
    lfo1InitButton->onClick = [this, initWidgetId]
    {
        apvts.state.setProperty(initWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*lfo1InitButton);

    lfo1CopyButton = widgetFactory.createStandaloneButton(copyWidgetId, inTheme);
    lfo1CopyButton->onClick = [this, copyWidgetId]
    {
        apvts.state.setProperty(copyWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*lfo1CopyButton);

    lfo1PasteButton = widgetFactory.createStandaloneButton(pasteWidgetId, inTheme);
    lfo1PasteButton->onClick = [this, pasteWidgetId]
    {
        apvts.state.setProperty(pasteWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*lfo1PasteButton);
}

void Lfo1Panel::setupIntParameterWithSlider(Theme& inTheme, WidgetFactory& widgetFactory,
                                            const juce::String& parameterId,
                                            std::unique_ptr<tss::Label>& label,
                                            std::unique_ptr<tss::Slider>& slider,
                                            std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
                                            std::unique_ptr<tss::HorizontalSeparator>& separator)
{
    label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(parameterId));
    addAndMakeVisible(*label);

    slider = widgetFactory.createIntParameterSlider(parameterId, inTheme);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        parameterId,
        *slider);
    addAndMakeVisible(*slider);

    separator = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*separator);
}

void Lfo1Panel::setupChoiceParameterWithComboBox(Theme& inTheme, WidgetFactory& widgetFactory,
                                                 const juce::String& parameterId,
                                                 std::unique_ptr<tss::Label>& label,
                                                 std::unique_ptr<tss::ComboBox>& comboBox,
                                                 std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>& attachment,
                                                 std::unique_ptr<tss::HorizontalSeparator>& separator)
{
    label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(parameterId));
    addAndMakeVisible(*label);

    comboBox = widgetFactory.createChoiceParameterComboBox(parameterId, inTheme);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        parameterId,
        *comboBox);
    addAndMakeVisible(*comboBox);

    separator = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*separator);
}
