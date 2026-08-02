#include "Lfo2Panel.h"

#include "GUI/Skins/Skin.h"
#include "GUI/Panels/Reusable/ModulePanelConfigBuilder.h"
#include "Shared/Definitions/PluginIDs.h"
#include "GUI/Factories/WidgetFactory.h"


ModulePanelLayout Lfo2Panel::createLayout()
{
    return makePatchEditModuleLayout(
        PluginIDs::PatchEditSection::Lfo2Module::kGroupId,
        {
            .initWidgetId = PluginIDs::PatchEditSection::Lfo2Module::StandaloneWidgets::kInit,
            .copyWidgetId = PluginIDs::PatchEditSection::Lfo2Module::StandaloneWidgets::kCopy,
            .pasteWidgetId = PluginIDs::PatchEditSection::Lfo2Module::StandaloneWidgets::kPaste,
            .pasteEnabledPropertyId = PluginIDs::PatchEditSection::Lfo2Module::StandaloneWidgets::kPasteEnabled
        },
        {
            PluginIDs::PatchEditSection::Lfo2Module::ParameterWidgets::kSpeed,
            PluginIDs::PatchEditSection::Lfo2Module::ParameterWidgets::kSpeedModByKeyboard,
            PluginIDs::PatchEditSection::Lfo2Module::ParameterWidgets::kRetriggerPoint,
            PluginIDs::PatchEditSection::Lfo2Module::ParameterWidgets::kAmplitude,
            PluginIDs::PatchEditSection::Lfo2Module::ParameterWidgets::kAmplitudeModByRamp2,
            PluginIDs::PatchEditSection::Lfo2Module::ParameterWidgets::kWaveform,
            PluginIDs::PatchEditSection::Lfo2Module::ParameterWidgets::kTriggerMode,
            PluginIDs::PatchEditSection::Lfo2Module::ParameterWidgets::kLag,
            PluginIDs::PatchEditSection::Lfo2Module::ParameterWidgets::kSampleInput,
            ""
        });
}

Lfo2Panel::Lfo2Panel(TSS::ISkin& skin, int width, int height, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts,
                         const ModuleHeaderDimensions& moduleHeaderDims, const ParameterCellDimensions& parameterCellDims)
    : BaseModulePanel(BaseModulePanel::Config{
          .skin = skin,
          .widgetFactory = widgetFactory,
          .apvts = apvts,
          .layout = createLayout(),
          .width = width,
          .height = height,
          .moduleHeaderDims = moduleHeaderDims,
          .parameterCellDims = parameterCellDims})
{
}
