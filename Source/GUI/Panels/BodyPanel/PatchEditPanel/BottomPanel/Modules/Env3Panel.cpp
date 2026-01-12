#include "Env3Panel.h"

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

Env3Panel::Env3Panel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    setupModuleHeader(inTheme, widgetFactory, PluginDescriptors::ModuleIds::kEnvelope3);
    setupInitCopyPasteButtons(inTheme, widgetFactory,
                              PluginDescriptors::StandaloneWidgetIds::kEnv3Init,
                              PluginDescriptors::StandaloneWidgetIds::kEnv3Copy,
                              PluginDescriptors::StandaloneWidgetIds::kEnv3Paste);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kEnv3Delay,
                                env3DelayLabel, env3DelaySlider, env3DelayAttachment, horizontalSeparator1);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kEnv3Attack,
                                env3AttackLabel, env3AttackSlider, env3AttackAttachment, horizontalSeparator2);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kEnv3Decay,
                                env3DecayLabel, env3DecaySlider, env3DecayAttachment, horizontalSeparator3);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kEnv3Sustain,
                                env3SustainLabel, env3SustainSlider, env3SustainAttachment, horizontalSeparator4);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kEnv3Release,
                                env3ReleaseLabel, env3ReleaseSlider, env3ReleaseAttachment, horizontalSeparator5);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kEnv3Amplitude,
                                env3AmplitudeLabel, env3AmplitudeSlider, env3AmplitudeAttachment, horizontalSeparator6);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kEnv3AmplitudeModByVelocity,
                                env3AmplitudeModByVelocityLabel, env3AmplitudeModByVelocitySlider, env3AmplitudeModByVelocityAttachment, horizontalSeparator7);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kEnv3TriggerMode,
                                     env3TriggerModeLabel, env3TriggerModeComboBox, env3TriggerModeAttachment, horizontalSeparator8);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kEnv3EnvelopeMode,
                                     env3EnvelopeModeLabel, env3EnvelopeModeComboBox, env3EnvelopeModeAttachment, horizontalSeparator9);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kEnv3Lfo1Trigger,
                                     env3Lfo1TriggerLabel, env3Lfo1TriggerComboBox, env3Lfo1TriggerAttachment, horizontalSeparator10);

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

    if (auto* separator = horizontalSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3AttackLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env3AttackSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3DecayLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env3DecaySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3SustainLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env3SustainSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3ReleaseLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env3ReleaseSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3AmplitudeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env3AmplitudeSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3AmplitudeModByVelocityLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = env3AmplitudeModByVelocitySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3TriggerModeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = env3TriggerModeComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator8.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3EnvelopeModeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = env3EnvelopeModeComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator9.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = env3Lfo1TriggerLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = env3Lfo1TriggerComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator10.get())
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

    if (auto* separator = horizontalSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = env3AttackLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env3AttackSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = env3DecayLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env3DecaySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = env3SustainLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env3SustainSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = env3ReleaseLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env3ReleaseSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = env3AmplitudeLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env3AmplitudeSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = env3AmplitudeModByVelocityLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = env3AmplitudeModByVelocitySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator7.get())
        separator->setTheme(inTheme);

    if (auto* label = env3TriggerModeLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = env3TriggerModeComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator8.get())
        separator->setTheme(inTheme);

    if (auto* label = env3EnvelopeModeLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = env3EnvelopeModeComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator9.get())
        separator->setTheme(inTheme);

    if (auto* label = env3Lfo1TriggerLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = env3Lfo1TriggerComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator10.get())
        separator->setTheme(inTheme);

    repaint();
}

void Env3Panel::setupModuleHeader(Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    env3ModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(moduleId));
    addAndMakeVisible(*env3ModuleHeader);
}

void Env3Panel::setupInitCopyPasteButtons(Theme& inTheme, WidgetFactory& widgetFactory,
                                         const juce::String& initWidgetId, const juce::String& copyWidgetId, const juce::String& pasteWidgetId)
{
    env3InitButton = widgetFactory.createStandaloneButton(initWidgetId, inTheme);
    env3InitButton->onClick = [this, initWidgetId]
    {
        apvts.state.setProperty(initWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env3InitButton);

    env3CopyButton = widgetFactory.createStandaloneButton(copyWidgetId, inTheme);
    env3CopyButton->onClick = [this, copyWidgetId]
    {
        apvts.state.setProperty(copyWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env3CopyButton);

    env3PasteButton = widgetFactory.createStandaloneButton(pasteWidgetId, inTheme);
    env3PasteButton->onClick = [this, pasteWidgetId]
    {
        apvts.state.setProperty(pasteWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*env3PasteButton);
}

void Env3Panel::setupIntParameterWithSlider(Theme& inTheme, WidgetFactory& widgetFactory,
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

void Env3Panel::setupChoiceParameterWithComboBox(Theme& inTheme, WidgetFactory& widgetFactory,
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
