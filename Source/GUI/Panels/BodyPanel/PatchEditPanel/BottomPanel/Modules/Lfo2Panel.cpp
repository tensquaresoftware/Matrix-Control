#include "Lfo2Panel.h"

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

Lfo2Panel::Lfo2Panel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    lfo2ModuleName = std::make_unique<tss::ModuleName>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kLfo2));
    addAndMakeVisible(*lfo2ModuleName);

    // Standalone Widgets
    lfo2InitButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kLfo2Init, inTheme);
    lfo2InitButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kLfo2Init,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*lfo2InitButton);

    lfo2CopyButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kLfo2Copy, inTheme);
    lfo2CopyButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kLfo2Copy,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*lfo2CopyButton);

    lfo2PasteButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kLfo2Paste, inTheme);
    lfo2PasteButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kLfo2Paste,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*lfo2PasteButton);

    // Speed
    lfo2SpeedLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2Speed));
    addAndMakeVisible(*lfo2SpeedLabel);

    lfo2SpeedSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kLfo2Speed, inTheme);
    lfo2SpeedAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2Speed,
        *lfo2SpeedSlider);
    addAndMakeVisible(*lfo2SpeedSlider);

    parameterSeparator1 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator1);

    // SpeedModByKeyboard
    lfo2SpeedModByKeyboardLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2SpeedModByKeyboard));
    addAndMakeVisible(*lfo2SpeedModByKeyboardLabel);

    lfo2SpeedModByKeyboardSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kLfo2SpeedModByKeyboard, inTheme);
    lfo2SpeedModByKeyboardAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2SpeedModByKeyboard,
        *lfo2SpeedModByKeyboardSlider);
    addAndMakeVisible(*lfo2SpeedModByKeyboardSlider);

    parameterSeparator2 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator2);

    // RetriggerPoint
    lfo2RetriggerPointLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2RetriggerPoint));
    addAndMakeVisible(*lfo2RetriggerPointLabel);

    lfo2RetriggerPointSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kLfo2RetriggerPoint, inTheme);
    lfo2RetriggerPointAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2RetriggerPoint,
        *lfo2RetriggerPointSlider);
    addAndMakeVisible(*lfo2RetriggerPointSlider);

    parameterSeparator3 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator3);

    // Amplitude
    lfo2AmplitudeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2Amplitude));
    addAndMakeVisible(*lfo2AmplitudeLabel);

    lfo2AmplitudeSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kLfo2Amplitude, inTheme);
    lfo2AmplitudeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2Amplitude,
        *lfo2AmplitudeSlider);
    addAndMakeVisible(*lfo2AmplitudeSlider);

    parameterSeparator4 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator4);

    // Amplitude Mod by Ramp 2
    lfo2AmplitudeModByRamp2Label = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2AmplitudeModByRamp2));
    addAndMakeVisible(*lfo2AmplitudeModByRamp2Label);

    lfo2AmplitudeModByRamp2Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kLfo2AmplitudeModByRamp2, inTheme);
    lfo2AmplitudeModByRamp2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2AmplitudeModByRamp2,
        *lfo2AmplitudeModByRamp2Slider);
    addAndMakeVisible(*lfo2AmplitudeModByRamp2Slider);

    parameterSeparator5 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator5);

    // Waveform
    lfo2WaveformLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2Waveform));
    addAndMakeVisible(*lfo2WaveformLabel);

    lfo2WaveformComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kLfo2Waveform, inTheme);
    lfo2WaveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2Waveform,
        *lfo2WaveformComboBox);
    addAndMakeVisible(*lfo2WaveformComboBox);

    parameterSeparator6 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator6);

    // Trigger Mode
    lfo2TriggerModeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2TriggerMode));
    addAndMakeVisible(*lfo2TriggerModeLabel);

    lfo2TriggerModeComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kLfo2TriggerMode, inTheme);
    lfo2TriggerModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2TriggerMode,
        *lfo2TriggerModeComboBox);
    addAndMakeVisible(*lfo2TriggerModeComboBox);

    parameterSeparator7 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator7);

    // Lag
    lfo2LagLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2Lag));
    addAndMakeVisible(*lfo2LagLabel);

    lfo2LagComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kLfo2Lag, inTheme);
    lfo2LagAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2Lag,
        *lfo2LagComboBox);
    addAndMakeVisible(*lfo2LagComboBox);

    parameterSeparator8 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator8);

    // Sample Input
    lfo2SampleInputLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kLfo2SampleInput));
    addAndMakeVisible(*lfo2SampleInputLabel);

    lfo2SampleInputComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kLfo2SampleInput, inTheme);
    lfo2SampleInputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kLfo2SampleInput,
        *lfo2SampleInputComboBox);
    addAndMakeVisible(*lfo2SampleInputComboBox);

    parameterSeparator9 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator9);

    parameterSeparator10 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator10);

    setSize(getWidth(), getHeight());
}

Lfo2Panel::~Lfo2Panel() = default;

void Lfo2Panel::paint(juce::Graphics& g)
{
    if (theme == nullptr)
    {
        return;
    }

    g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void Lfo2Panel::resized()
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

    if (lfo2ModuleName != nullptr)
    {
        lfo2ModuleName->setBounds(0, y, moduleNameWidth, moduleNameHeight);
    }

    if (lfo2PasteButton != nullptr)
    {
        lfo2PasteButton->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);
    }

    if (lfo2CopyButton != nullptr)
    {
        lfo2CopyButton->setBounds(panelWidth - buttonWidth * 2, y, buttonWidth, buttonHeight);
    }

    if (lfo2InitButton != nullptr)
    {
        lfo2InitButton->setBounds(panelWidth - buttonWidth * 3, y, buttonWidth, buttonHeight);
    }

    y += moduleNameHeight;

    if (lfo2SpeedLabel != nullptr)
    {
        lfo2SpeedLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo2SpeedSlider != nullptr)
    {
        lfo2SpeedSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (lfo2SpeedModByKeyboardLabel != nullptr)
    {
        lfo2SpeedModByKeyboardLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo2SpeedModByKeyboardSlider != nullptr)
    {
        lfo2SpeedModByKeyboardSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (lfo2RetriggerPointLabel != nullptr)
    {
        lfo2RetriggerPointLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo2RetriggerPointSlider != nullptr)
    {
        lfo2RetriggerPointSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (lfo2AmplitudeLabel != nullptr)
    {
        lfo2AmplitudeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo2AmplitudeSlider != nullptr)
    {
        lfo2AmplitudeSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (lfo2AmplitudeModByRamp2Label != nullptr)
    {
        lfo2AmplitudeModByRamp2Label->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo2AmplitudeModByRamp2Slider != nullptr)
    {
        lfo2AmplitudeModByRamp2Slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (lfo2WaveformLabel != nullptr)
    {
        lfo2WaveformLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo2WaveformComboBox != nullptr)
    {
        lfo2WaveformComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (lfo2TriggerModeLabel != nullptr)
    {
        lfo2TriggerModeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo2TriggerModeComboBox != nullptr)
    {
        lfo2TriggerModeComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (lfo2LagLabel != nullptr)
    {
        lfo2LagLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo2LagComboBox != nullptr)
    {
        lfo2LagComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (lfo2SampleInputLabel != nullptr)
    {
        lfo2SampleInputLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (lfo2SampleInputComboBox != nullptr)
    {
        lfo2SampleInputComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
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

void Lfo2Panel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (lfo2ModuleName != nullptr)
    {
        lfo2ModuleName->setTheme(inTheme);
    }

    if (lfo2InitButton != nullptr)
    {
        lfo2InitButton->setTheme(inTheme);
    }

    if (lfo2CopyButton != nullptr)
    {
        lfo2CopyButton->setTheme(inTheme);
    }

    if (lfo2PasteButton != nullptr)
    {
        lfo2PasteButton->setTheme(inTheme);
    }

    if (lfo2SpeedLabel != nullptr)
    {
        lfo2SpeedLabel->setTheme(inTheme);
    }

    if (lfo2SpeedSlider != nullptr)
    {
        lfo2SpeedSlider->setTheme(inTheme);
    }

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setTheme(inTheme);
    }

    if (lfo2SpeedModByKeyboardLabel != nullptr)
    {
        lfo2SpeedModByKeyboardLabel->setTheme(inTheme);
    }

    if (lfo2SpeedModByKeyboardSlider != nullptr)
    {
        lfo2SpeedModByKeyboardSlider->setTheme(inTheme);
    }

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setTheme(inTheme);
    }

    if (lfo2RetriggerPointLabel != nullptr)
    {
        lfo2RetriggerPointLabel->setTheme(inTheme);
    }

    if (lfo2RetriggerPointSlider != nullptr)
    {
        lfo2RetriggerPointSlider->setTheme(inTheme);
    }

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setTheme(inTheme);
    }

    if (lfo2AmplitudeLabel != nullptr)
    {
        lfo2AmplitudeLabel->setTheme(inTheme);
    }

    if (lfo2AmplitudeSlider != nullptr)
    {
        lfo2AmplitudeSlider->setTheme(inTheme);
    }

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setTheme(inTheme);
    }

    if (lfo2AmplitudeModByRamp2Label != nullptr)
    {
        lfo2AmplitudeModByRamp2Label->setTheme(inTheme);
    }

    if (lfo2AmplitudeModByRamp2Slider != nullptr)
    {
        lfo2AmplitudeModByRamp2Slider->setTheme(inTheme);
    }

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setTheme(inTheme);
    }

    if (lfo2WaveformLabel != nullptr)
    {
        lfo2WaveformLabel->setTheme(inTheme);
    }

    if (lfo2WaveformComboBox != nullptr)
    {
        lfo2WaveformComboBox->setTheme(inTheme);
    }

    if (lfo2WaveformComboBox != nullptr)
    {
        lfo2WaveformComboBox->setTheme(inTheme);
    }

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setTheme(inTheme);
    }

    if (lfo2TriggerModeLabel != nullptr)
    {
        lfo2TriggerModeLabel->setTheme(inTheme);
    }

    if (lfo2TriggerModeComboBox != nullptr)
    {
        lfo2TriggerModeComboBox->setTheme(inTheme);
    }

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setTheme(inTheme);
    }

    if (lfo2LagLabel != nullptr)
    {
        lfo2LagLabel->setTheme(inTheme);
    }

    if (lfo2LagComboBox != nullptr)
    {
        lfo2LagComboBox->setTheme(inTheme);
    }

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setTheme(inTheme);
    }

    if (lfo2SampleInputLabel != nullptr)
    {
        lfo2SampleInputLabel->setTheme(inTheme);
    }

    if (lfo2SampleInputComboBox != nullptr)
    {
        lfo2SampleInputComboBox->setTheme(inTheme);
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
