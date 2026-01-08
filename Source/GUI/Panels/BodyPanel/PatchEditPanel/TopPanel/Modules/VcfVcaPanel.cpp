#include "VcfVcaPanel.h"

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

VcfVcaPanel::VcfVcaPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    vcfVcaModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kVcfVca));
    addAndMakeVisible(*vcfVcaModuleHeader);

    // Standalone Widgets
    vcfVcaInitButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kVcfVcaInit, inTheme);
    vcfVcaInitButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kVcfVcaInit,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*vcfVcaInitButton);

    // Balance
    vcfBalanceLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVcfBalance));
    addAndMakeVisible(*vcfBalanceLabel);

    vcfBalanceSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kVcfBalance, inTheme);
    vcfBalanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVcfBalance,
        *vcfBalanceSlider);
    addAndMakeVisible(*vcfBalanceSlider);

    horizontalSeparator1 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator1);

    // Frequency
    vcfFrequencyLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVcfFequency));
    addAndMakeVisible(*vcfFrequencyLabel);

    vcfFrequencySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kVcfFequency, inTheme);
    vcfFrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVcfFequency,
        *vcfFrequencySlider);
    addAndMakeVisible(*vcfFrequencySlider);

    horizontalSeparator2 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator2);

    // Frequency Mod by ENV 1
    vcfFrequencyModByEnv1Label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVcfFrequencyModByEnv1));
    addAndMakeVisible(*vcfFrequencyModByEnv1Label);

    vcfFrequencyModByEnv1Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kVcfFrequencyModByEnv1, inTheme);
    vcfFrequencyModByEnv1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVcfFrequencyModByEnv1,
        *vcfFrequencyModByEnv1Slider);
    addAndMakeVisible(*vcfFrequencyModByEnv1Slider);

    horizontalSeparator3 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator3);

    // Frequency Mod by Pressure
    vcfFrequencyModByPressureLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVcfFrequencyModByPressure));
    addAndMakeVisible(*vcfFrequencyModByPressureLabel);

    vcfFrequencyModByPressureSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kVcfFrequencyModByPressure, inTheme);
    vcfFrequencyModByPressureAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVcfFrequencyModByPressure,
        *vcfFrequencyModByPressureSlider);
    addAndMakeVisible(*vcfFrequencyModByPressureSlider);

    horizontalSeparator4 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator4);

    // Resonance
    vcfResonanceLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVcfResonance));
    addAndMakeVisible(*vcfResonanceLabel);

    vcfResonanceSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kVcfResonance, inTheme);
    vcfResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVcfResonance,
        *vcfResonanceSlider);
    addAndMakeVisible(*vcfResonanceSlider);

    horizontalSeparator5 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator5);

    // VCA 1 Volume
    vca1VolumeLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVca1Volume));
    addAndMakeVisible(*vca1VolumeLabel);

    vca1VolumeSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kVca1Volume, inTheme);
    vca1VolumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVca1Volume,
        *vca1VolumeSlider);
    addAndMakeVisible(*vca1VolumeSlider);

    horizontalSeparator6 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator6);

    // VCA 1 Mod by Velocity
    vca1ModByVelocityLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVca1ModByVelocity));
    addAndMakeVisible(*vca1ModByVelocityLabel);

    vca1ModByVelocitySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kVca1ModByVelocity, inTheme);
    vca1ModByVelocityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVca1ModByVelocity,
        *vca1ModByVelocitySlider);
    addAndMakeVisible(*vca1ModByVelocitySlider);

    horizontalSeparator7 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator7);

    // VCA 2 Mod by ENV 2
    vca2ModByEnv2Label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVca2ModByEnv2));
    addAndMakeVisible(*vca2ModByEnv2Label);

    vca2ModByEnv2Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kVca2ModByEnv2, inTheme);
    vca2ModByEnv2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVca2ModByEnv2,
        *vca2ModByEnv2Slider);
    addAndMakeVisible(*vca2ModByEnv2Slider);

    horizontalSeparator8 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator8);

    // Levers
    vcfLeversLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVcfLevers));
    addAndMakeVisible(*vcfLeversLabel);

    vcfLeversComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kVcfLevers, inTheme);
    vcfLeversAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVcfLevers,
        *vcfLeversComboBox);
    addAndMakeVisible(*vcfLeversComboBox);

    horizontalSeparator9 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator9);

    // Keyboard Portamento
    vcfKeyboardPortamentoLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kVcfKeyboardPortamento));
    addAndMakeVisible(*vcfKeyboardPortamentoLabel);

    vcfKeyboardPortamentoComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kVcfKeyboardPortamento, inTheme);
    vcfKeyboardPortamentoAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kVcfKeyboardPortamento,
        *vcfKeyboardPortamentoComboBox);
    addAndMakeVisible(*vcfKeyboardPortamentoComboBox);

    horizontalSeparator10 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator10);

    setSize(getWidth(), getHeight());
}

VcfVcaPanel::~VcfVcaPanel() = default;

void VcfVcaPanel::paint(juce::Graphics& g)
{
        g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void VcfVcaPanel::resized()
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

    if (auto* header = vcfVcaModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    if (auto* button = vcfVcaInitButton.get())
        button->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);

    y += moduleHeaderHeight;

    if (auto* label = vcfBalanceLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vcfBalanceSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vcfFrequencyLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vcfFrequencySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vcfFrequencyModByEnv1Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vcfFrequencyModByEnv1Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vcfFrequencyModByPressureLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vcfFrequencyModByPressureSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vcfResonanceLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vcfResonanceSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vca1VolumeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vca1VolumeSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vca1ModByVelocityLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vca1ModByVelocitySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vca2ModByEnv2Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vca2ModByEnv2Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator8.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vcfLeversLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = vcfLeversComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator9.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vcfKeyboardPortamentoLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = vcfKeyboardPortamentoComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator10.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void VcfVcaPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = vcfVcaModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* button = vcfVcaInitButton.get())
        button->setTheme(inTheme);

    if (auto* label = vcfBalanceLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = vcfBalanceSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = vcfFrequencyLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = vcfFrequencySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = vcfFrequencyModByEnv1Label.get())
        label->setTheme(inTheme);

    if (auto* slider = vcfFrequencyModByEnv1Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = vcfFrequencyModByPressureLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = vcfFrequencyModByPressureSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = vcfResonanceLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = vcfResonanceSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = vca1VolumeLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = vca1VolumeSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = vca1ModByVelocityLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = vca1ModByVelocitySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator7.get())
        separator->setTheme(inTheme);

    if (auto* label = vca2ModByEnv2Label.get())
        label->setTheme(inTheme);

    if (auto* slider = vca2ModByEnv2Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator8.get())
        separator->setTheme(inTheme);

    if (auto* label = vcfLeversLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = vcfLeversComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator9.get())
        separator->setTheme(inTheme);

    if (auto* label = vcfKeyboardPortamentoLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = vcfKeyboardPortamentoComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator10.get())
        separator->setTheme(inTheme);

    repaint();
}
