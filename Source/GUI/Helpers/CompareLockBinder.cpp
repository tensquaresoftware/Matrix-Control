#include "GUI/Helpers/CompareLockBinder.h"

#include "Core/MIDI/EditorOutboundGate.h"
#include "Core/Services/DeviceTypeRegistry.h"
#include "GUI/Helpers/GrayedControlHelper.h"
#include "Shared/Definitions/MatrixDeviceTypes.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"

namespace
{
    namespace MutatorState = PluginIDs::PatchManagerSection::PatchMutatorModule::StateProperties;
    namespace FooterCopy = PluginDisplayNames::FooterPanel;
    namespace CompareMessages = PluginDisplayNames::PatchManagerSection::PatchMutatorModule::Messages;

    constexpr float kLockedAlpha = 0.5f;
    const juce::Identifier kDeviceDetectedId("deviceDetected");
    const juce::Identifier kDeviceTypeId(MatrixDeviceTypes::kApvtsPropertyName);

    void applySectionLock(juce::Component& component, bool locked)
    {
        // JUCE 8 getComponentAt() always descends into children when the parent hit-tests true.
        // setInterceptsMouseClicks(true, false) therefore only dims visually if paired with alpha —
        // it does NOT block child interaction. Use (false, false) so hitTest fails on the whole
        // subtree and clicks fall through to the parent (non-interactive while locked).
        component.setInterceptsMouseClicks(! locked, ! locked);
        component.setAlpha(locked ? kLockedAlpha : 1.0f);

        if (locked)
            component.giveAwayKeyboardFocus();
    }

    bool isOwnedDeviceLockFooter(const juce::String& text)
    {
        return text == juce::String(FooterCopy::kDeviceLockGuidance)
            || text == juce::String(FooterCopy::kUnsupportedMatrixDeviceFooter);
    }
}

namespace TSS
{
    CompareLockBinder::CompareLockBinder(juce::AudioProcessorValueTreeState& apvts,
                                         std::vector<juce::Component*> lockedComponents,
                                         bool lockOnCompare)
        : apvts_(apvts)
        , lockOnCompare_(lockOnCompare)
    {
        locked_.reserve(lockedComponents.size());
        for (auto* component : lockedComponents)
            locked_.emplace_back(component);

        apvts_.state.addListener(this);
        apply();
    }

    CompareLockBinder::~CompareLockBinder()
    {
        apvts_.state.removeListener(this);
    }

    void CompareLockBinder::valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier& property)
    {
        if (property.toString() == MutatorState::kCompareActive
            || property == kDeviceDetectedId
            || property == kDeviceTypeId)
        {
            apply();
        }
    }

    void CompareLockBinder::valueTreeRedirected(juce::ValueTree&)
    {
        apply();
    }

    void CompareLockBinder::syncDeviceLockFooter(bool deviceDetected,
                                                 MatrixDeviceTypes::Type deviceType,
                                                 bool compareActive)
    {
        const bool deviceLocked = ! Core::isEditorOutboundAllowed(deviceDetected, deviceType);

        if (deviceLocked)
        {
            const auto* message = deviceDetected
                                      ? FooterCopy::kUnsupportedMatrixDeviceFooter
                                      : FooterCopy::kDeviceLockGuidance;
            GrayedControlHelper::setFooterInfoMessage(apvts_, message);
            return;
        }

        if (! isOwnedDeviceLockFooter(apvts_.state.getProperty("uiMessageText").toString()))
            return;

        // Exact-string clear (match Compare): text + severity, then restore Compare copy if still active.
        apvts_.state.setProperty("uiMessageText", juce::String(), nullptr);
        apvts_.state.setProperty("uiMessageSeverity", juce::String(), nullptr);

        if (compareActive)
            GrayedControlHelper::setFooterInfoMessage(apvts_, CompareMessages::kCompareLockedFooter);
    }

    void CompareLockBinder::apply()
    {
        const bool compareActive = static_cast<bool>(
            apvts_.state.getProperty(MutatorState::kCompareActive, false));
        const bool deviceDetected = static_cast<bool>(
            apvts_.state.getProperty(kDeviceDetectedId, false));
        const auto deviceType = Core::DeviceTypeRegistry::fromApvtsProperty(
            apvts_.state.getProperty(kDeviceTypeId));

        const bool locked = lockOnCompare_
                                ? Core::isSectionLocked(deviceDetected, deviceType, compareActive)
                                : ! Core::isEditorOutboundAllowed(deviceDetected, deviceType);

        for (auto& safeComponent : locked_)
        {
            if (auto* component = safeComponent.getComponent())
                applySectionLock(*component, locked);
        }

        syncDeviceLockFooter(deviceDetected, deviceType, compareActive);
    }
}
