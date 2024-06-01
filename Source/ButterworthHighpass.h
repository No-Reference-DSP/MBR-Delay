/*
  ==============================================================================

    ButterworthHighpass.h
    Created: 31 May 2024 1:06:04pm
    Author:  Juan Cocina

  ==============================================================================
*/

#pragma once

#include <vector>
#include <iostream>
#include <cmath>
#include <stdio.h>

#define FREQ_MIN 20
#define FREQ_MAX 20000
#define PI 3.14159265
#define SQRT2 sqrt(2)

class ButterworthHighpass {
public:
    // Constructor
    ButterworthHighpass();
    // Destructor
    ~ButterworthHighpass();
    
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
    }
    
    inline void updatePreviousValues(float incoming, float outgoing)
    {
        out2 = out1;
        out1 = outgoing;
        
        in2 = in1;
        in1 = incoming;
    }
    
    inline void reset()
    {
        a0 = 1; a1 = 0; a2 = 0;
        b0 = 0; b1 = 0; b2 = 0;
        
        mFrequencyCutoff = 500.0f;
        
        in1 = 0; in2 = 0;
        out1 = 0; out2 = 0;
        setOmega();
    }
    
private:
    // Sample Rate
    float mSampleRate = -1;
    // Cutoff
    float mFrequencyCutoff = 0;
    
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
