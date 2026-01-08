#include "Lfo2Panel.h"

#include "../../../../../Themes/Theme.h"
#include "../../../../../Widgets/ModuleHeader.h"
#include "../../../../../Widgets/Label.h"
#include "../../../../../Widgets/Slider.h"
#include "../../../../../Widgets/ComboBox.h"
#include "../../../../../Widgets/Button.h"
#include "../../../../../Widgets/HorizontalSeparator.h"
#include "../../../../../../Shared/SynthDescriptors.h"
#include "../../../../../../GUI/Factories/WidgetFactory.h"

using tss::Theme;

Lfo2Panel::Lfo2Panel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    lfo2ModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kLfo2));
    addAndMakeVisible(*lfo2ModuleHeader);

    // Standalone Widgets
    lfo2InitButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kLfo2Init, inTheme);
    lfo2InitButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kLfo2Init,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*lfo2InitButton);

    lfo2CopyButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kLfo2Copy, inTheme);
    lfo2CopyButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kLfo2Copy,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*lfo2CopyButton);

    lfo2PasteButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kLfo2Paste, inTheme);
    lfo2PasteButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kLfo2Paste,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*lfo2PasteButton);

    // Speed
    lfo2SpeedLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2Speed));
    addAndMakeVisible(*lfo2SpeedLabel);

    lfo2SpeedSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kLfo2Speed, inTheme);
    lfo2SpeedAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2Speed,
        *lfo2SpeedSlider);
    addAndMakeVisible(*lfo2SpeedSlider);

    horizontalSeparator1 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator1);

    // SpeedModByKeyboard
    lfo2SpeedModByKeyboardLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2SpeedModByKeyboard));
    addAndMakeVisible(*lfo2SpeedModByKeyboardLabel);

    lfo2SpeedModByKeyboardSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kLfo2SpeedModByKeyboard, inTheme);
    lfo2SpeedModByKeyboardAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2SpeedModByKeyboard,
        *lfo2SpeedModByKeyboardSlider);
    addAndMakeVisible(*lfo2SpeedModByKeyboardSlider);

    horizontalSeparator2 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator2);

    // RetriggerPoint
    lfo2RetriggerPointLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2RetriggerPoint));
    addAndMakeVisible(*lfo2RetriggerPointLabel);

    lfo2RetriggerPointSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kLfo2RetriggerPoint, inTheme);
    lfo2RetriggerPointAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2RetriggerPoint,
        *lfo2RetriggerPointSlider);
    addAndMakeVisible(*lfo2RetriggerPointSlider);

    horizontalSeparator3 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator3);

    // Amplitude
    lfo2AmplitudeLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2Amplitude));
    addAndMakeVisible(*lfo2AmplitudeLabel);

    lfo2AmplitudeSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kLfo2Amplitude, inTheme);
    lfo2AmplitudeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2Amplitude,
        *lfo2AmplitudeSlider);
    addAndMakeVisible(*lfo2AmplitudeSlider);

    horizontalSeparator4 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator4);

    // Amplitude Mod by Ramp 2
    lfo2AmplitudeModByRamp2Label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2AmplitudeModByRamp2));
    addAndMakeVisible(*lfo2AmplitudeModByRamp2Label);

    lfo2AmplitudeModByRamp2Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kLfo2AmplitudeModByRamp2, inTheme);
    lfo2AmplitudeModByRamp2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2AmplitudeModByRamp2,
        *lfo2AmplitudeModByRamp2Slider);
    addAndMakeVisible(*lfo2AmplitudeModByRamp2Slider);

    horizontalSeparator5 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator5);

    // Waveform
    lfo2WaveformLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2Waveform));
    addAndMakeVisible(*lfo2WaveformLabel);

    lfo2WaveformComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kLfo2Waveform, inTheme);
    lfo2WaveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2Waveform,
        *lfo2WaveformComboBox);
    addAndMakeVisible(*lfo2WaveformComboBox);

    horizontalSeparator6 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator6);

    // Trigger Mode
    lfo2TriggerModeLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2TriggerMode));
    addAndMakeVisible(*lfo2TriggerModeLabel);

    lfo2TriggerModeComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kLfo2TriggerMode, inTheme);
    lfo2TriggerModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2TriggerMode,
        *lfo2TriggerModeComboBox);
    addAndMakeVisible(*lfo2TriggerModeComboBox);

    horizontalSeparator7 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator7);

    // Lag
    lfo2LagLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2Lag));
    addAndMakeVisible(*lfo2LagLabel);

    lfo2LagComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kLfo2Lag, inTheme);
    lfo2LagAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2Lag,
        *lfo2LagComboBox);
    addAndMakeVisible(*lfo2LagComboBox);

    horizontalSeparator8 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator8);

    // Sample Input
    lfo2SampleInputLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2SampleInput));
    addAndMakeVisible(*lfo2SampleInputLabel);

    lfo2SampleInputComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kLfo2SampleInput, inTheme);
    lfo2SampleInputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2SampleInput,
        *lfo2SampleInputComboBox);
    addAndMakeVisible(*lfo2SampleInputComboBox);

    horizontalSeparator9 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator9);

    horizontalSeparator10 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator10);

    setSize(getWidth(), getHeight());
}

Lfo2Panel::~Lfo2Panel() = default;

void Lfo2Panel::paint(juce::Graphics& g)
{
        g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void Lfo2Panel::resized()
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

    if (auto* header = lfo2ModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    if (auto* button = lfo2PasteButton.get())
        button->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);

    if (auto* button = lfo2CopyButton.get())
        button->setBounds(panelWidth - buttonWidth * 2, y, buttonWidth, buttonHeight);

    if (auto* button = lfo2InitButton.get())
        button->setBounds(panelWidth - buttonWidth * 3, y, buttonWidth, buttonHeight);

    y += moduleHeaderHeight;

    if (auto* label = lfo2SpeedLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = lfo2SpeedSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo2SpeedModByKeyboardLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = lfo2SpeedModByKeyboardSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo2RetriggerPointLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = lfo2RetriggerPointSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo2AmplitudeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = lfo2AmplitudeSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo2AmplitudeModByRamp2Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = lfo2AmplitudeModByRamp2Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo2WaveformLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = lfo2WaveformComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo2TriggerModeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = lfo2TriggerModeComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo2LagLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = lfo2LagComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator8.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo2SampleInputLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = lfo2SampleInputComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator9.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    y += labelHeight;

    if (auto* separator = horizontalSeparator10.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void Lfo2Panel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = lfo2ModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* button = lfo2InitButton.get())
        button->setTheme(inTheme);

    if (auto* button = lfo2CopyButton.get())
        button->setTheme(inTheme);

    if (auto* button = lfo2PasteButton.get())
        button->setTheme(inTheme);

    if (auto* label = lfo2SpeedLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = lfo2SpeedSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo2SpeedModByKeyboardLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = lfo2SpeedModByKeyboardSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo2RetriggerPointLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = lfo2RetriggerPointSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo2AmplitudeLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = lfo2AmplitudeSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo2AmplitudeModByRamp2Label.get())
        label->setTheme(inTheme);

    if (auto* slider = lfo2AmplitudeModByRamp2Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo2WaveformLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = lfo2WaveformComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* comboBox = lfo2WaveformComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo2TriggerModeLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = lfo2TriggerModeComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator7.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo2LagLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = lfo2LagComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator8.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo2SampleInputLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = lfo2SampleInputComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator9.get())
        separator->setTheme(inTheme);

    if (auto* separator = horizontalSeparator10.get())
        separator->setTheme(inTheme);

    repaint();
}
