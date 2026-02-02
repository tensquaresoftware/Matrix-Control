#include "OpenGlImageComponent.h"

namespace tss
{
    OpenGlImageComponent::OpenGlImageComponent()
    {
        setOpaque(false);
    }

    OpenGlImageComponent::~OpenGlImageComponent()
    {
        texture_.release();
    }

    void OpenGlImageComponent::init(juce::OpenGLContext&)
    {
        // Texture will be created on first render
    }

    void OpenGlImageComponent::render(juce::OpenGLContext&, bool)
    {
        if (!active_ || !visible_)
            return;

        if (!imageValid_ || sizeChanged_)
        {
            redrawImage(false);
            sizeChanged_ = false;
        }

        if (drawImage_ != nullptr && drawImage_->isValid())
        {
            uploadTextureToGpu();
        }
    }

    void OpenGlImageComponent::destroy(juce::OpenGLContext&)
    {
        texture_.release();
        drawImage_.reset();
    }

    void OpenGlImageComponent::redrawImage(bool force)
    {
        if (!active_)
            return;

        const bool needsRedraw = !imageValid_ || force || sizeChanged_;
        if (!needsRedraw && staticImage_)
            return;

        const auto width = getWidth();
        const auto height = getHeight();

        if (width <= 0 || height <= 0)
            return;

        const float pixelScale = getPixelScale();
        const int imageWidth = static_cast<int>(width * pixelScale);
        const int imageHeight = static_cast<int>(height * pixelScale);

        juce::ScopedLock lock(imageLock_);

        drawImage_ = std::make_unique<juce::Image>(juce::Image::ARGB,
                                                     imageWidth,
                                                     imageHeight,
                                                     true);

        juce::Graphics g(*drawImage_);
        g.addTransform(juce::AffineTransform::scale(pixelScale));

        paintToImage(g);

        imageValid_ = true;
    }

    void OpenGlImageComponent::setScaleFactor(float scale)
    {
        if (std::abs(scaleFactor_ - scale) < 0.001f)
            return;

        scaleFactor_ = scale;
        invalidateImage();
        redrawImage(true);
    }

    void OpenGlImageComponent::resized()
    {
        sizeChanged_ = true;
        invalidateImage();
    }

    void OpenGlImageComponent::invalidateImage()
    {
        imageValid_ = false;
    }

    float OpenGlImageComponent::getPixelScale() const
    {
        const auto* display = juce::Desktop::getInstance()
                                  .getDisplays()
                                  .getDisplayForRect(getScreenBounds());

        const float displayScale = display != nullptr ? static_cast<float>(display->scale) : 1.0f;
        return displayScale * scaleFactor_;
    }

    void OpenGlImageComponent::uploadTextureToGpu()
    {
        if (drawImage_ == nullptr || !drawImage_->isValid())
            return;

        juce::ScopedLock lock(imageLock_);

        if (!texture_.isValidSize(drawImage_->getWidth(), drawImage_->getHeight()))
        {
            texture_.release();
        }

        texture_.loadImage(*drawImage_);
    }
}
