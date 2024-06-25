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
    void setSampleRate(double sampleRate);
    void setFrequencyCutoff(int hz);
    void reset();
    
    inline void setOmega()
    {
        omega = tan((PI * mFrequencyCutoff) / mSampleRate);
    }
    
    inline void updatePreviousValues(float incoming, float outgoing)
    {
        out2 = out1;
        out1 = outgoing;
        
        in2 = in1;
        in1 = incoming;
    }
    
    inline void updateFrequencyCutoff()
    {
        mFrequencyCutoff = static_cast<float>(smoothedFrequencyCutoff.getNextValue());
    }
    
private:
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
