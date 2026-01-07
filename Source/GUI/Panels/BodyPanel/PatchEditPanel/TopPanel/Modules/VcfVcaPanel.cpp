#include "VcfVcaPanel.h"

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

VcfVcaPanel::VcfVcaPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    vcfVcaModuleName = std::make_unique<tss::ModuleName>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kVcfVca));
    addAndMakeVisible(*vcfVcaModuleName);

    // Standalone Widgets
    vcfVcaInitButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kVcfVcaInit, inTheme);
    vcfVcaInitButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kVcfVcaInit,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*vcfVcaInitButton);

    // Balance
    vcfBalanceLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVcfBalance));
    addAndMakeVisible(*vcfBalanceLabel);

    vcfBalanceSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kVcfBalance, inTheme);
    vcfBalanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVcfBalance,
        *vcfBalanceSlider);
    addAndMakeVisible(*vcfBalanceSlider);

    parameterSeparator1 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator1);

    // Frequency
    vcfFrequencyLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVcfFequency));
    addAndMakeVisible(*vcfFrequencyLabel);

    vcfFrequencySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kVcfFequency, inTheme);
    vcfFrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVcfFequency,
        *vcfFrequencySlider);
    addAndMakeVisible(*vcfFrequencySlider);

    parameterSeparator2 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator2);

    // Frequency Mod by ENV 1
    vcfFrequencyModByEnv1Label = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVcfFrequencyModByEnv1));
    addAndMakeVisible(*vcfFrequencyModByEnv1Label);

    vcfFrequencyModByEnv1Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kVcfFrequencyModByEnv1, inTheme);
    vcfFrequencyModByEnv1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVcfFrequencyModByEnv1,
        *vcfFrequencyModByEnv1Slider);
    addAndMakeVisible(*vcfFrequencyModByEnv1Slider);

    parameterSeparator3 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator3);

    // Frequency Mod by Pressure
    vcfFrequencyModByPressureLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVcfFrequencyModByPressure));
    addAndMakeVisible(*vcfFrequencyModByPressureLabel);

    vcfFrequencyModByPressureSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kVcfFrequencyModByPressure, inTheme);
    vcfFrequencyModByPressureAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVcfFrequencyModByPressure,
        *vcfFrequencyModByPressureSlider);
    addAndMakeVisible(*vcfFrequencyModByPressureSlider);

    parameterSeparator4 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator4);

    // Resonance
    vcfResonanceLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVcfResonance));
    addAndMakeVisible(*vcfResonanceLabel);

    vcfResonanceSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kVcfResonance, inTheme);
    vcfResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVcfResonance,
        *vcfResonanceSlider);
    addAndMakeVisible(*vcfResonanceSlider);

    parameterSeparator5 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator5);

    // VCA 1 Volume
    vca1VolumeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVca1Volume));
    addAndMakeVisible(*vca1VolumeLabel);

    vca1VolumeSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kVca1Volume, inTheme);
    vca1VolumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVca1Volume,
        *vca1VolumeSlider);
    addAndMakeVisible(*vca1VolumeSlider);

    parameterSeparator6 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator6);

    // VCA 1 Mod by Velocity
    vca1ModByVelocityLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVca1ModByVelocity));
    addAndMakeVisible(*vca1ModByVelocityLabel);

    vca1ModByVelocitySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kVca1ModByVelocity, inTheme);
    vca1ModByVelocityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVca1ModByVelocity,
        *vca1ModByVelocitySlider);
    addAndMakeVisible(*vca1ModByVelocitySlider);

    parameterSeparator7 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator7);

    // VCA 2 Mod by ENV 2
    vca2ModByEnv2Label = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVca2ModByEnv2));
    addAndMakeVisible(*vca2ModByEnv2Label);

    vca2ModByEnv2Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kVca2ModByEnv2, inTheme);
    vca2ModByEnv2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVca2ModByEnv2,
        *vca2ModByEnv2Slider);
    addAndMakeVisible(*vca2ModByEnv2Slider);

    parameterSeparator8 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator8);

    // Levers
    vcfLeversLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVcfLevers));
    addAndMakeVisible(*vcfLeversLabel);

    vcfLeversComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kVcfLevers, inTheme);
    vcfLeversAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVcfLevers,
        *vcfLeversComboBox);
    addAndMakeVisible(*vcfLeversComboBox);

    parameterSeparator9 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator9);

    // Keyboard Portamento
    vcfKeyboardPortamentoLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVcfKeyboardPortamento));
    addAndMakeVisible(*vcfKeyboardPortamentoLabel);

    vcfKeyboardPortamentoComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kVcfKeyboardPortamento, inTheme);
    vcfKeyboardPortamentoAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVcfKeyboardPortamento,
        *vcfKeyboardPortamentoComboBox);
    addAndMakeVisible(*vcfKeyboardPortamentoComboBox);

    parameterSeparator10 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator10);

    setSize(getWidth(), getHeight());
}

VcfVcaPanel::~VcfVcaPanel() = default;

void VcfVcaPanel::paint(juce::Graphics& g)
{
    if (theme == nullptr)
    {
        return;
    }

    g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void VcfVcaPanel::resized()
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

    if (vcfVcaModuleName != nullptr)
    {
        vcfVcaModuleName->setBounds(0, y, moduleNameWidth, moduleNameHeight);
    }

    if (vcfVcaInitButton != nullptr)
    {
        vcfVcaInitButton->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);
    }

    y += moduleNameHeight;

    if (vcfBalanceLabel != nullptr)
    {
        vcfBalanceLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (vcfBalanceSlider != nullptr)
    {
        vcfBalanceSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (vcfFrequencyLabel != nullptr)
    {
        vcfFrequencyLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (vcfFrequencySlider != nullptr)
    {
        vcfFrequencySlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (vcfFrequencyModByEnv1Label != nullptr)
    {
        vcfFrequencyModByEnv1Label->setBounds(0, y, labelWidth, labelHeight);
    }

    if (vcfFrequencyModByEnv1Slider != nullptr)
    {
        vcfFrequencyModByEnv1Slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (vcfFrequencyModByPressureLabel != nullptr)
    {
        vcfFrequencyModByPressureLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (vcfFrequencyModByPressureSlider != nullptr)
    {
        vcfFrequencyModByPressureSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (vcfResonanceLabel != nullptr)
    {
        vcfResonanceLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (vcfResonanceSlider != nullptr)
    {
        vcfResonanceSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (vca1VolumeLabel != nullptr)
    {
        vca1VolumeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (vca1VolumeSlider != nullptr)
    {
        vca1VolumeSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (vca1ModByVelocityLabel != nullptr)
    {
        vca1ModByVelocityLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (vca1ModByVelocitySlider != nullptr)
    {
        vca1ModByVelocitySlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (vca2ModByEnv2Label != nullptr)
    {
        vca2ModByEnv2Label->setBounds(0, y, labelWidth, labelHeight);
    }

    if (vca2ModByEnv2Slider != nullptr)
    {
        vca2ModByEnv2Slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (vcfLeversLabel != nullptr)
    {
        vcfLeversLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (vcfLeversComboBox != nullptr)
    {
        vcfLeversComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator9 != nullptr)
    {
        parameterSeparator9->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (vcfKeyboardPortamentoLabel != nullptr)
    {
        vcfKeyboardPortamentoLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (vcfKeyboardPortamentoComboBox != nullptr)
    {
        vcfKeyboardPortamentoComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator10 != nullptr)
    {
        parameterSeparator10->setBounds(0, y, separatorWidth, separatorHeight);
    }
}

void VcfVcaPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (vcfVcaModuleName != nullptr)
    {
        vcfVcaModuleName->setTheme(inTheme);
    }

    if (vcfVcaInitButton != nullptr)
    {
        vcfVcaInitButton->setTheme(inTheme);
    }

    if (vcfBalanceLabel != nullptr)
    {
        vcfBalanceLabel->setTheme(inTheme);
    }

    if (vcfBalanceSlider != nullptr)
    {
        vcfBalanceSlider->setTheme(inTheme);
    }

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setTheme(inTheme);
    }

    if (vcfFrequencyLabel != nullptr)
    {
        vcfFrequencyLabel->setTheme(inTheme);
    }

    if (vcfFrequencySlider != nullptr)
    {
        vcfFrequencySlider->setTheme(inTheme);
    }

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setTheme(inTheme);
    }

    if (vcfFrequencyModByEnv1Label != nullptr)
    {
        vcfFrequencyModByEnv1Label->setTheme(inTheme);
    }

    if (vcfFrequencyModByEnv1Slider != nullptr)
    {
        vcfFrequencyModByEnv1Slider->setTheme(inTheme);
    }

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setTheme(inTheme);
    }

    if (vcfFrequencyModByPressureLabel != nullptr)
    {
        vcfFrequencyModByPressureLabel->setTheme(inTheme);
    }

    if (vcfFrequencyModByPressureSlider != nullptr)
    {
        vcfFrequencyModByPressureSlider->setTheme(inTheme);
    }

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setTheme(inTheme);
    }

    if (vcfResonanceLabel != nullptr)
    {
        vcfResonanceLabel->setTheme(inTheme);
    }

    if (vcfResonanceSlider != nullptr)
    {
        vcfResonanceSlider->setTheme(inTheme);
    }

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setTheme(inTheme);
    }

    if (vca1VolumeLabel != nullptr)
    {
        vca1VolumeLabel->setTheme(inTheme);
    }

    if (vca1VolumeSlider != nullptr)
    {
        vca1VolumeSlider->setTheme(inTheme);
    }

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setTheme(inTheme);
    }

    if (vca1ModByVelocityLabel != nullptr)
    {
        vca1ModByVelocityLabel->setTheme(inTheme);
    }

    if (vca1ModByVelocitySlider != nullptr)
    {
        vca1ModByVelocitySlider->setTheme(inTheme);
    }

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setTheme(inTheme);
    }

    if (vca2ModByEnv2Label != nullptr)
    {
        vca2ModByEnv2Label->setTheme(inTheme);
    }

    if (vca2ModByEnv2Slider != nullptr)
    {
        vca2ModByEnv2Slider->setTheme(inTheme);
    }

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setTheme(inTheme);
    }

    if (vcfLeversLabel != nullptr)
    {
        vcfLeversLabel->setTheme(inTheme);
    }

    if (vcfLeversComboBox != nullptr)
    {
        vcfLeversComboBox->setTheme(inTheme);
    }

    if (parameterSeparator9 != nullptr)
    {
        parameterSeparator9->setTheme(inTheme);
    }

    if (vcfKeyboardPortamentoLabel != nullptr)
    {
        vcfKeyboardPortamentoLabel->setTheme(inTheme);
    }

    if (vcfKeyboardPortamentoComboBox != nullptr)
    {
        vcfKeyboardPortamentoComboBox->setTheme(inTheme);
    }

    if (parameterSeparator10 != nullptr)
    {
        parameterSeparator10->setTheme(inTheme);
    }

    repaint();
}
