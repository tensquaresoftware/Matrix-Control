#include "Env3Panel.h"

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

Env3Panel::Env3Panel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    env3ModuleName = std::make_unique<tss::ModuleName>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kEnvelope3));
    addAndMakeVisible(*env3ModuleName);

    // Standalone Widgets
    env3InitButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kEnv3Init, inTheme);
    env3InitButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kEnv3Init,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env3InitButton);

    env3CopyButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kEnv3Copy, inTheme);
    env3CopyButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kEnv3Copy,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env3CopyButton);

    env3PasteButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kEnv3Paste, inTheme);
    env3PasteButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kEnv3Paste,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env3PasteButton);

    // Delay
    env3DelayLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3Delay));
    addAndMakeVisible(*env3DelayLabel);

    env3DelaySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv3Delay, inTheme);
    env3DelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3Delay,
        *env3DelaySlider);
    addAndMakeVisible(*env3DelaySlider);

    parameterSeparator1 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator1);

    // Attack
    env3AttackLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3Attack));
    addAndMakeVisible(*env3AttackLabel);

    env3AttackSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv3Attack, inTheme);
    env3AttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3Attack,
        *env3AttackSlider);
    addAndMakeVisible(*env3AttackSlider);

    parameterSeparator2 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator2);

    // Decay
    env3DecayLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3Decay));
    addAndMakeVisible(*env3DecayLabel);

    env3DecaySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv3Decay, inTheme);
    env3DecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3Decay,
        *env3DecaySlider);
    addAndMakeVisible(*env3DecaySlider);

    parameterSeparator3 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator3);

    // Sustain
    env3SustainLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3Sustain));
    addAndMakeVisible(*env3SustainLabel);

    env3SustainSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv3Sustain, inTheme);
    env3SustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3Sustain,
        *env3SustainSlider);
    addAndMakeVisible(*env3SustainSlider);

    parameterSeparator4 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator4);

    // Release
    env3ReleaseLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3Release));
    addAndMakeVisible(*env3ReleaseLabel);

    env3ReleaseSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv3Release, inTheme);
    env3ReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3Release,
        *env3ReleaseSlider);
    addAndMakeVisible(*env3ReleaseSlider);

    parameterSeparator5 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator5);

    // Amplitude
    env3AmplitudeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3Amplitude));
    addAndMakeVisible(*env3AmplitudeLabel);

    env3AmplitudeSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv3Amplitude, inTheme);
    env3AmplitudeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3Amplitude,
        *env3AmplitudeSlider);
    addAndMakeVisible(*env3AmplitudeSlider);

    parameterSeparator6 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator6);

    // Amplitude Mod by Velocity
    env3AmplitudeModByVelocityLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3AmplitudeModByVelocity));
    addAndMakeVisible(*env3AmplitudeModByVelocityLabel);

    env3AmplitudeModByVelocitySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv3AmplitudeModByVelocity, inTheme);
    env3AmplitudeModByVelocityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3AmplitudeModByVelocity,
        *env3AmplitudeModByVelocitySlider);
    addAndMakeVisible(*env3AmplitudeModByVelocitySlider);

    parameterSeparator7 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator7);

    // Trigger Mode
    env3TriggerModeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3TriggerMode));
    addAndMakeVisible(*env3TriggerModeLabel);

    env3TriggerModeComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kEnv3TriggerMode, inTheme);
    env3TriggerModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3TriggerMode,
        *env3TriggerModeComboBox);
    addAndMakeVisible(*env3TriggerModeComboBox);

    parameterSeparator8 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator8);

    // Envelope Mode
    env3EnvelopeModeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3EnvelopeMode));
    addAndMakeVisible(*env3EnvelopeModeLabel);

    env3EnvelopeModeComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kEnv3EnvelopeMode, inTheme);
    env3EnvelopeModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3EnvelopeMode,
        *env3EnvelopeModeComboBox);
    addAndMakeVisible(*env3EnvelopeModeComboBox);

    parameterSeparator9 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator9);

    // LFO 1 Trigger
    env3Lfo1TriggerLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3Lfo1Trigger));
    addAndMakeVisible(*env3Lfo1TriggerLabel);

    env3Lfo1TriggerComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kEnv3Lfo1Trigger, inTheme);
    env3Lfo1TriggerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3Lfo1Trigger,
        *env3Lfo1TriggerComboBox);
    addAndMakeVisible(*env3Lfo1TriggerComboBox);

    parameterSeparator10 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator10);

    setSize(getWidth(), getHeight());
}

Env3Panel::~Env3Panel() = default;

void Env3Panel::paint(juce::Graphics& g)
{
    if (theme == nullptr)
    {
        return;
    }

    g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void Env3Panel::resized()
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

    if (env3ModuleName != nullptr)
    {
        env3ModuleName->setBounds(0, y, moduleNameWidth, moduleNameHeight);
    }

    if (env3PasteButton != nullptr)
    {
        env3PasteButton->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);
    }

    if (env3CopyButton != nullptr)
    {
        env3CopyButton->setBounds(panelWidth - buttonWidth * 2, y, buttonWidth, buttonHeight);
    }

    if (env3InitButton != nullptr)
    {
        env3InitButton->setBounds(panelWidth - buttonWidth * 3, y, buttonWidth, buttonHeight);
    }

    y += moduleNameHeight;

    if (env3DelayLabel != nullptr)
    {
        env3DelayLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env3DelaySlider != nullptr)
    {
        env3DelaySlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env3AttackLabel != nullptr)
    {
        env3AttackLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env3AttackSlider != nullptr)
    {
        env3AttackSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env3DecayLabel != nullptr)
    {
        env3DecayLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env3DecaySlider != nullptr)
    {
        env3DecaySlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env3SustainLabel != nullptr)
    {
        env3SustainLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env3SustainSlider != nullptr)
    {
        env3SustainSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env3ReleaseLabel != nullptr)
    {
        env3ReleaseLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env3ReleaseSlider != nullptr)
    {
        env3ReleaseSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env3AmplitudeLabel != nullptr)
    {
        env3AmplitudeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env3AmplitudeSlider != nullptr)
    {
        env3AmplitudeSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env3AmplitudeModByVelocityLabel != nullptr)
    {
        env3AmplitudeModByVelocityLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env3AmplitudeModByVelocitySlider != nullptr)
    {
        env3AmplitudeModByVelocitySlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env3TriggerModeLabel != nullptr)
    {
        env3TriggerModeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env3TriggerModeComboBox != nullptr)
    {
        env3TriggerModeComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env3EnvelopeModeLabel != nullptr)
    {
        env3EnvelopeModeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env3EnvelopeModeComboBox != nullptr)
    {
        env3EnvelopeModeComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator9 != nullptr)
    {
        parameterSeparator9->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env3Lfo1TriggerLabel != nullptr)
    {
        env3Lfo1TriggerLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env3Lfo1TriggerComboBox != nullptr)
    {
        env3Lfo1TriggerComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator10 != nullptr)
    {
        parameterSeparator10->setBounds(0, y, separatorWidth, separatorHeight);
    }
}

void Env3Panel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (env3ModuleName != nullptr)
    {
        env3ModuleName->setTheme(inTheme);
    }

    if (env3InitButton != nullptr)
    {
        env3InitButton->setTheme(inTheme);
    }

    if (env3CopyButton != nullptr)
    {
        env3CopyButton->setTheme(inTheme);
    }

    if (env3PasteButton != nullptr)
    {
        env3PasteButton->setTheme(inTheme);
    }

    if (env3DelayLabel != nullptr)
    {
        env3DelayLabel->setTheme(inTheme);
    }

    if (env3DelaySlider != nullptr)
    {
        env3DelaySlider->setTheme(inTheme);
    }

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setTheme(inTheme);
    }

    if (env3AttackLabel != nullptr)
    {
        env3AttackLabel->setTheme(inTheme);
    }

    if (env3AttackSlider != nullptr)
    {
        env3AttackSlider->setTheme(inTheme);
    }

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setTheme(inTheme);
    }

    if (env3DecayLabel != nullptr)
    {
        env3DecayLabel->setTheme(inTheme);
    }

    if (env3DecaySlider != nullptr)
    {
        env3DecaySlider->setTheme(inTheme);
    }

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setTheme(inTheme);
    }

    if (env3SustainLabel != nullptr)
    {
        env3SustainLabel->setTheme(inTheme);
    }

    if (env3SustainSlider != nullptr)
    {
        env3SustainSlider->setTheme(inTheme);
    }

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setTheme(inTheme);
    }

    if (env3ReleaseLabel != nullptr)
    {
        env3ReleaseLabel->setTheme(inTheme);
    }

    if (env3ReleaseSlider != nullptr)
    {
        env3ReleaseSlider->setTheme(inTheme);
    }

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setTheme(inTheme);
    }

    if (env3AmplitudeLabel != nullptr)
    {
        env3AmplitudeLabel->setTheme(inTheme);
    }

    if (env3AmplitudeSlider != nullptr)
    {
        env3AmplitudeSlider->setTheme(inTheme);
    }

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setTheme(inTheme);
    }

    if (env3AmplitudeModByVelocityLabel != nullptr)
    {
        env3AmplitudeModByVelocityLabel->setTheme(inTheme);
    }

    if (env3AmplitudeModByVelocitySlider != nullptr)
    {
        env3AmplitudeModByVelocitySlider->setTheme(inTheme);
    }

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setTheme(inTheme);
    }

    if (env3TriggerModeLabel != nullptr)
    {
        env3TriggerModeLabel->setTheme(inTheme);
    }

    if (env3TriggerModeComboBox != nullptr)
    {
        env3TriggerModeComboBox->setTheme(inTheme);
    }

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setTheme(inTheme);
    }

    if (env3EnvelopeModeLabel != nullptr)
    {
        env3EnvelopeModeLabel->setTheme(inTheme);
    }

    if (env3EnvelopeModeComboBox != nullptr)
    {
        env3EnvelopeModeComboBox->setTheme(inTheme);
    }

    if (parameterSeparator9 != nullptr)
    {
        parameterSeparator9->setTheme(inTheme);
    }

    if (env3Lfo1TriggerLabel != nullptr)
    {
        env3Lfo1TriggerLabel->setTheme(inTheme);
    }

    if (env3Lfo1TriggerComboBox != nullptr)
    {
        env3Lfo1TriggerComboBox->setTheme(inTheme);
    }

    if (parameterSeparator10 != nullptr)
    {
        parameterSeparator10->setTheme(inTheme);
    }

    repaint();
}
