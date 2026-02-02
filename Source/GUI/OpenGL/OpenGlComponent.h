#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>

namespace tss
{
    class OpenGlComponent
    {
    public:
        OpenGlComponent() = default;
        virtual ~OpenGlComponent() = default;

        virtual void init(juce::OpenGLContext& openGlContext) = 0;
        virtual void render(juce::OpenGLContext& openGlContext, bool animate) = 0;
        virtual void destroy(juce::OpenGLContext& openGlContext) = 0;

        void setActive(bool active) { active_ = active; }
        bool isActive() const { return active_; }

        void setVisible(bool visible) { visible_ = visible; }
        bool isVisible() const { return visible_; }

        void setAlwaysOnTop(bool alwaysOnTop) { alwaysOnTop_ = alwaysOnTop; }
        bool isAlwaysOnTop() const { return alwaysOnTop_; }

        virtual juce::Rectangle<int> getGlBounds() const = 0;

    protected:
        bool active_ {true};
        bool visible_ {true};
        bool alwaysOnTop_ {false};

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGlComponent)
    };
}
