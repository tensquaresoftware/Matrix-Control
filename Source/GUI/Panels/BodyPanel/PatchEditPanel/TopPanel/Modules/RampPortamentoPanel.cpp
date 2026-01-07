#include "RampPortamentoPanel.h"

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

RampPortamentoPanel::RampPortamentoPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    rampPortamentoModuleName = std::make_unique<tss::ModuleName>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kRampPortamento));
    addAndMakeVisible(*rampPortamentoModuleName);

    // Standalone Widgets
    rampPortamentoInitButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kRampPortamentoInit, inTheme);
    rampPortamentoInitButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kRampPortamentoInit,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*rampPortamentoInitButton);

    // Ramp 1 Rate
    ramp1RateLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kRamp1Rate));
    addAndMakeVisible(*ramp1RateLabel);

    ramp1RateSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kRamp1Rate, inTheme);
    ramp1RateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kRamp1Rate,
        *ramp1RateSlider);
    addAndMakeVisible(*ramp1RateSlider);

    parameterSeparator1 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator1);

    // Ramp 1 Trigger
    ramp1TriggerLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kRamp1Trigger));
    addAndMakeVisible(*ramp1TriggerLabel);

    ramp1TriggerComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kRamp1Trigger, inTheme);
    ramp1TriggerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kRamp1Trigger,
        *ramp1TriggerComboBox);
    addAndMakeVisible(*ramp1TriggerComboBox);

    parameterSeparator2 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator2);

    // Ramp 2 Rate
    ramp2RateLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kRamp2Rate));
    addAndMakeVisible(*ramp2RateLabel);

    ramp2RateSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kRamp2Rate, inTheme);
    ramp2RateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kRamp2Rate,
        *ramp2RateSlider);
    addAndMakeVisible(*ramp2RateSlider);

    parameterSeparator3 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator3);

    // Ramp 2 Trigger
    ramp2TriggerLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kRamp2Trigger));
    addAndMakeVisible(*ramp2TriggerLabel);

    ramp2TriggerComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kRamp2Trigger, inTheme);
    ramp2TriggerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kRamp2Trigger,
        *ramp2TriggerComboBox);
    addAndMakeVisible(*ramp2TriggerComboBox);

    parameterSeparator4 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator4);

    // Portamento Rate
    portamentoRateLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kPortamentoRate));
    addAndMakeVisible(*portamentoRateLabel);

    portamentoRateSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kPortamentoRate, inTheme);
    portamentoRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kPortamentoRate,
        *portamentoRateSlider);
    addAndMakeVisible(*portamentoRateSlider);

    parameterSeparator5 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator5);

    // Portamento Mod by Velocity
    portamentoModByVelocityLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kPortamentoModByVelocity));
    addAndMakeVisible(*portamentoModByVelocityLabel);

    portamentoModByVelocitySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kPortamentoModByVelocity, inTheme);
    portamentoModByVelocityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kPortamentoModByVelocity,
        *portamentoModByVelocitySlider);
    addAndMakeVisible(*portamentoModByVelocitySlider);

    parameterSeparator6 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator6);

    // Portamento Mode
    portamentoModeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kPortamentoMode));
    addAndMakeVisible(*portamentoModeLabel);

    portamentoModeComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kPortamentoMode, inTheme);
    portamentoModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kPortamentoMode,
        *portamentoModeComboBox);
    addAndMakeVisible(*portamentoModeComboBox);

    parameterSeparator7 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator7);

    // Portamento Legato
    portamentoLegatoLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kPortamentoLegato));
    addAndMakeVisible(*portamentoLegatoLabel);

    portamentoLegatoComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kPortamentoLegato, inTheme);
    portamentoLegatoAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kPortamentoLegato,
        *portamentoLegatoComboBox);
    addAndMakeVisible(*portamentoLegatoComboBox);

    parameterSeparator8 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator8);

    // Portamento Keyboard Mode
    portamentoKeyboardModeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kPortamentoKeyboardMode));
    addAndMakeVisible(*portamentoKeyboardModeLabel);

    portamentoKeyboardModeComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kPortamentoKeyboardMode, inTheme);
    portamentoKeyboardModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kPortamentoKeyboardMode,
        *portamentoKeyboardModeComboBox);
    addAndMakeVisible(*portamentoKeyboardModeComboBox);

    parameterSeparator9 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator9);

    parameterSeparator10 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator10);

    setSize(getWidth(), getHeight());
}

RampPortamentoPanel::~RampPortamentoPanel() = default;

void RampPortamentoPanel::paint(juce::Graphics& g)
{
    if (theme == nullptr)
    {
        return;
    }

    g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void RampPortamentoPanel::resized()
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

    if (rampPortamentoModuleName != nullptr)
    {
        rampPortamentoModuleName->setBounds(0, y, moduleNameWidth, moduleNameHeight);
    }

    if (rampPortamentoInitButton != nullptr)
    {
        rampPortamentoInitButton->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);
    }

    y += moduleNameHeight;

    if (ramp1RateLabel != nullptr)
    {
        ramp1RateLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (ramp1RateSlider != nullptr)
    {
        ramp1RateSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (ramp1TriggerLabel != nullptr)
    {
        ramp1TriggerLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (ramp1TriggerComboBox != nullptr)
    {
        ramp1TriggerComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (ramp2RateLabel != nullptr)
    {
        ramp2RateLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (ramp2RateSlider != nullptr)
    {
        ramp2RateSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (ramp2TriggerLabel != nullptr)
    {
        ramp2TriggerLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (ramp2TriggerComboBox != nullptr)
    {
        ramp2TriggerComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (portamentoRateLabel != nullptr)
    {
        portamentoRateLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (portamentoRateSlider != nullptr)
    {
        portamentoRateSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (portamentoModByVelocityLabel != nullptr)
    {
        portamentoModByVelocityLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (portamentoModByVelocitySlider != nullptr)
    {
        portamentoModByVelocitySlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (portamentoModeLabel != nullptr)
    {
        portamentoModeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (portamentoModeComboBox != nullptr)
    {
        portamentoModeComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (portamentoLegatoLabel != nullptr)
    {
        portamentoLegatoLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (portamentoLegatoComboBox != nullptr)
    {
        portamentoLegatoComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (portamentoKeyboardModeLabel != nullptr)
    {
        portamentoKeyboardModeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (portamentoKeyboardModeComboBox != nullptr)
    {
        portamentoKeyboardModeComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
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

void RampPortamentoPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (rampPortamentoModuleName != nullptr)
    {
        rampPortamentoModuleName->setTheme(inTheme);
    }

    if (rampPortamentoInitButton != nullptr)
    {
        rampPortamentoInitButton->setTheme(inTheme);
    }

    if (ramp1RateLabel != nullptr)
    {
        ramp1RateLabel->setTheme(inTheme);
    }

    if (ramp1RateSlider != nullptr)
    {
        ramp1RateSlider->setTheme(inTheme);
    }

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setTheme(inTheme);
    }

    if (ramp1TriggerLabel != nullptr)
    {
        ramp1TriggerLabel->setTheme(inTheme);
    }

    if (ramp1TriggerComboBox != nullptr)
    {
        ramp1TriggerComboBox->setTheme(inTheme);
    }

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setTheme(inTheme);
    }

    if (ramp2RateLabel != nullptr)
    {
        ramp2RateLabel->setTheme(inTheme);
    }

    if (ramp2RateSlider != nullptr)
    {
        ramp2RateSlider->setTheme(inTheme);
    }

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setTheme(inTheme);
    }

    if (ramp2TriggerLabel != nullptr)
    {
        ramp2TriggerLabel->setTheme(inTheme);
    }

    if (ramp2TriggerComboBox != nullptr)
    {
        ramp2TriggerComboBox->setTheme(inTheme);
    }

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setTheme(inTheme);
    }

    if (portamentoRateLabel != nullptr)
    {
        portamentoRateLabel->setTheme(inTheme);
    }

    if (portamentoRateSlider != nullptr)
    {
        portamentoRateSlider->setTheme(inTheme);
    }

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setTheme(inTheme);
    }

    if (portamentoModByVelocityLabel != nullptr)
    {
        portamentoModByVelocityLabel->setTheme(inTheme);
    }

    if (portamentoModByVelocitySlider != nullptr)
    {
        portamentoModByVelocitySlider->setTheme(inTheme);
    }

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setTheme(inTheme);
    }

    if (portamentoModeLabel != nullptr)
    {
        portamentoModeLabel->setTheme(inTheme);
    }

    if (portamentoModeComboBox != nullptr)
    {
        portamentoModeComboBox->setTheme(inTheme);
    }

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setTheme(inTheme);
    }

    if (portamentoLegatoLabel != nullptr)
    {
        portamentoLegatoLabel->setTheme(inTheme);
    }

    if (portamentoLegatoComboBox != nullptr)
    {
        portamentoLegatoComboBox->setTheme(inTheme);
    }

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setTheme(inTheme);
    }

    if (portamentoKeyboardModeLabel != nullptr)
    {
        portamentoKeyboardModeLabel->setTheme(inTheme);
    }

    if (portamentoKeyboardModeComboBox != nullptr)
    {
        portamentoKeyboardModeComboBox->setTheme(inTheme);
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
