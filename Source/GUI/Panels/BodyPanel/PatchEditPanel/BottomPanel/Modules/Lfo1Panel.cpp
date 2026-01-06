#include "Lfo1Panel.h"

#include "../../../../../Themes/Theme.h"
#include "../../../../../Widgets/ModuleName.h"
#include "../../../../../Widgets/ParameterLabel.h"
#include "../../../../../Widgets/Slider.h"
#include "../../../../../Widgets/ComboBox.h"
#include "../../../../../Widgets/Button.h"
#include "../../../../../Widgets/ParameterSeparator.h"
#include "../../../../../../Shared/SynthDescriptors.h"
#include "../../../../../../GUI/Factories/WidgetFactory.h"

using tss::Theme;

Lfo1Panel::Lfo1Panel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    lfo1ModuleName = std::make_unique<tss::ModuleName>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kLfo1));
    addAndMakeVisible(*lfo1ModuleName);

    // Standalone Widgets
    lfo1InitButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kLfo1Init, inTheme);
    lfo1InitButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kLfo1Init,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*lfo1InitButton);

    lfo1CopyButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kLfo1Copy, inTheme);
    lfo1CopyButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kLfo1Copy,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*lfo1CopyButton);

    lfo1PasteButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kLfo1Paste, inTheme);
    lfo1PasteButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kLfo1Paste,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*lfo1PasteButton);

    // Speed
    lfo1SpeedLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo1Speed));
    addAndMakeVisible(*lfo1SpeedLabel);

    lfo1SpeedSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kLfo1Speed, inTheme);
    lfo1SpeedAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo1Speed,
        *lfo1SpeedSlider);
    addAndMakeVisible(*lfo1SpeedSlider);

    parameterSeparator1 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator1);

    // SpeedModByPressure
    lfo1SpeedModByPressureLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo1SpeedModByPressure));
    addAndMakeVisible(*lfo1SpeedModByPressureLabel);

    lfo1SpeedModByPressureSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kLfo1SpeedModByPressure, inTheme);
    lfo1SpeedModByPressureAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo1SpeedModByPressure,
        *lfo1SpeedModByPressureSlider);
    addAndMakeVisible(*lfo1SpeedModByPressureSlider);

    parameterSeparator2 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator2);

    // RetriggerPoint
    lfo1RetriggerPointLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo1RetriggerPoint));
    addAndMakeVisible(*lfo1RetriggerPointLabel);

    lfo1RetriggerPointSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kLfo1RetriggerPoint, inTheme);
    lfo1RetriggerPointAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo1RetriggerPoint,
        *lfo1RetriggerPointSlider);
    addAndMakeVisible(*lfo1RetriggerPointSlider);

    parameterSeparator3 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator3);

    // Amplitude
    lfo1AmplitudeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo1Amplitude));
    addAndMakeVisible(*lfo1AmplitudeLabel);

    lfo1AmplitudeSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kLfo1Amplitude, inTheme);
    lfo1AmplitudeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo1Amplitude,
        *lfo1AmplitudeSlider);
    addAndMakeVisible(*lfo1AmplitudeSlider);

    parameterSeparator4 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator4);

    // Amplitude Mod by Ramp 1
    lfo1AmplitudeModByRamp1Label = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo1AmplitudeModByRamp1));
    addAndMakeVisible(*lfo1AmplitudeModByRamp1Label);

    lfo1AmplitudeModByRamp1Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kLfo1AmplitudeModByRamp1, inTheme);
    lfo1AmplitudeModByRamp1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo1AmplitudeModByRamp1,
        *lfo1AmplitudeModByRamp1Slider);
    addAndMakeVisible(*lfo1AmplitudeModByRamp1Slider);

    parameterSeparator5 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator5);

    // Waveform
    lfo1WaveformLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo1Waveform));
    addAndMakeVisible(*lfo1WaveformLabel);

    lfo1WaveformComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kLfo1Waveform, inTheme);
    lfo1WaveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo1Waveform,
        *lfo1WaveformComboBox);
    addAndMakeVisible(*lfo1WaveformComboBox);

    parameterSeparator6 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator6);

    // Trigger Mode
    lfo1TriggerModeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo1TriggerMode));
    addAndMakeVisible(*lfo1TriggerModeLabel);

    lfo1TriggerModeComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kLfo1TriggerMode, inTheme);
    lfo1TriggerModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo1TriggerMode,
        *lfo1TriggerModeComboBox);
    addAndMakeVisible(*lfo1TriggerModeComboBox);

    parameterSeparator7 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator7);

    // Lag
    lfo1LagLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo1Lag));
    addAndMakeVisible(*lfo1LagLabel);

    lfo1LagComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kLfo1Lag, inTheme);
    lfo1LagAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo1Lag,
        *lfo1LagComboBox);
    addAndMakeVisible(*lfo1LagComboBox);

    parameterSeparator8 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator8);

    // Sample Input
    lfo1SampleInputLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo1SampleInput));
    addAndMakeVisible(*lfo1SampleInputLabel);

    lfo1SampleInputComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kLfo1SampleInput, inTheme);
    lfo1SampleInputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo1SampleInput,
        *lfo1SampleInputComboBox);
    addAndMakeVisible(*lfo1SampleInputComboBox);

    parameterSeparator9 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator9);

    parameterSeparator10 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator10);

    setSize(getWidth(), getHeight());
}

Lfo1Panel::~Lfo1Panel() = default;

void Lfo1Panel::paint(juce::Graphics& g)
{
    if (theme == nullptr)
    {
        return;
    }

    g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void Lfo1Panel::resized()
{
    const auto moduleNameHeight = tss::ModuleName::getHeight();
    const auto moduleNameWidth = tss::ModuleName::getNormalWidth();
    const auto buttonWidth = tss::Button::getDefaultWidth();
    const auto buttonHeight = tss::Button::getHeight();
    const auto labelWidth = tss::ParameterLabel::getWidth();
    const auto labelHeight = tss::ParameterLabel::getHeight();
    const auto sliderWidth = tss::Slider::getWidth();
    const auto sliderHeight = tss::Slider::getHeight();
    const auto comboBoxWidth = tss::ComboBox::getNormalWidth();
    const auto comboBoxHeight = tss::ComboBox::getHeight();
    const auto separatorWidth = tss::ParameterSeparator::getWidth();
    const auto separatorHeight = tss::ParameterSeparator::getHeight();
    const auto panelWidth = getWidth();

    int y = 0;

    if (lfo1ModuleName != nullptr)
    {
        lfo1ModuleName->setBounds(0, y, moduleNameWidth, moduleNameHeight);
    }

    if (lfo1PasteButton != nullptr)
    {
        lfo1PasteButton->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);
    }

    if (lfo1CopyButton != nullptr)
    {
        lfo1CopyButton->setBounds(panelWidth - buttonWidth * 2, y, buttonWidth, buttonHeight);
    }

    if (lfo1InitButton != nullptr)
    {
        lfo1InitButton->setBounds(panelWidth - buttonWidth * 3, y, buttonWidth, buttonHeight);
    }

    y += moduleNameHeight;

    if (lfo1SpeedLabel != nullptr)
    {
        lfo1SpeedLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo1SpeedSlider != nullptr)
    {
        lfo1SpeedSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (lfo1SpeedModByPressureLabel != nullptr)
    {
        lfo1SpeedModByPressureLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo1SpeedModByPressureSlider != nullptr)
    {
        lfo1SpeedModByPressureSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (lfo1RetriggerPointLabel != nullptr)
    {
        lfo1RetriggerPointLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo1RetriggerPointSlider != nullptr)
    {
        lfo1RetriggerPointSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (lfo1AmplitudeLabel != nullptr)
    {
        lfo1AmplitudeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo1AmplitudeSlider != nullptr)
    {
        lfo1AmplitudeSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (lfo1AmplitudeModByRamp1Label != nullptr)
    {
        lfo1AmplitudeModByRamp1Label->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo1AmplitudeModByRamp1Slider != nullptr)
    {
        lfo1AmplitudeModByRamp1Slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (lfo1WaveformLabel != nullptr)
    {
        lfo1WaveformLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo1WaveformComboBox != nullptr)
    {
        lfo1WaveformComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (lfo1TriggerModeLabel != nullptr)
    {
        lfo1TriggerModeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo1TriggerModeComboBox != nullptr)
    {
        lfo1TriggerModeComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (lfo1LagLabel != nullptr)
    {
        lfo1LagLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo1LagComboBox != nullptr)
    {
        lfo1LagComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (lfo1SampleInputLabel != nullptr)
    {
        lfo1SampleInputLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo1SampleInputComboBox != nullptr)
    {
        lfo1SampleInputComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator9 != nullptr)
    {
        parameterSeparator9->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    y += labelHeight;

    if (parameterSeparator10 != nullptr)
    {
        parameterSeparator10->setBounds(0, y, separatorWidth, separatorHeight);
    }
}

void Lfo1Panel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (lfo1ModuleName != nullptr)
    {
        lfo1ModuleName->setTheme(inTheme);
    }

    if (lfo1InitButton != nullptr)
    {
        lfo1InitButton->setTheme(inTheme);
    }

    if (lfo1CopyButton != nullptr)
    {
        lfo1CopyButton->setTheme(inTheme);
    }

    if (lfo1PasteButton != nullptr)
    {
        lfo1PasteButton->setTheme(inTheme);
    }

    if (lfo1SpeedLabel != nullptr)
    {
        lfo1SpeedLabel->setTheme(inTheme);
    }

    if (lfo1SpeedSlider != nullptr)
    {
        lfo1SpeedSlider->setTheme(inTheme);
    }

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setTheme(inTheme);
    }

    if (lfo1SpeedModByPressureLabel != nullptr)
    {
        lfo1SpeedModByPressureLabel->setTheme(inTheme);
    }

    if (lfo1SpeedModByPressureSlider != nullptr)
    {
        lfo1SpeedModByPressureSlider->setTheme(inTheme);
    }

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setTheme(inTheme);
    }

    if (lfo1RetriggerPointLabel != nullptr)
    {
        lfo1RetriggerPointLabel->setTheme(inTheme);
    }

    if (lfo1RetriggerPointSlider != nullptr)
    {
        lfo1RetriggerPointSlider->setTheme(inTheme);
    }

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setTheme(inTheme);
    }

    if (lfo1AmplitudeLabel != nullptr)
    {
        lfo1AmplitudeLabel->setTheme(inTheme);
    }

    if (lfo1AmplitudeSlider != nullptr)
    {
        lfo1AmplitudeSlider->setTheme(inTheme);
    }

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setTheme(inTheme);
    }

    if (lfo1AmplitudeModByRamp1Label != nullptr)
    {
        lfo1AmplitudeModByRamp1Label->setTheme(inTheme);
    }

    if (lfo1AmplitudeModByRamp1Slider != nullptr)
    {
        lfo1AmplitudeModByRamp1Slider->setTheme(inTheme);
    }

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setTheme(inTheme);
    }

    if (lfo1WaveformLabel != nullptr)
    {
        lfo1WaveformLabel->setTheme(inTheme);
    }

    if (lfo1WaveformComboBox != nullptr)
    {
        lfo1WaveformComboBox->setTheme(inTheme);
    }

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setTheme(inTheme);
    }

    if (lfo1TriggerModeLabel != nullptr)
    {
        lfo1TriggerModeLabel->setTheme(inTheme);
    }

    if (lfo1TriggerModeComboBox != nullptr)
    {
        lfo1TriggerModeComboBox->setTheme(inTheme);
    }

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setTheme(inTheme);
    }

    if (lfo1LagLabel != nullptr)
    {
        lfo1LagLabel->setTheme(inTheme);
    }

    if (lfo1LagComboBox != nullptr)
    {
        lfo1LagComboBox->setTheme(inTheme);
    }

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setTheme(inTheme);
    }

    if (lfo1SampleInputLabel != nullptr)
    {
        lfo1SampleInputLabel->setTheme(inTheme);
    }

    if (lfo1SampleInputComboBox != nullptr)
    {
        lfo1SampleInputComboBox->setTheme(inTheme);
    }

    if (parameterSeparator9 != nullptr)
    {
        parameterSeparator9->setTheme(inTheme);
    }

    if (parameterSeparator10 != nullptr)
    {
        parameterSeparator10->setTheme(inTheme);
    }

    repaint();
}
