#include "PopupMenuPositioner.h"
#include "ComboBox.h"

namespace TSS
{
    PopupMenuDimensions PopupMenuPositioner::calculateDimensions(
        const ComboBox& comboBox,
        int popupWidth,
        int popupHeight,
        int /* borderThickness */)
    {
        return calculateDimensions(static_cast<const juce::Component&>(comboBox),
                                   DimensionsArgs{
                                       .popupWidth = popupWidth,
                                       .popupHeight = popupHeight,
                                       .verticalMargin = comboBox.getScaledVerticalMargin(),
                                       .placement = comboBox.getPopupVerticalPlacement()});
    }

    PopupMenuDimensions PopupMenuPositioner::calculateDimensions(
        const juce::Component& anchor,
        const DimensionsArgs& args)
    {
        auto* topLevelComponent = anchor.getTopLevelComponent();
        if (topLevelComponent == nullptr)
            return { args.popupWidth, args.popupHeight, 0, 0, false };

        const auto topLevelScreenBounds = topLevelComponent->getScreenBounds();
        const auto belowPosition = positionBelow(anchor, args.verticalMargin);
        bool opensAbove = false;
        const int resolvedY = resolveYPosition(anchor,
                                               ResolveYArgs{
                                                   .desiredYBelow = belowPosition.getY(),
                                                   .popupHeight = args.popupHeight,
                                                   .screenBounds = topLevelScreenBounds,
                                                   .verticalMargin = args.verticalMargin,
                                                   .placement = args.placement},
                                               opensAbove);
        const int resolvedX = adjustXPosition(anchor,
                                              belowPosition.getX(),
                                              args.popupWidth,
                                              topLevelScreenBounds);
        const juce::Point<int> adjustedPosition { resolvedX, resolvedY };
        const auto relativePosition = adjustedPosition - topLevelScreenBounds.getPosition();

        return { args.popupWidth, args.popupHeight, relativePosition.getX(), relativePosition.getY(), opensAbove };
    }

    juce::Point<int> PopupMenuPositioner::positionBelow(
        const juce::Component& anchor,
        int verticalMargin)
    {
        return anchor.localPointToGlobal(juce::Point<int>(0, anchor.getHeight() + verticalMargin));
    }

    juce::Point<int> PopupMenuPositioner::positionAbove(
        const juce::Component& anchor,
        int popupHeight,
        int verticalMargin)
    {
        return anchor.localPointToGlobal(juce::Point<int>(0, -popupHeight - verticalMargin));
    }

    int PopupMenuPositioner::adjustXPosition(
        const juce::Component& anchor,
        int desiredX,
        int popupWidth,
        const juce::Rectangle<int>& screenBounds)
    {
        if (desiredX + popupWidth > screenBounds.getRight())
        {
            const auto anchorRight = anchor.localPointToGlobal(juce::Point<int>(anchor.getWidth(), 0)).getX();
            return anchorRight - popupWidth;
        }

        return desiredX;
    }

    int PopupMenuPositioner::resolveYPosition(
        const juce::Component& anchor,
        const ResolveYArgs& args,
        bool& opensAbove)
    {
        switch (args.placement)
        {
            case PopupVerticalPlacement::Above:
                opensAbove = true;
                return positionAbove(anchor, args.popupHeight, args.verticalMargin).getY();

            case PopupVerticalPlacement::Below:
                opensAbove = false;
                return args.desiredYBelow;

            case PopupVerticalPlacement::Auto:
            default:
                if (args.desiredYBelow + args.popupHeight > args.screenBounds.getBottom())
                {
                    opensAbove = true;
                    return positionAbove(anchor, args.popupHeight, args.verticalMargin).getY();
                }

                opensAbove = false;
                return args.desiredYBelow;
        }
    }
}
