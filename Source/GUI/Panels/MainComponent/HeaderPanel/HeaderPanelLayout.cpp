// Extracted from HeaderPanel.cpp for modular maintenance.
// Horizontal packet layout for header controls.

#include "HeaderPanel.h"

#include "GUI/Layout/ScaledLayout.h"
#include "GUI/Widgets/ComboBox.h"
#include "GUI/Widgets/Label.h"
#include "GUI/Widgets/Led.h"
#include "GUI/Widgets/PeakIndicator.h"
#include "GUI/Widgets/Slider.h"

namespace
{
    struct HeaderLayoutMetrics
    {
        float gap = 0.0f;
        float packetExternalGap = 0.0f;
        float controlHeight = 0.0f;
        int controlHeightPx = 0;
        int controlY = 0;
        int ledY = 0;
        float ledSize = 0.0f;
        int ledSizePx = 0;
        float editorMidiFromLabelWidth = 0.0f;
        float midiToLabelWidth = 0.0f;
        float keyboardFromLabelWidth = 0.0f;
        float audioFromLabelWidth = 0.0f;
        float inputGainLabelWidth = 0.0f;
        float portComboWidth = 0.0f;
        float inputGainSliderWidth = 0.0f;
        float peakIndicatorWidth = 0.0f;
        int logoX = 0;
        int logoWidth = 0;
        int logoHeight = 0;
        float contentStartX = 0.0f;

        static HeaderLayoutMetrics make(const HeaderPanelDimensions& dimensions,
                                        float uiScale,
                                        juce::Rectangle<int> bounds)
        {
            HeaderLayoutMetrics m;
            const float sf = uiScale;
            m.gap = static_cast<float>(dimensions.gap) * sf;
            m.packetExternalGap = static_cast<float>(dimensions.packetExternalGap) * sf;
            m.controlHeight = static_cast<float>(dimensions.controlHeight) * sf;
            m.controlHeightPx = juce::roundToInt(m.controlHeight);

            const int contentYOffset = TSS::ScaledLayout::scaledInt(
                static_cast<float>(dimensions.contentVerticalOffset), sf);
            m.controlY = bounds.getY() + (bounds.getHeight() - m.controlHeightPx) / 2 + contentYOffset;

            m.editorMidiFromLabelWidth = static_cast<float>(dimensions.editorMidiFromLabelWidth) * sf;
            m.midiToLabelWidth = static_cast<float>(dimensions.midiToLabelWidth) * sf;
            m.keyboardFromLabelWidth = static_cast<float>(dimensions.keyboardFromLabelWidth) * sf;
            m.audioFromLabelWidth = static_cast<float>(dimensions.audioFromLabelWidth) * sf;
            m.inputGainLabelWidth = static_cast<float>(dimensions.inputGainLabelWidth) * sf;
            m.portComboWidth = static_cast<float>(dimensions.portComboBoxWidth) * sf;
            m.inputGainSliderWidth = static_cast<float>(dimensions.inputGainSliderWidth) * sf;
            m.peakIndicatorWidth = static_cast<float>(dimensions.peakIndicatorWidth) * sf;
            m.ledSize = static_cast<float>(dimensions.ledSize) * sf;
            m.ledSizePx = juce::roundToInt(m.ledSize);
            m.ledY = bounds.getY() + (bounds.getHeight() - m.ledSizePx) / 2 + contentYOffset;

            const float leftPadding = static_cast<float>(dimensions.leftPadding) * sf;
            const float logoGapAfter = static_cast<float>(dimensions.logoGapAfter) * sf;
            m.logoWidth = TSS::ScaledLayout::scaledInt(static_cast<float>(dimensions.logoWidth), sf);
            m.logoHeight = TSS::ScaledLayout::scaledInt(static_cast<float>(dimensions.logoHeight), sf);
            m.logoX = juce::roundToInt(static_cast<float>(bounds.getX()) + leftPadding);
            m.contentStartX = static_cast<float>(m.logoX + m.logoWidth) + logoGapAfter;
            return m;
        }
    };

    class PacketPlacer
    {
    public:
        PacketPlacer(float startX, const HeaderLayoutMetrics& metrics, float uiScale)
            : x_(startX)
            , y_(metrics.controlY)
            , h_(metrics.controlHeightPx)
            , ledY_(metrics.ledY)
            , ledH_(metrics.ledSizePx)
            , gap_(metrics.gap)
            , packetExternalGap_(metrics.packetExternalGap)
            , ledSize_(metrics.ledSize)
            , uiScale_(uiScale)
        {
        }

        void placeLabel(TSS::Label& label, float labelWidth)
        {
            label.setBounds(juce::roundToInt(x_), y_, juce::roundToInt(labelWidth), h_);
            label.setUiScale(uiScale_);
            x_ += labelWidth + gap_;
        }

        void placeCombo(TSS::ComboBox& combo, float comboWidth)
        {
            combo.setBounds(juce::roundToInt(x_), y_, juce::roundToInt(comboWidth), h_);
            combo.setUiScale(uiScale_);
            x_ += comboWidth + gap_;
        }

        void placeLed(TSS::Led& led)
        {
            led.setBounds(juce::roundToInt(x_), ledY_, juce::roundToInt(ledSize_), ledH_);
            led.setUiScale(uiScale_);
            x_ += ledSize_ + gap_;
        }

        void placeSlider(TSS::Slider& slider, float sliderWidth)
        {
            slider.setBounds(juce::roundToInt(x_), y_, juce::roundToInt(sliderWidth), h_);
            slider.setUiScale(uiScale_);
            x_ += sliderWidth + gap_;
        }

        void placePeak(TSS::PeakIndicator& peak, float peakWidth)
        {
            peak.setBounds(juce::roundToInt(x_), y_, juce::roundToInt(peakWidth), h_);
            peak.setUiScale(uiScale_);
            x_ += peakWidth + gap_;
        }

        void endPacket()
        {
            x_ += packetExternalGap_ - gap_;
        }

    private:
        float x_;
        int y_;
        int h_;
        int ledY_;
        int ledH_;
        float gap_;
        float packetExternalGap_;
        float ledSize_;
        float uiScale_;
    };
}

void HeaderPanel::resized()
{
    const auto metrics = HeaderLayoutMetrics::make(dimensions_, uiScale_, getLocalBounds());

    logo_.setBounds(metrics.logoX,
                    metrics.controlY + dimensions_.logoVerticalOffset,
                    metrics.logoWidth,
                    metrics.logoHeight);
    logo_.setUiScale(uiScale_);

    PacketPlacer placer(metrics.contentStartX, metrics, uiScale_);

    placer.placeLed(instrumentActivityLed_);
    placer.placeLabel(keyboardFromLabel_, metrics.keyboardFromLabelWidth);
    placer.placeCombo(keyboardFromComboBox_, metrics.portComboWidth);
    placer.endPacket();

    placer.placeLed(editorActivityLed_);
    placer.placeLabel(midiFromLabel_, metrics.editorMidiFromLabelWidth);
    placer.placeCombo(midiFromComboBox_, metrics.portComboWidth);
    placer.endPacket();

    placer.placeLed(midiToActivityLed_);
    placer.placeLabel(midiToLabel_, metrics.midiToLabelWidth);
    placer.placeCombo(midiToComboBox_, metrics.portComboWidth);
    placer.endPacket();

    if (!isPluginMode_)
    {
        placer.placeLabel(audioFromLabel_, metrics.audioFromLabelWidth);
        placer.placeCombo(audioFromComboBox_, metrics.portComboWidth);
        placer.endPacket();

        placer.placeLabel(inputGainLabel_, metrics.inputGainLabelWidth);
        placer.placeSlider(inputGainSlider_, metrics.inputGainSliderWidth);
        placer.placePeak(peakIndicator_, metrics.peakIndicatorWidth);
        placer.endPacket();
    }
}
