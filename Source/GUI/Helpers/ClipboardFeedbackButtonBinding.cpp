#include "GUI/Helpers/ClipboardFeedbackButtonBinding.h"

#include "GUI/Widgets/Button.h"
#include "Shared/Definitions/PluginIDs.h"

namespace TSS
{

ClipboardFeedbackButtonBinding::ClipboardFeedbackButtonBinding(juce::AudioProcessorValueTreeState& apvts,
                                                               Button& button,
                                                               const juce::String& rolePropertyId,
                                                               bool isSourceRole)
    : state_(apvts.state)
    , button_(button)
    , rolePropertyId_(rolePropertyId)
    , isSourceRole_(isSourceRole)
{
    state_.addListener(this);
    apply();
}

ClipboardFeedbackButtonBinding::~ClipboardFeedbackButtonBinding()
{
    state_.removeListener(this);
    button_.setAlpha(1.0f);
    button_.setToggleState(false, juce::dontSendNotification);
}

void ClipboardFeedbackButtonBinding::valueTreePropertyChanged(juce::ValueTree&,
                                                              const juce::Identifier& property)
{
    const auto name = property.toString();
    if (name == PluginIDs::ClipboardFeedback::kActive
        || name == PluginIDs::ClipboardFeedback::kCopyLit
        || name == rolePropertyId_
        || name.endsWith("PasteEnabled")
        || name.endsWith("PasteBankEnabled")
        || name == PluginIDs::PatchManagerSection::InternalPatchesModule::StandaloneWidgets::kPastePatchEnabled)
    {
        apply();
    }
}

void ClipboardFeedbackButtonBinding::valueTreeRedirected(juce::ValueTree&)
{
    apply();
}

void ClipboardFeedbackButtonBinding::apply()
{
    const bool sessionActive = static_cast<bool>(
        state_.getProperty(PluginIDs::ClipboardFeedback::kActive, false));
    const bool roleActive = static_cast<bool>(state_.getProperty(rolePropertyId_, false));
    const bool copyLit = static_cast<bool>(
        state_.getProperty(PluginIDs::ClipboardFeedback::kCopyLit, true));

    const bool participate = sessionActive && roleActive;

    if (! participate
        || ! button_.isEnabled()
        || (! isSourceRole_ && button_.isInactiveAppearance()))
    {
        button_.setToggleState(false, juce::dontSendNotification);
        button_.setAlpha(1.0f);
        return;
    }

    const bool lit = isSourceRole_ ? copyLit : ! copyLit;
    // Lit = red On text; dim = base Off text. Keep full alpha so border stays normal.
    button_.setToggleState(lit, juce::dontSendNotification);
    button_.setAlpha(1.0f);
}

} // namespace TSS
