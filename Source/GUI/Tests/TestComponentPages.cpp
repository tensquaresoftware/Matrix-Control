// Extracted from TestComponent.cpp for modular maintenance.
// Test-page construction, skinning, visibility, and content-host layout.

#include "TestComponent.h"

#include <functional>

#include "GUI/Factories/WidgetFactory.h"
#include "GUI/Skins/ISkin.h"
#include "GUI/Tests/TestActivityLeds.h"
#include "GUI/Tests/TestButtons.h"
#include "GUI/Tests/TestComboBoxes.h"
#include "GUI/Tests/TestHierarchicalComboBoxes.h"
#include "GUI/Tests/TestEnvelopeDisplays.h"
#include "GUI/Tests/TestGroupLabels.h"
#include "GUI/Tests/TestHorizontalSeparators.h"
#include "GUI/Tests/TestLabels.h"
#include "GUI/Tests/TestModulationBusCells.h"
#include "GUI/Tests/TestModulationBusHeaders.h"
#include "GUI/Tests/TestModuleHeaders.h"
#include "GUI/Tests/TestNumberBoxes.h"
#include "GUI/Tests/TestParameterCells.h"
#include "GUI/Tests/TestPatchNameDisplays.h"
#include "GUI/Tests/TestPeakIndicators.h"
#include "GUI/Tests/TestPopupMenus.h"
#include "GUI/Tests/TestSectionHeaders.h"
#include "GUI/Tests/TestSliders.h"
#include "GUI/Tests/TestToggles.h"
#include "GUI/Tests/TestScaleColumns.h"
#include "GUI/Tests/TestTrackGeneratorDisplays.h"

namespace
{
    struct TestPageEntry
    {
        TestComponent::TestWidgetType type;
        juce::Component* component = nullptr;
        std::function<int()> preferredWidth;
        std::function<int()> preferredHeight;
    };

    template <typename Page>
    TestPageEntry makeTestPageEntry(TestComponent::TestWidgetType type, Page* page)
    {
        return {
            type,
            page,
            [page] { return page != nullptr ? page->getPreferredWidth() : 0; },
            [page] { return page != nullptr ? page->getPreferredHeight() : 0; }
        };
    }

    template <typename Page>
    void applyPageSkin(Page* page, TSS::ISkin& skin)
    {
        if (page != nullptr)
            page->setSkin(skin);
    }

    void setVisibleForType(TestComponent::TestWidgetType type,
                           juce::Component* component,
                           int selectedId)
    {
        if (component != nullptr)
            component->setVisible(selectedId == static_cast<int>(type));
    }

    struct PageLayoutState
    {
        float contentZoom = 1.0f;
        int contentWidth = 0;
        int contentHeight = 0;
    };

    void layoutVisiblePage(const TestPageEntry& page, PageLayoutState& state)
    {
        if (page.component == nullptr || !page.component->isVisible())
            return;

        state.contentWidth = juce::jmax(state.contentWidth, page.preferredWidth());
        state.contentHeight = juce::jmax(state.contentHeight, page.preferredHeight());

        const int zoomedWidth = juce::jmax(
            1, juce::roundToInt(static_cast<float>(page.preferredWidth()) * state.contentZoom));
        const int zoomedHeight = juce::jmax(
            1, juce::roundToInt(static_cast<float>(page.preferredHeight()) * state.contentZoom));
        page.component->setBounds(0, 0, zoomedWidth, zoomedHeight);
    }

    bool tryLayoutSelectedPage(const TestPageEntry& page, int selectedId, PageLayoutState& state)
    {
        if (static_cast<int>(page.type) != selectedId || page.component == nullptr)
            return false;

        state.contentWidth = page.preferredWidth();
        state.contentHeight = page.preferredHeight();

        const int zoomedWidth = juce::jmax(
            1, juce::roundToInt(static_cast<float>(state.contentWidth) * state.contentZoom));
        const int zoomedHeight = juce::jmax(
            1, juce::roundToInt(static_cast<float>(state.contentHeight) * state.contentZoom));
        page.component->setBounds(0, 0, zoomedWidth, zoomedHeight);
        return true;
    }
}

void TestComponent::createTestPages(juce::AudioProcessorValueTreeState& apvts)
{
    createBasicWidgetTestPages();
    createHeaderAndDisplayTestPages();
    createCellAndMenuTestPages(apvts);
}

void TestComponent::createBasicWidgetTestPages()
{
    const auto& patchManager = layoutDimensions_.body.shared.patchManager;

    testButtons_ = std::make_unique<TestButtons>(*skin_);
    testContentHost_->addAndMakeVisible(*testButtons_);

    testToggles_ = std::make_unique<TestToggles>(*skin_, patchManager.patchMutator.toggles);
    testContentHost_->addAndMakeVisible(*testToggles_);

    testSliders_ = std::make_unique<TestSliders>(*skin_);
    testContentHost_->addAndMakeVisible(*testSliders_);

    testComboBoxes_ = std::make_unique<TestComboBoxes>(*skin_);
    testContentHost_->addAndMakeVisible(*testComboBoxes_);

    testHierarchicalComboBoxes_ = std::make_unique<TestHierarchicalComboBoxes>(
        *skin_,
        patchManager.patchMutator.comboBoxes.patchMutatorHistoryWidth,
        patchManager.patchMutator.comboBoxes.standardHeight);
    testContentHost_->addAndMakeVisible(*testHierarchicalComboBoxes_);

    testNumberBoxes_ = std::make_unique<TestNumberBoxes>(*skin_, patchManager.internalPatches.numberBoxes);
    testContentHost_->addAndMakeVisible(*testNumberBoxes_);

    testLabels_ = std::make_unique<TestLabels>(*skin_);
    testContentHost_->addAndMakeVisible(*testLabels_);

    testGroupLabels_ = std::make_unique<TestGroupLabels>(*skin_, patchManager.internalPatches.groupLabels);
    testContentHost_->addAndMakeVisible(*testGroupLabels_);
}

void TestComponent::createHeaderAndDisplayTestPages()
{
    const auto& patchEdit = layoutDimensions_.body.patchEdit;
    const int testRowWidth = layoutDimensions_.editor.width - (2 * kPadding_);

    auto moduleHeaderDims = layoutDimensions_.patchEditModuleHeader;
    moduleHeaderDims.patchEditTitleBandWidth = TestScaleColumns::bandWidthForTestRow(testRowWidth);
    testModuleHeaders_ = std::make_unique<TestModuleHeaders>(*skin_, moduleHeaderDims);
    testContentHost_->addAndMakeVisible(*testModuleHeaders_);

    constexpr int kSectionHeaderLineStartDesign = 86;
    const int sectionHeaderTestWidthAtFive = patchEdit.sectionHeaderWidth / 5;
    const int sectionHeaderTestWidth = kSectionHeaderLineStartDesign
        + (sectionHeaderTestWidthAtFive - kSectionHeaderLineStartDesign) / 2;
    testSectionHeaders_ = std::make_unique<TestSectionHeaders>(
        *skin_,
        sectionHeaderTestWidth,
        patchEdit.sectionHeaderHeight);
    testContentHost_->addAndMakeVisible(*testSectionHeaders_);

    testEnvelopeDisplays_ = std::make_unique<TestEnvelopeDisplays>(*skin_, patchEdit.displays.childBand);
    testContentHost_->addAndMakeVisible(*testEnvelopeDisplays_);

    testTrackGeneratorDisplays_ = std::make_unique<TestTrackGeneratorDisplays>(*skin_, patchEdit.displays.trackGeneratorBand);
    testContentHost_->addAndMakeVisible(*testTrackGeneratorDisplays_);

    testPatchNameDisplays_ = std::make_unique<TestPatchNameDisplays>(*skin_, patchEdit.displays.patchName);
    testContentHost_->addAndMakeVisible(*testPatchNameDisplays_);

    testHorizontalSeparators_ = std::make_unique<TestHorizontalSeparators>(
        *skin_,
        layoutDimensions_.patchEditParameterCell.separatorWidth,
        layoutDimensions_.patchEditParameterCell.horizontalSeparatorHeight);
    testContentHost_->addAndMakeVisible(*testHorizontalSeparators_);

    testActivityLeds_ = std::make_unique<TestActivityLeds>(*skin_, layoutDimensions_.activityLed.size);
    testContentHost_->addAndMakeVisible(*testActivityLeds_);

    testPeakIndicators_ = std::make_unique<TestPeakIndicators>(
        *skin_,
        layoutDimensions_.activityLed.size,
        layoutDimensions_.patchEditParameterCell.comboBoxHeight);
    testContentHost_->addAndMakeVisible(*testPeakIndicators_);
}

void TestComponent::createCellAndMenuTestPages(juce::AudioProcessorValueTreeState& apvts)
{
    const auto& matrixMod = layoutDimensions_.body.shared.matrixModulation;
    const auto& patchManager = layoutDimensions_.body.shared.patchManager;

    testParameterCells_ = std::make_unique<TestParameterCells>(
        *skin_,
        *widgetFactory_,
        apvts,
        layoutDimensions_.patchEditParameterCell);
    testContentHost_->addAndMakeVisible(*testParameterCells_);

    testModulationBusCells_ = std::make_unique<TestModulationBusCells>(
        *skin_,
        *widgetFactory_,
        apvts,
        matrixMod.busCell,
        matrixMod.width,
        matrixMod.modulationBusRowHeight);
    testContentHost_->addAndMakeVisible(*testModulationBusCells_);

    testModulationBusHeaders_ = std::make_unique<TestModulationBusHeaders>(
        *skin_,
        matrixMod.busHeaderWidth,
        matrixMod.busHeaderHeight,
        matrixMod.busHeader);
    testContentHost_->addAndMakeVisible(*testModulationBusHeaders_);

    testPopupMenus_ = std::make_unique<TestPopupMenus>(
        *skin_,
        patchManager.computerPatches.comboBoxes.patchManagerComputerPatchesWidth,
        patchManager.computerPatches.comboBoxes.standardHeight);
    testContentHost_->addAndMakeVisible(*testPopupMenus_);
}

void TestComponent::applySkinToTestPages(TSS::ISkin& skin)
{
    applyPageSkin(testButtons_.get(), skin);
    applyPageSkin(testToggles_.get(), skin);
    applyPageSkin(testSliders_.get(), skin);
    applyPageSkin(testComboBoxes_.get(), skin);
    applyPageSkin(testHierarchicalComboBoxes_.get(), skin);
    applyPageSkin(testNumberBoxes_.get(), skin);
    applyPageSkin(testLabels_.get(), skin);
    applyPageSkin(testGroupLabels_.get(), skin);
    applyPageSkin(testModuleHeaders_.get(), skin);
    applyPageSkin(testSectionHeaders_.get(), skin);
    applyPageSkin(testEnvelopeDisplays_.get(), skin);
    applyPageSkin(testTrackGeneratorDisplays_.get(), skin);
    applyPageSkin(testPatchNameDisplays_.get(), skin);
    applyPageSkin(testHorizontalSeparators_.get(), skin);
    applyPageSkin(testActivityLeds_.get(), skin);
    applyPageSkin(testPeakIndicators_.get(), skin);
    applyPageSkin(testParameterCells_.get(), skin);
    applyPageSkin(testModulationBusCells_.get(), skin);
    applyPageSkin(testModulationBusHeaders_.get(), skin);
    applyPageSkin(testPopupMenus_.get(), skin);
}

void TestComponent::populateWidgetSelector()
{
    widgetSelector_.addItem("Button", static_cast<int>(TestWidgetType::Button));
    widgetSelector_.addItem("Toggle", static_cast<int>(TestWidgetType::Toggle));
    widgetSelector_.addItem("Slider", static_cast<int>(TestWidgetType::Slider));
    widgetSelector_.addItem("ComboBox", static_cast<int>(TestWidgetType::ComboBox));
    widgetSelector_.addItem("HierarchicalComboBox", static_cast<int>(TestWidgetType::HierarchicalComboBox));
    widgetSelector_.addItem("NumberBox", static_cast<int>(TestWidgetType::NumberBox));
    widgetSelector_.addItem("Label", static_cast<int>(TestWidgetType::Label));
    widgetSelector_.addItem("GroupLabel", static_cast<int>(TestWidgetType::GroupLabel));
    widgetSelector_.addItem("ModuleHeader", static_cast<int>(TestWidgetType::ModuleHeader));
    widgetSelector_.addItem("SectionHeader", static_cast<int>(TestWidgetType::SectionHeader));
    widgetSelector_.addItem("EnvelopeDisplay", static_cast<int>(TestWidgetType::EnvelopeDisplay));
    widgetSelector_.addItem("TrackGeneratorDisplay", static_cast<int>(TestWidgetType::TrackGeneratorDisplay));
    widgetSelector_.addItem("PatchNameDisplay", static_cast<int>(TestWidgetType::PatchNameDisplay));
    widgetSelector_.addItem("HorizontalSeparator", static_cast<int>(TestWidgetType::HorizontalSeparator));
    widgetSelector_.addItem("Led", static_cast<int>(TestWidgetType::ActivityLed));
    widgetSelector_.addItem("PeakIndicator", static_cast<int>(TestWidgetType::PeakIndicator));
    widgetSelector_.addItem("ParameterCell", static_cast<int>(TestWidgetType::ParameterCell));
    widgetSelector_.addItem("ModulationBusCell", static_cast<int>(TestWidgetType::ModulationBusCell));
    widgetSelector_.addItem("ModulationBusHeader", static_cast<int>(TestWidgetType::ModulationBusHeader));
    widgetSelector_.addItem("PopupMenu", static_cast<int>(TestWidgetType::PopupMenu));
}

void TestComponent::updateVisibleTests()
{
    const int selectedId = widgetSelector_.getSelectedId();

    setVisibleForType(TestWidgetType::Button, testButtons_.get(), selectedId);
    setVisibleForType(TestWidgetType::Toggle, testToggles_.get(), selectedId);
    setVisibleForType(TestWidgetType::Slider, testSliders_.get(), selectedId);
    setVisibleForType(TestWidgetType::ComboBox, testComboBoxes_.get(), selectedId);
    setVisibleForType(TestWidgetType::HierarchicalComboBox, testHierarchicalComboBoxes_.get(), selectedId);
    setVisibleForType(TestWidgetType::NumberBox, testNumberBoxes_.get(), selectedId);
    setVisibleForType(TestWidgetType::Label, testLabels_.get(), selectedId);
    setVisibleForType(TestWidgetType::GroupLabel, testGroupLabels_.get(), selectedId);
    setVisibleForType(TestWidgetType::ModuleHeader, testModuleHeaders_.get(), selectedId);
    setVisibleForType(TestWidgetType::SectionHeader, testSectionHeaders_.get(), selectedId);
    setVisibleForType(TestWidgetType::EnvelopeDisplay, testEnvelopeDisplays_.get(), selectedId);
    setVisibleForType(TestWidgetType::TrackGeneratorDisplay, testTrackGeneratorDisplays_.get(), selectedId);
    setVisibleForType(TestWidgetType::PatchNameDisplay, testPatchNameDisplays_.get(), selectedId);
    setVisibleForType(TestWidgetType::HorizontalSeparator, testHorizontalSeparators_.get(), selectedId);
    setVisibleForType(TestWidgetType::ActivityLed, testActivityLeds_.get(), selectedId);
    setVisibleForType(TestWidgetType::PeakIndicator, testPeakIndicators_.get(), selectedId);
    setVisibleForType(TestWidgetType::ParameterCell, testParameterCells_.get(), selectedId);
    setVisibleForType(TestWidgetType::ModulationBusCell, testModulationBusCells_.get(), selectedId);
    setVisibleForType(TestWidgetType::ModulationBusHeader, testModulationBusHeaders_.get(), selectedId);
    setVisibleForType(TestWidgetType::PopupMenu, testPopupMenus_.get(), selectedId);

    layoutTestContentHost();
    repaintVisualDebugLayers();
}

void TestComponent::layoutTestContentHost()
{
    if (testContentHost_ == nullptr)
        return;

    const TestPageEntry pages[] =
    {
        makeTestPageEntry(TestWidgetType::Button, testButtons_.get()),
        makeTestPageEntry(TestWidgetType::Toggle, testToggles_.get()),
        makeTestPageEntry(TestWidgetType::Slider, testSliders_.get()),
        makeTestPageEntry(TestWidgetType::ComboBox, testComboBoxes_.get()),
        makeTestPageEntry(TestWidgetType::HierarchicalComboBox, testHierarchicalComboBoxes_.get()),
        makeTestPageEntry(TestWidgetType::NumberBox, testNumberBoxes_.get()),
        makeTestPageEntry(TestWidgetType::Label, testLabels_.get()),
        makeTestPageEntry(TestWidgetType::GroupLabel, testGroupLabels_.get()),
        makeTestPageEntry(TestWidgetType::ModuleHeader, testModuleHeaders_.get()),
        makeTestPageEntry(TestWidgetType::SectionHeader, testSectionHeaders_.get()),
        makeTestPageEntry(TestWidgetType::EnvelopeDisplay, testEnvelopeDisplays_.get()),
        makeTestPageEntry(TestWidgetType::TrackGeneratorDisplay, testTrackGeneratorDisplays_.get()),
        makeTestPageEntry(TestWidgetType::PatchNameDisplay, testPatchNameDisplays_.get()),
        makeTestPageEntry(TestWidgetType::HorizontalSeparator, testHorizontalSeparators_.get()),
        makeTestPageEntry(TestWidgetType::ActivityLed, testActivityLeds_.get()),
        makeTestPageEntry(TestWidgetType::PeakIndicator, testPeakIndicators_.get()),
        makeTestPageEntry(TestWidgetType::ParameterCell, testParameterCells_.get()),
        makeTestPageEntry(TestWidgetType::ModulationBusCell, testModulationBusCells_.get()),
        makeTestPageEntry(TestWidgetType::ModulationBusHeader, testModulationBusHeaders_.get()),
        makeTestPageEntry(TestWidgetType::PopupMenu, testPopupMenus_.get()),
    };

    const int selectedId = widgetSelector_.getSelectedId();
    PageLayoutState state { contentZoom_ };

    for (const auto& page : pages)
        layoutVisiblePage(page, state);

    if (state.contentWidth == 0 || state.contentHeight == 0)
    {
        for (const auto& page : pages)
        {
            if (tryLayoutSelectedPage(page, selectedId, state))
                break;
        }
    }

    const int viewportHeight = juce::jmax(1, testViewport_.getHeight());
    baseContentWidth_ = juce::jmax(state.contentWidth, 1);
    baseContentHeight_ = juce::jmax(state.contentHeight, viewportHeight);
    applyContentZoom();
}
