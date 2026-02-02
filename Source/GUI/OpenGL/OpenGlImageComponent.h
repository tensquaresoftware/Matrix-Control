#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>

#include "OpenGlComponent.h"

namespace tss
{
    class OpenGlImageComponent : public juce::Component, public OpenGlComponent
    {
    public:
        OpenGlImageComponent();
        ~OpenGlImageComponent() override;

        void init(juce::OpenGLContext& openGlContext) override;
        void render(juce::OpenGLContext& openGlContext, bool animate) override;
        void destroy(juce::OpenGLContext& openGlContext) override;

        juce::Rectangle<int> getGlBounds() const override { return getBounds(); }

        virtual void redrawImage(bool force = false);
        virtual void paintToImage(juce::Graphics& g) = 0;

        void setStaticImage(bool isStatic) { staticImage_ = isStatic; }
        bool isStaticImage() const { return staticImage_; }

        void setScaleFactor(float scale);
        float getScaleFactor() const { return scaleFactor_; }

        void resized() override;

    protected:
        void invalidateImage();
        float getPixelScale() const;

        std::unique_ptr<juce::Image> drawImage_;
        juce::OpenGLTexture texture_;
        juce::CriticalSection imageLock_;

        bool imageValid_ {false};
        bool staticImage_ {false};
        bool sizeChanged_ {false};
        float scaleFactor_ {1.0f};

    private:
        void uploadTextureToGpu();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGlImageComponent)
    };
}
