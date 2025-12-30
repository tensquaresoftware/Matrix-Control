#include "Dco1Panel.h"

#include "../../../../Skin/Skin.h"
#include "../../../../Skin/SkinDimensions.h"
#include "../../../../Widgets/ModuleName.h"
#include "../../../../Widgets/ParameterLabel.h"
#include "../../../../Widgets/Slider.h"
#include "../../../../Widgets/ComboBox.h"
#include "../../../../Widgets/Button.h"
#include "../../../../Widgets/ParameterSeparator.h"

using tss::Skin;

Dco1Panel::Dco1Panel(Skin& newSkin)
    : skin(&newSkin)
    , dco1ModuleName(std::make_unique<tss::ModuleName>(newSkin, "DCO 1"))
    , dco1InitButton(std::make_unique<tss::Button>(newSkin, tss::SkinDimensions::Widget::Button::kDefaultWidth, "I"))
    , dco1CopyButton(std::make_unique<tss::Button>(newSkin, tss::SkinDimensions::Widget::Button::kDefaultWidth, "C"))
    , dco1PasteButton(std::make_unique<tss::Button>(newSkin, tss::SkinDimensions::Widget::Button::kDefaultWidth, "P"))
    , dco1FrequencyLabel(std::make_unique<tss::ParameterLabel>(newSkin, "FREQUENCY"))
    , dco1FrequencySlider(std::make_unique<tss::Slider>(newSkin))
    , parameterSeparator1(std::make_unique<tss::ParameterSeparator>(newSkin))
    , dco1FrequencyModByLfo1Label(std::make_unique<tss::ParameterLabel>(newSkin, "FREQ MOD BY LFO 1"))
    , dco1FrequencyModByLfo1Slider(std::make_unique<tss::Slider>(newSkin))
    , parameterSeparator2(std::make_unique<tss::ParameterSeparator>(newSkin))
    , dco1SyncLabel(std::make_unique<tss::ParameterLabel>(newSkin, "SYNC"))
    , dco1SyncComboBox(std::make_unique<tss::ComboBox>(newSkin))
    , parameterSeparator3(std::make_unique<tss::ParameterSeparator>(newSkin))
    , dco1PulseWidthLabel(std::make_unique<tss::ParameterLabel>(newSkin, "PULSE WIDTH"))
    , dco1PulseWidthSlider(std::make_unique<tss::Slider>(newSkin))
    , parameterSeparator4(std::make_unique<tss::ParameterSeparator>(newSkin))
    , dco1PulseWidthModByLfo2Label(std::make_unique<tss::ParameterLabel>(newSkin, "PW MOD BY LFO 2"))
    , dco1PulseWidthModByLfo2Slider(std::make_unique<tss::Slider>(newSkin))
    , parameterSeparator5(std::make_unique<tss::ParameterSeparator>(newSkin))
    , dco1WaveShapeLabel(std::make_unique<tss::ParameterLabel>(newSkin, "WAVE SHAPE"))
    , dco1WaveShapeSlider(std::make_unique<tss::Slider>(newSkin))
    , parameterSeparator6(std::make_unique<tss::ParameterSeparator>(newSkin))
    , dco1WaveSelectLabel(std::make_unique<tss::ParameterLabel>(newSkin, "WAVE SELECT"))
    , dco1WaveSelectComboBox(std::make_unique<tss::ComboBox>(newSkin))
    , parameterSeparator7(std::make_unique<tss::ParameterSeparator>(newSkin))
    , dco1LeversLabel(std::make_unique<tss::ParameterLabel>(newSkin, "LEVERS"))
    , dco1LeversComboBox(std::make_unique<tss::ComboBox>(newSkin))
    , parameterSeparator8(std::make_unique<tss::ParameterSeparator>(newSkin))
    , dco1KeyboardPortamentoLabel(std::make_unique<tss::ParameterLabel>(newSkin, "KEYBOARD/PORTAMENTO"))
    , dco1KeyboardPortamentoComboBox(std::make_unique<tss::ComboBox>(newSkin))
    , parameterSeparator9(std::make_unique<tss::ParameterSeparator>(newSkin))
    , dco1KeyClickLabel(std::make_unique<tss::ParameterLabel>(newSkin, "KEY CLICK"))
    , dco1KeyClickComboBox(std::make_unique<tss::ComboBox>(newSkin))
    , parameterSeparator10(std::make_unique<tss::ParameterSeparator>(newSkin))
{
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

    setSize(tss::SkinDimensions::PatchEditModulePanel::kWidth, tss::SkinDimensions::PatchEditModulePanel::kHeight);
}

Dco1Panel::~Dco1Panel() = default;

void Dco1Panel::paint(juce::Graphics& g)
{
    if (skin == nullptr)
    {
        return;
    }

    g.fillAll(skin->getPatchEditModulePanelBackgroundColour());
}

void Dco1Panel::resized()
{
    const auto moduleNameHeight = tss::SkinDimensions::Widget::ModuleName::kHeight;
    const auto moduleNameWidth = tss::SkinDimensions::Widget::ModuleName::kNormalWidth;
    const auto buttonWidth = tss::SkinDimensions::Widget::Button::kDefaultWidth;
    const auto buttonHeight = tss::SkinDimensions::Widget::Button::kHeight;
    const auto labelWidth = tss::SkinDimensions::Widget::ParameterLabel::kWidth;
    const auto labelHeight = tss::SkinDimensions::Widget::ParameterLabel::kHeight;
    const auto sliderWidth = tss::SkinDimensions::Widget::Slider::kWidth;
    const auto sliderHeight = tss::SkinDimensions::Widget::Slider::kHeight;
    const auto comboBoxWidth = tss::SkinDimensions::Widget::ComboBox::kNormalWidth;
    const auto comboBoxHeight = tss::SkinDimensions::Widget::ComboBox::kHeight;
    const auto separatorWidth = tss::SkinDimensions::Widget::ParameterSeparator::kWidth;
    const auto separatorHeight = tss::SkinDimensions::Widget::ParameterSeparator::kHeight;
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

void Dco1Panel::setSkin(Skin& newSkin)
{
    skin = &newSkin;

    if (dco1ModuleName != nullptr)
    {
        dco1ModuleName->setSkin(newSkin);
    }

    if (dco1InitButton != nullptr)
    {
        dco1InitButton->setSkin(newSkin);
    }

    if (dco1CopyButton != nullptr)
    {
        dco1CopyButton->setSkin(newSkin);
    }

    if (dco1PasteButton != nullptr)
    {
        dco1PasteButton->setSkin(newSkin);
    }

    if (dco1FrequencyLabel != nullptr)
    {
        dco1FrequencyLabel->setSkin(newSkin);
    }

    if (dco1FrequencySlider != nullptr)
    {
        dco1FrequencySlider->setSkin(newSkin);
    }

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setSkin(newSkin);
    }

    if (dco1FrequencyModByLfo1Label != nullptr)
    {
        dco1FrequencyModByLfo1Label->setSkin(newSkin);
    }

    if (dco1FrequencyModByLfo1Slider != nullptr)
    {
        dco1FrequencyModByLfo1Slider->setSkin(newSkin);
    }

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setSkin(newSkin);
    }

    if (dco1SyncLabel != nullptr)
    {
        dco1SyncLabel->setSkin(newSkin);
    }

    if (dco1SyncComboBox != nullptr)
    {
        dco1SyncComboBox->setSkin(newSkin);
    }

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setSkin(newSkin);
    }

    if (dco1PulseWidthLabel != nullptr)
    {
        dco1PulseWidthLabel->setSkin(newSkin);
    }

    if (dco1PulseWidthSlider != nullptr)
    {
        dco1PulseWidthSlider->setSkin(newSkin);
    }

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setSkin(newSkin);
    }

    if (dco1PulseWidthModByLfo2Label != nullptr)
    {
        dco1PulseWidthModByLfo2Label->setSkin(newSkin);
    }

    if (dco1PulseWidthModByLfo2Slider != nullptr)
    {
        dco1PulseWidthModByLfo2Slider->setSkin(newSkin);
    }

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setSkin(newSkin);
    }

    if (dco1WaveShapeLabel != nullptr)
    {
        dco1WaveShapeLabel->setSkin(newSkin);
    }

    if (dco1WaveShapeSlider != nullptr)
    {
        dco1WaveShapeSlider->setSkin(newSkin);
    }

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setSkin(newSkin);
    }

    if (dco1WaveSelectLabel != nullptr)
    {
        dco1WaveSelectLabel->setSkin(newSkin);
    }

    if (dco1WaveSelectComboBox != nullptr)
    {
        dco1WaveSelectComboBox->setSkin(newSkin);
    }

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setSkin(newSkin);
    }

    if (dco1LeversLabel != nullptr)
    {
        dco1LeversLabel->setSkin(newSkin);
    }

    if (dco1LeversComboBox != nullptr)
    {
        dco1LeversComboBox->setSkin(newSkin);
    }

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setSkin(newSkin);
    }

    if (dco1KeyboardPortamentoLabel != nullptr)
    {
        dco1KeyboardPortamentoLabel->setSkin(newSkin);
    }

    if (dco1KeyboardPortamentoComboBox != nullptr)
    {
        dco1KeyboardPortamentoComboBox->setSkin(newSkin);
    }

    if (parameterSeparator9 != nullptr)
    {
        parameterSeparator9->setSkin(newSkin);
    }

    if (dco1KeyClickLabel != nullptr)
    {
        dco1KeyClickLabel->setSkin(newSkin);
    }

    if (dco1KeyClickComboBox != nullptr)
    {
        dco1KeyClickComboBox->setSkin(newSkin);
    }

    if (parameterSeparator10 != nullptr)
    {
        parameterSeparator10->setSkin(newSkin);
    }

    repaint();
}

