#include "RampPortamentoPanel.h"

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

RampPortamentoPanel::RampPortamentoPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    rampPortamentoModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kRampPortamento));
    addAndMakeVisible(*rampPortamentoModuleHeader);

    // Standalone Widgets
    rampPortamentoInitButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kRampPortamentoInit, inTheme);
    rampPortamentoInitButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kRampPortamentoInit,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*rampPortamentoInitButton);

    // Ramp 1 Rate
    ramp1RateLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kRamp1Rate));
    addAndMakeVisible(*ramp1RateLabel);

    ramp1RateSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kRamp1Rate, inTheme);
    ramp1RateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kRamp1Rate,
        *ramp1RateSlider);
    addAndMakeVisible(*ramp1RateSlider);

    parameterSeparator1 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator1);

    // Ramp 1 Trigger
    ramp1TriggerLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kRamp1Trigger));
    addAndMakeVisible(*ramp1TriggerLabel);

    ramp1TriggerComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kRamp1Trigger, inTheme);
    ramp1TriggerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kRamp1Trigger,
        *ramp1TriggerComboBox);
    addAndMakeVisible(*ramp1TriggerComboBox);

    parameterSeparator2 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator2);

    // Ramp 2 Rate
    ramp2RateLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kRamp2Rate));
    addAndMakeVisible(*ramp2RateLabel);

    ramp2RateSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kRamp2Rate, inTheme);
    ramp2RateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kRamp2Rate,
        *ramp2RateSlider);
    addAndMakeVisible(*ramp2RateSlider);

    parameterSeparator3 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator3);

    // Ramp 2 Trigger
    ramp2TriggerLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kRamp2Trigger));
    addAndMakeVisible(*ramp2TriggerLabel);

    ramp2TriggerComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kRamp2Trigger, inTheme);
    ramp2TriggerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kRamp2Trigger,
        *ramp2TriggerComboBox);
    addAndMakeVisible(*ramp2TriggerComboBox);

    parameterSeparator4 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator4);

    // Portamento Rate
    portamentoRateLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kPortamentoRate));
    addAndMakeVisible(*portamentoRateLabel);

    portamentoRateSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kPortamentoRate, inTheme);
    portamentoRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kPortamentoRate,
        *portamentoRateSlider);
    addAndMakeVisible(*portamentoRateSlider);

    parameterSeparator5 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator5);

    // Portamento Mod by Velocity
    portamentoModByVelocityLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kPortamentoModByVelocity));
    addAndMakeVisible(*portamentoModByVelocityLabel);

    portamentoModByVelocitySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kPortamentoModByVelocity, inTheme);
    portamentoModByVelocityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kPortamentoModByVelocity,
        *portamentoModByVelocitySlider);
    addAndMakeVisible(*portamentoModByVelocitySlider);

    parameterSeparator6 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator6);

    // Portamento Mode
    portamentoModeLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kPortamentoMode));
    addAndMakeVisible(*portamentoModeLabel);

    portamentoModeComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kPortamentoMode, inTheme);
    portamentoModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kPortamentoMode,
        *portamentoModeComboBox);
    addAndMakeVisible(*portamentoModeComboBox);

    parameterSeparator7 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator7);

    // Portamento Legato
    portamentoLegatoLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kPortamentoLegato));
    addAndMakeVisible(*portamentoLegatoLabel);

    portamentoLegatoComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kPortamentoLegato, inTheme);
    portamentoLegatoAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kPortamentoLegato,
        *portamentoLegatoComboBox);
    addAndMakeVisible(*portamentoLegatoComboBox);

    parameterSeparator8 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator8);

    // Portamento Keyboard Mode
    portamentoKeyboardModeLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::Type::Parameter, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kPortamentoKeyboardMode));
    addAndMakeVisible(*portamentoKeyboardModeLabel);

    portamentoKeyboardModeComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kPortamentoKeyboardMode, inTheme);
    portamentoKeyboardModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kPortamentoKeyboardMode,
        *portamentoKeyboardModeComboBox);
    addAndMakeVisible(*portamentoKeyboardModeComboBox);

    parameterSeparator9 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator9);

    parameterSeparator10 = std::make_unique<tss::ParameterSeparator>(inTheme, tss::ParameterSeparator::Type::PatchEditModule);
    addAndMakeVisible(*parameterSeparator10);

    setSize(getWidth(), getHeight());
}

RampPortamentoPanel::~RampPortamentoPanel() = default;

void RampPortamentoPanel::paint(juce::Graphics& g)
{
        g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void RampPortamentoPanel::resized()
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

    if (auto* header = rampPortamentoModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    if (auto* button = rampPortamentoInitButton.get())
        button->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);

    y += moduleHeaderHeight;

    if (auto* label = ramp1RateLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = ramp1RateSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = ramp1TriggerLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = ramp1TriggerComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = ramp2RateLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = ramp2RateSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = ramp2TriggerLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = ramp2TriggerComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = portamentoRateLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = portamentoRateSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = portamentoModByVelocityLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = portamentoModByVelocitySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = portamentoModeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = portamentoModeComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = portamentoLegatoLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = portamentoLegatoComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator8.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = portamentoKeyboardModeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = portamentoKeyboardModeComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = parameterSeparator9.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    y += labelHeight;

    if (auto* separator = parameterSeparator10.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void RampPortamentoPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = rampPortamentoModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* button = rampPortamentoInitButton.get())
        button->setTheme(inTheme);

    if (auto* label = ramp1RateLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = ramp1RateSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = ramp1TriggerLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = ramp1TriggerComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = parameterSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = ramp2RateLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = ramp2RateSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = ramp2TriggerLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = ramp2TriggerComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = parameterSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = portamentoRateLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = portamentoRateSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = portamentoModByVelocityLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = portamentoModByVelocitySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = parameterSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = portamentoModeLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = portamentoModeComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = parameterSeparator7.get())
        separator->setTheme(inTheme);

    if (auto* label = portamentoLegatoLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = portamentoLegatoComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = parameterSeparator8.get())
        separator->setTheme(inTheme);

    if (auto* label = portamentoKeyboardModeLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = portamentoKeyboardModeComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = parameterSeparator9.get())
        separator->setTheme(inTheme);

    if (auto* separator = parameterSeparator10.get())
        separator->setTheme(inTheme);

    repaint();
}
