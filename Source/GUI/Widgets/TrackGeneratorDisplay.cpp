#include "TrackGeneratorDisplay.h"

#include "GUI/Themes/Skin.h"
#include "GUI/Themes/ColourChart.h"

namespace tss
{
    TrackGeneratorDisplay::TrackGeneratorDisplay(tss::Skin& skin, int width, int height)
        : skin_(&skin)
        , width_(width)
        , height_(height)
    {
        setOpaque(false);
        setSize(width_, height_);
        updateSkinCache();
        cachedPointValues_ = pointValues_;
    }

    void TrackGeneratorDisplay::setSkin(tss::Skin& skin)
    {
        skin_ = &skin;
        updateSkinCache();
        invalidateCache();
    }

    void TrackGeneratorDisplay::paint(juce::Graphics& g)
    {
        if (skin_ == nullptr)
            return;

        const auto bounds = getLocalBounds().toFloat();
        const auto contentBounds = bounds.reduced(0.0f, static_cast<float>(kVerticalPadding_));

        drawBackground(g, contentBounds);
        drawBorder(g, contentBounds);
        drawTriangle(g, contentBounds);
        
        if (!cacheValid_)
            regenerateCache();
        
        const auto destRect = contentBounds.toFloat();
        g.drawImage(cachedImage_, destRect, juce::RectanglePlacement::fillDestination);
    }


    void TrackGeneratorDisplay::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto backgroundColour = skin_->getTrackGeneratorDisplayBackgroundColour();
        g.setColour(backgroundColour);
        g.fillRect(bounds);
    }

    void TrackGeneratorDisplay::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto borderColour = skin_->getTrackGeneratorDisplayBorderColour();
        g.setColour(borderColour);
        g.drawRect(bounds, static_cast<float>(kBorderThickness_));
    }

    void TrackGeneratorDisplay::drawTriangle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto triangleColour = skin_->getTrackGeneratorDisplayBorderColour();
        const auto triangleHeight = kTriangleBase_ * std::sqrt(3.0f) * 0.5f;
        const auto centreX = bounds.getCentreX();
        const auto baseY = bounds.getY();

        juce::Path triangle;
        triangle.addTriangle(centreX - kTriangleBase_ * 0.5f, baseY,
                             centreX + kTriangleBase_ * 0.5f, baseY,
                             centreX, baseY - triangleHeight);

        g.setColour(triangleColour);
        g.fillPath(triangle);
    }
    
    void TrackGeneratorDisplay::setOnValueChanged(ValueChangedCallback callback)
    {
        onValueChanged_ = std::move(callback);
    }
    
    void TrackGeneratorDisplay::setTrackPoint1(int value)
    {
        const int clampedValue = juce::jlimit(kMinValue_, kMaxValue_, value);
        
        if (pointValues_[0] != clampedValue)
        {
            pointValues_[0] = clampedValue;
            invalidateCache();
        }
    }
    
    void TrackGeneratorDisplay::setTrackPoint2(int value)
    {
        const int clampedValue = juce::jlimit(kMinValue_, kMaxValue_, value);
        
        if (pointValues_[1] != clampedValue)
        {
            pointValues_[1] = clampedValue;
            invalidateCache();
        }
    }
    
    void TrackGeneratorDisplay::setTrackPoint3(int value)
    {
        const int clampedValue = juce::jlimit(kMinValue_, kMaxValue_, value);
        
        if (pointValues_[2] != clampedValue)
        {
            pointValues_[2] = clampedValue;
            invalidateCache();
        }
    }
    
    void TrackGeneratorDisplay::setTrackPoint4(int value)
    {
        const int clampedValue = juce::jlimit(kMinValue_, kMaxValue_, value);
        
        if (pointValues_[3] != clampedValue)
        {
            pointValues_[3] = clampedValue;
            invalidateCache();
        }
    }
    
    void TrackGeneratorDisplay::setTrackPoint5(int value)
    {
        const int clampedValue = juce::jlimit(kMinValue_, kMaxValue_, value);
        
        if (pointValues_[4] != clampedValue)
        {
            pointValues_[4] = clampedValue;
            invalidateCache();
        }
    }
    
    float TrackGeneratorDisplay::getPixelScale() const
    {
        if (auto* display = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay())
            return static_cast<float>(display->scale);
        return 1.0f;
    }
    
    void TrackGeneratorDisplay::regenerateCache()
    {
        if (skin_ == nullptr)
            return;
        
        bool valuesChanged = false;
        for (size_t i = 0; i < kPointCount_; ++i)
        {
            if (cachedPointValues_[i] != pointValues_[i])
            {
                valuesChanged = true;
                break;
            }
        }
        
        if (!valuesChanged && cacheValid_)
            return;
        
        const float scale = getPixelScale();
        const int cacheWidth = juce::roundToInt(static_cast<float>(width_) * scale);
        const int cacheHeight = juce::roundToInt(static_cast<float>(height_ - kVerticalPadding_ * 2) * scale);
        
        cachedImage_ = juce::Image(juce::Image::ARGB, cacheWidth, cacheHeight, true);
        
        juce::Graphics g(cachedImage_);
        g.addTransform(juce::AffineTransform::scale(scale));
        
        const auto bounds = juce::Rectangle<float>(0.0f, 0.0f,
                                                    static_cast<float>(width_),
                                                    static_cast<float>(height_ - kVerticalPadding_ * 2));
        
        drawCurve(g, bounds);
        
        cachedPointValues_ = pointValues_;
        cacheValid_ = true;
    }
    
    void TrackGeneratorDisplay::invalidateCache()
    {
        cacheValid_ = false;
        repaint();
    }
    
    void TrackGeneratorDisplay::updateSkinCache()
    {
        if (skin_ == nullptr)
            return;
        
        cachedCurveColour_ = juce::Colour(ColourChart::kGreen4);
    }
    
    void TrackGeneratorDisplay::drawCurve(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto drawArea = bounds.reduced(kPadding_);
        
        if (drawArea.getWidth() <= 0.0f || drawArea.getHeight() <= 0.0f)
            return;
        
        g.setColour(cachedCurveColour_);
        
        const float halfPixel = 0.5f;
        
        for (int i = 0; i < kPointCount_; ++i)
        {
            const auto p = calculatePointPosition(i, drawArea);
            const float cx = std::round(p.x) + halfPixel;
            const float cy = std::round(p.y) + halfPixel;
            
            if (i < kPointCount_ - 1)
            {
                const auto p2 = calculatePointPosition(i + 1, drawArea);
                const float cx2 = std::round(p2.x) + halfPixel;
                const float cy2 = std::round(p2.y) + halfPixel;
                g.drawLine(cx, cy, cx2, cy2, kLineThickness_);
            }
            
            g.fillEllipse(cx - kPointRadius_, cy - kPointRadius_,
                         kPointRadius_ * 2.0f, kPointRadius_ * 2.0f);
        }
    }
    
    juce::Point<float> TrackGeneratorDisplay::calculatePointPosition(int pointIndex, 
                                                                      const juce::Rectangle<float>& bounds) const
    {
        const float xPosition = bounds.getX() + (bounds.getWidth() * pointIndex) / (kPointCount_ - 1);
        
        const float normalizedValue = static_cast<float>(pointValues_[static_cast<size_t>(pointIndex)]) / kMaxValue_;
        const float yPosition = bounds.getBottom() - (bounds.getHeight() * normalizedValue);
        
        return juce::Point<float>(xPosition, yPosition);
    }
    
    int TrackGeneratorDisplay::findPointAtPosition(const juce::Point<float>& position, 
                                                    const juce::Rectangle<float>& bounds) const
    {
        const auto drawArea = bounds.reduced(kPadding_);
        
        for (int i = 0; i < kPointCount_; ++i)
        {
            const auto pointPos = calculatePointPosition(i, drawArea);
            const float distance = position.getDistanceFrom(pointPos);
            
            if (distance <= kHitZoneRadius_)
                return i;
        }
        
        return -1;
    }
    
    void TrackGeneratorDisplay::mouseDown(const juce::MouseEvent& e)
    {
        const auto bounds = getLocalBounds().toFloat();
        const auto contentBounds = bounds.reduced(0.0f, static_cast<float>(kVerticalPadding_));
        
        draggedPointIndex_ = findPointAtPosition(e.position, contentBounds);
    }
    
    void TrackGeneratorDisplay::mouseDrag(const juce::MouseEvent& e)
    {
        if (draggedPointIndex_ < 0)
            return;
        
        const auto bounds = getLocalBounds().toFloat();
        const auto contentBounds = bounds.reduced(0.0f, static_cast<float>(kVerticalPadding_));
        const auto drawArea = contentBounds.reduced(kPadding_);
        
        const float relativeY = e.position.y - drawArea.getY();
        const float normalizedValue = 1.0f - (relativeY / drawArea.getHeight());
        
        const int newValue = juce::jlimit(kMinValue_, kMaxValue_, 
                                          static_cast<int>(normalizedValue * kMaxValue_ + 0.5f));
        
        const auto index = static_cast<size_t>(draggedPointIndex_);
        if (pointValues_[index] != newValue)
        {
            pointValues_[index] = newValue;
            invalidateCache();
            
            if (onValueChanged_)
                onValueChanged_(draggedPointIndex_, newValue);
        }
    }
    
    void TrackGeneratorDisplay::mouseUp(const juce::MouseEvent&)
    {
        draggedPointIndex_ = -1;
    }
}
