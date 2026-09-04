#include "DimensionFactoryBuilders.h"

#include "GUI/Layout/Design/Design.h"

namespace DimensionFactoryBuilders
{
    using namespace TSS::Design;
    namespace BW = Atoms::Widths::Button;

    ButtonDimensions buildButtonDimensions()
    {
        return {
            .initWidth = BW::kInit,
            .copyWidth = BW::kCopy,
            .pasteWidth = BW::kPaste,
            .height = Atoms::Heights::kButton,
            .patchManagerBankSelectWidth = BW::kPatchManagerBankSelect,
            .patchManagerCopyBankWidth = BW::kPatchManagerCopyBank,
            .patchManagerPasteBankWidth = BW::kPatchManagerPasteBank,
            .patchManagerImportBankWidth = BW::kPatchManagerImportBank,
            .patchManagerExportBankWidth = BW::kPatchManagerExportBank,
            .internalPatchesInitWidth = BW::kInternalPatchesInit,
            .internalPatchesCopyWidth = BW::kInternalPatchesCopy,
            .internalPatchesPasteWidth = BW::kInternalPatchesPaste,
            .internalPatchesStoreWidth = BW::kInternalPatchesStore,
            .computerPatchesLoadWidth = BW::kComputerPatchesLoad,
            .computerPatchesSaveWidth = BW::kComputerPatchesSave,
            .computerPatchesSaveAsWidth = BW::kComputerPatchesSaveAs,
            .patchMutatorMutateWidth = BW::kPatchMutatorMutate,
            .patchMutatorRetryWidth = BW::kPatchMutatorRetry,
            .patchMutatorCompareWidth = BW::kPatchMutatorCompare,
            .patchMutatorDeleteWidth = BW::kPatchMutatorDelete,
            .patchMutatorClearWidth = BW::kPatchMutatorClear,
            .patchMutatorExportWidth = BW::kPatchMutatorExport,
            .patchMutatorHistoryNavWidth = BW::kPatchMutatorHistoryNav,
        };
    }

    ParameterCellDimensions buildPatchEditParameterCellDimensions()
    {
        return {
            .rowHeight = Recipes::ParameterCell::kHeight,
            .labelWidth = Recipes::Label::kPatchEditModule,
            .controlWidth = Recipes::ComboBox::kPatchEditModule,
            .separatorWidth = PanelWidgets::Widths::HorizontalSeparator::kPatchEditModule,
            .labelHeight = Atoms::Heights::kLabel,
            .sliderHeight = Atoms::Heights::kSlider,
            .comboBoxHeight = Atoms::Heights::kComboBox,
            .horizontalSeparatorHeight = Atoms::Heights::kHorizontalSeparator,
        };
    }

    ParameterCellDimensions buildMasterEditParameterCellDimensions()
    {
        return {
            .rowHeight = Recipes::ParameterCell::kHeight,
            .labelWidth = Recipes::Label::kMasterEditModule,
            .controlWidth = Recipes::ComboBox::kMasterEditModule,
            .separatorWidth = PanelWidgets::Widths::HorizontalSeparator::kMasterEditModule,
            .labelHeight = Atoms::Heights::kLabel,
            .sliderHeight = Atoms::Heights::kSlider,
            .comboBoxHeight = Atoms::Heights::kComboBox,
            .horizontalSeparatorHeight = Atoms::Heights::kHorizontalSeparator,
        };
    }

    PopupMenuLayoutDimensions buildPopupMenuLayoutDimensions()
    {
        return {
            .itemHeight = Atoms::Widths::PopupMenu::kItemHeight,
            .verticalMargin = Atoms::Widths::PopupMenu::kVerticalMargin,
            .scrollbarWidth = Atoms::Widths::PopupMenu::kScrollbarWidth,
            .minThumbHeight = Atoms::Widths::PopupMenu::kMinThumbHeight,
            .maxScrollHeight = Atoms::Widths::PopupMenu::kMaxScrollHeight,
            .borderThickness = Atoms::Widths::PopupMenu::kBorderThickness,
            .textLeftPadding = Atoms::Widths::PopupMenu::kTextLeftPadding,
        };
    }

    ModuleHeaderDimensions buildPatchEditModuleHeaderDimensions(const ButtonDimensions& buttons)
    {
        return {
            .height = Atoms::Heights::kModuleHeader,
            .buttonHeight = buttons.height,
            .patchEditTitleBandWidth = PanelWidgets::Widths::ModuleHeader::kPatchEditModule,
            .masterEditTitleBandWidth = PanelWidgets::Widths::ModuleHeader::kMasterEditModule,
            .patchManagerTitleBandWidth = PanelWidgets::Widths::ModuleHeader::kPatchManagerModule,
            .initWidth = buttons.initWidth,
            .copyWidth = buttons.copyWidth,
            .pasteWidth = buttons.pasteWidth,
            .textLeftPadding = Atoms::Widths::ModuleHeader::kTextLeftPadding,
            .textAreaHeight = Atoms::Widths::ModuleHeader::kTextAreaHeight,
            .lineThickness = Atoms::Widths::ModuleHeader::kLineThickness,
        };
    }

    ModulationBusHeaderDimensions buildModulationBusHeaderDimensions()
    {
        return {
            .busNumberTextWidth = Atoms::Widths::ModulationBusHeader::kBusNumberTextWidth,
            .busSourceTextWidth = Atoms::Widths::ModulationBusHeader::kBusSourceTextWidth,
            .busAmountTextWidth = Atoms::Widths::ModulationBusHeader::kBusAmountTextWidth,
            .busDestinationTextWidth = Atoms::Widths::ModulationBusHeader::kBusDestinationTextWidth,
            .interControlGap = Atoms::Widths::ModulationBus::kInterControlGap,
        };
    }

    DisplayBandDimensions buildDisplayBandDimensions(int width, int height, bool includeSustainHitZone)
    {
        return {
            .width = width,
            .height = height,
            .borderThickness = Atoms::Widths::DisplayBand::kBorderThickness,
            .paddingTop = Atoms::Widths::DisplayBand::kPaddingTop,
            .paddingBottom = Atoms::Widths::DisplayBand::kPaddingBottom,
            .triangleBase = Atoms::Widths::DisplayBand::kTriangleBase,
            .curvePadding = Atoms::Widths::DisplayBand::kCurvePadding,
            .curvePointRadius = Atoms::Widths::DisplayBand::kCurvePointRadius,
            .curveLineThickness = Atoms::Widths::DisplayBand::kCurveLineThickness,
            .minCurveSegmentWidth = Atoms::Widths::DisplayBand::kMinCurveSegmentWidth,
            .pointHitZoneRadius = Atoms::Widths::DisplayBand::kPointHitZoneRadius,
            .sustainSegmentHitZone = includeSustainHitZone ? Atoms::Widths::DisplayBand::kSustainSegmentHitZone : 0,
        };
    }

    ModulationBusCellDimensions buildModulationBusCellDimensions(const ButtonDimensions& buttons)
    {
        return {
            .panelWidth = Recipes::ModulationBusCell::kWidth,
            .panelHeight = Recipes::ModulationBusCell::kHeight,
            .busNumberLabelWidth = Atoms::Widths::Label::kModulationBusNumber,
            .busNumberLabelHeight = Atoms::Heights::kLabel,
            .sourceComboBoxWidth = Atoms::Widths::ComboBox::kMatrixModulationSource,
            .sourceComboBoxHeight = Atoms::Heights::kComboBox,
            .amountSliderWidth = Atoms::Widths::ParameterCell::kControl,
            .amountSliderHeight = Atoms::Heights::kSlider,
            .destinationComboBoxWidth = Atoms::Widths::ComboBox::kMatrixModulationDestination,
            .destinationComboBoxHeight = Atoms::Heights::kComboBox,
            .initButtonWidth = buttons.initWidth,
            .initButtonHeight = Atoms::Heights::kLabel,
            .separatorWidth = Recipes::ModulationBusCell::kWidth,
            .separatorHeight = Atoms::Heights::kHorizontalSeparator,
            .interControlGap = Atoms::Widths::ModulationBus::kInterControlGap,
            .reorderDragThreshold = Atoms::Widths::ModulationBusCell::kReorderDragThreshold,
        };
    }

    PatchManagerModuleLayoutDimensions buildPatchManagerModuleLayout()
    {
        return {
            .contentRowHeight = Recipes::PatchManagerModule::kContentRowHeight,
            .interControlGap = Recipes::PatchManagerModule::kInterControlGap,
            .columnGap = Recipes::PatchManagerModule::kColumnGap,
        };
    }

    BankUtilityPanelDimensions buildBankUtilityPanelDimensions(
        int width, int height, const ModuleHeaderDimensions& moduleHeader, const ButtonDimensions& buttons)
    {
        return {
            .width = width,
            .height = height,
            .moduleHeader = moduleHeader,
            .buttons = buttons,
            .layout = buildPatchManagerModuleLayout(),
            .selectorToUtilityGap = Recipes::BankUtilityModule::kSelectorToUtilityGap,
        };
    }

    InternalPatchesPanelDimensions buildInternalPatchesPanelDimensions(
        int width, int height, const ModuleHeaderDimensions& moduleHeader, const ButtonDimensions& buttons)
    {
        return {
            .width = width,
            .height = height,
            .moduleHeader = moduleHeader,
            .groupLabels = {
                .internalPatchesBrowserWidth = Atoms::Widths::GroupLabel::kInternalPatchesBrowser,
                .internalPatchesMemoryWidth = Atoms::Widths::GroupLabel::kInternalPatchesMemory,
                .computerPatchesBrowserWidth = Atoms::Widths::GroupLabel::kComputerPatchesBrowser,
                .computerPatchesStorageWidth = Atoms::Widths::GroupLabel::kComputerPatchesStorage,
                .height = Atoms::Heights::kGroupLabel,
            },
            .numberBoxes = {
                .bankNumberWidth = Atoms::Widths::NumberBox::kPatchManagerBankNumber,
                .patchNumberWidth = Atoms::Widths::NumberBox::kPatchManagerPatchNumber,
                .height = Atoms::Heights::kNumberBox,
            },
            .buttons = buttons,
            .layout = buildPatchManagerModuleLayout(),
        };
    }

    ComputerPatchesPanelDimensions buildComputerPatchesPanelDimensions(
        int width, int height, const ModuleHeaderDimensions& moduleHeader, const ButtonDimensions& buttons)
    {
        return {
            .width = width,
            .height = height,
            .moduleHeader = moduleHeader,
            .groupLabels = {
                .internalPatchesBrowserWidth = Atoms::Widths::GroupLabel::kInternalPatchesBrowser,
                .internalPatchesMemoryWidth = Atoms::Widths::GroupLabel::kInternalPatchesMemory,
                .computerPatchesBrowserWidth = Atoms::Widths::GroupLabel::kComputerPatchesBrowser,
                .computerPatchesStorageWidth = Atoms::Widths::GroupLabel::kComputerPatchesStorage,
                .height = Atoms::Heights::kGroupLabel,
            },
            .comboBoxes = {
                .standardHeight = Atoms::Heights::kComboBox,
                .matrixModulationSourceWidth = Atoms::Widths::ComboBox::kMatrixModulationSource,
                .matrixModulationDestinationWidth = Atoms::Widths::ComboBox::kMatrixModulationDestination,
                .patchManagerComputerPatchesWidth = Atoms::Widths::ComboBox::kPatchManagerComputerPatches,
                .patchMutatorHistoryWidth = Atoms::Widths::ComboBox::kPatchMutatorHistory,
                .popup = buildPopupMenuLayoutDimensions(),
            },
            .buttons = buttons,
            .layout = buildPatchManagerModuleLayout(),
        };
    }

    PatchMutatorPanelDimensions buildPatchMutatorPanelDimensions(
        int width, int height, const ModuleHeaderDimensions& moduleHeader, const ButtonDimensions& buttons)
    {
        return {
            .width = width,
            .height = height,
            .moduleHeader = moduleHeader,
            .labels = {
                .modulationBusNumberWidth = Atoms::Widths::Label::kModulationBusNumber,
                .patchManagerSelectBankWidth = Atoms::Widths::Label::kPatchManagerSelectBank,
                .patchMutatorWidth = Atoms::Widths::Label::kPatchMutator,
                .height = Atoms::Heights::kLabel,
            },
            .sliders = {
                .standardWidth = Recipes::Slider::kStandard,
                .standardHeight = Atoms::Heights::kSlider,
                .patchMutatorWidth = Atoms::Widths::Slider::kPatchMutator,
            },
            .toggles = {
                .patchMutatorWidth = Atoms::Widths::Toggle::kPatchMutator,
                .height = Atoms::Heights::kToggle,
            },
            .comboBoxes = {
                .standardHeight = Atoms::Heights::kComboBox,
                .matrixModulationSourceWidth = Atoms::Widths::ComboBox::kMatrixModulationSource,
                .matrixModulationDestinationWidth = Atoms::Widths::ComboBox::kMatrixModulationDestination,
                .patchManagerComputerPatchesWidth = Atoms::Widths::ComboBox::kPatchManagerComputerPatches,
                .patchMutatorHistoryWidth = Atoms::Widths::ComboBox::kPatchMutatorHistory,
                .popup = buildPopupMenuLayoutDimensions(),
            },
            .buttons = buttons,
            .layout = buildPatchManagerModuleLayout(),
        };
    }
}
