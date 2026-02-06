#include "MiddlePanel.h"

#include "GUI/Themes/Skin.h"
#include "Shared/PluginDescriptors.h"


MiddlePanel::~MiddlePanel()
{
    if (apvts_ != nullptr && apvts_->state.isValid())
        apvts_->state.removeListener(this);
}

MiddlePanel::MiddlePanel(tss::Skin& skin, juce::AudioProcessorValueTreeState& apvts)
    : skin_(&skin)
    , apvts_(&apvts)
    , envelope1Display_(skin, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kWidth, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kHeight)
    , envelope2Display_(skin, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kWidth, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kHeight)
    , envelope3Display_(skin, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kWidth, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kHeight)
    , trackGeneratorDisplay_(skin, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kWidth, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kHeight)
    , patchNameDisplay_(skin, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kWidth, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kHeight)
{
    setOpaque(false);
    setSize(getWidth(), getHeight());

    apvts_->state.addListener(this);
    syncTrackGeneratorDisplayFromApvts();

    trackGeneratorDisplay_.setOnValueChanged([this](int pointIndex, int newValue)
    {
        if (apvts_ == nullptr)
            return;

        const char* parameterIds[] = {
            PluginDescriptors::ParameterIds::kTrackPoint1,
            PluginDescriptors::ParameterIds::kTrackPoint2,
            PluginDescriptors::ParameterIds::kTrackPoint3,
            PluginDescriptors::ParameterIds::kTrackPoint4,
            PluginDescriptors::ParameterIds::kTrackPoint5
        };

        if (pointIndex < 0 || pointIndex >= 5)
            return;

        if (auto* param = apvts_->getParameter(parameterIds[pointIndex]))
        {
            const float normalised = static_cast<float>(newValue) / kTrackPointMax;
            param->setValueNotifyingHost(normalised);
        }
    });

    addAndMakeVisible(envelope1Display_);
    addAndMakeVisible(envelope2Display_);
    addAndMakeVisible(envelope3Display_);
    addAndMakeVisible(trackGeneratorDisplay_);
    addAndMakeVisible(patchNameDisplay_);
}

void MiddlePanel::resized()
{
    const auto childWidth = PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kWidth;
    const auto childHeight = PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kHeight;

    envelope1Display_.setBounds(0, 0, childWidth, childHeight);
    envelope2Display_.setBounds(envelope1Display_.getBounds().getRight() + kSpacing, 0, childWidth, childHeight);
    envelope3Display_.setBounds(envelope2Display_.getBounds().getRight() + kSpacing, 0, childWidth, childHeight);
    trackGeneratorDisplay_.setBounds(envelope3Display_.getBounds().getRight() + kSpacing, 0, childWidth, childHeight);
    patchNameDisplay_.setBounds(trackGeneratorDisplay_.getBounds().getRight() + kSpacing, 0, childWidth, childHeight);
}

void MiddlePanel::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged,
                                          const juce::Identifier& property)
{
    if (property != juce::Identifier("value"))
        return;

    const juce::String typeStr = treeWhosePropertyHasChanged.getType().toString();
    if (typeStr != "PARAM")
        return;

    const juce::var idVar = treeWhosePropertyHasChanged.getProperty("id");
    if (!idVar.isString())
        return;

    const juce::String paramId = idVar.toString();
    if (paramId == PluginDescriptors::ParameterIds::kTrackPoint1)
    {
        trackGeneratorDisplay_.setTrackPoint1(getTrackPointValueFromApvts(paramId));
        return;
    }
    if (paramId == PluginDescriptors::ParameterIds::kTrackPoint2)
    {
        trackGeneratorDisplay_.setTrackPoint2(getTrackPointValueFromApvts(paramId));
        return;
    }
    if (paramId == PluginDescriptors::ParameterIds::kTrackPoint3)
    {
        trackGeneratorDisplay_.setTrackPoint3(getTrackPointValueFromApvts(paramId));
        return;
    }
    if (paramId == PluginDescriptors::ParameterIds::kTrackPoint4)
    {
        trackGeneratorDisplay_.setTrackPoint4(getTrackPointValueFromApvts(paramId));
        return;
    }
    if (paramId == PluginDescriptors::ParameterIds::kTrackPoint5)
    {
        trackGeneratorDisplay_.setTrackPoint5(getTrackPointValueFromApvts(paramId));
    }
}

void MiddlePanel::syncTrackGeneratorDisplayFromApvts()
{
    if (apvts_ == nullptr)
        return;

    trackGeneratorDisplay_.setTrackPoint1(getTrackPointValueFromApvts(PluginDescriptors::ParameterIds::kTrackPoint1));
    trackGeneratorDisplay_.setTrackPoint2(getTrackPointValueFromApvts(PluginDescriptors::ParameterIds::kTrackPoint2));
    trackGeneratorDisplay_.setTrackPoint3(getTrackPointValueFromApvts(PluginDescriptors::ParameterIds::kTrackPoint3));
    trackGeneratorDisplay_.setTrackPoint4(getTrackPointValueFromApvts(PluginDescriptors::ParameterIds::kTrackPoint4));
    trackGeneratorDisplay_.setTrackPoint5(getTrackPointValueFromApvts(PluginDescriptors::ParameterIds::kTrackPoint5));
}

int MiddlePanel::getTrackPointValueFromApvts(const juce::String& parameterId) const
{
    if (apvts_ == nullptr)
        return 0;

    if (auto* param = apvts_->getParameter(parameterId))
    {
        const float normalised = param->getValue();
        return juce::jlimit(0, kTrackPointMax, juce::roundToInt(normalised * static_cast<float>(kTrackPointMax)));
    }

    return 0;
}

void MiddlePanel::setSkin(tss::Skin& skin)
{
    skin_ = &skin;
    envelope1Display_.setSkin(skin);
    envelope2Display_.setSkin(skin);
    envelope3Display_.setSkin(skin);
    trackGeneratorDisplay_.setSkin(skin);
    patchNameDisplay_.setSkin(skin);
}

