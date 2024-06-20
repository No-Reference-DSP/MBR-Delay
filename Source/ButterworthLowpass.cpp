/*
  ==============================================================================

    ButterworthLowpass.cpp
    Created: 30 May 2024 4:40:19pm
    Author:  Juan Cocina

  ==============================================================================
*/

#include "ButterworthLowpass.h"

ButterworthLowpass::ButterworthLowpass()
{
    reset();
}

ButterworthLowpass::~ButterworthLowpass()
{
    
}

//=====================================================================
// Getters ===
float ButterworthLowpass::filter(float in)
{
    jassert(mSampleRate > 0);
    float out = (b0*in) + (b1*in1) + (b2*in2) - (a1*out1) - (a2*out2);
    updatePreviousValues(in, out);
    
    return out;
}
//=====================================================================
//=====================================================================
// Setters ===
void ButterworthLowpass::update()
{
    // check for updates on omega
    setOmega();
    updateFrequencyCutoff();
    
    // fed forward b
    b0 = (pow(omega,2)/(1 + (omega*SQRT2) + pow(omega,2)));
    b1 = 2*(b0);
    b2 = b0;
    
    // fed backward a
    a0 = 1;
    a1 = (2*(pow(omega,2) -1))/(1 + omega*SQRT2 + pow(omega,2));
    a2 = (pow(omega,2) - omega*SQRT2 + 1)/(1 + omega*SQRT2 + pow(omega,2));
}

void ButterworthLowpass::setSampleRate(double sampleRate)
{
    mSampleRate = sampleRate;
    // for smoothing
    smoothedFrequencyCutoff.reset(sampleRate, 0.03);
}

void ButterworthLowpass::setFrequencyCutoff(int hz)
{
    smoothedFrequencyCutoff.setTargetValue(hz);
}

void ButterworthLowpass::reset()
{
    a0 = 1; a1 = 0; a2 = 0;
    b0 = 0; b1 = 0; b2 = 0;
    
    in1 = 0; in2 = 0;
    out1 = 0; out2 = 0;
    setOmega();
}
//=====================================================================
