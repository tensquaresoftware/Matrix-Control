#include "Env2Panel.h"

#include "../../../../../Themes/Theme.h"
#include "../../../../../Widgets/ModuleHeader.h"
#include "../../../../../Widgets/Label.h"
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
    env2ModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kEnvelope2));
    addAndMakeVisible(*env2ModuleHeader);

    // Standalone Widgets
    env2InitButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kEnv2Init, inTheme);
    env2InitButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kEnv2Init,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env2InitButton);

    env2CopyButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kEnv2Copy, inTheme);
    env2CopyButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kEnv2Copy,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env2CopyButton);

    env2PasteButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kEnv2Paste, inTheme);
    env2PasteButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kEnv2Paste,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env2PasteButton);

    // Delay
    env2DelayLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2Delay));
    addAndMakeVisible(*env2DelayLabel);

    env2DelaySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv2Delay, inTheme);
    env2DelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2Delay,
        *env2DelaySlider);
    addAndMakeVisible(*env2DelaySlider);

    parameterSeparator1 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator1);

    // Attack
    env2AttackLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2Attack));
    addAndMakeVisible(*env2AttackLabel);

    env2AttackSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv2Attack, inTheme);
    env2AttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2Attack,
        *env2AttackSlider);
    addAndMakeVisible(*env2AttackSlider);

    parameterSeparator2 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator2);

    // Decay
    env2DecayLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2Decay));
    addAndMakeVisible(*env2DecayLabel);

    env2DecaySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv2Decay, inTheme);
    env2DecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2Decay,
        *env2DecaySlider);
    addAndMakeVisible(*env2DecaySlider);

    parameterSeparator3 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator3);

    // Sustain
    env2SustainLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2Sustain));
    addAndMakeVisible(*env2SustainLabel);

    env2SustainSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv2Sustain, inTheme);
    env2SustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2Sustain,
        *env2SustainSlider);
    addAndMakeVisible(*env2SustainSlider);

    parameterSeparator4 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator4);

    // Release
    env2ReleaseLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2Release));
    addAndMakeVisible(*env2ReleaseLabel);

    env2ReleaseSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv2Release, inTheme);
    env2ReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2Release,
        *env2ReleaseSlider);
    addAndMakeVisible(*env2ReleaseSlider);

    parameterSeparator5 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator5);

    // Amplitude
    env2AmplitudeLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2Amplitude));
    addAndMakeVisible(*env2AmplitudeLabel);

    env2AmplitudeSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv2Amplitude, inTheme);
    env2AmplitudeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2Amplitude,
        *env2AmplitudeSlider);
    addAndMakeVisible(*env2AmplitudeSlider);

    parameterSeparator6 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator6);

    // Amplitude Mod by Velocity
    env2AmplitudeModByVelocityLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2AmplitudeModByVelocity));
    addAndMakeVisible(*env2AmplitudeModByVelocityLabel);

    env2AmplitudeModByVelocitySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv2AmplitudeModByVelocity, inTheme);
    env2AmplitudeModByVelocityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2AmplitudeModByVelocity,
        *env2AmplitudeModByVelocitySlider);
    addAndMakeVisible(*env2AmplitudeModByVelocitySlider);

    parameterSeparator7 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator7);

    // Trigger Mode
    env2TriggerModeLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2TriggerMode));
    addAndMakeVisible(*env2TriggerModeLabel);

    env2TriggerModeComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kEnv2TriggerMode, inTheme);
    env2TriggerModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2TriggerMode,
        *env2TriggerModeComboBox);
    addAndMakeVisible(*env2TriggerModeComboBox);

    parameterSeparator8 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator8);

    // Envelope Mode
    env2EnvelopeModeLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2EnvelopeMode));
    addAndMakeVisible(*env2EnvelopeModeLabel);

    env2EnvelopeModeComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kEnv2EnvelopeMode, inTheme);
    env2EnvelopeModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2EnvelopeMode,
        *env2EnvelopeModeComboBox);
    addAndMakeVisible(*env2EnvelopeModeComboBox);

    parameterSeparator9 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator9);

    // LFO 1 Trigger
    env2Lfo1TriggerLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv2Lfo1Trigger));
    addAndMakeVisible(*env2Lfo1TriggerLabel);

    env2Lfo1TriggerComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kEnv2Lfo1Trigger, inTheme);
    env2Lfo1TriggerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv2Lfo1Trigger,
        *env2Lfo1TriggerComboBox);
    addAndMakeVisible(*env2Lfo1TriggerComboBox);

    parameterSeparator10 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator10);

    setSize(getWidth(), getHeight());
}

Env2Panel::~Env2Panel() = default;

void Env2Panel::paint(juce::Graphics& g)
{
        g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void Env2Panel::resized()
{
    const auto moduleHeaderHeight = tss::ModuleHeader::getHeight();
    const auto moduleHeaderWidth = tss::ModuleHeader::getNormalWidth();
    const auto buttonWidth = tss::Button::getDefaultWidth();
    const auto buttonHeight = tss::Button::getHeight();
    const auto labelWidth = tss::Label::getWidth(tss::Label::Type::Parameter);
    const auto labelHeight = tss::Label::getHeight();
    const auto sliderWidth = tss::Slider::getWidth();
    const auto sliderHeight = tss::Slider::getHeight();
    const auto comboBoxWidth = tss::ComboBox::getNormalWidth();
    const auto comboBoxHeight = tss::ComboBox::getHeight();
    const auto separatorWidth = tss::ParameterSeparator::getWidth(tss::ParameterSeparator::Type::PatchEditModule);
    const auto separatorHeight = tss::ParameterSeparator::getHeight();
    const auto panelWidth = getWidth();

    int y = 0;

    if (auto* header = env2ModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    if (auto* button = env2PasteButton.get())
        button->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);

    if (auto* button = env2CopyButton.get())
        button->setBounds(panelWidth - buttonWidth * 2, y, buttonWidth, buttonHeight);

    if (auto* button = env2InitButton.get())
        button->setBounds(panelWidth - buttonWidth * 3, y, buttonWidth, buttonHeight);

    y += moduleHeaderHeight;

    if (auto* label = env2DelayLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env2DelaySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env2AttackLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env2AttackSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env2DecayLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env2DecaySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env2SustainLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env2SustainSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env2ReleaseLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env2ReleaseSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env2AmplitudeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env2AmplitudeSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env2AmplitudeModByVelocityLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env2AmplitudeModByVelocitySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env2TriggerModeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = env2TriggerModeComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator8.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env2EnvelopeModeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = env2EnvelopeModeComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator9.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env2Lfo1TriggerLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = env2Lfo1TriggerComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator10.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void Env2Panel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = env2ModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* button = env2InitButton.get())
        button->setTheme(inTheme);

    if (auto* button = env2CopyButton.get())
        button->setTheme(inTheme);

    if (auto* button = env2PasteButton.get())
        button->setTheme(inTheme);

    if (auto* label = env2DelayLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env2DelaySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = env2AttackLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env2AttackSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = env2DecayLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env2DecaySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = env2SustainLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env2SustainSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = env2ReleaseLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env2ReleaseSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = env2AmplitudeLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env2AmplitudeSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = env2AmplitudeModByVelocityLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env2AmplitudeModByVelocitySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator7.get())
        separator->setTheme(inTheme);

    if (auto* label = env2TriggerModeLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = env2TriggerModeComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = parameterSeparator8.get())
        separator->setTheme(inTheme);

    if (auto* label = env2EnvelopeModeLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = env2EnvelopeModeComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = parameterSeparator9.get())
        separator->setTheme(inTheme);

    if (auto* label = env2Lfo1TriggerLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = env2Lfo1TriggerComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = parameterSeparator10.get())
        separator->setTheme(inTheme);

    repaint();
}
