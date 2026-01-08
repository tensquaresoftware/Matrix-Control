#include "Dco2Panel.h"

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

Dco2Panel::Dco2Panel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    dco2ModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kDco2));
    addAndMakeVisible(*dco2ModuleHeader);

    // Standalone Widgets
    dco2InitButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kDco2Init, inTheme);
    dco2InitButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kDco2Init,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*dco2InitButton);

    dco2CopyButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kDco2Copy, inTheme);
    dco2CopyButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kDco2Copy,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*dco2CopyButton);

    dco2PasteButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kDco2Paste, inTheme);
    dco2PasteButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kDco2Paste,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*dco2PasteButton);

    // Frequency
    dco2FrequencyLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2Frequency));
    addAndMakeVisible(*dco2FrequencyLabel);

    dco2FrequencySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco2Frequency, inTheme);
    dco2FrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2Frequency,
        *dco2FrequencySlider);
    addAndMakeVisible(*dco2FrequencySlider);

    horizontalSeparator1 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator1);

    // Frequency Mod by LFO 1
    dco2FrequencyModByLfo1Label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2FrequencyModByLfo1));
    addAndMakeVisible(*dco2FrequencyModByLfo1Label);

    dco2FrequencyModByLfo1Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco2FrequencyModByLfo1, inTheme);
    dco2FrequencyModByLfo1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2FrequencyModByLfo1,
        *dco2FrequencyModByLfo1Slider);
    addAndMakeVisible(*dco2FrequencyModByLfo1Slider);

    horizontalSeparator2 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator2);

    // Detune
    dco2DetuneLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2Detune));
    addAndMakeVisible(*dco2DetuneLabel);

    dco2DetuneSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco2Detune, inTheme);
    dco2DetuneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2Detune,
        *dco2DetuneSlider);
    addAndMakeVisible(*dco2DetuneSlider);

    horizontalSeparator3 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator3);

    // Pulse Width
    dco2PulseWidthLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2PulseWidth));
    addAndMakeVisible(*dco2PulseWidthLabel);

    dco2PulseWidthSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco2PulseWidth, inTheme);
    dco2PulseWidthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2PulseWidth,
        *dco2PulseWidthSlider);
    addAndMakeVisible(*dco2PulseWidthSlider);

    horizontalSeparator4 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator4);

    // Pulse Width Mod by LFO 2
    dco2PulseWidthModByLfo2Label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2PulseWidthModByLfo2));
    addAndMakeVisible(*dco2PulseWidthModByLfo2Label);

    dco2PulseWidthModByLfo2Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco2PulseWidthModByLfo2, inTheme);
    dco2PulseWidthModByLfo2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2PulseWidthModByLfo2,
        *dco2PulseWidthModByLfo2Slider);
    addAndMakeVisible(*dco2PulseWidthModByLfo2Slider);

    horizontalSeparator5 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator5);

    // Wave Shape
    dco2WaveShapeLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2WaveShape));
    addAndMakeVisible(*dco2WaveShapeLabel);

    dco2WaveShapeSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco2WaveShape, inTheme);
    dco2WaveShapeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2WaveShape,
        *dco2WaveShapeSlider);
    addAndMakeVisible(*dco2WaveShapeSlider);

    horizontalSeparator6 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator6);

    // Wave Select
    dco2WaveSelectLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2WaveSelect));
    addAndMakeVisible(*dco2WaveSelectLabel);

    dco2WaveSelectComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco2WaveSelect, inTheme);
    dco2WaveSelectAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2WaveSelect,
        *dco2WaveSelectComboBox);
    addAndMakeVisible(*dco2WaveSelectComboBox);

    horizontalSeparator7 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator7);

    // Levers
    dco2LeversLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2Levers));
    addAndMakeVisible(*dco2LeversLabel);

    dco2LeversComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco2Levers, inTheme);
    dco2LeversAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2Levers,
        *dco2LeversComboBox);
    addAndMakeVisible(*dco2LeversComboBox);

    horizontalSeparator8 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator8);

    // Keyboard Portamento
    dco2KeyboardPortamentoLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2KeyboardPortamento));
    addAndMakeVisible(*dco2KeyboardPortamentoLabel);

    dco2KeyboardPortamentoComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco2KeyboardPortamento, inTheme);
    dco2KeyboardPortamentoAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2KeyboardPortamento,
        *dco2KeyboardPortamentoComboBox);
    addAndMakeVisible(*dco2KeyboardPortamentoComboBox);

    horizontalSeparator9 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator9);

    // Key Click
    dco2KeyClickLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2KeyClick));
    addAndMakeVisible(*dco2KeyClickLabel);

    dco2KeyClickComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco2KeyClick, inTheme);
    dco2KeyClickAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2KeyClick,
        *dco2KeyClickComboBox);
    addAndMakeVisible(*dco2KeyClickComboBox);

    horizontalSeparator10 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator10);

    setSize(getWidth(), getHeight());
}

Dco2Panel::~Dco2Panel() = default;

void Dco2Panel::paint(juce::Graphics& g)
{
        g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void Dco2Panel::resized()
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

    if (auto* header = dco2ModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    if (auto* button = dco2PasteButton.get())
        button->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);

    if (auto* button = dco2CopyButton.get())
        button->setBounds(panelWidth - buttonWidth * 2, y, buttonWidth, buttonHeight);

    if (auto* button = dco2InitButton.get())
        button->setBounds(panelWidth - buttonWidth * 3, y, buttonWidth, buttonHeight);

    y += moduleHeaderHeight;

    if (auto* label = dco2FrequencyLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco2FrequencySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2FrequencyModByLfo1Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco2FrequencyModByLfo1Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2DetuneLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco2DetuneSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2PulseWidthLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco2PulseWidthSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2PulseWidthModByLfo2Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco2PulseWidthModByLfo2Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2WaveShapeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco2WaveShapeSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2WaveSelectLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco2WaveSelectComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2LeversLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco2LeversComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator8.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2KeyboardPortamentoLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco2KeyboardPortamentoComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator9.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2KeyClickLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco2KeyClickComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator10.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void Dco2Panel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = dco2ModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* button = dco2InitButton.get())
        button->setTheme(inTheme);

    if (auto* button = dco2CopyButton.get())
        button->setTheme(inTheme);

    if (auto* button = dco2PasteButton.get())
        button->setTheme(inTheme);

    if (auto* label = dco2FrequencyLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = dco2FrequencySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2FrequencyModByLfo1Label.get())
        label->setTheme(inTheme);

    if (auto* slider = dco2FrequencyModByLfo1Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2DetuneLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = dco2DetuneSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2PulseWidthLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = dco2PulseWidthSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2PulseWidthModByLfo2Label.get())
        label->setTheme(inTheme);

    if (auto* slider = dco2PulseWidthModByLfo2Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2WaveShapeLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = dco2WaveShapeSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2WaveSelectLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco2WaveSelectComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator7.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2LeversLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco2LeversComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator8.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2KeyboardPortamentoLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco2KeyboardPortamentoComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator9.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2KeyClickLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco2KeyClickComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator10.get())
        separator->setTheme(inTheme);

    repaint();
}
