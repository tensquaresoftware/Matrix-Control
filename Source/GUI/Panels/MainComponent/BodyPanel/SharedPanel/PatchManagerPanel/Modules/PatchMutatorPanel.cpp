#include "PatchMutatorPanel.h"
#include "PatchMutatorPanelInternal.h"

#include <vector>

#include "GUI/Factories/WidgetFactory.h"
#include "GUI/Widgets/Button.h"
#include "GUI/Widgets/HierarchicalComboBox.h"
#include "GUI/Widgets/Label.h"
#include "GUI/Widgets/ModuleHeader.h"
#include "GUI/Widgets/Slider.h"
#include "GUI/Widgets/Toggle.h"

using namespace PatchMutatorPanelInternal;

class PatchMutatorPanel::ActionEnabledPropertyListener : public juce::ValueTree::Listener
{
public:
    ActionEnabledPropertyListener(juce::ValueTree state, std::vector<ActionEnabledBinding> bindings)
        : state_(std::move(state))
        , bindings_(std::move(bindings))
    {
        state_.addListener(this);
        syncFromState();
    }

    ~ActionEnabledPropertyListener() override
    {
        state_.removeListener(this);
    }

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged,
                                  const juce::Identifier& property) override
    {
        for (const auto& binding : bindings_)
        {
            if (property.toString() != binding.propertyId || binding.button == nullptr)
                continue;

            binding.button->setEnabled(static_cast<bool>(
                treeWhosePropertyHasChanged.getProperty(property, false)));
        }
    }

    void valueTreeChildAdded(juce::ValueTree&, juce::ValueTree&) override {}
    void valueTreeChildRemoved(juce::ValueTree&, juce::ValueTree&, int) override {}
    void valueTreeChildOrderChanged(juce::ValueTree&, int, int) override {}
    void valueTreeParentChanged(juce::ValueTree&) override {}
    void valueTreeRedirected(juce::ValueTree&) override
    {
        syncFromState();
    }

private:
    void syncFromState()
    {
        for (const auto& binding : bindings_)
        {
            if (binding.button == nullptr)
                continue;

            binding.button->setEnabled(static_cast<bool>(
                state_.getProperty(binding.propertyId, false)));
        }
    }

    juce::ValueTree state_;
    std::vector<ActionEnabledBinding> bindings_;
};

PatchMutatorPanel::PatchMutatorPanel(TSS::ISkin& skin,
                                     const PatchMutatorPanelDimensions& dims,
                                     WidgetFactory& widgetFactory,
                                     juce::AudioProcessorValueTreeState& apvts)
    : dims_(dims)
    , skin_(&skin)
    , apvts_(apvts)
{
    setOpaque(false);
    setupModuleHeader(skin, widgetFactory);
    setupAmountLine(skin, widgetFactory);
    setupRandomLine(skin, widgetFactory);
    setupHistoryLine(skin, widgetFactory);

    if (! apvts_.state.hasProperty(MutatorState::kCompareActive))
        apvts_.state.setProperty(MutatorState::kCompareActive, false, nullptr);

    actionEnabledListener_ = std::make_unique<ActionEnabledPropertyListener>(
        apvts_.state,
        std::vector<ActionEnabledBinding>{
            { MutatorState::kMutateEnabled, mutateButton_.get() },
            { MutatorState::kRetryEnabled, retryButton_.get() },
            { MutatorState::kExportEnabled, exportButton_.get() },
            { MutatorState::kDeleteEnabled, deleteButton_.get() },
            { MutatorState::kClearEnabled, clearButton_.get() } });

    apvts_.state.addListener(this);
    refreshRecipeFromApvts();
    refreshHistoryComboBox();
    refreshCompareUiState();

    setSize(dims_.width, dims_.height);
}

PatchMutatorPanel::~PatchMutatorPanel()
{
    stopTimer();
    apvts_.state.removeListener(this);
}

void PatchMutatorPanel::valueTreePropertyChanged(juce::ValueTree&,
                                                 const juce::Identifier& property)
{
    if (recipeHydrating_)
        return;

    const auto name = property.toString();

    if (isRecipeProperty(name))
        refreshRecipeFromApvts();

    if (isHistoryComboRefreshProperty(name))
        scheduleHistoryComboBoxRefresh();

    if (isCompareUiRefreshProperty(name))
        refreshCompareUiState();
}

void PatchMutatorPanel::valueTreeRedirected(juce::ValueTree&)
{
    refreshRecipeFromApvts();
    refreshHistoryComboBox();
    refreshCompareUiState();
}

bool PatchMutatorPanel::isRecipeProperty(const juce::String& propertyName)
{
    return isRecipePropertyId(propertyName);
}

void PatchMutatorPanel::refreshRecipeFromApvts()
{
    auto& state = apvts_.state;

    recipeHydrating_ = true;

    const auto clampRecipePercentProperty = [&state](const char* propertyId, int defaultValue)
    {
        const int raw = state.hasProperty(propertyId)
                            ? static_cast<int>(state.getProperty(propertyId))
                            : defaultValue;
        const int clamped = juce::jlimit(1, 100, raw);

        if (! state.hasProperty(propertyId) || static_cast<int>(state.getProperty(propertyId)) != clamped)
            state.setProperty(propertyId, clamped, nullptr);
    };

    clampRecipePercentProperty(MutatorWidgets::kAmount, 50);
    clampRecipePercentProperty(MutatorWidgets::kRandom, 25);

    hydrateIntSlider(amountSlider_.get(), state, MutatorWidgets::kAmount, 50);
    hydrateIntSlider(randomSlider_.get(), state, MutatorWidgets::kRandom, 25);
    hydrateRecipeTogglesFromApvts(state);
    recipeHydrating_ = false;
}

void PatchMutatorPanel::hydrateRecipeTogglesFromApvts(const juce::ValueTree& state)
{
    hydrateToggleBinding(dco1Toggle_.get(), state, MutatorWidgets::kEnableDco1);
    hydrateToggleBinding(dco2Toggle_.get(), state, MutatorWidgets::kEnableDco2);
    hydrateToggleBinding(vcfVcaToggle_.get(), state, MutatorWidgets::kEnableVcfVca);
    hydrateToggleBinding(fmTrackToggle_.get(), state, MutatorWidgets::kEnableFmTrack);
    hydrateToggleBinding(rampPortamentoToggle_.get(), state, MutatorWidgets::kEnableRampPortamento);
    hydrateToggleBinding(env1Toggle_.get(), state, MutatorWidgets::kEnableEnvelope1);
    hydrateToggleBinding(env2Toggle_.get(), state, MutatorWidgets::kEnableEnvelope2);
    hydrateToggleBinding(env3Toggle_.get(), state, MutatorWidgets::kEnableEnvelope3);
    hydrateToggleBinding(lfo1Toggle_.get(), state, MutatorWidgets::kEnableLfo1);
    hydrateToggleBinding(lfo2Toggle_.get(), state, MutatorWidgets::kEnableLfo2);
    hydrateToggleBinding(enableMatrixModToggle_.get(), state, MutatorWidgets::kEnableMatrixMod);
}

void PatchMutatorPanel::timerCallback()
{
    compareBlinkVisible_ = ! compareBlinkVisible_;
    if (compareButton_ != nullptr)
    {
        // Lit = red On text; dim = normal Off text. Keep alpha at 1 so the border never greys out.
        compareButton_->setToggleState(compareBlinkVisible_, juce::dontSendNotification);
        compareButton_->setAlpha(1.0f);
    }
}

void PatchMutatorPanel::connectButtonToApvts(TSS::Button* button, const char* widgetId)
{
    if (button == nullptr)
        return;

    button->onClick = [this, widgetId]
    {
        apvts_.state.setProperty(widgetId, juce::Time::getCurrentTime().toMilliseconds(), nullptr);
    };
}

void PatchMutatorPanel::connectToggleToApvts(TSS::Toggle* toggle, const char* widgetId)
{
    if (toggle == nullptr)
        return;

    toggle->onStateChange = [this, toggle, widgetId]
    {
        if (recipeHydrating_)
            return;

        apvts_.state.setProperty(widgetId, toggle->getToggleState(), nullptr);
    };
}
