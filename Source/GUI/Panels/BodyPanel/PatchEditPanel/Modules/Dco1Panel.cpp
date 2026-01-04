#include "Dco1Panel.h"

#include "../../../../Themes/Theme.h"
#include "../../../../Widgets/ModuleName.h"
#include "../../../../Widgets/ParameterLabel.h"
#include "../../../../Widgets/Slider.h"
#include "../../../../Widgets/ComboBox.h"
#include "../../../../Widgets/Button.h"
#include "../../../../Widgets/ParameterSeparator.h"
#include "../../../../../Shared/SynthDescriptors.h"
#include "../../../../../GUI/Factories/WidgetFactory.h"

using tss::Theme;

Dco1Panel::Dco1Panel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    dco1ModuleName = std::make_unique<tss::ModuleName>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kDco1));
    addAndMakeVisible(*dco1ModuleName);

    // Standalone Widgets
    dco1InitButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kDco1Init, inTheme);
    dco1InitButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kDco1Init,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*dco1InitButton);

    dco1CopyButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kDco1Copy, inTheme);
    dco1CopyButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kDco1Copy,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*dco1CopyButton);

    dco1PasteButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kDco1Paste, inTheme);
    dco1PasteButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kDco1Paste,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*dco1PasteButton);

    // Frequency
    dco1FrequencyLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1Frequency));
    addAndMakeVisible(*dco1FrequencyLabel);

    dco1FrequencySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco1Frequency, inTheme);
    dco1FrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1Frequency,
        *dco1FrequencySlider);
    addAndMakeVisible(*dco1FrequencySlider);

    parameterSeparator1 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator1);

    // Frequency Mod by LFO 1
    dco1FrequencyModByLfo1Label = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1FrequencyModByLfo1));
    addAndMakeVisible(*dco1FrequencyModByLfo1Label);

    dco1FrequencyModByLfo1Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco1FrequencyModByLfo1, inTheme);
    dco1FrequencyModByLfo1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1FrequencyModByLfo1,
        *dco1FrequencyModByLfo1Slider);
    addAndMakeVisible(*dco1FrequencyModByLfo1Slider);

    parameterSeparator2 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator2);

    // Sync
    dco1SyncLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1Sync));
    addAndMakeVisible(*dco1SyncLabel);

    dco1SyncComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco1Sync, inTheme);
    dco1SyncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1Sync,
        *dco1SyncComboBox);
    addAndMakeVisible(*dco1SyncComboBox);

    parameterSeparator3 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator3);

    // Pulse Width
    dco1PulseWidthLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1PulseWidth));
    addAndMakeVisible(*dco1PulseWidthLabel);

    dco1PulseWidthSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco1PulseWidth, inTheme);
    dco1PulseWidthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1PulseWidth,
        *dco1PulseWidthSlider);
    addAndMakeVisible(*dco1PulseWidthSlider);

    parameterSeparator4 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator4);

    // Pulse Width Mod by LFO 2
    dco1PulseWidthModByLfo2Label = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1PulseWidthModByLfo2));
    addAndMakeVisible(*dco1PulseWidthModByLfo2Label);

    dco1PulseWidthModByLfo2Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco1PulseWidthModByLfo2, inTheme);
    dco1PulseWidthModByLfo2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1PulseWidthModByLfo2,
        *dco1PulseWidthModByLfo2Slider);
    addAndMakeVisible(*dco1PulseWidthModByLfo2Slider);

    parameterSeparator5 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator5);

    // Wave Shape
    dco1WaveShapeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1WaveShape));
    addAndMakeVisible(*dco1WaveShapeLabel);

    dco1WaveShapeSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco1WaveShape, inTheme);
    dco1WaveShapeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1WaveShape,
        *dco1WaveShapeSlider);
    addAndMakeVisible(*dco1WaveShapeSlider);

    parameterSeparator6 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator6);

    // Wave Select
    dco1WaveSelectLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1WaveSelect));
    addAndMakeVisible(*dco1WaveSelectLabel);

    dco1WaveSelectComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco1WaveSelect, inTheme);
    dco1WaveSelectAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1WaveSelect,
        *dco1WaveSelectComboBox);
    addAndMakeVisible(*dco1WaveSelectComboBox);

    parameterSeparator7 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator7);

    // Levers
    dco1LeversLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1Levers));
    addAndMakeVisible(*dco1LeversLabel);

    dco1LeversComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco1Levers, inTheme);
    dco1LeversAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1Levers,
        *dco1LeversComboBox);
    addAndMakeVisible(*dco1LeversComboBox);

    parameterSeparator8 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator8);

    // Keyboard Portamento
    dco1KeyboardPortamentoLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1KeyboardPortamento));
    addAndMakeVisible(*dco1KeyboardPortamentoLabel);

    dco1KeyboardPortamentoComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco1KeyboardPortamento, inTheme);
    dco1KeyboardPortamentoAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1KeyboardPortamento,
        *dco1KeyboardPortamentoComboBox);
    addAndMakeVisible(*dco1KeyboardPortamentoComboBox);

    parameterSeparator9 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator9);

    // Key Click
    dco1KeyClickLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1KeyClick));
    addAndMakeVisible(*dco1KeyClickLabel);

    dco1KeyClickComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco1KeyClick, inTheme);
    dco1KeyClickAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco1KeyClick,
        *dco1KeyClickComboBox);
    addAndMakeVisible(*dco1KeyClickComboBox);

    parameterSeparator10 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator10);

    setSize(getWidth(), getHeight());
}

Dco1Panel::~Dco1Panel() = default;

void Dco1Panel::paint(juce::Graphics& g)
{
    if (theme == nullptr)
    {
        return;
    }

    g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void Dco1Panel::resized()
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

    if (dco1ModuleName != nullptr)
    {
        dco1ModuleName->setBounds(0, y, moduleNameWidth, moduleNameHeight);
    }

    if (dco1PasteButton != nullptr)
    {
        dco1PasteButton->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);
    }

    if (dco1CopyButton != nullptr)
    {
        dco1CopyButton->setBounds(panelWidth - buttonWidth * 2, y, buttonWidth, buttonHeight);
    }

    if (dco1InitButton != nullptr)
    {
        dco1InitButton->setBounds(panelWidth - buttonWidth * 3, y, buttonWidth, buttonHeight);
    }

    y += moduleNameHeight;

    if (dco1FrequencyLabel != nullptr)
    {
        dco1FrequencyLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco1FrequencySlider != nullptr)
    {
        dco1FrequencySlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco1FrequencyModByLfo1Label != nullptr)
    {
        dco1FrequencyModByLfo1Label->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco1FrequencyModByLfo1Slider != nullptr)
    {
        dco1FrequencyModByLfo1Slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco1SyncLabel != nullptr)
    {
        dco1SyncLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco1SyncComboBox != nullptr)
    {
        dco1SyncComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco1PulseWidthLabel != nullptr)
    {
        dco1PulseWidthLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco1PulseWidthSlider != nullptr)
    {
        dco1PulseWidthSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco1PulseWidthModByLfo2Label != nullptr)
    {
        dco1PulseWidthModByLfo2Label->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco1PulseWidthModByLfo2Slider != nullptr)
    {
        dco1PulseWidthModByLfo2Slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco1WaveShapeLabel != nullptr)
    {
        dco1WaveShapeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco1WaveShapeSlider != nullptr)
    {
        dco1WaveShapeSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco1WaveSelectLabel != nullptr)
    {
        dco1WaveSelectLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco1WaveSelectComboBox != nullptr)
    {
        dco1WaveSelectComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco1LeversLabel != nullptr)
    {
        dco1LeversLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco1LeversComboBox != nullptr)
    {
        dco1LeversComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco1KeyboardPortamentoLabel != nullptr)
    {
        dco1KeyboardPortamentoLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco1KeyboardPortamentoComboBox != nullptr)
    {
        dco1KeyboardPortamentoComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator9 != nullptr)
    {
        parameterSeparator9->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco1KeyClickLabel != nullptr)
    {
        dco1KeyClickLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco1KeyClickComboBox != nullptr)
    {
        dco1KeyClickComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator10 != nullptr)
    {
        parameterSeparator10->setBounds(0, y, separatorWidth, separatorHeight);
    }
}

void Dco1Panel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (dco1ModuleName != nullptr)
    {
        dco1ModuleName->setTheme(inTheme);
    }

    if (dco1InitButton != nullptr)
    {
        dco1InitButton->setTheme(inTheme);
    }

    if (dco1CopyButton != nullptr)
    {
        dco1CopyButton->setTheme(inTheme);
    }

    if (dco1PasteButton != nullptr)
    {
        dco1PasteButton->setTheme(inTheme);
    }

    if (dco1FrequencyLabel != nullptr)
    {
        dco1FrequencyLabel->setTheme(inTheme);
    }

    if (dco1FrequencySlider != nullptr)
    {
        dco1FrequencySlider->setTheme(inTheme);
    }

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setTheme(inTheme);
    }

    if (dco1FrequencyModByLfo1Label != nullptr)
    {
        dco1FrequencyModByLfo1Label->setTheme(inTheme);
    }

    if (dco1FrequencyModByLfo1Slider != nullptr)
    {
        dco1FrequencyModByLfo1Slider->setTheme(inTheme);
    }

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setTheme(inTheme);
    }

    if (dco1SyncLabel != nullptr)
    {
        dco1SyncLabel->setTheme(inTheme);
    }

    if (dco1SyncComboBox != nullptr)
    {
        dco1SyncComboBox->setTheme(inTheme);
    }

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setTheme(inTheme);
    }

    if (dco1PulseWidthLabel != nullptr)
    {
        dco1PulseWidthLabel->setTheme(inTheme);
    }

    if (dco1PulseWidthSlider != nullptr)
    {
        dco1PulseWidthSlider->setTheme(inTheme);
    }

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setTheme(inTheme);
    }

    if (dco1PulseWidthModByLfo2Label != nullptr)
    {
        dco1PulseWidthModByLfo2Label->setTheme(inTheme);
    }

    if (dco1PulseWidthModByLfo2Slider != nullptr)
    {
        dco1PulseWidthModByLfo2Slider->setTheme(inTheme);
    }

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setTheme(inTheme);
    }

    if (dco1WaveShapeLabel != nullptr)
    {
        dco1WaveShapeLabel->setTheme(inTheme);
    }

    if (dco1WaveShapeSlider != nullptr)
    {
        dco1WaveShapeSlider->setTheme(inTheme);
    }

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setTheme(inTheme);
    }

    if (dco1WaveSelectLabel != nullptr)
    {
        dco1WaveSelectLabel->setTheme(inTheme);
    }

    if (dco1WaveSelectComboBox != nullptr)
    {
        dco1WaveSelectComboBox->setTheme(inTheme);
    }

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setTheme(inTheme);
    }

    if (dco1LeversLabel != nullptr)
    {
        dco1LeversLabel->setTheme(inTheme);
    }

    if (dco1LeversComboBox != nullptr)
    {
        dco1LeversComboBox->setTheme(inTheme);
    }

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setTheme(inTheme);
    }

    if (dco1KeyboardPortamentoLabel != nullptr)
    {
        dco1KeyboardPortamentoLabel->setTheme(inTheme);
    }

    if (dco1KeyboardPortamentoComboBox != nullptr)
    {
        dco1KeyboardPortamentoComboBox->setTheme(inTheme);
    }

    if (parameterSeparator9 != nullptr)
    {
        parameterSeparator9->setTheme(inTheme);
    }

    if (dco1KeyClickLabel != nullptr)
    {
        dco1KeyClickLabel->setTheme(inTheme);
    }

    if (dco1KeyClickComboBox != nullptr)
    {
        dco1KeyClickComboBox->setTheme(inTheme);
    }

    if (parameterSeparator10 != nullptr)
    {
        parameterSeparator10->setTheme(inTheme);
    }

    repaint();
}

