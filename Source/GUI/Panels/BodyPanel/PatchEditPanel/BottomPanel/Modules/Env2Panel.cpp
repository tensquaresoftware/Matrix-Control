#include "Env2Panel.h"

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

Env2Panel::Env2Panel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    env2ModuleName = std::make_unique<tss::ModuleName>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kEnvelope2));
    addAndMakeVisible(*env2ModuleName);

    // Standalone Widgets
    env2InitButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kEnv2Init, inTheme);
    env2InitButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kEnv2Init,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env2InitButton);

    env2CopyButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kEnv2Copy, inTheme);
    env2CopyButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kEnv2Copy,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env2CopyButton);

    env2PasteButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kEnv2Paste, inTheme);
    env2PasteButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kEnv2Paste,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env2PasteButton);

    // Delay
    env2DelayLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2Delay));
    addAndMakeVisible(*env2DelayLabel);

    env2DelaySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv2Delay, inTheme);
    env2DelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2Delay,
        *env2DelaySlider);
    addAndMakeVisible(*env2DelaySlider);

    parameterSeparator1 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator1);

    // Attack
    env2AttackLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2Attack));
    addAndMakeVisible(*env2AttackLabel);

    env2AttackSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv2Attack, inTheme);
    env2AttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2Attack,
        *env2AttackSlider);
    addAndMakeVisible(*env2AttackSlider);

    parameterSeparator2 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator2);

    // Decay
    env2DecayLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2Decay));
    addAndMakeVisible(*env2DecayLabel);

    env2DecaySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv2Decay, inTheme);
    env2DecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2Decay,
        *env2DecaySlider);
    addAndMakeVisible(*env2DecaySlider);

    parameterSeparator3 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator3);

    // Sustain
    env2SustainLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2Sustain));
    addAndMakeVisible(*env2SustainLabel);

    env2SustainSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv2Sustain, inTheme);
    env2SustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2Sustain,
        *env2SustainSlider);
    addAndMakeVisible(*env2SustainSlider);

    parameterSeparator4 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator4);

    // Release
    env2ReleaseLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2Release));
    addAndMakeVisible(*env2ReleaseLabel);

    env2ReleaseSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv2Release, inTheme);
    env2ReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2Release,
        *env2ReleaseSlider);
    addAndMakeVisible(*env2ReleaseSlider);

    parameterSeparator5 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator5);

    // Amplitude
    env2AmplitudeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2Amplitude));
    addAndMakeVisible(*env2AmplitudeLabel);

    env2AmplitudeSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv2Amplitude, inTheme);
    env2AmplitudeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2Amplitude,
        *env2AmplitudeSlider);
    addAndMakeVisible(*env2AmplitudeSlider);

    parameterSeparator6 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator6);

    // Amplitude Mod by Velocity
    env2AmplitudeModByVelocityLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2AmplitudeModByVelocity));
    addAndMakeVisible(*env2AmplitudeModByVelocityLabel);

    env2AmplitudeModByVelocitySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv2AmplitudeModByVelocity, inTheme);
    env2AmplitudeModByVelocityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2AmplitudeModByVelocity,
        *env2AmplitudeModByVelocitySlider);
    addAndMakeVisible(*env2AmplitudeModByVelocitySlider);

    parameterSeparator7 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator7);

    // Trigger Mode
    env2TriggerModeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2TriggerMode));
    addAndMakeVisible(*env2TriggerModeLabel);

    env2TriggerModeComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kEnv2TriggerMode, inTheme);
    env2TriggerModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2TriggerMode,
        *env2TriggerModeComboBox);
    addAndMakeVisible(*env2TriggerModeComboBox);

    parameterSeparator8 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator8);

    // Envelope Mode
    env2EnvelopeModeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2EnvelopeMode));
    addAndMakeVisible(*env2EnvelopeModeLabel);

    env2EnvelopeModeComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kEnv2EnvelopeMode, inTheme);
    env2EnvelopeModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2EnvelopeMode,
        *env2EnvelopeModeComboBox);
    addAndMakeVisible(*env2EnvelopeModeComboBox);

    parameterSeparator9 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator9);

    // LFO 1 Trigger
    env2Lfo1TriggerLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2Lfo1Trigger));
    addAndMakeVisible(*env2Lfo1TriggerLabel);

    env2Lfo1TriggerComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kEnv2Lfo1Trigger, inTheme);
    env2Lfo1TriggerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2Lfo1Trigger,
        *env2Lfo1TriggerComboBox);
    addAndMakeVisible(*env2Lfo1TriggerComboBox);

    parameterSeparator10 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator10);

    setSize(getWidth(), getHeight());
}

Env2Panel::~Env2Panel() = default;

void Env2Panel::paint(juce::Graphics& g)
{
    if (theme == nullptr)
    {
        return;
    }

    g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void Env2Panel::resized()
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

    if (env2ModuleName != nullptr)
    {
        env2ModuleName->setBounds(0, y, moduleNameWidth, moduleNameHeight);
    }

    if (env2PasteButton != nullptr)
    {
        env2PasteButton->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);
    }

    if (env2CopyButton != nullptr)
    {
        env2CopyButton->setBounds(panelWidth - buttonWidth * 2, y, buttonWidth, buttonHeight);
    }

    if (env2InitButton != nullptr)
    {
        env2InitButton->setBounds(panelWidth - buttonWidth * 3, y, buttonWidth, buttonHeight);
    }

    y += moduleNameHeight;

    if (env2DelayLabel != nullptr)
    {
        env2DelayLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env2DelaySlider != nullptr)
    {
        env2DelaySlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env2AttackLabel != nullptr)
    {
        env2AttackLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env2AttackSlider != nullptr)
    {
        env2AttackSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env2DecayLabel != nullptr)
    {
        env2DecayLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env2DecaySlider != nullptr)
    {
        env2DecaySlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env2SustainLabel != nullptr)
    {
        env2SustainLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env2SustainSlider != nullptr)
    {
        env2SustainSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env2ReleaseLabel != nullptr)
    {
        env2ReleaseLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env2ReleaseSlider != nullptr)
    {
        env2ReleaseSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env2AmplitudeLabel != nullptr)
    {
        env2AmplitudeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env2AmplitudeSlider != nullptr)
    {
        env2AmplitudeSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env2AmplitudeModByVelocityLabel != nullptr)
    {
        env2AmplitudeModByVelocityLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env2AmplitudeModByVelocitySlider != nullptr)
    {
        env2AmplitudeModByVelocitySlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env2TriggerModeLabel != nullptr)
    {
        env2TriggerModeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env2TriggerModeComboBox != nullptr)
    {
        env2TriggerModeComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env2EnvelopeModeLabel != nullptr)
    {
        env2EnvelopeModeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env2EnvelopeModeComboBox != nullptr)
    {
        env2EnvelopeModeComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator9 != nullptr)
    {
        parameterSeparator9->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (env2Lfo1TriggerLabel != nullptr)
    {
        env2Lfo1TriggerLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (env2Lfo1TriggerComboBox != nullptr)
    {
        env2Lfo1TriggerComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator10 != nullptr)
    {
        parameterSeparator10->setBounds(0, y, separatorWidth, separatorHeight);
    }
}

void Env2Panel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (env2ModuleName != nullptr)
    {
        env2ModuleName->setTheme(inTheme);
    }

    if (env2InitButton != nullptr)
    {
        env2InitButton->setTheme(inTheme);
    }

    if (env2CopyButton != nullptr)
    {
        env2CopyButton->setTheme(inTheme);
    }

    if (env2PasteButton != nullptr)
    {
        env2PasteButton->setTheme(inTheme);
    }

    if (env2DelayLabel != nullptr)
    {
        env2DelayLabel->setTheme(inTheme);
    }

    if (env2DelaySlider != nullptr)
    {
        env2DelaySlider->setTheme(inTheme);
    }

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setTheme(inTheme);
    }

    if (env2AttackLabel != nullptr)
    {
        env2AttackLabel->setTheme(inTheme);
    }

    if (env2AttackSlider != nullptr)
    {
        env2AttackSlider->setTheme(inTheme);
    }

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setTheme(inTheme);
    }

    if (env2DecayLabel != nullptr)
    {
        env2DecayLabel->setTheme(inTheme);
    }

    if (env2DecaySlider != nullptr)
    {
        env2DecaySlider->setTheme(inTheme);
    }

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setTheme(inTheme);
    }

    if (env2SustainLabel != nullptr)
    {
        env2SustainLabel->setTheme(inTheme);
    }

    if (env2SustainSlider != nullptr)
    {
        env2SustainSlider->setTheme(inTheme);
    }

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setTheme(inTheme);
    }

    if (env2ReleaseLabel != nullptr)
    {
        env2ReleaseLabel->setTheme(inTheme);
    }

    if (env2ReleaseSlider != nullptr)
    {
        env2ReleaseSlider->setTheme(inTheme);
    }

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setTheme(inTheme);
    }

    if (env2AmplitudeLabel != nullptr)
    {
        env2AmplitudeLabel->setTheme(inTheme);
    }

    if (env2AmplitudeSlider != nullptr)
    {
        env2AmplitudeSlider->setTheme(inTheme);
    }

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setTheme(inTheme);
    }

    if (env2AmplitudeModByVelocityLabel != nullptr)
    {
        env2AmplitudeModByVelocityLabel->setTheme(inTheme);
    }

    if (env2AmplitudeModByVelocitySlider != nullptr)
    {
        env2AmplitudeModByVelocitySlider->setTheme(inTheme);
    }

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setTheme(inTheme);
    }

    if (env2TriggerModeLabel != nullptr)
    {
        env2TriggerModeLabel->setTheme(inTheme);
    }

    if (env2TriggerModeComboBox != nullptr)
    {
        env2TriggerModeComboBox->setTheme(inTheme);
    }

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setTheme(inTheme);
    }

    if (env2EnvelopeModeLabel != nullptr)
    {
        env2EnvelopeModeLabel->setTheme(inTheme);
    }

    if (env2EnvelopeModeComboBox != nullptr)
    {
        env2EnvelopeModeComboBox->setTheme(inTheme);
    }

    if (parameterSeparator9 != nullptr)
    {
        parameterSeparator9->setTheme(inTheme);
    }

    if (env2Lfo1TriggerLabel != nullptr)
    {
        env2Lfo1TriggerLabel->setTheme(inTheme);
    }

    if (env2Lfo1TriggerComboBox != nullptr)
    {
        env2Lfo1TriggerComboBox->setTheme(inTheme);
    }

    if (parameterSeparator10 != nullptr)
    {
        parameterSeparator10->setTheme(inTheme);
    }

    repaint();
}
