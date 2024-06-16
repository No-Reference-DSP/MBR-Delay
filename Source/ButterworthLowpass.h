/*
  ==============================================================================

    ButterworthLowpass.h
    Created: 30 May 2024 4:40:19pm
    Author:  Juan Cocina

  ==============================================================================
*/

#pragma once

#include <vector>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <JuceHeader.h>

#define FREQ_MIN 20
#define FREQ_MAX 20000
#define PI 3.14159265
#define SQRT2 sqrt(2)

class ButterworthLowpass {
public:
    // Constructor
    ButterworthLowpass();
    
    // Destructor
    ~ButterworthLowpass();
    
    // Getters
    float filter(float val);
    
    // Setters
    void update();
    
    inline void setOmega()
    {
        omega = tan((PI * mFrequencyCutoff) / mSampleRate);
    }
    
    inline void setSampleRate(double sampleRate)
    {
        mSampleRate = sampleRate;
        // for smoothing
        smoothedFrequencyCutoff.reset(sampleRate, 0.03);
    }
    
    inline void updatePreviousValues(float incoming, float outgoing)
    {
        out2 = out1;
        out1 = outgoing;
        
        in2 = in1;
        in1 = incoming;
    }
    
    inline void setFrequencyCutoff(int hz)
    {
        smoothedFrequencyCutoff.setTargetValue(hz);
    }
    
    inline void updateFrequencyCutoff()
    {
        mFrequencyCutoff = static_cast<float>(smoothedFrequencyCutoff.getNextValue());
    }

    inline void reset()
    {
        a0 = 1; a1 = 0; a2 = 0;
        b0 = 0; b1 = 0; b2 = 0;
        
        in1 = 0; in2 = 0;
        out1 = 0; out2 = 0;
        setOmega();
    }
    
private:
    bool mBypass = true;
    // Sample Rate
    float mSampleRate = -1;
    // Cutoff
    float mFrequencyCutoff;
    juce::SmoothedValue<int, juce::ValueSmoothingTypes::Linear> smoothedFrequencyCutoff;
    
    // Coefficients
    float a0;
    float a1;
    float a2;
    float b0;
    float b1;
    float b2;
    float omega;
    
    // prev values
    float in1;
    float in2;
    float out1;
    float out2;
};
