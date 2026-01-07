#include "Env1Panel.h"

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

Env1Panel::Env1Panel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    env1ModuleName = std::make_unique<tss::ModuleName>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kEnvelope1));
    addAndMakeVisible(*env1ModuleName);

    // Standalone Widgets
    env1InitButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kEnv1Init, inTheme);
    env1InitButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kEnv1Init,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env1InitButton);

    env1CopyButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kEnv1Copy, inTheme);
    env1CopyButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kEnv1Copy,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env1CopyButton);

    env1PasteButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kEnv1Paste, inTheme);
    env1PasteButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kEnv1Paste,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env1PasteButton);

    // Delay
    env1DelayLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv1Delay));
    addAndMakeVisible(*env1DelayLabel);

    env1DelaySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv1Delay, inTheme);
    env1DelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv1Delay,
        *env1DelaySlider);
    addAndMakeVisible(*env1DelaySlider);

    parameterSeparator1 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator1);

    // Attack
    env1AttackLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv1Attack));
    addAndMakeVisible(*env1AttackLabel);

    env1AttackSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv1Attack, inTheme);
    env1AttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv1Attack,
        *env1AttackSlider);
    addAndMakeVisible(*env1AttackSlider);

    parameterSeparator2 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator2);

    // Decay
    env1DecayLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv1Decay));
    addAndMakeVisible(*env1DecayLabel);

    env1DecaySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv1Decay, inTheme);
    env1DecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv1Decay,
        *env1DecaySlider);
    addAndMakeVisible(*env1DecaySlider);

    parameterSeparator3 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator3);

    // Sustain
    env1SustainLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv1Sustain));
    addAndMakeVisible(*env1SustainLabel);

    env1SustainSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv1Sustain, inTheme);
    env1SustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv1Sustain,
        *env1SustainSlider);
    addAndMakeVisible(*env1SustainSlider);

    parameterSeparator4 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator4);

    // Release
    env1ReleaseLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv1Release));
    addAndMakeVisible(*env1ReleaseLabel);

    env1ReleaseSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv1Release, inTheme);
    env1ReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv1Release,
        *env1ReleaseSlider);
    addAndMakeVisible(*env1ReleaseSlider);

    parameterSeparator5 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator5);

    // Amplitude
    env1AmplitudeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv1Amplitude));
    addAndMakeVisible(*env1AmplitudeLabel);

    env1AmplitudeSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv1Amplitude, inTheme);
    env1AmplitudeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv1Amplitude,
        *env1AmplitudeSlider);
    addAndMakeVisible(*env1AmplitudeSlider);

    parameterSeparator6 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator6);

    // Amplitude Mod by Velocity
    env1AmplitudeModByVelocityLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv1AmplitudeModByVelocity));
    addAndMakeVisible(*env1AmplitudeModByVelocityLabel);

    env1AmplitudeModByVelocitySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv1AmplitudeModByVelocity, inTheme);
    env1AmplitudeModByVelocityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv1AmplitudeModByVelocity,
        *env1AmplitudeModByVelocitySlider);
    addAndMakeVisible(*env1AmplitudeModByVelocitySlider);

    parameterSeparator7 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator7);

    // Trigger Mode
    env1TriggerModeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv1TriggerMode));
    addAndMakeVisible(*env1TriggerModeLabel);

    env1TriggerModeComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kEnv1TriggerMode, inTheme);
    env1TriggerModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv1TriggerMode,
        *env1TriggerModeComboBox);
    addAndMakeVisible(*env1TriggerModeComboBox);

    parameterSeparator8 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator8);

    // Envelope Mode
    env1EnvelopeModeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv1EnvelopeMode));
    addAndMakeVisible(*env1EnvelopeModeLabel);

    env1EnvelopeModeComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kEnv1EnvelopeMode, inTheme);
    env1EnvelopeModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv1EnvelopeMode,
        *env1EnvelopeModeComboBox);
    addAndMakeVisible(*env1EnvelopeModeComboBox);

    parameterSeparator9 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator9);

    // LFO 1 Trigger
    env1Lfo1TriggerLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv1Lfo1Trigger));
    addAndMakeVisible(*env1Lfo1TriggerLabel);

    env1Lfo1TriggerComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kEnv1Lfo1Trigger, inTheme);
    env1Lfo1TriggerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv1Lfo1Trigger,
        *env1Lfo1TriggerComboBox);
    addAndMakeVisible(*env1Lfo1TriggerComboBox);

    parameterSeparator10 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator10);

    setSize(getWidth(), getHeight());
}

Env1Panel::~Env1Panel() = default;

void Env1Panel::paint(juce::Graphics& g)
{
    if (theme == nullptr)
    {
        return;
    }

    g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void Env1Panel::resized()
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

    if (env1ModuleName != nullptr)
    {
        env1ModuleName->setBounds(0, y, moduleNameWidth, moduleNameHeight);
    }

    if (env1PasteButton != nullptr)
    {
        env1PasteButton->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);
    }

    if (env1CopyButton != nullptr)
    {
        env1CopyButton->setBounds(panelWidth - buttonWidth * 2, y, buttonWidth, buttonHeight);
    }

    if (env1InitButton != nullptr)
    {
        env1InitButton->setBounds(panelWidth - buttonWidth * 3, y, buttonWidth, buttonHeight);
    }

    y += moduleNameHeight;

    if (env1DelayLabel != nullptr)
    {
        env1DelayLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env1DelaySlider != nullptr)
    {
        env1DelaySlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env1AttackLabel != nullptr)
    {
        env1AttackLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env1AttackSlider != nullptr)
    {
        env1AttackSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env1DecayLabel != nullptr)
    {
        env1DecayLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env1DecaySlider != nullptr)
    {
        env1DecaySlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env1SustainLabel != nullptr)
    {
        env1SustainLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env1SustainSlider != nullptr)
    {
        env1SustainSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env1ReleaseLabel != nullptr)
    {
        env1ReleaseLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env1ReleaseSlider != nullptr)
    {
        env1ReleaseSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env1AmplitudeLabel != nullptr)
    {
        env1AmplitudeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env1AmplitudeSlider != nullptr)
    {
        env1AmplitudeSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env1AmplitudeModByVelocityLabel != nullptr)
    {
        env1AmplitudeModByVelocityLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env1AmplitudeModByVelocitySlider != nullptr)
    {
        env1AmplitudeModByVelocitySlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env1TriggerModeLabel != nullptr)
    {
        env1TriggerModeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env1TriggerModeComboBox != nullptr)
    {
        env1TriggerModeComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env1EnvelopeModeLabel != nullptr)
    {
        env1EnvelopeModeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env1EnvelopeModeComboBox != nullptr)
    {
        env1EnvelopeModeComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator9 != nullptr)
    {
        parameterSeparator9->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env1Lfo1TriggerLabel != nullptr)
    {
        env1Lfo1TriggerLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env1Lfo1TriggerComboBox != nullptr)
    {
        env1Lfo1TriggerComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator10 != nullptr)
    {
        parameterSeparator10->setBounds(0, y, separatorWidth, separatorHeight);
    }
}

void Env1Panel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (env1ModuleName != nullptr)
    {
        env1ModuleName->setTheme(inTheme);
    }

    if (env1InitButton != nullptr)
    {
        env1InitButton->setTheme(inTheme);
    }

    if (env1CopyButton != nullptr)
    {
        env1CopyButton->setTheme(inTheme);
    }

    if (env1PasteButton != nullptr)
    {
        env1PasteButton->setTheme(inTheme);
    }

    if (env1DelayLabel != nullptr)
    {
        env1DelayLabel->setTheme(inTheme);
    }

    if (env1DelaySlider != nullptr)
    {
        env1DelaySlider->setTheme(inTheme);
    }

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setTheme(inTheme);
    }

    if (env1AttackLabel != nullptr)
    {
        env1AttackLabel->setTheme(inTheme);
    }

    if (env1AttackSlider != nullptr)
    {
        env1AttackSlider->setTheme(inTheme);
    }

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setTheme(inTheme);
    }

    if (env1DecayLabel != nullptr)
    {
        env1DecayLabel->setTheme(inTheme);
    }

    if (env1DecaySlider != nullptr)
    {
        env1DecaySlider->setTheme(inTheme);
    }

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setTheme(inTheme);
    }

    if (env1SustainLabel != nullptr)
    {
        env1SustainLabel->setTheme(inTheme);
    }

    if (env1SustainSlider != nullptr)
    {
        env1SustainSlider->setTheme(inTheme);
    }

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setTheme(inTheme);
    }

    if (env1ReleaseLabel != nullptr)
    {
        env1ReleaseLabel->setTheme(inTheme);
    }

    if (env1ReleaseSlider != nullptr)
    {
        env1ReleaseSlider->setTheme(inTheme);
    }

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setTheme(inTheme);
    }

    if (env1AmplitudeLabel != nullptr)
    {
        env1AmplitudeLabel->setTheme(inTheme);
    }

    if (env1AmplitudeSlider != nullptr)
    {
        env1AmplitudeSlider->setTheme(inTheme);
    }

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setTheme(inTheme);
    }

    if (env1AmplitudeModByVelocityLabel != nullptr)
    {
        env1AmplitudeModByVelocityLabel->setTheme(inTheme);
    }

    if (env1AmplitudeModByVelocitySlider != nullptr)
    {
        env1AmplitudeModByVelocitySlider->setTheme(inTheme);
    }

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setTheme(inTheme);
    }

    if (env1TriggerModeLabel != nullptr)
    {
        env1TriggerModeLabel->setTheme(inTheme);
    }

    if (env1TriggerModeComboBox != nullptr)
    {
        env1TriggerModeComboBox->setTheme(inTheme);
    }

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setTheme(inTheme);
    }

    if (env1EnvelopeModeLabel != nullptr)
    {
        env1EnvelopeModeLabel->setTheme(inTheme);
    }

    if (env1EnvelopeModeComboBox != nullptr)
    {
        env1EnvelopeModeComboBox->setTheme(inTheme);
    }

    if (parameterSeparator9 != nullptr)
    {
        parameterSeparator9->setTheme(inTheme);
    }

    if (env1Lfo1TriggerLabel != nullptr)
    {
        env1Lfo1TriggerLabel->setTheme(inTheme);
    }

    if (env1Lfo1TriggerComboBox != nullptr)
    {
        env1Lfo1TriggerComboBox->setTheme(inTheme);
    }

    if (parameterSeparator10 != nullptr)
    {
        parameterSeparator10->setTheme(inTheme);
    }

    repaint();
}
