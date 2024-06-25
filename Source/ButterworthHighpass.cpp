/*
 
***
** MBR-Delay Plugin
** Copyright (C) 2024 No Reference DSP
***
 This file is part of MBR-Delay
 MBR-Delay is free software: you can redistribute it and/or modify it under the terms of the GNU General
 Public License as published by the Free Software Foundation, either version 3 of the License, or (at
 your option) any later version.

 MBR-Delay is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
 implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License along with MBR-Delay.
 If not, see <https://www.gnu.org/licenses/>.
 
*/

#include "ButterworthHighpass.h"

ButterworthHighpass::ButterworthHighpass()
{
    reset();
}

ButterworthHighpass::~ButterworthHighpass()
{
    
}

//=====================================================================
// Getters ===
float ButterworthHighpass::filter(float in)
{
    jassert(mSampleRate > 0);
    float out = (b0*in) + (b1*in1) + (b2*in2) - (a1*out1) - (a2*out2);
    updatePreviousValues(in, out);
    
    return out;
}
//=====================================================================
//=====================================================================
// Setters
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

void ButterworthHighpass::setSampleRate(double sampleRate)
{
    mSampleRate = sampleRate;
    // for smoothing
    smoothedFrequencyCutoff.reset(sampleRate, 0.03);
}

void ButterworthHighpass::setFrequencyCutoff(int hz)
{
    smoothedFrequencyCutoff.setTargetValue(hz);
}

void ButterworthHighpass::reset()
{
    a0 = 1; a1 = 0; a2 = 0;
    b0 = 0; b1 = 0; b2 = 0;
    
    in1 = 0; in2 = 0;
    out1 = 0; out2 = 0;
    setOmega();
}
//=====================================================================
