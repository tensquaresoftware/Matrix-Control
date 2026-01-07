#include "FmTrackPanel.h"

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

FmTrackPanel::FmTrackPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    fmTrackModuleName = std::make_unique<tss::ModuleName>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kFmTrack) );
    addAndMakeVisible(*fmTrackModuleName);

    // Standalone Widgets
    fmTrackInitButton = widgetFactory.createStandaloneButton(SynthDescriptors::WidgetIds::kFmTrackInit, inTheme);
    fmTrackInitButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::WidgetIds::kFmTrackInit,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*fmTrackInitButton);

    // FM Amount
    fmAmountLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kFmAmount));
    addAndMakeVisible(*fmAmountLabel);

    fmAmountSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kFmAmount, inTheme);
    fmAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kFmAmount,
        *fmAmountSlider);
    addAndMakeVisible(*fmAmountSlider);

    parameterSeparator1 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator1);

    // FM Mod by ENV 3
    fmModByEnv3Label = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kFmModByEnv3));
    addAndMakeVisible(*fmModByEnv3Label);

    fmModByEnv3Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kFmModByEnv3, inTheme);
    fmModByEnv3Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kFmModByEnv3,
        *fmModByEnv3Slider);
    addAndMakeVisible(*fmModByEnv3Slider);

    parameterSeparator2 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator2);

    // FM Mod by Pressure
    fmModByPressureLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kFmModByPressure));
    addAndMakeVisible(*fmModByPressureLabel);

    fmModByPressureSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kFmModByPressure, inTheme);
    fmModByPressureAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kFmModByPressure,
        *fmModByPressureSlider);
    addAndMakeVisible(*fmModByPressureSlider);

    parameterSeparator3 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator3);

    // Track Point 1
    trackPoint1Label = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kTrackPoint1));
    addAndMakeVisible(*trackPoint1Label);

    trackPoint1Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kTrackPoint1, inTheme);
    trackPoint1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kTrackPoint1,
        *trackPoint1Slider);
    addAndMakeVisible(*trackPoint1Slider);

    parameterSeparator4 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator4);

    // Track Point 2
    trackPoint2Label = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kTrackPoint2));
    addAndMakeVisible(*trackPoint2Label);

    trackPoint2Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kTrackPoint2, inTheme);
    trackPoint2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kTrackPoint2,
        *trackPoint2Slider);
    addAndMakeVisible(*trackPoint2Slider);

    parameterSeparator5 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator5);

    // Track Point 3
    trackPoint3Label = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kTrackPoint3));
    addAndMakeVisible(*trackPoint3Label);

    trackPoint3Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kTrackPoint3, inTheme);
    trackPoint3Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kTrackPoint3,
        *trackPoint3Slider);
    addAndMakeVisible(*trackPoint3Slider);

    parameterSeparator6 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator6);

    // Track Point 4
    trackPoint4Label = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kTrackPoint4));
    addAndMakeVisible(*trackPoint4Label);

    trackPoint4Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kTrackPoint4, inTheme);
    trackPoint4Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kTrackPoint4,
        *trackPoint4Slider);
    addAndMakeVisible(*trackPoint4Slider);

    parameterSeparator7 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator7);

    // Track Point 5
    trackPoint5Label = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kTrackPoint5));
    addAndMakeVisible(*trackPoint5Label);

    trackPoint5Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kTrackPoint5, inTheme);
    trackPoint5Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kTrackPoint5,
        *trackPoint5Slider);
    addAndMakeVisible(*trackPoint5Slider);

    parameterSeparator8 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator8);

    // Track Input
    trackInputLabel = std::make_unique<tss::ParameterLabel>(
        inTheme, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kTrackInput));
    addAndMakeVisible(*trackInputLabel);

    trackInputComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kTrackInput, inTheme);
    trackInputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kTrackInput,
        *trackInputComboBox);
    addAndMakeVisible(*trackInputComboBox);

    parameterSeparator9 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator9);

    parameterSeparator10 = std::make_unique<tss::ParameterSeparator>(inTheme);
    addAndMakeVisible(*parameterSeparator10);

    setSize(getWidth(), getHeight());
}

FmTrackPanel::~FmTrackPanel() = default;

void FmTrackPanel::paint(juce::Graphics& g)
{
    if (theme == nullptr)
    {
        return;
    }

    g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void FmTrackPanel::resized()
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

    if (fmTrackModuleName != nullptr)
    {
        fmTrackModuleName->setBounds(0, y, moduleNameWidth, moduleNameHeight);
    }

    if (fmTrackInitButton != nullptr)
    {
        fmTrackInitButton->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);
    }

    y += moduleNameHeight;

    if (fmAmountLabel != nullptr)
    {
        fmAmountLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (fmAmountSlider != nullptr)
    {
        fmAmountSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (fmModByEnv3Label != nullptr)
    {
        fmModByEnv3Label->setBounds(0, y, labelWidth, labelHeight);
    }

    if (fmModByEnv3Slider != nullptr)
    {
        fmModByEnv3Slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (fmModByPressureLabel != nullptr)
    {
        fmModByPressureLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (fmModByPressureSlider != nullptr)
    {
        fmModByPressureSlider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (trackPoint1Label != nullptr)
    {
        trackPoint1Label->setBounds(0, y, labelWidth, labelHeight);
    }

    if (trackPoint1Slider != nullptr)
    {
        trackPoint1Slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (trackPoint2Label != nullptr)
    {
        trackPoint2Label->setBounds(0, y, labelWidth, labelHeight);
    }

    if (trackPoint2Slider != nullptr)
    {
        trackPoint2Slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (trackPoint3Label != nullptr)
    {
        trackPoint3Label->setBounds(0, y, labelWidth, labelHeight);
    }

    if (trackPoint3Slider != nullptr)
    {
        trackPoint3Slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (trackPoint4Label != nullptr)
    {
        trackPoint4Label->setBounds(0, y, labelWidth, labelHeight);
    }

    if (trackPoint4Slider != nullptr)
    {
        trackPoint4Slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (trackPoint5Label != nullptr)
    {
        trackPoint5Label->setBounds(0, y, labelWidth, labelHeight);
    }

    if (trackPoint5Slider != nullptr)
    {
        trackPoint5Slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }

    y += labelHeight;

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setBounds(0, y, separatorWidth, separatorHeight);
    }

    y += separatorHeight;

    if (trackInputLabel != nullptr)
    {
        trackInputLabel->setBounds(0, y, labelWidth, labelHeight);
    }

    if (trackInputComboBox != nullptr)
    {
        trackInputComboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
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

void FmTrackPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (fmTrackModuleName != nullptr)
    {
        fmTrackModuleName->setTheme(inTheme);
    }

    if (fmTrackInitButton != nullptr)
    {
        fmTrackInitButton->setTheme(inTheme);
    }

    if (fmAmountLabel != nullptr)
    {
        fmAmountLabel->setTheme(inTheme);
    }

    if (fmAmountSlider != nullptr)
    {
        fmAmountSlider->setTheme(inTheme);
    }

    if (parameterSeparator1 != nullptr)
    {
        parameterSeparator1->setTheme(inTheme);
    }

    if (fmModByEnv3Label != nullptr)
    {
        fmModByEnv3Label->setTheme(inTheme);
    }

    if (fmModByEnv3Slider != nullptr)
    {
        fmModByEnv3Slider->setTheme(inTheme);
    }

    if (parameterSeparator2 != nullptr)
    {
        parameterSeparator2->setTheme(inTheme);
    }

    if (fmModByPressureLabel != nullptr)
    {
        fmModByPressureLabel->setTheme(inTheme);
    }

    if (fmModByPressureSlider != nullptr)
    {
        fmModByPressureSlider->setTheme(inTheme);
    }

    if (parameterSeparator3 != nullptr)
    {
        parameterSeparator3->setTheme(inTheme);
    }

    if (trackPoint1Label != nullptr)
    {
        trackPoint1Label->setTheme(inTheme);
    }

    if (trackPoint1Slider != nullptr)
    {
        trackPoint1Slider->setTheme(inTheme);
    }

    if (parameterSeparator4 != nullptr)
    {
        parameterSeparator4->setTheme(inTheme);
    }

    if (trackPoint2Label != nullptr)
    {
        trackPoint2Label->setTheme(inTheme);
    }

    if (trackPoint2Slider != nullptr)
    {
        trackPoint2Slider->setTheme(inTheme);
    }

    if (parameterSeparator5 != nullptr)
    {
        parameterSeparator5->setTheme(inTheme);
    }

    if (trackPoint3Label != nullptr)
    {
        trackPoint3Label->setTheme(inTheme);
    }

    if (trackPoint3Slider != nullptr)
    {
        trackPoint3Slider->setTheme(inTheme);
    }

    if (parameterSeparator6 != nullptr)
    {
        parameterSeparator6->setTheme(inTheme);
    }

    if (trackPoint4Label != nullptr)
    {
        trackPoint4Label->setTheme(inTheme);
    }

    if (trackPoint4Slider != nullptr)
    {
        trackPoint4Slider->setTheme(inTheme);
    }

    if (parameterSeparator7 != nullptr)
    {
        parameterSeparator7->setTheme(inTheme);
    }

    if (trackPoint5Label != nullptr)
    {
        trackPoint5Label->setTheme(inTheme);
    }

    if (trackPoint5Slider != nullptr)
    {
        trackPoint5Slider->setTheme(inTheme);
    }

    if (parameterSeparator8 != nullptr)
    {
        parameterSeparator8->setTheme(inTheme);
    }

    if (trackInputLabel != nullptr)
    {
        trackInputLabel->setTheme(inTheme);
    }

    if (trackInputComboBox != nullptr)
    {
        trackInputComboBox->setTheme(inTheme);
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
