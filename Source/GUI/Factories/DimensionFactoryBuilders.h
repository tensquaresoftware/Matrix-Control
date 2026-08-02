#pragma once

#include "GUI/Layout/PanelDimensions.h"

// Internal helpers for DimensionFactory — not part of the public factory API.
namespace DimensionFactoryBuilders
{
    ButtonDimensions buildButtonDimensions();
    ParameterCellDimensions buildPatchEditParameterCellDimensions();
    ParameterCellDimensions buildMasterEditParameterCellDimensions();
    PopupMenuLayoutDimensions buildPopupMenuLayoutDimensions();
    ModuleHeaderDimensions buildPatchEditModuleHeaderDimensions(const ButtonDimensions& buttons);
    ModulationBusHeaderDimensions buildModulationBusHeaderDimensions();
    DisplayBandDimensions buildDisplayBandDimensions(int width, int height, bool includeSustainHitZone);
    ModulationBusCellDimensions buildModulationBusCellDimensions(const ButtonDimensions& buttons);
    PatchManagerModuleLayoutDimensions buildPatchManagerModuleLayout();
    BankUtilityPanelDimensions buildBankUtilityPanelDimensions(
        int width, int height, const ModuleHeaderDimensions& moduleHeader, const ButtonDimensions& buttons);
    InternalPatchesPanelDimensions buildInternalPatchesPanelDimensions(
        int width, int height, const ModuleHeaderDimensions& moduleHeader, const ButtonDimensions& buttons);
    ComputerPatchesPanelDimensions buildComputerPatchesPanelDimensions(
        int width, int height, const ModuleHeaderDimensions& moduleHeader, const ButtonDimensions& buttons);
    PatchMutatorPanelDimensions buildPatchMutatorPanelDimensions(
        int width, int height, const ModuleHeaderDimensions& moduleHeader, const ButtonDimensions& buttons);
}
