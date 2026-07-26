#include "PluginEditor.h"

#include <juce_gui_basics/juce_gui_basics.h>

#include "Core/Audio/AudioPassthroughProcessor.h"
#include "Core/Audio/StandaloneAudioInputRouter.h"
#include "Core/MIDI/MidiActivityTracker.h"
#include "Core/PluginProcessor.h"
#include "Core/Services/PatchFileNameReconciler.h"
#include "Core/Services/MutatorDeleteWarningPolicy.h"
#include "GUI/Layout/ScaledLayout.h"
#include "GUI/Widgets/ComboBox.h"
#include "GUI/Panels/MainComponent/HeaderPanel/HeaderPanel.h"
#include "GUI/Panels/MainComponent/BodyPanel/PatchEditPanel/PatchEditPanel.h"
#include "GUI/Panels/MainComponent/BodyPanel/PatchEditPanel/PatchEditDisplaysPanel/PatchEditDisplaysPanel.h"
#include "GUI/Panels/MainComponent/BodyPanel/PatchEditPanel/PatchEditDisplaysPanel/Modules/PatchNameDisplayPanel.h"
#include "GUI/Settings/SettingsPanel.h"
#include "GUI/Settings/SettingsWindow.h"
#include "GUI/About/AboutWindow.h"
#include "GUI/Dialogs/BankTransferProgressDialog.h"
#include "GUI/Dialogs/MasterInitConfirmDialog.h"
#include "Skins/Skin.h"
#include "Factories/WidgetFactory.h"
#include "Shared/Definitions/PluginIDs.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginAudioConstants.h"
#include "Shared/Definitions/MatrixDeviceTypes.h"

using TSS::SkinColourId;

namespace
{
bool isMessageThread()
{
    if (auto* mm = juce::MessageManager::getInstanceWithoutCreating())
        return mm->isThisTheMessageThread();

    return false;
}

/** True when macOS NSAlert places the first registered button as rightmost default. */
bool usesMacOsNativeAlertButtonOrder()
{
   #if JUCE_MAC
    return juce::LookAndFeel::getDefaultLookAndFeel().isUsingNativeAlertWindows();
   #else
    return false;
   #endif
}

/** Prevent non-primary AlertWindow buttons from consuming Return when focused. */
void configureOrderedAlertButtons(juce::AlertWindow& alert,
                                  const juce::String& cancelLabel,
                                  const juce::String& primaryLabel,
                                  const juce::String& middleLabel)
{
    auto* cancel = alert.getButton(cancelLabel);
    auto* primary = alert.getButton(primaryLabel);
    jassert(cancel != nullptr);
    jassert(primary != nullptr);

    if (cancel != nullptr)
        cancel->setWantsKeyboardFocus(false);

    if (middleLabel.isNotEmpty())
    {
        auto* middle = alert.getButton(middleLabel);
        jassert(middle != nullptr);

        if (middle != nullptr)
            middle->setWantsKeyboardFocus(false);
    }

    // Primary keeps keyboard focus so Return activates it; Cancel/middle are click-only.
    if (primary != nullptr)
        primary->grabKeyboardFocus();
}

/** Visual LTR: Cancel → [middle] → primary (rightmost = default).
    Semantic codes (stable across platforms): Cancel/Escape/OOR → 0, primary → 1, middle → 2.

    macOS native: register primary-first (NSAlert rightmost-first).
    Windows/Linux: controlled AlertWindow with Cancel-first layout and Return/Escape wired
    by role — preferred over TaskDialog so LTR + Enter=primary both hold. */
int showOrderedConfirmAlert(juce::MessageBoxIconType iconType,
                            const juce::String& title,
                            const juce::String& message,
                            const juce::String& cancelLabel,
                            const juce::String& primaryLabel,
                            juce::Component* associatedComponent,
                            const juce::String& middleLabel = {})
{
    jassert(cancelLabel.isNotEmpty());
    jassert(primaryLabel.isNotEmpty());

    const bool hasMiddle = middleLabel.isNotEmpty();

    if (usesMacOsNativeAlertButtonOrder())
    {
        auto options = juce::MessageBoxOptions()
                           .withIconType(iconType)
                           .withTitle(title)
                           .withMessage(message)
                           .withButton(primaryLabel);

        if (hasMiddle)
            options = options.withButton(middleLabel);

        options = options.withButton(cancelLabel).withAssociatedComponent(associatedComponent);

        const int numButtons = options.getNumButtons();
        const int raw = juce::NativeMessageBox::show(options);

        if (raw < 0 || raw >= numButtons)
            return 0;

        return (raw + 1) % numButtons;
    }

   #if JUCE_MODAL_LOOPS_PERMITTED
    juce::AlertWindow alert(title, message, iconType, associatedComponent);
    alert.addButton(cancelLabel, 0, juce::KeyPress(juce::KeyPress::escapeKey));

    if (hasMiddle)
        alert.addButton(middleLabel, 2);

    alert.addButton(primaryLabel, 1, juce::KeyPress(juce::KeyPress::returnKey));
    configureOrderedAlertButtons(alert, cancelLabel, primaryLabel, middleLabel);
    return alert.runModalLoop();
   #else
    jassertfalse;
    juce::ignoreUnused(iconType, title, message, associatedComponent, hasMiddle,
                       cancelLabel, primaryLabel, middleLabel);
    return 0;
   #endif
}

/** Sync Delete confirm with optional "Don't ask again" checkbox.
    Uses AlertWindow on all platforms (native NSAlert cannot host a checkbox).
    Codes: Cancel/Escape → 0, Delete/Return → 1. */
struct MutatorDeleteConfirmResult
{
    bool confirmed = false;
    bool dontAskAgain = false;
};

MutatorDeleteConfirmResult showMutatorDeleteConfirmAlert(juce::Component* associatedComponent)
{
    namespace Dialog = PluginDisplayNames::Dialogs::MutatorDeleteConfirm;

   #if JUCE_MODAL_LOOPS_PERMITTED
    juce::AlertWindow alert(Dialog::kTitle,
                            Dialog::kBody,
                            juce::MessageBoxIconType::WarningIcon,
                            associatedComponent);
    juce::ToggleButton dontAskAgain(Dialog::kDontAskAgain);
    dontAskAgain.setSize(360, 24);
    // AlertWindow paints customComponent->getName() above the control; clear it so
    // "Don't ask again" appears only once (ToggleButton text), not as a duplicate label.
    dontAskAgain.setName({});
    // Keep Enter on Delete (primary); checkbox is click-only, same as Cancel.
    dontAskAgain.setWantsKeyboardFocus(false);
    dontAskAgain.setMouseClickGrabsKeyboardFocus(false);
    alert.addCustomComponent(&dontAskAgain);
    alert.addButton(Dialog::kCancel, 0, juce::KeyPress(juce::KeyPress::escapeKey));
    alert.addButton(Dialog::kDelete, 1, juce::KeyPress(juce::KeyPress::returnKey));
    configureOrderedAlertButtons(alert, Dialog::kCancel, Dialog::kDelete, {});

    const int result = alert.runModalLoop();
    return { result == 1, dontAskAgain.getToggleState() };
   #else
    jassertfalse;
    juce::ignoreUnused(associatedComponent);
    return {};
   #endif
}
} // namespace

class PluginEditor::HeaderRefreshTimer : private juce::Timer
{
public:
    HeaderRefreshTimer(PluginProcessor& processor, HeaderPanel& headerPanel, PluginEditor& owner)
        : processor_(processor)
        , headerPanel_(headerPanel)
        , owner_(owner)
    {
        startTimerHz(30);
    }

private:
    void timerCallback() override
    {
        if (processor_.isStandalone())
        {
            if (audioFromRefreshAttempts_ < 60)
            {
                const auto names = processor_.getAudioInputSourceNames();

                if (names.isEmpty())
                {
                    ++audioFromRefreshAttempts_;
                    owner_.refreshAudioFromCombo();
                }
            }

            headerPanel_.getPeakIndicator().setLevel(
                processor_.getAudioPassthroughProcessor().getPeakLevel());
        }

        const auto& tracker = processor_.getMidiActivityTracker();
        headerPanel_.getInstrumentActivityLed().setLevel(
            tracker.getActivityLevel(Core::MidiActivityTracker::Path::kInstrument));
        headerPanel_.getEditorActivityLed().setLevel(
            tracker.getActivityLevel(Core::MidiActivityTracker::Path::kMidiFromInbound));
        headerPanel_.getMidiToActivityLed().setLevel(
            tracker.getActivityLevel(Core::MidiActivityTracker::Path::kOutbound));
    }

    PluginProcessor& processor_;
    HeaderPanel& headerPanel_;
    PluginEditor& owner_;
    int audioFromRefreshAttempts_ = 0;
};

class PluginEditor::ClipboardFeedbackPhaseTimer : private juce::Timer,
                                                  private juce::ValueTree::Listener
{
public:
    explicit ClipboardFeedbackPhaseTimer(juce::AudioProcessorValueTreeState& apvts)
        : state_(apvts.state)
    {
        state_.addListener(this);
        syncTimerFromState();
    }

    ~ClipboardFeedbackPhaseTimer() override
    {
        state_.removeListener(this);
        stopTimer();
    }

private:
    void timerCallback() override
    {
        const bool copyLit = static_cast<bool>(
            state_.getProperty(PluginIDs::ClipboardFeedback::kCopyLit, true));
        state_.setProperty(PluginIDs::ClipboardFeedback::kCopyLit, ! copyLit, nullptr);
    }

    void valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier& property) override
    {
        if (property.toString() == PluginIDs::ClipboardFeedback::kActive)
            syncTimerFromState();
    }

    void valueTreeChildAdded(juce::ValueTree&, juce::ValueTree&) override {}
    void valueTreeChildRemoved(juce::ValueTree&, juce::ValueTree&, int) override {}
    void valueTreeChildOrderChanged(juce::ValueTree&, int, int) override {}
    void valueTreeParentChanged(juce::ValueTree&) override {}
    void valueTreeRedirected(juce::ValueTree&) override { syncTimerFromState(); }

    void syncTimerFromState()
    {
        const bool active = static_cast<bool>(
            state_.getProperty(PluginIDs::ClipboardFeedback::kActive, false));
        if (active)
            startTimerHz(2);
        else
            stopTimer();
    }

    juce::ValueTree state_;
};


PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p)
    , pluginProcessor(p)
{
    // Prefer OS-native alerts where LookAndFeel reports them (macOS/Windows).
    // On Linux, JUCE forces isUsingNativeAlertWindows() false and draws AlertWindow.
    juce::LookAndFeel::getDefaultLookAndFeel().setUsingNativeAlertWindows(true);

    pluginProcessor.setPatchFolderPicker([safeThis = juce::Component::SafePointer<PluginEditor>(this)]() -> juce::File
    {
        if (safeThis == nullptr)
            return {};

        juce::File startDirectory;
        const auto persistedPath = safeThis->pluginProcessor.getApvts().state.getProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kFolderPath,
            juce::String()).toString();

        if (persistedPath.isNotEmpty())
        {
            const juce::File persistedFolder(persistedPath);
            if (persistedFolder.isDirectory())
                startDirectory = persistedFolder;
        }

        juce::FileChooser chooser("Select patch folder",
                                  startDirectory,
                                  juce::String(),
                                  true,
                                  false,
                                  safeThis.getComponent());

        if (chooser.browseForDirectory())
            return chooser.getResult();

        return {};
    });

    pluginProcessor.setMutatorExportFolderPicker(
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)]() -> juce::File
        {
            if (safeThis == nullptr)
                return {};

            juce::FileChooser chooser("Select mutation export folder",
                                      juce::File(),
                                      juce::String(),
                                      true,
                                      false,
                                      safeThis.getComponent());

            if (chooser.browseForDirectory())
                return chooser.getResult();

            return {};
        });

    pluginProcessor.setMutatorDefragLimitModalGate(
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)](std::function<void()> onConfirmed)
        {
            if (! isMessageThread() || safeThis == nullptr)
                return;

            namespace Dialog = PluginDisplayNames::Dialogs::MutatorHistoryDefrag;

            const int result = showOrderedConfirmAlert(
                juce::MessageBoxIconType::QuestionIcon,
                Dialog::kTitle,
                Dialog::kBody,
                Dialog::kCancel,
                Dialog::kConfirm,
                safeThis.getComponent());

            if (result == 1 && onConfirmed)
                onConfirmed();
        });

    pluginProcessor.setMutatorExportCollisionModalGate(
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)](
            std::function<void(Core::ExportCollisionResolution)> onResolved)
        {
            if (! onResolved)
                return;

            if (! isMessageThread() || safeThis == nullptr)
            {
                onResolved(Core::ExportCollisionResolution::kCancel);
                return;
            }

            namespace Msg = PluginDisplayNames::PatchManagerSection::PatchMutatorModule::Messages;

            switch (showOrderedConfirmAlert(
                juce::MessageBoxIconType::QuestionIcon,
                Msg::kExportCollisionTitle,
                Msg::kExportCollisionMessage,
                Msg::kExportCollisionCancel,
                Msg::kExportCollisionOverwrite,
                safeThis.getComponent(),
                Msg::kExportCollisionKeep))
            {
                case 1: onResolved(Core::ExportCollisionResolution::kOverwrite); break;
                case 2: onResolved(Core::ExportCollisionResolution::kKeep); break;
                default: onResolved(Core::ExportCollisionResolution::kCancel); break;
            }
        });

    pluginProcessor.setMutatorHistoryGateModalGate(
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)]() -> Core::MutatorHistoryGateChoice
        {
            if (! isMessageThread() || safeThis == nullptr)
                return Core::MutatorHistoryGateChoice::kCancel;

            namespace Msg = PluginDisplayNames::PatchManagerSection::PatchMutatorModule::Messages;

            switch (showOrderedConfirmAlert(
                juce::MessageBoxIconType::QuestionIcon,
                Msg::kHistoryGateTitle,
                Msg::kHistoryGateMessage,
                Msg::kHistoryGateCancel,
                Msg::kHistoryGateExport,
                safeThis.getComponent(),
                Msg::kHistoryGateDiscard))
            {
                case 1: return Core::MutatorHistoryGateChoice::kExport;
                case 2: return Core::MutatorHistoryGateChoice::kDiscard;
                default: return Core::MutatorHistoryGateChoice::kCancel;
            }
        });

    pluginProcessor.setUnsavedEditConfirmModalGate(
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)]() -> bool
        {
            if (! isMessageThread() || safeThis == nullptr)
                return false;

            namespace Dialog = PluginDisplayNames::Dialogs::UnsavedEditConfirm;

            return showOrderedConfirmAlert(
                       juce::MessageBoxIconType::WarningIcon,
                       Dialog::kTitle,
                       Dialog::kBody,
                       Dialog::kCancel,
                       Dialog::kContinue,
                       safeThis.getComponent())
                   == 1;
        });

    pluginProcessor.setMutatorFlushConfirmModalGate(
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)]() -> bool
        {
            if (! isMessageThread() || safeThis == nullptr)
            {
                jassertfalse;
                return false;
            }

            namespace Dialog = PluginDisplayNames::Dialogs::MutatorFlushConfirm;

            return showOrderedConfirmAlert(
                       juce::MessageBoxIconType::WarningIcon,
                       Dialog::kTitle,
                       Dialog::kBody,
                       Dialog::kCancel,
                       Dialog::kContinue,
                       safeThis.getComponent())
                   == 1;
        });

    pluginProcessor.setMutatorDeleteConfirmModalGate(
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)]() -> bool
        {
            if (! isMessageThread() || safeThis == nullptr)
            {
                jassertfalse;
                return false;
            }

            using namespace PluginIDs::Settings::MutatorDeleteWarningPolicy;

            const int policyRaw = static_cast<int>(safeThis->pluginProcessor.getApvts().state.getProperty(
                PluginIDs::Settings::kMutatorDeleteWarningPolicy,
                kDefault));
            const int policy =
                (policyRaw == kWarnAlways || policyRaw == kNeverWarn) ? policyRaw : kDefault;

            if (! Core::MutatorDeleteWarning::shouldPrompt(policy))
                return true;

            const auto result = showMutatorDeleteConfirmAlert(safeThis.getComponent());

            if (result.confirmed && result.dontAskAgain)
            {
                safeThis->pluginProcessor.getApvts().state.setProperty(
                    PluginIDs::Settings::kMutatorDeleteWarningPolicy,
                    kNeverWarn,
                    nullptr);

                if (auto* settingsPanel = safeThis->getSettingsPanelIfOpen())
                    settingsPanel->getMutatorDeleteWarningPolicyCombo().setSelectedId(
                        kNeverWarn, juce::dontSendNotification);
            }

            return result.confirmed;
        });

    pluginProcessor.setPatchSaveFilePicker(
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)](
            juce::File suggestedFolder, juce::String suggestedStem) -> juce::File
        {
            if (safeThis == nullptr)
                return {};

            juce::FileChooser chooser("Save patch as",
                                      suggestedFolder.getChildFile(suggestedStem + ".syx"),
                                      "*.syx",
                                      true,
                                      false,
                                      safeThis.getComponent());

            if (chooser.browseForFileToSave(true))
                return chooser.getResult();

            return {};
        });

    pluginProcessor.setPatchNameReconciliationPicker(
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)](
            juce::String internalSanitized, juce::String fileSanitized)
            -> std::optional<Core::NameReconciliationChoice>
        {
            if (! isMessageThread() || safeThis == nullptr)
                return std::nullopt;

            namespace Dialog = PluginDisplayNames::Dialogs::PatchNameReconciliation;
            const auto body = juce::String(Dialog::kBodyTemplate)
                                  .replace("{INTERNAL}", internalSanitized)
                                  .replace("{FILENAME}", fileSanitized);

            switch (showOrderedConfirmAlert(
                juce::MessageBoxIconType::QuestionIcon,
                Dialog::kTitle,
                body,
                Dialog::kCancel,
                Dialog::kFilename,
                safeThis.getComponent(),
                Dialog::kInternal))
            {
                case 1: return Core::NameReconciliationChoice::kFilename;
                case 2: return Core::NameReconciliationChoice::kInternal;
                default: return std::nullopt;
            }
        });

    pluginProcessor.setBankExportFolderPicker(
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)]() -> juce::File
        {
            if (safeThis == nullptr)
                return {};

            juce::File startDirectory;
            const auto persistedPath = safeThis->pluginProcessor.getApvts().state.getProperty(
                PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kFolderPath,
                juce::String()).toString();
            if (persistedPath.isNotEmpty())
            {
                const juce::File persistedFolder(persistedPath);
                if (persistedFolder.isDirectory())
                    startDirectory = persistedFolder;
            }

            juce::FileChooser chooser("Select bank export folder",
                                      startDirectory,
                                      juce::String(),
                                      true,
                                      false,
                                      safeThis.getComponent());

            if (chooser.browseForDirectory())
                return chooser.getResult();

            return {};
        });

    pluginProcessor.setBankImportFolderPicker(
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)]() -> juce::File
        {
            if (safeThis == nullptr)
                return {};

            juce::File startDirectory;
            const auto persistedPath = safeThis->pluginProcessor.getApvts().state.getProperty(
                PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kFolderPath,
                juce::String()).toString();
            if (persistedPath.isNotEmpty())
            {
                const juce::File persistedFolder(persistedPath);
                if (persistedFolder.isDirectory())
                    startDirectory = persistedFolder;
            }

            juce::FileChooser chooser("Select bank import folder",
                                      startDirectory,
                                      juce::String(),
                                      true,
                                      false,
                                      safeThis.getComponent());

            if (chooser.browseForDirectory())
                return chooser.getResult();

            return {};
        });

    pluginProcessor.setBankImportConfirmGate(
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)]() -> bool
        {
            if (! isMessageThread() || safeThis == nullptr)
                return false;

            namespace Dialog = PluginDisplayNames::Dialogs::BankImportConfirm;

            return showOrderedConfirmAlert(
                       juce::MessageBoxIconType::WarningIcon,
                       Dialog::kTitle,
                       Dialog::kBody,
                       Dialog::kCancel,
                       Dialog::kContinue,
                       safeThis.getComponent())
                   == 1;
        });

    pluginProcessor.setBankTransferProgressPresenter(Core::BankTransferProgressPresenter {
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)](
            const juce::String& title, const juce::String& message, int totalSteps, std::function<void()> onCancel)
        {
            if (safeThis != nullptr)
                safeThis->showBankTransferProgressDialog(title, message, totalSteps, std::move(onCancel));
        },
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)](int completedSteps)
        {
            if (safeThis != nullptr && safeThis->bankTransferProgressDialog_ != nullptr)
                safeThis->bankTransferProgressDialog_->setProgress(completedSteps);
        },
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)](const juce::String& message)
        {
            if (safeThis != nullptr && safeThis->bankTransferProgressDialog_ != nullptr)
                safeThis->bankTransferProgressDialog_->setMessage(message);
        },
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)](bool enabled)
        {
            if (safeThis != nullptr && safeThis->bankTransferProgressDialog_ != nullptr)
                safeThis->bankTransferProgressDialog_->setCancelEnabled(enabled);
        },
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)]
        {
            if (safeThis != nullptr)
                safeThis->hideBankTransferProgressDialog();
        }
    });

    skinBlack_ = TSS::Skin::create(TSS::Skin::ColourVariant::Black);
    skinCream_ = TSS::Skin::create(TSS::Skin::ColourVariant::Cream);

    const int savedSkinId = pluginProcessor.getSkinVariantId();
    skin_ = (savedSkinId == PluginIDs::Settings::SkinVariants::kCream)
        ? skinCream_.get()
        : skinBlack_.get();

    widgetFactory_ = std::make_unique<WidgetFactory>(pluginProcessor.getApvts());
    layoutDimensions_ = WidgetFactory::buildGuiLayoutDimensions();
    TSS::ComboBox::setPopupLayoutDimensions(layoutDimensions_.popupMenu);

    setOpaque(true);
    setWantsKeyboardFocus(false);
    setInterceptsMouseClicks(true, true);

    mainComponent_ = std::make_unique<MainComponent>(
        *skin_, layoutDimensions_, *widgetFactory_, pluginProcessor.getApvts(), pluginProcessor.getPatchFileService());
    addAndMakeVisible(*mainComponent_);

    mainComponent_->setBusReorderHandler(
        [this](int fromBus, int toBus)
        {
            pluginProcessor.swapMatrixModBusContents(fromBus, toBus);
        });

    {
        auto& patchNameDisplayPanel = mainComponent_->getBodyPanel()
            .getPatchEditPanel()
            .getPatchEditDisplaysPanel()
            .getPatchNameDisplayPanel();

        patchNameDisplayPanel.setCanEditProvider(
            [this]() { return pluginProcessor.canEditPatchName(); });

        patchNameDisplayPanel.setRenameCommitHandler(
            [this](const juce::String& newName)
            {
                pluginProcessor.commitPatchNameRename(newName);
            });
    }

    mainComponent_->setMasterInitConfirmationGate(
        [this](const juce::String& /*initPropertyId*/,
               const juce::String& moduleDisplayName,
               std::function<void()> onConfirmed)
        {
            openMasterInitConfirmDialog(moduleDisplayName, std::move(onConfirmed));
        });

#if JUCE_DEBUG
    const auto editorWidth = layoutDimensions_.editor.width;
    const auto editorHeight = layoutDimensions_.editor.height;
    testComponent_ = std::make_unique<TestComponent>(
        *skin_,
        pluginProcessor.getApvts(),
        pluginProcessor.getApvts().state,
        editorWidth,
        editorHeight);
    addChildComponent(*testComponent_);
    testComponent_->setVisible(false);
#endif

    updateSkin();

    auto& headerPanel = mainComponent_->getHeaderPanel();

    const int savedScaleId = pluginProcessor.getGuiScaleId();
    const float savedUiScale = PluginIDs::Settings::ScaleLevels::getUiScale(savedScaleId);
    applyUiScale(savedUiScale);

    restoreHeaderPanelFromState(headerPanel);
    wireHeaderPanel(headerPanel);

    headerPanel.setPluginMode(!pluginProcessor.isStandalone());
    headerPanel.refreshPortLists();

    pluginProcessor.restoreMidiPortsForHost();

    // Combo must mirror open reality after restore/sync (APVTS may have been cleared on failure).
    headerPanel.selectMidiFromPort(
        pluginProcessor.getApvts().state.getProperty("midiInputPortId", juce::String()).toString());
    headerPanel.selectMidiToPort(
        pluginProcessor.getApvts().state.getProperty("midiOutputPortId", juce::String()).toString());

    if (pluginProcessor.isStandalone())
    {
        const auto savedKeyboardFromPortId = pluginProcessor.getApvts().state.getProperty("keyboardFromPortId", juce::String()).toString();
        if (savedKeyboardFromPortId.isNotEmpty()
            && ! pluginProcessor.setKeyboardFromPort(savedKeyboardFromPortId))
        {
            // Open failure or MIDI From conflict — drop the dead selection.
            pluginProcessor.setKeyboardFromPort({});
        }

        headerPanel.selectKeyboardFromPort(
            pluginProcessor.getApvts().state.getProperty("keyboardFromPortId", juce::String()).toString());

        refreshAudioFromCombo(&headerPanel);

        const auto savedAudioFromSourceId = pluginProcessor.getApvts().state.getProperty("audioFromSourceId", juce::String()).toString();
        if (savedAudioFromSourceId.isNotEmpty())
        {
            headerPanel.selectAudioFromSourceId(savedAudioFromSourceId);
            pluginProcessor.setAudioFromSourceId(savedAudioFromSourceId);
        }

        const float savedInputGainDb = static_cast<float>(
            pluginProcessor.getApvts().state.getProperty("inputGainDb", 0.0f));
        const int gainIndex = PluginAudioConstants::inputGainDbToIndex(savedInputGainDb);
        headerPanel.getInputGainSlider().setValue(gainIndex, juce::dontSendNotification);
        pluginProcessor.setInputGainDb(PluginAudioConstants::inputGainIndexToDb(gainIndex));
    }

    pluginProcessor.syncHardwareLatencyFromState();

    setResizable(false, false);

    headerPanel.getMidiFromComboBox().onChange = [this, &headerPanel]
    {
        const auto previousPortId = pluginProcessor.getApvts().state.getProperty("midiInputPortId", juce::String()).toString();
        const auto selectedPortId = headerPanel.getSelectedMidiFromPortIdentifier();

        if (pluginProcessor.setMidiInputPort(selectedPortId))
            return;

        headerPanel.selectMidiFromPort(previousPortId);
        if (previousPortId.isNotEmpty())
            pluginProcessor.setMidiInputPort(previousPortId);
    };

    headerPanel.getMidiToComboBox().onChange = [this, &headerPanel]
    {
        const auto previousPortId = pluginProcessor.getApvts().state.getProperty("midiOutputPortId", juce::String()).toString();
        const auto selectedPortId = headerPanel.getSelectedMidiToPortIdentifier();

        if (pluginProcessor.setMidiOutputPort(selectedPortId))
            return;

        headerPanel.selectMidiToPort(previousPortId);
        if (previousPortId.isNotEmpty())
            pluginProcessor.setMidiOutputPort(previousPortId);
    };

    headerPanel.getKeyboardFromComboBox().onChange = [this, &headerPanel]
    {
        if (!pluginProcessor.isStandalone())
            return;

        const auto previousPortId = pluginProcessor.getApvts().state.getProperty("keyboardFromPortId", juce::String()).toString();
        const auto selectedPortId = headerPanel.getSelectedKeyboardFromPortIdentifier();

        if (pluginProcessor.setKeyboardFromPort(selectedPortId))
            return;

        headerPanel.selectKeyboardFromPort(previousPortId);
        if (previousPortId.isNotEmpty())
            pluginProcessor.setKeyboardFromPort(previousPortId);
    };

    headerPanel.getInputGainSlider().onValueChange = [this, &headerPanel]
    {
        if (!pluginProcessor.isStandalone())
            return;

        const int index = static_cast<int>(std::round(headerPanel.getInputGainSlider().getValue()));
        pluginProcessor.setInputGainDb(PluginAudioConstants::inputGainIndexToDb(index));
    };

    headerPanel.getAudioFromComboBox().onChange = [this, &headerPanel]
    {
        if (!pluginProcessor.isStandalone())
            return;

        pluginProcessor.setAudioFromSourceId(headerPanel.getSelectedAudioFromSourceId());
    };

    headerRefreshTimer_ = std::make_unique<HeaderRefreshTimer>(pluginProcessor, headerPanel, *this);
    clipboardFeedbackPhaseTimer_ = std::make_unique<ClipboardFeedbackPhaseTimer>(pluginProcessor.getApvts());
    attachStandaloneAudioDeviceListener();
    pluginProcessor.getApvts().state.addListener(this);

    setWantsKeyboardFocus(true);
    syncUiScaleFromEditor();
#if JUCE_DEBUG
    layoutUiElementsTestComponent();
#endif
    repaint();
}

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(skin_->getColour(SkinColourId::kHeaderPanelBackground));
}

void PluginEditor::resized()
{
    const int baseWidth = layoutDimensions_.editor.width;
    if (baseWidth <= 0)
        return;

    if (pluginProcessor.isStandalone())
    {
        const int targetWidth = juce::roundToInt(static_cast<float>(layoutDimensions_.editor.width) * appliedUiScale_);
        const int targetHeight = juce::roundToInt(static_cast<float>(layoutDimensions_.editor.height) * appliedUiScale_);

        setResizeLimits(targetWidth, targetHeight, targetWidth, targetHeight);

        if (getWidth() != targetWidth || getHeight() != targetHeight)
        {
            setSize(targetWidth, targetHeight);
            return;
        }
    }

    if (auto* comp = mainComponent_.get())
        comp->setBounds(getLocalBounds());

    if (settingsWindow_ != nullptr && settingsWindow_->isVisible())
        settingsWindow_->setBounds(getLocalBounds());

    if (aboutWindow_ != nullptr && aboutWindow_->isVisible())
        aboutWindow_->setBounds(getLocalBounds());

#if JUCE_DEBUG
    layoutUiElementsTestComponent();
#endif
    syncUiScaleFromEditor();

    if (pluginProcessor.isStandalone())
        syncStandaloneWindowSize();
}

void PluginEditor::syncStandaloneWindowSize()
{
    if (auto* window = findParentComponentOfClass<juce::DocumentWindow>())
        window->setContentComponentSize(getWidth(), getHeight());
}

void PluginEditor::syncUiScaleFromEditor()
{
    const int baseWidth = layoutDimensions_.editor.width;
    if (baseWidth <= 0)
        return;

    const float uiScale = TSS::ScaledLayout::uiScaleFromEditorBounds(getWidth(), baseWidth);

    if (auto* comp = mainComponent_.get())
        comp->setUiScale(uiScale);

    if (auto* comp = mainComponent_.get())
    {
        int matchingScaleId = 0;
        const int layoutPercentRounded = juce::roundToInt(uiScale * 100.0f);
        for (int id = PluginIDs::Settings::ScaleLevels::kMin; id <= PluginIDs::Settings::ScaleLevels::kMax; ++id)
        {
            const int presetPercentRounded = juce::roundToInt(
                PluginIDs::Settings::ScaleLevels::getUiScale(id) * 100.0f);
            if (layoutPercentRounded == presetPercentRounded)
            {
                matchingScaleId = id;
                break;
            }
        }

        if (matchingScaleId != 0)
            comp->getHeaderPanel().setCurrentUiScaleId(matchingScaleId);
    }

    if (settingsWindow_ != nullptr && settingsWindow_->isVisible())
        updateSettingsWindowLayout(uiScale);

    if (aboutWindow_ != nullptr && aboutWindow_->isVisible())
        updateAboutWindowLayout(uiScale);

    if (masterInitConfirmDialog_ != nullptr && masterInitConfirmDialog_->isVisible())
        updateMasterInitConfirmDialogLayout(uiScale);

    if (bankTransferProgressDialog_ != nullptr && bankTransferProgressDialog_->isVisible())
        updateBankTransferProgressDialogLayout(uiScale);
}

void PluginEditor::updateSettingsWindowLayout(float uiScale)
{
    if (settingsWindow_ == nullptr)
        return;

    settingsWindow_->setUiScale(uiScale);
    settingsWindow_->setBounds(getLocalBounds());
}

void PluginEditor::updateAboutWindowLayout(float uiScale)
{
    if (aboutWindow_ == nullptr)
        return;

    aboutWindow_->setUiScale(uiScale);
    aboutWindow_->setBounds(getLocalBounds());
}

void PluginEditor::updateMasterInitConfirmDialogLayout(float uiScale)
{
    if (masterInitConfirmDialog_ == nullptr)
        return;

    masterInitConfirmDialog_->setUiScale(uiScale);
    masterInitConfirmDialog_->setBounds(getLocalBounds());
}

void PluginEditor::updateBankTransferProgressDialogLayout(float uiScale)
{
    if (bankTransferProgressDialog_ == nullptr)
        return;

    bankTransferProgressDialog_->setUiScale(uiScale);
    bankTransferProgressDialog_->setBounds(getLocalBounds());
}

void PluginEditor::applySkinFromItemId(int skinItemId, bool persistToState)
{
    skin_ = (skinItemId == PluginIDs::Settings::SkinVariants::kBlack)
        ? skinBlack_.get()
        : skinCream_.get();
    updateSkin();

    if (persistToState)
        pluginProcessor.setSkinVariantId(skinItemId);

    if (auto* comp = mainComponent_.get())
        comp->getHeaderPanel().setCurrentSkinItemId(skinItemId);
}

void PluginEditor::applyUiScaleFromItemId(int scaleId, bool persistToState)
{
    const float uiScale = PluginIDs::Settings::ScaleLevels::getUiScale(scaleId);
    applyUiScale(uiScale);
    updateSettingsWindowLayout(uiScale);
    updateAboutWindowLayout(uiScale);

    if (persistToState)
        pluginProcessor.setGuiScaleId(scaleId);

    if (auto* comp = mainComponent_.get())
        comp->getHeaderPanel().setCurrentUiScaleId(scaleId);
}

void PluginEditor::wireHeaderPanel(HeaderPanel& headerPanel)
{
    headerPanel.onSkinSelected = [this](int skinItemId)
    {
        applySkinFromItemId(skinItemId, true);
    };

    headerPanel.onUiScaleSelected = [this](int scaleId)
    {
        applyUiScaleFromItemId(scaleId, true);
    };

    headerPanel.onUiScaleReset = [this]
    {
        applyUiScaleFromItemId(PluginIDs::Settings::ScaleLevels::k100, true);
    };

    headerPanel.onSettingsRequested = [this]
    {
        if (settingsWindow_ != nullptr && settingsWindow_->isVisible())
            closeSettingsWindow();
        else
            openSettingsWindow();
    };

    headerPanel.onAboutRequested = [this]
    {
        if (aboutWindow_ != nullptr && aboutWindow_->isVisible())
            closeAboutWindow();
        else
            openAboutWindow();
    };

    headerPanel.onAudioMidiSettingsRequested = []
    {
        Core::StandaloneAudioInputRouter::showAudioMidiSettingsDialog();
    };

#if JUCE_DEBUG
    headerPanel.onUiTestsToggleRequested = [this]
    {
        setUiElementsTestVisible(!uiElementsTestVisible_);
    };
#endif
}

void PluginEditor::restoreHeaderPanelFromState(HeaderPanel& headerPanel)
{
    headerPanel.setCurrentUiScaleId(pluginProcessor.getGuiScaleId());
    headerPanel.setCurrentSkinItemId(pluginProcessor.getSkinVariantId());
}

void PluginEditor::mouseDown(const juce::MouseEvent& event)
{
#if JUCE_DEBUG
    if (uiElementsTestVisible_
        && testComponent_ != nullptr
        && testComponent_->isVisible()
        && testComponent_->getBounds().contains(event.getPosition()))
    {
        testComponent_->grabKeyboardFocus();
        return;
    }
#else
    juce::ignoreUnused(event);
#endif

    unfocusAllComponents();
    grabKeyboardFocus();
}

bool PluginEditor::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::escapeKey)
    {
        if (settingsWindow_ != nullptr && settingsWindow_->isVisible())
            return false;
        if (aboutWindow_ != nullptr && aboutWindow_->isVisible())
            return false;
        if (masterInitConfirmDialog_ != nullptr && masterInitConfirmDialog_->isVisible())
            return false;
        if (bankTransferProgressDialog_ != nullptr && bankTransferProgressDialog_->isVisible())
            return false;

        if (! pluginProcessor.clearClipboardFeedbackFromEscape())
            return false;

        return true;
    }

#if JUCE_DEBUG
    if (uiElementsTestVisible_
        && testComponent_ != nullptr
        && testComponent_->isVisible()
        && testComponent_->keyPressed(key))
    {
        return true;
    }
#endif

    return juce::AudioProcessorEditor::keyPressed(key);
}

void PluginEditor::updateSkin()
{
    if (auto* widget = mainComponent_.get())
        widget->setSkin(*skin_);

    if (settingsWindow_ != nullptr)
        settingsWindow_->setSkin(*skin_);

    if (aboutWindow_ != nullptr)
        aboutWindow_->setSkin(*skin_);

    if (bankTransferProgressDialog_ != nullptr)
        bankTransferProgressDialog_->setSkin(*skin_);

#if JUCE_DEBUG
    if (testComponent_ != nullptr)
        testComponent_->setSkin(*skin_);
#endif

    repaint();
}

void PluginEditor::applyUiScale(float uiScale)
{
    appliedUiScale_ = uiScale;

    const int targetWidth = juce::roundToInt(static_cast<float>(layoutDimensions_.editor.width) * uiScale);
    const int targetHeight = juce::roundToInt(static_cast<float>(layoutDimensions_.editor.height) * uiScale);

    if (pluginProcessor.isStandalone())
        setResizeLimits(targetWidth, targetHeight, targetWidth, targetHeight);

    setSize(targetWidth, targetHeight);

    if (pluginProcessor.isStandalone())
        syncStandaloneWindowSize();
}

#if JUCE_DEBUG
void PluginEditor::setUiElementsTestVisible(bool visible)
{
    if (uiElementsTestVisible_ == visible)
        return;

    uiElementsTestVisible_ = visible;

    if (auto* main = mainComponent_.get())
        main->setUiElementsTestVisible(visible);

    if (auto* test = testComponent_.get())
    {
        test->setVisible(visible);
        if (visible)
        {
            test->toFront(false);
            test->grabKeyboardFocus();
        }
    }

    layoutUiElementsTestComponent();
    repaint();
}

void PluginEditor::layoutUiElementsTestComponent()
{
    if (!uiElementsTestVisible_ || mainComponent_ == nullptr || testComponent_ == nullptr)
        return;

    testComponent_->setBounds(mainComponent_->getUiElementsTestAreaBounds());
}
#endif

PluginEditor::~PluginEditor()
{
    pluginProcessor.setMutatorDefragLimitModalGate({});
    pluginProcessor.setMutatorExportCollisionModalGate({});
    pluginProcessor.setMutatorHistoryGateModalGate({});
    pluginProcessor.setUnsavedEditConfirmModalGate({});
    pluginProcessor.setMutatorFlushConfirmModalGate({});
    pluginProcessor.setMutatorDeleteConfirmModalGate({});
    pluginProcessor.setPatchNameReconciliationPicker({});
    pluginProcessor.setBankExportFolderPicker({});
    pluginProcessor.setBankImportFolderPicker({});
    pluginProcessor.setBankImportConfirmGate({});
    pluginProcessor.setBankTransferProgressPresenter({});

    pluginProcessor.getApvts().state.removeListener(this);
    detachStandaloneAudioDeviceListener();
    closeSettingsWindow();
    closeAboutWindow();
}

SettingsPanel* PluginEditor::getSettingsPanelIfOpen()
{
    if (settingsWindow_ == nullptr || !settingsWindow_->isVisible())
        return nullptr;

    return &settingsWindow_->getSettingsPanel();
}

void PluginEditor::openSettingsWindow()
{
    closeAboutWindow();

    if (settingsWindow_ == nullptr)
    {
        const bool isPluginMode = !pluginProcessor.isStandalone();
        settingsWindow_ = std::make_unique<SettingsWindow>(
            *skin_,
            isPluginMode,
            [this](SettingsPanel& panel)
            {
                wireSettingsPanel(panel);
            },
            [this] { closeSettingsWindow(); });
        addChildComponent(*settingsWindow_);
    }
    else
    {
        settingsWindow_->setSkin(*skin_);
    }

    const int baseWidth = layoutDimensions_.editor.width;
    const float uiScale = (baseWidth > 0)
        ? TSS::ScaledLayout::uiScaleFromEditorBounds(getWidth(), baseWidth)
        : 1.0f;
    updateSettingsWindowLayout(uiScale);

    settingsWindow_->setVisible(true);
    restoreSettingsPanelFromState(settingsWindow_->getSettingsPanel());
    settingsWindow_->toFront(true);
    settingsWindow_->grabKeyboardFocus();
}

void PluginEditor::closeSettingsWindow()
{
    if (settingsWindow_ != nullptr)
        settingsWindow_->setVisible(false);
}

void PluginEditor::openAboutWindow()
{
    closeSettingsWindow();

    if (aboutWindow_ == nullptr)
    {
        aboutWindow_ = std::make_unique<AboutWindow>(
            *skin_,
            [this] { closeAboutWindow(); });
        addChildComponent(*aboutWindow_);
    }
    else
    {
        aboutWindow_->setSkin(*skin_);
    }

    const int baseWidth = layoutDimensions_.editor.width;
    const float uiScale = (baseWidth > 0)
        ? TSS::ScaledLayout::uiScaleFromEditorBounds(getWidth(), baseWidth)
        : 1.0f;
    updateAboutWindowLayout(uiScale);

    aboutWindow_->setVisible(true);
    aboutWindow_->toFront(true);
    aboutWindow_->grabKeyboardFocus();
}

void PluginEditor::closeAboutWindow()
{
    if (aboutWindow_ != nullptr)
        aboutWindow_->setVisible(false);
}

void PluginEditor::openMasterInitConfirmDialog(const juce::String& moduleDisplayName,
                                               std::function<void()> onConfirm)
{
    closeSettingsWindow();
    closeAboutWindow();

    if (masterInitConfirmDialog_ == nullptr)
    {
        masterInitConfirmDialog_ = std::make_unique<MasterInitConfirmDialog>(
            *skin_,
            [this] { closeMasterInitConfirmDialog(); });
        addChildComponent(*masterInitConfirmDialog_);
    }
    else
    {
        masterInitConfirmDialog_->setSkin(*skin_);
    }

    masterInitConfirmDialog_->prepareForShow(moduleDisplayName, std::move(onConfirm));

    const int baseWidth = layoutDimensions_.editor.width;
    const float uiScale = (baseWidth > 0)
        ? TSS::ScaledLayout::uiScaleFromEditorBounds(getWidth(), baseWidth)
        : 1.0f;
    updateMasterInitConfirmDialogLayout(uiScale);

    masterInitConfirmDialog_->setVisible(true);
    masterInitConfirmDialog_->toFront(true);
    masterInitConfirmDialog_->grabKeyboardFocus();
}

void PluginEditor::closeMasterInitConfirmDialog()
{
    if (masterInitConfirmDialog_ != nullptr)
        masterInitConfirmDialog_->setVisible(false);
}

void PluginEditor::showBankTransferProgressDialog(const juce::String& title,
                                                  const juce::String& message,
                                                  int totalSteps,
                                                  std::function<void()> onCancelRequested)
{
    closeSettingsWindow();
    closeAboutWindow();

    if (bankTransferProgressDialog_ == nullptr)
    {
        bankTransferProgressDialog_ = std::make_unique<BankTransferProgressDialog>(*skin_);
        addChildComponent(*bankTransferProgressDialog_);
    }
    else
    {
        bankTransferProgressDialog_->setSkin(*skin_);
    }

    bankTransferProgressDialog_->prepareForShow(title, message, totalSteps, std::move(onCancelRequested));

    const int baseWidth = layoutDimensions_.editor.width;
    const float uiScale = (baseWidth > 0)
        ? TSS::ScaledLayout::uiScaleFromEditorBounds(getWidth(), baseWidth)
        : 1.0f;
    updateBankTransferProgressDialogLayout(uiScale);

    bankTransferProgressDialog_->setVisible(true);
    bankTransferProgressDialog_->toFront(true);
    bankTransferProgressDialog_->grabKeyboardFocus();
}

void PluginEditor::hideBankTransferProgressDialog()
{
    if (bankTransferProgressDialog_ != nullptr)
        bankTransferProgressDialog_->setVisible(false);
}

void PluginEditor::restoreSettingsPanelFromState(SettingsPanel& panel)
{
    if (!pluginProcessor.isStandalone())
        panel.getHardwareLatencySlider().setValue(pluginProcessor.getHardwareLatencyMs(), juce::dontSendNotification);

    const int policy = static_cast<int>(pluginProcessor.getApvts().state.getProperty(
        PluginIDs::Settings::kComputerPatchesNameReconciliationPolicy,
        PluginIDs::Settings::NameReconciliationPolicy::kDefault));
    panel.getNameReconciliationPolicyCombo().setSelectedId(policy, juce::dontSendNotification);

    const int unsavedPolicyRaw = static_cast<int>(pluginProcessor.getApvts().state.getProperty(
        PluginIDs::Settings::kUnsavedEditWarningPolicy,
        PluginIDs::Settings::UnsavedEditWarningPolicy::kDefault));
    const int unsavedPolicy =
        (unsavedPolicyRaw == PluginIDs::Settings::UnsavedEditWarningPolicy::kWarnAlways
         || unsavedPolicyRaw == PluginIDs::Settings::UnsavedEditWarningPolicy::kNeverWarn)
            ? unsavedPolicyRaw
            : PluginIDs::Settings::UnsavedEditWarningPolicy::kDefault;
    panel.getUnsavedEditWarningPolicyCombo().setSelectedId(unsavedPolicy, juce::dontSendNotification);

    const int mutatorDeletePolicyRaw = static_cast<int>(pluginProcessor.getApvts().state.getProperty(
        PluginIDs::Settings::kMutatorDeleteWarningPolicy,
        PluginIDs::Settings::MutatorDeleteWarningPolicy::kDefault));
    const int mutatorDeletePolicy =
        (mutatorDeletePolicyRaw == PluginIDs::Settings::MutatorDeleteWarningPolicy::kWarnAlways
         || mutatorDeletePolicyRaw == PluginIDs::Settings::MutatorDeleteWarningPolicy::kNeverWarn)
            ? mutatorDeletePolicyRaw
            : PluginIDs::Settings::MutatorDeleteWarningPolicy::kDefault;
    if (mutatorDeletePolicy != mutatorDeletePolicyRaw)
    {
        pluginProcessor.getApvts().state.setProperty(
            PluginIDs::Settings::kMutatorDeleteWarningPolicy,
            mutatorDeletePolicy,
            nullptr);
    }
    panel.getMutatorDeleteWarningPolicyCombo().setSelectedId(mutatorDeletePolicy, juce::dontSendNotification);
}

void PluginEditor::wireSettingsPanel(SettingsPanel& panel)
{
    panel.getHardwareLatencySlider().onValueChange = [this, &panel]
    {
        pluginProcessor.setHardwareLatencyMs(static_cast<float>(panel.getHardwareLatencySlider().getValue()));
    };

    panel.getNameReconciliationPolicyCombo().onChange = [this, &panel]
    {
        pluginProcessor.getApvts().state.setProperty(
            PluginIDs::Settings::kComputerPatchesNameReconciliationPolicy,
            panel.getNameReconciliationPolicyCombo().getSelectedId(),
            nullptr);
    };

    panel.getUnsavedEditWarningPolicyCombo().onChange = [this, &panel]
    {
        pluginProcessor.getApvts().state.setProperty(
            PluginIDs::Settings::kUnsavedEditWarningPolicy,
            panel.getUnsavedEditWarningPolicyCombo().getSelectedId(),
            nullptr);
    };

    panel.getMutatorDeleteWarningPolicyCombo().onChange = [this, &panel]
    {
        using namespace PluginIDs::Settings::MutatorDeleteWarningPolicy;

        const int selectedId = panel.getMutatorDeleteWarningPolicyCombo().getSelectedId();
        if (selectedId != kWarnAlways && selectedId != kNeverWarn)
            return;

        pluginProcessor.getApvts().state.setProperty(
            PluginIDs::Settings::kMutatorDeleteWarningPolicy,
            selectedId,
            nullptr);
    };
}

void PluginEditor::refreshAudioFromCombo(HeaderPanel* headerOverride)
{
    if (!pluginProcessor.isStandalone())
        return;

    const auto names = pluginProcessor.getAudioInputSourceNames();
    const auto ids = pluginProcessor.getAudioInputSourceIds();

    auto* header = headerOverride;

    if (header == nullptr && mainComponent_ != nullptr)
        header = &mainComponent_->getHeaderPanel();

    juce::String sourceIdToRestore = pluginProcessor.getApvts().state.getProperty(
        "audioFromSourceId", juce::String()).toString();

    if (header != nullptr)
    {
        if (sourceIdToRestore.isEmpty())
            sourceIdToRestore = header->getSelectedAudioFromSourceId();

        header->populateAudioFromCombo(names, ids);

        if (sourceIdToRestore.isNotEmpty() && ids.contains(sourceIdToRestore))
        {
            header->selectAudioFromSourceId(sourceIdToRestore);
            pluginProcessor.setAudioFromSourceId(sourceIdToRestore);
            return;
        }

        if (sourceIdToRestore.isEmpty())
        {
            const auto effectiveSourceId = header->getSelectedAudioFromSourceId();

            if (effectiveSourceId.isNotEmpty())
            {
                header->selectAudioFromSourceId(effectiveSourceId);
                pluginProcessor.setAudioFromSourceId(effectiveSourceId);
            }
            else if (! ids.isEmpty())
            {
                header->selectAudioFromSourceId(ids[0]);
                pluginProcessor.setAudioFromSourceId(ids[0]);
            }
            else
            {
                header->selectAudioFromSourceId({});
            }
        }
        else
        {
            // Stale id not in catalog: keep APVTS preference; avoid writing combo fallback of the wrong kind.
            header->selectAudioFromSourceId(sourceIdToRestore);
        }

        return;
    }

    if (sourceIdToRestore.isEmpty() && ! ids.isEmpty())
        sourceIdToRestore = ids[0];

    if (sourceIdToRestore.isNotEmpty() && (ids.isEmpty() || ids.contains(sourceIdToRestore)))
        pluginProcessor.setAudioFromSourceId(sourceIdToRestore);
}

void PluginEditor::attachStandaloneAudioDeviceListener()
{
    if (!pluginProcessor.isStandalone())
        return;

    Core::StandaloneAudioInputRouter::addAudioDeviceChangeListener(*this);
}

void PluginEditor::detachStandaloneAudioDeviceListener()
{
    Core::StandaloneAudioInputRouter::removeAudioDeviceChangeListener(*this);
}

void PluginEditor::changeListenerCallback(juce::ChangeBroadcaster*)
{
    refreshAudioFromCombo();

    if (! pluginProcessor.isStandalone())
        return;

    const auto sourceId = pluginProcessor.getApvts().state.getProperty("audioFromSourceId", juce::String()).toString();
    if (sourceId.isNotEmpty())
        Core::StandaloneAudioInputRouter::enableInputMonitoring();
}

void PluginEditor::valueTreePropertyChanged(juce::ValueTree&,
                                            const juce::Identifier& property)
{
    const auto propertyName = property.toString();

    if (propertyName == "midiInputPortId" || propertyName == "midiOutputPortId"
        || propertyName == "keyboardFromPortId")
    {
        juce::MessageManager::callAsync(
            [safeThis = juce::Component::SafePointer<PluginEditor>(this), propertyName]
            {
                if (safeThis == nullptr || safeThis->mainComponent_ == nullptr)
                    return;

                auto& header = safeThis->mainComponent_->getHeaderPanel();
                auto& state = safeThis->pluginProcessor.getApvts().state;

                if (propertyName == "midiInputPortId")
                    header.selectMidiFromPort(state.getProperty("midiInputPortId", juce::String()).toString());
                else if (propertyName == "midiOutputPortId")
                    header.selectMidiToPort(state.getProperty("midiOutputPortId", juce::String()).toString());
                else if (safeThis->pluginProcessor.isStandalone())
                    header.selectKeyboardFromPort(
                        state.getProperty("keyboardFromPortId", juce::String()).toString());
            });
        return;
    }

    if (! pluginProcessor.isStandalone())
        return;

    if (propertyName == MatrixDeviceTypes::kApvtsPropertyName
        || propertyName == "deviceDetected")
    {
        juce::MessageManager::callAsync(
            [safeThis = juce::Component::SafePointer<PluginEditor>(this)]
            {
                if (safeThis != nullptr)
                    safeThis->refreshAudioFromCombo();
            });
    }
}

void PluginEditor::valueTreeRedirected(juce::ValueTree&)
{
    if (! pluginProcessor.isStandalone())
        return;

    juce::MessageManager::callAsync(
        [safeThis = juce::Component::SafePointer<PluginEditor>(this)]
        {
            if (safeThis != nullptr)
                safeThis->refreshAudioFromCombo();
        });
}
