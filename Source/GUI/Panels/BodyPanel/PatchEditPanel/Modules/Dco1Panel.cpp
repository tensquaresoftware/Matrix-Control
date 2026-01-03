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

Dco1Panel::Dco1Panel(Theme& newTheme, WidgetFactory& widgetFactory)
    : theme(&newTheme)
{
    dco1ModuleName = std::make_unique<tss::ModuleName>(
        newTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kDco1)
    );

    dco1InitButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kDco1Init, newTheme);
    dco1CopyButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kDco1Copy, newTheme);
    dco1PasteButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kDco1Paste, newTheme);

    dco1FrequencyLabel = std::make_unique<tss::ParameterLabel>(
        newTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1Frequency)
    );
    dco1FrequencySlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco1Frequency, newTheme);
    parameterSeparator1 = std::make_unique<tss::ParameterSeparator>(newTheme);

    dco1FrequencyModByLfo1Label = std::make_unique<tss::ParameterLabel>(
        newTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1FrequencyModByLfo1)
    );
    dco1FrequencyModByLfo1Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco1FrequencyModByLfo1, newTheme);
    parameterSeparator2 = std::make_unique<tss::ParameterSeparator>(newTheme);

    dco1SyncLabel = std::make_unique<tss::ParameterLabel>(
        newTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1Sync)
    );
    dco1SyncComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco1Sync, newTheme);
    parameterSeparator3 = std::make_unique<tss::ParameterSeparator>(newTheme);

    dco1PulseWidthLabel = std::make_unique<tss::ParameterLabel>(
        newTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1PulseWidth)
    );
    dco1PulseWidthSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco1PulseWidth, newTheme);
    parameterSeparator4 = std::make_unique<tss::ParameterSeparator>(newTheme);

    dco1PulseWidthModByLfo2Label = std::make_unique<tss::ParameterLabel>(
        newTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1PulseWidthModByLfo2)
    );
    dco1PulseWidthModByLfo2Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco1PulseWidthModByLfo2, newTheme);
    parameterSeparator5 = std::make_unique<tss::ParameterSeparator>(newTheme);

    dco1WaveShapeLabel = std::make_unique<tss::ParameterLabel>(
        newTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1WaveShape)
    );
    dco1WaveShapeSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kDco1WaveShape, newTheme);
    parameterSeparator6 = std::make_unique<tss::ParameterSeparator>(newTheme);

    dco1WaveSelectLabel = std::make_unique<tss::ParameterLabel>(
        newTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1WaveSelect)
    );
    dco1WaveSelectComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco1WaveSelect, newTheme);
    parameterSeparator7 = std::make_unique<tss::ParameterSeparator>(newTheme);

    dco1LeversLabel = std::make_unique<tss::ParameterLabel>(
        newTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1Levers)
    );
    dco1LeversComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco1Levers, newTheme);
    parameterSeparator8 = std::make_unique<tss::ParameterSeparator>(newTheme);

    dco1KeyboardPortamentoLabel = std::make_unique<tss::ParameterLabel>(
        newTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1KeyboardPortamento)
    );
    dco1KeyboardPortamentoComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco1KeyboardPortamento, newTheme);
    parameterSeparator9 = std::make_unique<tss::ParameterSeparator>(newTheme);

    dco1KeyClickLabel = std::make_unique<tss::ParameterLabel>(
        newTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kDco1KeyClick)
    );
    dco1KeyClickComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kDco1KeyClick, newTheme);
    parameterSeparator10 = std::make_unique<tss::ParameterSeparator>(newTheme);

    addAndMakeVisible(*dco1ModuleName);
    addAndMakeVisible(*dco1InitButton);
    addAndMakeVisible(*dco1CopyButton);
    addAndMakeVisible(*dco1PasteButton);
    addAndMakeVisible(*dco1FrequencyLabel);
    addAndMakeVisible(*dco1FrequencySlider);
    addAndMakeVisible(*parameterSeparator1);
    addAndMakeVisible(*dco1FrequencyModByLfo1Label);
    addAndMakeVisible(*dco1FrequencyModByLfo1Slider);
    addAndMakeVisible(*parameterSeparator2);
    addAndMakeVisible(*dco1SyncLabel);
    addAndMakeVisible(*dco1SyncComboBox);
    addAndMakeVisible(*parameterSeparator3);
    addAndMakeVisible(*dco1PulseWidthLabel);
    addAndMakeVisible(*dco1PulseWidthSlider);
    addAndMakeVisible(*parameterSeparator4);
    addAndMakeVisible(*dco1PulseWidthModByLfo2Label);
    addAndMakeVisible(*dco1PulseWidthModByLfo2Slider);
    addAndMakeVisible(*parameterSeparator5);
    addAndMakeVisible(*dco1WaveShapeLabel);
    addAndMakeVisible(*dco1WaveShapeSlider);
    addAndMakeVisible(*parameterSeparator6);
    addAndMakeVisible(*dco1WaveSelectLabel);
    addAndMakeVisible(*dco1WaveSelectComboBox);
    addAndMakeVisible(*parameterSeparator7);
    addAndMakeVisible(*dco1LeversLabel);
    addAndMakeVisible(*dco1LeversComboBox);
    addAndMakeVisible(*parameterSeparator8);
    addAndMakeVisible(*dco1KeyboardPortamentoLabel);
    addAndMakeVisible(*dco1KeyboardPortamentoComboBox);
    addAndMakeVisible(*parameterSeparator9);
    addAndMakeVisible(*dco1KeyClickLabel);
    addAndMakeVisible(*dco1KeyClickComboBox);
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

void Dco1Panel::setTheme(Theme& newTheme)
{
    theme = &newTheme;

    if (dco1ModuleName != nullptr)
    {
        dco1ModuleName->setTheme(newTheme);
    }

    if (dco1InitButton != nullptr)
    {
        dco1InitButton->setTheme(newTheme);
    }

    if (dco1CopyButton != nullptr)
    {
        dco1CopyButton->setTheme(newTheme);
    }

    if (dco1PasteButton != nullptr)
    {
        dco1PasteButton->setTheme(newTheme);
    }

    if (dco1FrequencyLabel != nullptr)
    {
        dco1FrequencyLabel->setTheme(newTheme);
    }

    if (dco1FrequencySlider != nullptr)
    {
        dco1FrequencySlider->setTheme(newTheme);
    }

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setTheme(newTheme);
    }

    if (dco1FrequencyModByLfo1Label != nullptr)
    {
        dco1FrequencyModByLfo1Label->setTheme(newTheme);
    }

    if (dco1FrequencyModByLfo1Slider != nullptr)
    {
        dco1FrequencyModByLfo1Slider->setTheme(newTheme);
    }

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setTheme(newTheme);
    }

    if (dco1SyncLabel != nullptr)
    {
        dco1SyncLabel->setTheme(newTheme);
    }

    if (dco1SyncComboBox != nullptr)
    {
        dco1SyncComboBox->setTheme(newTheme);
    }

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setTheme(newTheme);
    }

    if (dco1PulseWidthLabel != nullptr)
    {
        dco1PulseWidthLabel->setTheme(newTheme);
    }

    if (dco1PulseWidthSlider != nullptr)
    {
        dco1PulseWidthSlider->setTheme(newTheme);
    }

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setTheme(newTheme);
    }

    if (dco1PulseWidthModByLfo2Label != nullptr)
    {
        dco1PulseWidthModByLfo2Label->setTheme(newTheme);
    }

    if (dco1PulseWidthModByLfo2Slider != nullptr)
    {
        dco1PulseWidthModByLfo2Slider->setTheme(newTheme);
    }

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setTheme(newTheme);
    }

    if (dco1WaveShapeLabel != nullptr)
    {
        dco1WaveShapeLabel->setTheme(newTheme);
    }

    if (dco1WaveShapeSlider != nullptr)
    {
        dco1WaveShapeSlider->setTheme(newTheme);
    }

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setTheme(newTheme);
    }

    if (dco1WaveSelectLabel != nullptr)
    {
        dco1WaveSelectLabel->setTheme(newTheme);
    }

    if (dco1WaveSelectComboBox != nullptr)
    {
        dco1WaveSelectComboBox->setTheme(newTheme);
    }

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setTheme(newTheme);
    }

    if (dco1LeversLabel != nullptr)
    {
        dco1LeversLabel->setTheme(newTheme);
    }

    if (dco1LeversComboBox != nullptr)
    {
        dco1LeversComboBox->setTheme(newTheme);
    }

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setTheme(newTheme);
    }

    if (dco1KeyboardPortamentoLabel != nullptr)
    {
        dco1KeyboardPortamentoLabel->setTheme(newTheme);
    }

    if (dco1KeyboardPortamentoComboBox != nullptr)
    {
        dco1KeyboardPortamentoComboBox->setTheme(newTheme);
    }

    if (parameterSeparator9 != nullptr)
    {
        parameterSeparator9->setTheme(newTheme);
    }

    if (dco1KeyClickLabel != nullptr)
    {
        dco1KeyClickLabel->setTheme(newTheme);
    }

    if (dco1KeyClickComboBox != nullptr)
    {
        dco1KeyClickComboBox->setTheme(newTheme);
    }

    if (parameterSeparator10 != nullptr)
    {
        parameterSeparator10->setTheme(newTheme);
    }

    repaint();
}

