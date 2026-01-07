#include "Dco2Panel.h"

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

Dco2Panel::Dco2Panel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    dco2ModuleName = std::make_unique<tss::ModuleName>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kDco2));
    addAndMakeVisible(*dco2ModuleName);

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
    dco2FrequencyLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2Frequency));
    addAndMakeVisible(*dco2FrequencyLabel);

    dco2FrequencySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco2Frequency, inTheme);
    dco2FrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2Frequency,
        *dco2FrequencySlider);
    addAndMakeVisible(*dco2FrequencySlider);

    parameterSeparator1 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator1);

    // Frequency Mod by LFO 1
    dco2FrequencyModByLfo1Label = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2FrequencyModByLfo1));
    addAndMakeVisible(*dco2FrequencyModByLfo1Label);

    dco2FrequencyModByLfo1Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco2FrequencyModByLfo1, inTheme);
    dco2FrequencyModByLfo1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2FrequencyModByLfo1,
        *dco2FrequencyModByLfo1Slider);
    addAndMakeVisible(*dco2FrequencyModByLfo1Slider);

    parameterSeparator2 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator2);

    // Detune
    dco2DetuneLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2Detune));
    addAndMakeVisible(*dco2DetuneLabel);

    dco2DetuneSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco2Detune, inTheme);
    dco2DetuneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2Detune,
        *dco2DetuneSlider);
    addAndMakeVisible(*dco2DetuneSlider);

    parameterSeparator3 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator3);

    // Pulse Width
    dco2PulseWidthLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2PulseWidth));
    addAndMakeVisible(*dco2PulseWidthLabel);

    dco2PulseWidthSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco2PulseWidth, inTheme);
    dco2PulseWidthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2PulseWidth,
        *dco2PulseWidthSlider);
    addAndMakeVisible(*dco2PulseWidthSlider);

    parameterSeparator4 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator4);

    // Pulse Width Mod by LFO 2
    dco2PulseWidthModByLfo2Label = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2PulseWidthModByLfo2));
    addAndMakeVisible(*dco2PulseWidthModByLfo2Label);

    dco2PulseWidthModByLfo2Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco2PulseWidthModByLfo2, inTheme);
    dco2PulseWidthModByLfo2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2PulseWidthModByLfo2,
        *dco2PulseWidthModByLfo2Slider);
    addAndMakeVisible(*dco2PulseWidthModByLfo2Slider);

    parameterSeparator5 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator5);

    // Wave Shape
    dco2WaveShapeLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2WaveShape));
    addAndMakeVisible(*dco2WaveShapeLabel);

    dco2WaveShapeSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco2WaveShape, inTheme);
    dco2WaveShapeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2WaveShape,
        *dco2WaveShapeSlider);
    addAndMakeVisible(*dco2WaveShapeSlider);

    parameterSeparator6 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator6);

    // Wave Select
    dco2WaveSelectLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2WaveSelect));
    addAndMakeVisible(*dco2WaveSelectLabel);

    dco2WaveSelectComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco2WaveSelect, inTheme);
    dco2WaveSelectAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2WaveSelect,
        *dco2WaveSelectComboBox);
    addAndMakeVisible(*dco2WaveSelectComboBox);

    parameterSeparator7 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator7);

    // Levers
    dco2LeversLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2Levers));
    addAndMakeVisible(*dco2LeversLabel);

    dco2LeversComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco2Levers, inTheme);
    dco2LeversAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2Levers,
        *dco2LeversComboBox);
    addAndMakeVisible(*dco2LeversComboBox);

    parameterSeparator8 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator8);

    // Keyboard Portamento
    dco2KeyboardPortamentoLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2KeyboardPortamento));
    addAndMakeVisible(*dco2KeyboardPortamentoLabel);

    dco2KeyboardPortamentoComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco2KeyboardPortamento, inTheme);
    dco2KeyboardPortamentoAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2KeyboardPortamento,
        *dco2KeyboardPortamentoComboBox);
    addAndMakeVisible(*dco2KeyboardPortamentoComboBox);

    parameterSeparator9 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator9);

    // Key Click
    dco2KeyClickLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco2KeyClick));
    addAndMakeVisible(*dco2KeyClickLabel);

    dco2KeyClickComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco2KeyClick, inTheme);
    dco2KeyClickAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kDco2KeyClick,
        *dco2KeyClickComboBox);
    addAndMakeVisible(*dco2KeyClickComboBox);

    parameterSeparator10 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator10);

    setSize(getWidth(), getHeight());
}

Dco2Panel::~Dco2Panel() = default;

void Dco2Panel::paint(juce::Graphics& g)
{
    if (theme == nullptr)
    {
        return;
    }

    g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void Dco2Panel::resized()
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

    if (dco2ModuleName != nullptr)
    {
        dco2ModuleName->setBounds(0, y, moduleNameWidth, moduleNameHeight);
    }

    if (dco2PasteButton != nullptr)
    {
        dco2PasteButton->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);
    }

    if (dco2CopyButton != nullptr)
    {
        dco2CopyButton->setBounds(panelWidth - buttonWidth * 2, y, buttonWidth, buttonHeight);
    }

    if (dco2InitButton != nullptr)
    {
        dco2InitButton->setBounds(panelWidth - buttonWidth * 3, y, buttonWidth, buttonHeight);
    }

    y += moduleNameHeight;

    if (dco2FrequencyLabel != nullptr)
    {
        dco2FrequencyLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco2FrequencySlider != nullptr)
    {
        dco2FrequencySlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco2FrequencyModByLfo1Label != nullptr)
    {
        dco2FrequencyModByLfo1Label->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco2FrequencyModByLfo1Slider != nullptr)
    {
        dco2FrequencyModByLfo1Slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco2DetuneLabel != nullptr)
    {
        dco2DetuneLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco2DetuneSlider != nullptr)
    {
        dco2DetuneSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco2PulseWidthLabel != nullptr)
    {
        dco2PulseWidthLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco2PulseWidthSlider != nullptr)
    {
        dco2PulseWidthSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco2PulseWidthModByLfo2Label != nullptr)
    {
        dco2PulseWidthModByLfo2Label->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco2PulseWidthModByLfo2Slider != nullptr)
    {
        dco2PulseWidthModByLfo2Slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco2WaveShapeLabel != nullptr)
    {
        dco2WaveShapeLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco2WaveShapeSlider != nullptr)
    {
        dco2WaveShapeSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco2WaveSelectLabel != nullptr)
    {
        dco2WaveSelectLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco2WaveSelectComboBox != nullptr)
    {
        dco2WaveSelectComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco2LeversLabel != nullptr)
    {
        dco2LeversLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco2LeversComboBox != nullptr)
    {
        dco2LeversComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco2KeyboardPortamentoLabel != nullptr)
    {
        dco2KeyboardPortamentoLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco2KeyboardPortamentoComboBox != nullptr)
    {
        dco2KeyboardPortamentoComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator9 != nullptr)
    {
        parameterSeparator9->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (dco2KeyClickLabel != nullptr)
    {
        dco2KeyClickLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (dco2KeyClickComboBox != nullptr)
    {
        dco2KeyClickComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }

    y += labelHeight;

    if (parameterSeparator10 != nullptr)
    {
        parameterSeparator10->setBounds(0, y, separatorWidth, separatorHeight);
    }
}

void Dco2Panel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (dco2ModuleName != nullptr)
    {
        dco2ModuleName->setTheme(inTheme);
    }

    if (dco2InitButton != nullptr)
    {
        dco2InitButton->setTheme(inTheme);
    }

    if (dco2CopyButton != nullptr)
    {
        dco2CopyButton->setTheme(inTheme);
    }

    if (dco2PasteButton != nullptr)
    {
        dco2PasteButton->setTheme(inTheme);
    }

    if (dco2FrequencyLabel != nullptr)
    {
        dco2FrequencyLabel->setTheme(inTheme);
    }

    if (dco2FrequencySlider != nullptr)
    {
        dco2FrequencySlider->setTheme(inTheme);
    }

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setTheme(inTheme);
    }

    if (dco2FrequencyModByLfo1Label != nullptr)
    {
        dco2FrequencyModByLfo1Label->setTheme(inTheme);
    }

    if (dco2FrequencyModByLfo1Slider != nullptr)
    {
        dco2FrequencyModByLfo1Slider->setTheme(inTheme);
    }

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setTheme(inTheme);
    }

    if (dco2DetuneLabel != nullptr)
    {
        dco2DetuneLabel->setTheme(inTheme);
    }

    if (dco2DetuneSlider != nullptr)
    {
        dco2DetuneSlider->setTheme(inTheme);
    }

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setTheme(inTheme);
    }

    if (dco2PulseWidthLabel != nullptr)
    {
        dco2PulseWidthLabel->setTheme(inTheme);
    }

    if (dco2PulseWidthSlider != nullptr)
    {
        dco2PulseWidthSlider->setTheme(inTheme);
    }

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setTheme(inTheme);
    }

    if (dco2PulseWidthModByLfo2Label != nullptr)
    {
        dco2PulseWidthModByLfo2Label->setTheme(inTheme);
    }

    if (dco2PulseWidthModByLfo2Slider != nullptr)
    {
        dco2PulseWidthModByLfo2Slider->setTheme(inTheme);
    }

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setTheme(inTheme);
    }

    if (dco2WaveShapeLabel != nullptr)
    {
        dco2WaveShapeLabel->setTheme(inTheme);
    }

    if (dco2WaveShapeSlider != nullptr)
    {
        dco2WaveShapeSlider->setTheme(inTheme);
    }

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setTheme(inTheme);
    }

    if (dco2WaveSelectLabel != nullptr)
    {
        dco2WaveSelectLabel->setTheme(inTheme);
    }

    if (dco2WaveSelectComboBox != nullptr)
    {
        dco2WaveSelectComboBox->setTheme(inTheme);
    }

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setTheme(inTheme);
    }

    if (dco2LeversLabel != nullptr)
    {
        dco2LeversLabel->setTheme(inTheme);
    }

    if (dco2LeversComboBox != nullptr)
    {
        dco2LeversComboBox->setTheme(inTheme);
    }

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setTheme(inTheme);
    }

    if (dco2KeyboardPortamentoLabel != nullptr)
    {
        dco2KeyboardPortamentoLabel->setTheme(inTheme);
    }

    if (dco2KeyboardPortamentoComboBox != nullptr)
    {
        dco2KeyboardPortamentoComboBox->setTheme(inTheme);
    }

    if (parameterSeparator9 != nullptr)
    {
        parameterSeparator9->setTheme(inTheme);
    }

    if (dco2KeyClickLabel != nullptr)
    {
        dco2KeyClickLabel->setTheme(inTheme);
    }

    if (dco2KeyClickComboBox != nullptr)
    {
        dco2KeyClickComboBox->setTheme(inTheme);
    }

    if (parameterSeparator10 != nullptr)
    {
        parameterSeparator10->setTheme(inTheme);
    }

    repaint();
}
