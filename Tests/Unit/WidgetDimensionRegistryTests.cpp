#include <juce_core/juce_core.h>

#include "GUI/Layout/Design/Design.h"
#include "GUI/Layout/WidgetDimensionRegistry.h"
#include "Shared/Definitions/Matrix1000Limits.h"
#include "Shared/Definitions/PluginDescriptors.h"
#include "Shared/Definitions/PluginIDs.h"

namespace
{
    namespace BW = TSS::Design::Atoms::Widths::Button;

    void expectButtonWidth(juce::UnitTest& test,
                           const juce::String& widgetId,
                           int expectedWidth,
                           const juce::String& context)
    {
        const auto width = WidgetDimensionRegistry::resolveStandaloneButtonWidth(widgetId);
        test.expect(width.has_value(), context + ": no resolvable width");
        if (! width.has_value())
            return;

        test.expect(*width == expectedWidth,
                    context + ": expected " + juce::String(expectedWidth)
                        + " got " + juce::String(*width));
    }

    void expectPositiveButtonWidth(juce::UnitTest& test,
                                   const juce::String& widgetId,
                                   const juce::String& missingLabel,
                                   const juce::String& nonPositiveLabel)
    {
        const auto width = WidgetDimensionRegistry::resolveStandaloneButtonWidth(widgetId);
        test.expect(width.has_value(), missingLabel + widgetId);
        if (width.has_value())
            test.expect(*width > 0, nonPositiveLabel + widgetId);
    }

    void expectButtonsResolvableInDescriptors(
        juce::UnitTest& test,
        const std::vector<PluginDescriptors::StandaloneWidgetDescriptor>& widgets,
        const juce::String& missingLabel,
        const juce::String& nonPositiveLabel)
    {
        for (const auto& widget : widgets)
        {
            if (widget.widgetType != PluginDescriptors::StandaloneWidgetType::kButton)
                continue;

            expectPositiveButtonWidth(test, widget.widgetId, missingLabel, nonPositiveLabel);
        }
    }

    void expectAllStandaloneButtonsResolvable(juce::UnitTest& test)
    {
        const std::vector<const std::vector<PluginDescriptors::StandaloneWidgetDescriptor>*> widgetSets = {
            &PluginDescriptors::PatchEditSection::Dco1Module::kStandaloneWidgets,
            &PluginDescriptors::PatchEditSection::Dco2Module::kStandaloneWidgets,
            &PluginDescriptors::PatchEditSection::VcfVcaModule::kStandaloneWidgets,
            &PluginDescriptors::PatchEditSection::FmTrackModule::kStandaloneWidgets,
            &PluginDescriptors::PatchEditSection::RampPortamentoModule::kStandaloneWidgets,
            &PluginDescriptors::PatchEditSection::Envelope1Module::kStandaloneWidgets,
            &PluginDescriptors::PatchEditSection::Envelope2Module::kStandaloneWidgets,
            &PluginDescriptors::PatchEditSection::Envelope3Module::kStandaloneWidgets,
            &PluginDescriptors::PatchEditSection::Lfo1Module::kStandaloneWidgets,
            &PluginDescriptors::PatchEditSection::Lfo2Module::kStandaloneWidgets,
            &PluginDescriptors::MatrixModulationSection::kStandaloneWidgets,
            &PluginDescriptors::PatchManagerSection::BankUtilityModule::kStandaloneWidgets,
            &PluginDescriptors::PatchManagerSection::InternalPatchesModule::kStandaloneWidgets,
            &PluginDescriptors::PatchManagerSection::ComputerPatchesModule::kStandaloneWidgets,
            &PluginDescriptors::PatchManagerSection::PatchMutatorModule::kStandaloneWidgets,
            &PluginDescriptors::MasterEditSection::kStandaloneWidgets,
        };

        for (const auto* widgets : widgetSets)
            expectButtonsResolvableInDescriptors(
                test,
                *widgets,
                "Missing registry width for button widgetId: ",
                "Non-positive registry width for button widgetId: ");

        for (int bus = 0; bus < Matrix1000Limits::kModulationBusCount; ++bus)
            expectButtonsResolvableInDescriptors(
                test,
                PluginDescriptors::MatrixModulationSection::kModulationBusStandaloneWidgets[static_cast<size_t>(bus)],
                "Missing registry width for bus button widgetId: ",
                "Non-positive registry width for bus button widgetId: ");
    }

    struct ExplicitButtonWidthCase
    {
        const char* widgetId;
        int expectedWidth;
        const char* context;
    };

    constexpr ExplicitButtonWidthCase kExplicitButtonWidthCases[] = {
        { PluginIDs::PatchManagerSection::BankUtilityModule::StandaloneWidgets::kCopyBank,
          BW::kPatchManagerCopyBank, "copy bank" },
        { PluginIDs::PatchManagerSection::BankUtilityModule::StandaloneWidgets::kPasteBank,
          BW::kPatchManagerPasteBank, "paste bank" },
        { PluginIDs::PatchManagerSection::BankUtilityModule::StandaloneWidgets::kImportBank,
          BW::kPatchManagerImportBank, "import bank" },
        { PluginIDs::PatchManagerSection::BankUtilityModule::StandaloneWidgets::kExportBank,
          BW::kPatchManagerExportBank, "export bank" },
        { PluginIDs::PatchManagerSection::BankUtilityModule::StandaloneWidgets::kSelectBank0,
          BW::kPatchManagerBankSelect, "select bank" },
        { PluginIDs::PatchManagerSection::InternalPatchesModule::StandaloneWidgets::kInitPatch,
          BW::kInternalPatchesInit, "internal init" },
        { PluginIDs::PatchManagerSection::InternalPatchesModule::StandaloneWidgets::kCopyPatch,
          BW::kInternalPatchesCopy, "internal copy" },
        { PluginIDs::PatchManagerSection::InternalPatchesModule::StandaloneWidgets::kPastePatch,
          BW::kInternalPatchesPaste, "internal paste" },
        { PluginIDs::PatchManagerSection::InternalPatchesModule::StandaloneWidgets::kStorePatch,
          BW::kInternalPatchesStore, "internal store" },
        { PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kOpenPatchFolder,
          BW::kComputerPatchesLoad, "computer load" },
        { PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSavePatchAs,
          BW::kComputerPatchesSaveAs, "computer save as" },
        { PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSavePatchFile,
          BW::kComputerPatchesSave, "computer save" },
        { PluginIDs::PatchManagerSection::PatchMutatorModule::StandaloneWidgets::kMutate,
          BW::kPatchMutatorMutate, "mutate" },
        { PluginIDs::PatchManagerSection::PatchMutatorModule::StandaloneWidgets::kRetry,
          BW::kPatchMutatorRetry, "retry" },
        { PluginIDs::PatchManagerSection::PatchMutatorModule::StandaloneWidgets::kCompare,
          BW::kPatchMutatorCompare, "compare" },
        { PluginIDs::PatchManagerSection::PatchMutatorModule::StandaloneWidgets::kDelete,
          BW::kPatchMutatorDelete, "delete" },
        { PluginIDs::PatchManagerSection::PatchMutatorModule::StandaloneWidgets::kClear,
          BW::kPatchMutatorClear, "clear" },
        { PluginIDs::PatchManagerSection::PatchMutatorModule::StandaloneWidgets::kExport,
          BW::kPatchMutatorExport, "export" },
        { PluginIDs::PatchManagerSection::PatchMutatorModule::StandaloneWidgets::kHistoryPrevious,
          BW::kPatchMutatorHistoryNav, "history previous" },
        { PluginIDs::PatchManagerSection::PatchMutatorModule::StandaloneWidgets::kHistoryNext,
          BW::kPatchMutatorHistoryNav, "history next" },
        { PluginIDs::PatchEditSection::Dco1Module::StandaloneWidgets::kInit,
          BW::kInit, "dco1 init suffix" },
        { PluginIDs::PatchEditSection::Dco1Module::StandaloneWidgets::kCopy,
          BW::kCopy, "dco1 copy suffix" },
        { PluginIDs::PatchEditSection::Dco1Module::StandaloneWidgets::kPaste,
          BW::kPaste, "dco1 paste suffix" },
    };

    void expectExplicitButtonWidths(juce::UnitTest& test)
    {
        for (const auto& c : kExplicitButtonWidthCases)
            expectButtonWidth(test, c.widgetId, c.expectedWidth, c.context);
    }
}

class WidgetDimensionRegistryTests : public juce::UnitTest
{
public:
    WidgetDimensionRegistryTests() : juce::UnitTest("WidgetDimensionRegistry Tests") {}

    void runTest() override
    {
        beginTest("All standalone button descriptors resolve a width");
        expectAllStandaloneButtonsResolvable(*this);

        beginTest("Explicit standalone button widths match design atoms");
        expectExplicitButtonWidths(*this);
    }
};

static WidgetDimensionRegistryTests widgetDimensionRegistryTests;
