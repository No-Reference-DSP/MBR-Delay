/*
  ==============================================================================

    ButterworthHighpass.cpp
    Created: 31 May 2024 1:06:04pm
    Author:  Juan Cocina

  ==============================================================================
*/

#include "ButterworthHighpass.h"

ButterworthHighpass::ButterworthHighpass()
{
    reset();
}

ButterworthHighpass::~ButterworthHighpass()
{
    
}

void ButterworthHighpass::update()
{
    // check Omega for udpates
    updateFrequencyCutoff();
    setOmega();

    // b is fed forward
    b0 = 1 /(pow(omega,2) + (SQRT2*omega) + 1);
    b1 = -2*b0;
    b2 = b0;
    // a is fed backward
    a0 = 1;
    a1 = (2*(pow(omega,2)-1)) * b0;
    a2 = (pow(omega,2) - (SQRT2*omega) + 1) * b0;
}

float ButterworthHighpass::filter(float in)
{
    jassert(mSampleRate > 0);
    float out = (b0*in) + (b1*in1) + (b2*in2) - (a1*out1) - (a2*out2);
    updatePreviousValues(in, out);
    
    return out;
}
