/*
  ==============================================================================

    GrayKnob.h
    Created: 4 Jun 2024 4:40:18pm
    Author:  Juan Cocina

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FeedbackLAF : public juce::LookAndFeel_V4
{
public:
    // Custom Rotary Display adapted from JUCE custom lookandfeel tut
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
    {
        auto radius = (float) juce::jmin(width/2, height/2) - 4.0f;
        auto centreX = (float) x + (float) width * 0.5f;
        auto centreY = (float) y + (float) height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // fill
        g.setColour (juce::Colour::fromString("FFCCDCDD"));
        g.fillEllipse (rx, ry, rw, rw);

        // outline
//        g.setColour (juce::Colours::red);
//        g.drawEllipse (rx, ry, rw, rw, 1.0f);

        juce::Path p;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 5.0f;
        p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));

        // pointer
        g.setColour (juce::Colour::fromString("FFF5751D"));
        g.fillPath (p);
    }
    
    // Custom text box placement
    juce::Slider::SliderLayout getSliderLayout (juce::Slider& slider) override
    {
        juce::Slider::SliderLayout layout = juce::LookAndFeel_V2::getSliderLayout(slider);
        
        layout.textBoxBounds = juce::Rectangle<int> (55, 0, 47, 21);
        layout.sliderBounds = juce::Rectangle<int> (0,28,79,79);

        return layout;
    }
};
