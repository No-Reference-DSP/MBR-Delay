/*
  ==============================================================================

    FilterLAF.h
    Created: 6 Jun 2024 2:00:20pm
    Author:  Juan Cocina

  ==============================================================================
*/

#pragma once

class FilterLAF : public juce::LookAndFeel_V4
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
        g.setColour (juce::Colour::fromString("FF86B879"));
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
        g.setColour (juce::Colour::fromString("FFD9D9D9"));
        g.fillPath (p);
    }
    
    // Custom text box placement
    juce::Slider::SliderLayout getSliderLayout (juce::Slider& slider) override
    {
        juce::Slider::SliderLayout layout = juce::LookAndFeel_V2::getSliderLayout(slider);
        
        layout.textBoxBounds = juce::Rectangle<int> (0, 83, 69, 25);
        layout.sliderBounds = juce::Rectangle<int> (49, 0, 74, 74);

        return layout;
    }
    
    juce::Label* createSliderTextBox(juce::Slider& slider) override
    {
        juce::Label* l = juce::LookAndFeel_V2::createSliderTextBox(slider);
        l->setColour(juce::Label::textColourId, juce::Colours::black);

        return l;
    }
};
