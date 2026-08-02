#include "MiscPanel.h"

#include "GUI/Skins/Skin.h"
#include "GUI/Panels/Reusable/ModulePanelConfigBuilder.h"
#include "Shared/Definitions/PluginIDs.h"
#include "GUI/Factories/WidgetFactory.h"


ModulePanelLayout MiscPanel::createLayout()
{
    return makeMasterEditModuleLayout(
        PluginIDs::MasterEditSection::MiscModule::kGroupId,
        PluginIDs::MasterEditSection::MiscModule::StandaloneWidgets::kInit,
        {
            PluginIDs::MasterEditSection::MiscModule::ParameterWidgets::kMasterTune,
            PluginIDs::MasterEditSection::MiscModule::ParameterWidgets::kMasterTranspose,
            PluginIDs::MasterEditSection::MiscModule::ParameterWidgets::kBendRange,
            PluginIDs::MasterEditSection::MiscModule::ParameterWidgets::kUnisonEnable,
            PluginIDs::MasterEditSection::MiscModule::ParameterWidgets::kVolumeInvertEnable,
            PluginIDs::MasterEditSection::MiscModule::ParameterWidgets::kBankLockEnable,
            PluginIDs::MasterEditSection::MiscModule::ParameterWidgets::kMemoryProtectEnable,
            ""
        });
}

MiscPanel::MiscPanel(const Config& config)
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
