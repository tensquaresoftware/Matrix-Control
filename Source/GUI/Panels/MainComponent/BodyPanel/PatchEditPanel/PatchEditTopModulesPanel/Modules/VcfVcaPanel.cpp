#include "VcfVcaPanel.h"

#include "GUI/Skins/Skin.h"
#include "GUI/Panels/Reusable/ModulePanelConfigBuilder.h"
#include "Shared/Definitions/PluginIDs.h"
#include "GUI/Factories/WidgetFactory.h"


ModulePanelLayout VcfVcaPanel::createLayout()
{
    return makePatchEditInitOnlyModuleLayout(
        PluginIDs::PatchEditSection::VcfVcaModule::kGroupId,
        PluginIDs::PatchEditSection::VcfVcaModule::StandaloneWidgets::kInit,
        {
            PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kBalance,
            PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kFrequency,
            PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kFrequencyModByEnv1,
            PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kFrequencyModByPressure,
            PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kResonance,
            PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kVca1Volume,
            PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kVca1ModByVelocity,
            PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kVca2ModByEnv2,
            PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kLevers,
            PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kKeyboardPortamento
        });
}

VcfVcaPanel::VcfVcaPanel(const Config& config)
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
