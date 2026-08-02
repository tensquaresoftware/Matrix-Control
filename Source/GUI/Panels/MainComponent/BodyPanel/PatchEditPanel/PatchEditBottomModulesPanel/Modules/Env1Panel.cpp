#include "Env1Panel.h"

#include "GUI/Skins/Skin.h"
#include "GUI/Panels/Reusable/ModulePanelConfigBuilder.h"
#include "Shared/Definitions/PluginIDs.h"
#include "GUI/Factories/WidgetFactory.h"


ModulePanelLayout Env1Panel::createLayout()
{
    return makePatchEditModuleLayout(
        PluginIDs::PatchEditSection::Envelope1Module::kGroupId,
        {
            .initWidgetId = PluginIDs::PatchEditSection::Envelope1Module::StandaloneWidgets::kInit,
            .copyWidgetId = PluginIDs::PatchEditSection::Envelope1Module::StandaloneWidgets::kCopy,
            .pasteWidgetId = PluginIDs::PatchEditSection::Envelope1Module::StandaloneWidgets::kPaste,
            .pasteEnabledPropertyId = PluginIDs::PatchEditSection::Envelope1Module::StandaloneWidgets::kPasteEnabled
        },
        {
            PluginIDs::PatchEditSection::Envelope1Module::ParameterWidgets::kDelay,
            PluginIDs::PatchEditSection::Envelope1Module::ParameterWidgets::kAttack,
            PluginIDs::PatchEditSection::Envelope1Module::ParameterWidgets::kDecay,
            PluginIDs::PatchEditSection::Envelope1Module::ParameterWidgets::kSustain,
            PluginIDs::PatchEditSection::Envelope1Module::ParameterWidgets::kRelease,
            PluginIDs::PatchEditSection::Envelope1Module::ParameterWidgets::kAmplitude,
            PluginIDs::PatchEditSection::Envelope1Module::ParameterWidgets::kAmplitudeModByVelocity,
            PluginIDs::PatchEditSection::Envelope1Module::ParameterWidgets::kTriggerMode,
            PluginIDs::PatchEditSection::Envelope1Module::ParameterWidgets::kEnvelopeMode,
            PluginIDs::PatchEditSection::Envelope1Module::ParameterWidgets::kLfo1Trigger
        });
}

Env1Panel::Env1Panel(TSS::ISkin& skin, int width, int height, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts,
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
