#include "Env3Panel.h"

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

Env3Panel::Env3Panel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    env3ModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kEnvelope3));
    addAndMakeVisible(*env3ModuleHeader);

    // Standalone Widgets
    env3InitButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kEnv3Init, inTheme);
    env3InitButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kEnv3Init,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env3InitButton);

    env3CopyButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kEnv3Copy, inTheme);
    env3CopyButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kEnv3Copy,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env3CopyButton);

    env3PasteButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kEnv3Paste, inTheme);
    env3PasteButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kEnv3Paste,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env3PasteButton);

    // Delay
    env3DelayLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3Delay));
    addAndMakeVisible(*env3DelayLabel);

    env3DelaySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv3Delay, inTheme);
    env3DelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3Delay,
        *env3DelaySlider);
    addAndMakeVisible(*env3DelaySlider);

    parameterSeparator1 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator1);

    // Attack
    env3AttackLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3Attack));
    addAndMakeVisible(*env3AttackLabel);

    env3AttackSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv3Attack, inTheme);
    env3AttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3Attack,
        *env3AttackSlider);
    addAndMakeVisible(*env3AttackSlider);

    parameterSeparator2 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator2);

    // Decay
    env3DecayLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3Decay));
    addAndMakeVisible(*env3DecayLabel);

    env3DecaySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv3Decay, inTheme);
    env3DecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3Decay,
        *env3DecaySlider);
    addAndMakeVisible(*env3DecaySlider);

    parameterSeparator3 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator3);

    // Sustain
    env3SustainLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3Sustain));
    addAndMakeVisible(*env3SustainLabel);

    env3SustainSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv3Sustain, inTheme);
    env3SustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3Sustain,
        *env3SustainSlider);
    addAndMakeVisible(*env3SustainSlider);

    parameterSeparator4 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator4);

    // Release
    env3ReleaseLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3Release));
    addAndMakeVisible(*env3ReleaseLabel);

    env3ReleaseSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv3Release, inTheme);
    env3ReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3Release,
        *env3ReleaseSlider);
    addAndMakeVisible(*env3ReleaseSlider);

    parameterSeparator5 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator5);

    // Amplitude
    env3AmplitudeLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3Amplitude));
    addAndMakeVisible(*env3AmplitudeLabel);

    env3AmplitudeSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv3Amplitude, inTheme);
    env3AmplitudeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3Amplitude,
        *env3AmplitudeSlider);
    addAndMakeVisible(*env3AmplitudeSlider);

    parameterSeparator6 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator6);

    // Amplitude Mod by Velocity
    env3AmplitudeModByVelocityLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3AmplitudeModByVelocity));
    addAndMakeVisible(*env3AmplitudeModByVelocityLabel);

    env3AmplitudeModByVelocitySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kEnv3AmplitudeModByVelocity, inTheme);
    env3AmplitudeModByVelocityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3AmplitudeModByVelocity,
        *env3AmplitudeModByVelocitySlider);
    addAndMakeVisible(*env3AmplitudeModByVelocitySlider);

    parameterSeparator7 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator7);

    // Trigger Mode
    env3TriggerModeLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3TriggerMode));
    addAndMakeVisible(*env3TriggerModeLabel);

    env3TriggerModeComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kEnv3TriggerMode, inTheme);
    env3TriggerModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3TriggerMode,
        *env3TriggerModeComboBox);
    addAndMakeVisible(*env3TriggerModeComboBox);

    parameterSeparator8 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator8);

    // Envelope Mode
    env3EnvelopeModeLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3EnvelopeMode));
    addAndMakeVisible(*env3EnvelopeModeLabel);

    env3EnvelopeModeComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kEnv3EnvelopeMode, inTheme);
    env3EnvelopeModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3EnvelopeMode,
        *env3EnvelopeModeComboBox);
    addAndMakeVisible(*env3EnvelopeModeComboBox);

    parameterSeparator9 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator9);

    // LFO 1 Trigger
    env3Lfo1TriggerLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kEnv3Lfo1Trigger));
    addAndMakeVisible(*env3Lfo1TriggerLabel);

    env3Lfo1TriggerComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kEnv3Lfo1Trigger, inTheme);
    env3Lfo1TriggerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kEnv3Lfo1Trigger,
        *env3Lfo1TriggerComboBox);
    addAndMakeVisible(*env3Lfo1TriggerComboBox);

    parameterSeparator10 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator10);

    setSize(getWidth(), getHeight());
}

Env3Panel::~Env3Panel() = default;

void Env3Panel::paint(juce::Graphics& g)
{
        g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void Env3Panel::resized()
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

    if (auto* header = env3ModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    if (auto* button = env3PasteButton.get())
        button->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);

    if (auto* button = env3CopyButton.get())
        button->setBounds(panelWidth - buttonWidth * 2, y, buttonWidth, buttonHeight);

    if (auto* button = env3InitButton.get())
        button->setBounds(panelWidth - buttonWidth * 3, y, buttonWidth, buttonHeight);

    y += moduleHeaderHeight;

    if (auto* label = env3DelayLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env3DelaySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3AttackLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env3AttackSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3DecayLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env3DecaySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3SustainLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env3SustainSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3ReleaseLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env3ReleaseSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3AmplitudeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env3AmplitudeSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3AmplitudeModByVelocityLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env3AmplitudeModByVelocitySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3TriggerModeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = env3TriggerModeComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator8.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3EnvelopeModeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = env3EnvelopeModeComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator9.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3Lfo1TriggerLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = env3Lfo1TriggerComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator10.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void Env3Panel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = env3ModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* button = env3InitButton.get())
        button->setTheme(inTheme);

    if (auto* button = env3CopyButton.get())
        button->setTheme(inTheme);

    if (auto* button = env3PasteButton.get())
        button->setTheme(inTheme);

    if (auto* label = env3DelayLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env3DelaySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = env3AttackLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env3AttackSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = env3DecayLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env3DecaySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = env3SustainLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env3SustainSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = env3ReleaseLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env3ReleaseSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = env3AmplitudeLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env3AmplitudeSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = env3AmplitudeModByVelocityLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env3AmplitudeModByVelocitySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator7.get())
        separator->setTheme(inTheme);

    if (auto* label = env3TriggerModeLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = env3TriggerModeComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = parameterSeparator8.get())
        separator->setTheme(inTheme);

    if (auto* label = env3EnvelopeModeLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = env3EnvelopeModeComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = parameterSeparator9.get())
        separator->setTheme(inTheme);

    if (auto* label = env3Lfo1TriggerLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = env3Lfo1TriggerComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = parameterSeparator10.get())
        separator->setTheme(inTheme);

    repaint();
}
