#pragma once

#include "OpenGlImageComponent.h"

namespace tss
{
    template <class ComponentType>
    class OpenGlAutoImageComponent : public ComponentType
    {
    public:
        template <typename... Args>
        explicit OpenGlAutoImageComponent(Args&&... args)
            : ComponentType(std::forward<Args>(args)...)
        {
        }

        ~OpenGlAutoImageComponent() override = default;

        void mouseDown(const juce::MouseEvent& e) override
        {
            ComponentType::mouseDown(e);
            redoImage();
        }

        void mouseUp(const juce::MouseEvent& e) override
        {
            ComponentType::mouseUp(e);
            redoImage();
        }

        void mouseEnter(const juce::MouseEvent& e) override
        {
            ComponentType::mouseEnter(e);
            redoImage();
        }

        void mouseExit(const juce::MouseEvent& e) override
        {
            ComponentType::mouseExit(e);
            redoImage();
        }

        void mouseDrag(const juce::MouseEvent& e) override
        {
            ComponentType::mouseDrag(e);
            redoImage();
        }

        void focusGained(juce::Component::FocusChangeType cause) override
        {
            ComponentType::focusGained(cause);
            redoImage();
        }

        void focusLost(juce::Component::FocusChangeType cause) override
        {
            ComponentType::focusLost(cause);
            redoImage();
        }

    protected:
        virtual void redoImage()
        {
            if (auto* imageComponent = dynamic_cast<OpenGlImageComponent*>(this))
            {
                imageComponent->redrawImage(true);
            }
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGlAutoImageComponent)
    };
}
