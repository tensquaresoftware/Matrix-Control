#include "Env3Panel.h"

#include "GUI/Skins/Skin.h"
#include "GUI/Panels/Reusable/ModulePanelConfigBuilder.h"
#include "Shared/Definitions/PluginIDs.h"
#include "GUI/Factories/WidgetFactory.h"


ModulePanelLayout Env3Panel::createLayout()
{
    return makePatchEditModuleLayout(
        PluginIDs::PatchEditSection::Envelope3Module::kGroupId,
        {
            .initWidgetId = PluginIDs::PatchEditSection::Envelope3Module::StandaloneWidgets::kInit,
            .copyWidgetId = PluginIDs::PatchEditSection::Envelope3Module::StandaloneWidgets::kCopy,
            .pasteWidgetId = PluginIDs::PatchEditSection::Envelope3Module::StandaloneWidgets::kPaste,
            .pasteEnabledPropertyId = PluginIDs::PatchEditSection::Envelope3Module::StandaloneWidgets::kPasteEnabled
        },
        {
            PluginIDs::PatchEditSection::Envelope3Module::ParameterWidgets::kDelay,
            PluginIDs::PatchEditSection::Envelope3Module::ParameterWidgets::kAttack,
            PluginIDs::PatchEditSection::Envelope3Module::ParameterWidgets::kDecay,
            PluginIDs::PatchEditSection::Envelope3Module::ParameterWidgets::kSustain,
            PluginIDs::PatchEditSection::Envelope3Module::ParameterWidgets::kRelease,
            PluginIDs::PatchEditSection::Envelope3Module::ParameterWidgets::kAmplitude,
            PluginIDs::PatchEditSection::Envelope3Module::ParameterWidgets::kAmplitudeModByVelocity,
            PluginIDs::PatchEditSection::Envelope3Module::ParameterWidgets::kTriggerMode,
            PluginIDs::PatchEditSection::Envelope3Module::ParameterWidgets::kEnvelopeMode,
            PluginIDs::PatchEditSection::Envelope3Module::ParameterWidgets::kLfo1Trigger
        });
}

Env3Panel::Env3Panel(const Config& config)
    : BaseModulePanel(BaseModulePanel::Config{
          .skin = config.skin,
          .widgetFactory = config.widgetFactory,
          .apvts = config.apvts,
          .layout = createLayout(),
          .width = config.width,
          .height = config.height,
          .moduleHeaderDims = config.moduleHeaderDims,
          .parameterCellDims = config.parameterCellDims})
{
}
