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

void ButterworthLowpass::update()
{
    // check for updates on omega
    setOmega();
    
    // fed forward b
    b0 = (pow(omega,2)/(1 + (omega*SQRT2) + pow(omega,2)));
    b1 = 2*(b0);
    b2 = b0;
    
    // fed backward a
    a0 = 1;
    a1 = (2*(pow(omega,2) -1))/(1 + omega*SQRT2 + pow(omega,2));
    a2 = (pow(omega,2) - omega*SQRT2 + 1)/(1 + omega*SQRT2 + pow(omega,2));
}

float ButterworthLowpass::filter(float in)
{
    float out = (b0*in) + (b1*in1) + (b2*in2) - (a1*out1) - (a2*out2);
    updatePreviousValues(in, out);
    
    return out;
}
