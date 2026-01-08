#include "Env1Panel.h"

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

Env1Panel::Env1Panel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    setupModuleHeader(inTheme, widgetFactory, SynthDescriptors::ModuleIds::kEnvelope1);
    setupInitCopyPasteButtons(inTheme, widgetFactory,
                              SynthDescriptors::StandaloneWidgetIds::kEnv1Init,
                              SynthDescriptors::StandaloneWidgetIds::kEnv1Copy,
                              SynthDescriptors::StandaloneWidgetIds::kEnv1Paste);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kEnv1Delay,
                                env1DelayLabel, env1DelaySlider, env1DelayAttachment, horizontalSeparator1);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kEnv1Attack,
                                env1AttackLabel, env1AttackSlider, env1AttackAttachment, horizontalSeparator2);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kEnv1Decay,
                                env1DecayLabel, env1DecaySlider, env1DecayAttachment, horizontalSeparator3);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kEnv1Sustain,
                                env1SustainLabel, env1SustainSlider, env1SustainAttachment, horizontalSeparator4);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kEnv1Release,
                                env1ReleaseLabel, env1ReleaseSlider, env1ReleaseAttachment, horizontalSeparator5);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kEnv1Amplitude,
                                env1AmplitudeLabel, env1AmplitudeSlider, env1AmplitudeAttachment, horizontalSeparator6);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kEnv1AmplitudeModByVelocity,
                                env1AmplitudeModByVelocityLabel, env1AmplitudeModByVelocitySlider, env1AmplitudeModByVelocityAttachment, horizontalSeparator7);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     SynthDescriptors::ParameterIds::kEnv1TriggerMode,
                                     env1TriggerModeLabel, env1TriggerModeComboBox, env1TriggerModeAttachment, horizontalSeparator8);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     SynthDescriptors::ParameterIds::kEnv1EnvelopeMode,
                                     env1EnvelopeModeLabel, env1EnvelopeModeComboBox, env1EnvelopeModeAttachment, horizontalSeparator9);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     SynthDescriptors::ParameterIds::kEnv1Lfo1Trigger,
                                     env1Lfo1TriggerLabel, env1Lfo1TriggerComboBox, env1Lfo1TriggerAttachment, horizontalSeparator10);

    setSize(getWidth(), getHeight());
}

Env1Panel::~Env1Panel() = default;

void Env1Panel::paint(juce::Graphics& g)
{
        g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void Env1Panel::resized()
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

    if (auto* header = env1ModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    if (auto* button = env1PasteButton.get())
        button->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);

    if (auto* button = env1CopyButton.get())
        button->setBounds(panelWidth - buttonWidth * 2, y, buttonWidth, buttonHeight);

    if (auto* button = env1InitButton.get())
        button->setBounds(panelWidth - buttonWidth * 3, y, buttonWidth, buttonHeight);

    y += moduleHeaderHeight;

    if (auto* label = env1DelayLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env1DelaySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env1AttackLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env1AttackSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env1DecayLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env1DecaySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env1SustainLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env1SustainSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env1ReleaseLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env1ReleaseSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env1AmplitudeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env1AmplitudeSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env1AmplitudeModByVelocityLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env1AmplitudeModByVelocitySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env1TriggerModeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = env1TriggerModeComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator8.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env1EnvelopeModeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = env1EnvelopeModeComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator9.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env1Lfo1TriggerLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = env1Lfo1TriggerComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator10.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void Env1Panel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = env1ModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* button = env1InitButton.get())
        button->setTheme(inTheme);

    if (auto* button = env1CopyButton.get())
        button->setTheme(inTheme);

    if (auto* button = env1PasteButton.get())
        button->setTheme(inTheme);

    if (auto* label = env1DelayLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env1DelaySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = env1AttackLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env1AttackSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = env1DecayLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env1DecaySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = env1SustainLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env1SustainSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = env1ReleaseLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env1ReleaseSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = env1AmplitudeLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env1AmplitudeSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = env1AmplitudeModByVelocityLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env1AmplitudeModByVelocitySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator7.get())
        separator->setTheme(inTheme);

    if (auto* label = env1TriggerModeLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = env1TriggerModeComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator8.get())
        separator->setTheme(inTheme);

    if (auto* label = env1EnvelopeModeLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = env1EnvelopeModeComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator9.get())
        separator->setTheme(inTheme);

    if (auto* label = env1Lfo1TriggerLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = env1Lfo1TriggerComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator10.get())
        separator->setTheme(inTheme);

    repaint();
}

void Env1Panel::setupModuleHeader(Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    env1ModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(moduleId));
    addAndMakeVisible(*env1ModuleHeader);
}

void Env1Panel::setupInitCopyPasteButtons(Theme& inTheme, WidgetFactory& widgetFactory,
                                         const juce::String& initWidgetId, const juce::String& copyWidgetId, const juce::String& pasteWidgetId)
{
    env1InitButton = widgetFactory.createStandaloneButton(initWidgetId, inTheme);
    env1InitButton->onClick = [this, initWidgetId]
    {
        apvts.state.setProperty(initWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env1InitButton);

    env1CopyButton = widgetFactory.createStandaloneButton(copyWidgetId, inTheme);
    env1CopyButton->onClick = [this, copyWidgetId]
    {
        apvts.state.setProperty(copyWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env1CopyButton);

    env1PasteButton = widgetFactory.createStandaloneButton(pasteWidgetId, inTheme);
    env1PasteButton->onClick = [this, pasteWidgetId]
    {
        apvts.state.setProperty(pasteWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env1PasteButton);
}

void Env1Panel::setupIntParameterWithSlider(Theme& inTheme, WidgetFactory& widgetFactory,
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

void Env1Panel::setupChoiceParameterWithComboBox(Theme& inTheme, WidgetFactory& widgetFactory,
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
