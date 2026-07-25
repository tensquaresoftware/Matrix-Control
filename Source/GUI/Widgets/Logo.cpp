#include "Logo.h"

#include "GUI/Looks/LookBuilders.h"
#include "GUI/Skins/ColourChart.h"
#include "GUI/Skins/ISkin.h"
#include "GUI/Skins/SkinValues.h"
#include "Shared/Definitions/PluginDisplayNames.h"

namespace TSS
{
    Logo::Logo(ISkin& skin, int width, int height)
        : Label(width,
                height,
                brandLabelLookFromSkin(skin),
                PluginDisplayNames::kPluginName)
        , skin_(&skin)
    {
        setInterceptsMouseClicks(true, false);
    }

    void Logo::setSkin(ISkin& skin)
    {
        skin_ = &skin;
        setLook(brandLabelLookFromSkin(skin));
        applyTextColour();
    }

    void Logo::setHighlighted(bool highlighted)
    {
        if (isHighlighted_ == highlighted)
            return;

        isHighlighted_ = highlighted;
        applyTextColour();
    }

    void Logo::applyTextColour()
    {
        if (skin_ == nullptr)
            return;

        auto look = brandLabelLookFromSkin(*skin_);
        look.text = isHighlighted_
                        ? juce::Colour(ColourChart::kWhite)
                        : skin_->getColour(SkinColourId::kDarkPanelText);
        setLook(look);
    }

    void Logo::mouseUp(const juce::MouseEvent& e)
    {
        if (e.getNumberOfClicks() > 1)
            return;

        // Shift+Ctrl: Debug toggles the UI test harness; Release is a hard no-op
        // (must not fall through to Shift → Settings).
        if (e.mods.isShiftDown() && e.mods.isCtrlDown())
        {
            stopTimer();
            pendingAudioMidiSettings_ = false;
#if JUCE_DEBUG
            if (onUiTestsToggleRequested)
                onUiTestsToggleRequested();
#endif
            return;
        }

        if (e.mods.isShiftDown())
        {
            stopTimer();
            pendingAudioMidiSettings_ = false;

            if (onSettingsRequested)
                onSettingsRequested();
            return;
        }

        // Alt (Windows/Linux) / Option (macOS): defer via the same click/double-click
        // timer so Alt+double-click only resets UI scale (does not also open Audio/MIDI).
        pendingAudioMidiSettings_ = e.mods.isAltDown();
        startTimer(200);
    }

    void Logo::mouseDoubleClick(const juce::MouseEvent&)
    {
        stopTimer();
        pendingAudioMidiSettings_ = false;

        if (onUiScaleReset)
            onUiScaleReset();
    }

    void Logo::timerCallback()
    {
        stopTimer();

        if (pendingAudioMidiSettings_)
        {
            pendingAudioMidiSettings_ = false;

            if (onAudioMidiSettingsRequested)
                onAudioMidiSettingsRequested();

            return;
        }

        if (onPopupRequested)
            onPopupRequested();
    }

    void Logo::mouseEnter(const juce::MouseEvent&)
    {
        setHighlighted(true);
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    void Logo::mouseExit(const juce::MouseEvent&)
    {
        setHighlighted(false);
        setMouseCursor(juce::MouseCursor::NormalCursor);
    }
}
