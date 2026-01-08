#include "Dco1Panel.h"

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

Dco1Panel::Dco1Panel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    dco1ModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kDco1));
    addAndMakeVisible(*dco1ModuleHeader);

    // Standalone Widgets
    dco1InitButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kDco1Init, inTheme);
    dco1InitButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kDco1Init,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*dco1InitButton);

    dco1CopyButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kDco1Copy, inTheme);
    dco1CopyButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kDco1Copy,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*dco1CopyButton);

    dco1PasteButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kDco1Paste, inTheme);
    dco1PasteButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kDco1Paste,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*dco1PasteButton);

    // Frequency
    dco1FrequencyLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1Frequency));
    addAndMakeVisible(*dco1FrequencyLabel);

    dco1FrequencySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco1Frequency, inTheme);
    dco1FrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1Frequency,
        *dco1FrequencySlider);
    addAndMakeVisible(*dco1FrequencySlider);

    horizontalSeparator1 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator1);

    // Frequency Mod by LFO 1
    dco1FrequencyModByLfo1Label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1FrequencyModByLfo1));
    addAndMakeVisible(*dco1FrequencyModByLfo1Label);

    dco1FrequencyModByLfo1Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco1FrequencyModByLfo1, inTheme);
    dco1FrequencyModByLfo1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1FrequencyModByLfo1,
        *dco1FrequencyModByLfo1Slider);
    addAndMakeVisible(*dco1FrequencyModByLfo1Slider);

    horizontalSeparator2 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator2);

    // Sync
    dco1SyncLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1Sync));
    addAndMakeVisible(*dco1SyncLabel);

    dco1SyncComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco1Sync, inTheme);
    dco1SyncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1Sync,
        *dco1SyncComboBox);
    addAndMakeVisible(*dco1SyncComboBox);

    horizontalSeparator3 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator3);

    // Pulse Width
    dco1PulseWidthLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1PulseWidth));
    addAndMakeVisible(*dco1PulseWidthLabel);

    dco1PulseWidthSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco1PulseWidth, inTheme);
    dco1PulseWidthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1PulseWidth,
        *dco1PulseWidthSlider);
    addAndMakeVisible(*dco1PulseWidthSlider);

    horizontalSeparator4 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator4);

    // Pulse Width Mod by LFO 2
    dco1PulseWidthModByLfo2Label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1PulseWidthModByLfo2));
    addAndMakeVisible(*dco1PulseWidthModByLfo2Label);

    dco1PulseWidthModByLfo2Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco1PulseWidthModByLfo2, inTheme);
    dco1PulseWidthModByLfo2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1PulseWidthModByLfo2,
        *dco1PulseWidthModByLfo2Slider);
    addAndMakeVisible(*dco1PulseWidthModByLfo2Slider);

    horizontalSeparator5 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator5);

    // Wave Shape
    dco1WaveShapeLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1WaveShape));
    addAndMakeVisible(*dco1WaveShapeLabel);

    dco1WaveShapeSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco1WaveShape, inTheme);
    dco1WaveShapeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1WaveShape,
        *dco1WaveShapeSlider);
    addAndMakeVisible(*dco1WaveShapeSlider);

    horizontalSeparator6 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator6);

    // Wave Select
    dco1WaveSelectLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1WaveSelect));
    addAndMakeVisible(*dco1WaveSelectLabel);

    dco1WaveSelectComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco1WaveSelect, inTheme);
    dco1WaveSelectAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1WaveSelect,
        *dco1WaveSelectComboBox);
    addAndMakeVisible(*dco1WaveSelectComboBox);

    horizontalSeparator7 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator7);

    // Levers
    dco1LeversLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1Levers));
    addAndMakeVisible(*dco1LeversLabel);

    dco1LeversComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco1Levers, inTheme);
    dco1LeversAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1Levers,
        *dco1LeversComboBox);
    addAndMakeVisible(*dco1LeversComboBox);

    horizontalSeparator8 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator8);

    // Keyboard Portamento
    dco1KeyboardPortamentoLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1KeyboardPortamento));
    addAndMakeVisible(*dco1KeyboardPortamentoLabel);

    dco1KeyboardPortamentoComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco1KeyboardPortamento, inTheme);
    dco1KeyboardPortamentoAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1KeyboardPortamento,
        *dco1KeyboardPortamentoComboBox);
    addAndMakeVisible(*dco1KeyboardPortamentoComboBox);

    horizontalSeparator9 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator9);

    // Key Click
    dco1KeyClickLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1KeyClick));
    addAndMakeVisible(*dco1KeyClickLabel);

    dco1KeyClickComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco1KeyClick, inTheme);
    dco1KeyClickAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1KeyClick,
        *dco1KeyClickComboBox);
    addAndMakeVisible(*dco1KeyClickComboBox);

    horizontalSeparator10 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator10);

    setSize(getWidth(), getHeight());
}

Dco1Panel::~Dco1Panel() = default;

void Dco1Panel::paint(juce::Graphics& g)
{
        g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void Dco1Panel::resized()
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

    if (auto* header = dco1ModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    if (auto* button = dco1PasteButton.get())
        button->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);

    if (auto* button = dco1CopyButton.get())
        button->setBounds(panelWidth - buttonWidth * 2, y, buttonWidth, buttonHeight);

    if (auto* button = dco1InitButton.get())
        button->setBounds(panelWidth - buttonWidth * 3, y, buttonWidth, buttonHeight);

    y += moduleHeaderHeight;

    if (auto* label = dco1FrequencyLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco1FrequencySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1FrequencyModByLfo1Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco1FrequencyModByLfo1Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1SyncLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco1SyncComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1PulseWidthLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco1PulseWidthSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1PulseWidthModByLfo2Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco1PulseWidthModByLfo2Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1WaveShapeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco1WaveShapeSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1WaveSelectLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco1WaveSelectComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1LeversLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco1LeversComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator8.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1KeyboardPortamentoLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco1KeyboardPortamentoComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator9.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1KeyClickLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco1KeyClickComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator10.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void Dco1Panel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = dco1ModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* button = dco1InitButton.get())
        button->setTheme(inTheme);

    if (auto* button = dco1CopyButton.get())
        button->setTheme(inTheme);

    if (auto* button = dco1PasteButton.get())
        button->setTheme(inTheme);

    if (auto* label = dco1FrequencyLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = dco1FrequencySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1FrequencyModByLfo1Label.get())
        label->setTheme(inTheme);

    if (auto* slider = dco1FrequencyModByLfo1Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1SyncLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco1SyncComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1PulseWidthLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = dco1PulseWidthSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1PulseWidthModByLfo2Label.get())
        label->setTheme(inTheme);

    if (auto* slider = dco1PulseWidthModByLfo2Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1WaveShapeLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = dco1WaveShapeSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1WaveSelectLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco1WaveSelectComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator7.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1LeversLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco1LeversComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator8.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1KeyboardPortamentoLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco1KeyboardPortamentoComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator9.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1KeyClickLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco1KeyClickComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator10.get())
        separator->setTheme(inTheme);

    repaint();
}

