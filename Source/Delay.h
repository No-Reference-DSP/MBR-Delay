/*
  ==============================================================================

    Delay.h
    Created: 14 May 2024 1:32:37pm
    Author:  Juan Cocina

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <stdexcept>

#define DELAY_BUFFER_SIZE (44100 * 2)

using namespace std;

class Delay {
public:
    Delay();
    ~Delay();
    
    float doLeftDelay(float leftChannelData);
    float doRightDelay(float rightChannelData);
    
    inline void incrementWrite()
    {
        if(++writeIndex >= DELAY_BUFFER_SIZE)
            writeIndex = 0;
    }
    inline void incrementRead() {
        if(++readIndex >= DELAY_BUFFER_SIZE)
            readIndex = 0;
    }
    
    inline void setReadIndex(double sampleRate)
    {
        readIndex = sampleRate - 1;
    }
//    inline void setSampleRate(double sampleRate);
//    inline void performInterpolation(float [], float [], double frac);

    
private:
    // read and write index
    int readIndex = 0;
    int writeIndex = 0;
    
    // delay buffer
    float leftDelayBuffer[DELAY_BUFFER_SIZE] = {0};
    float rightDelayBuffer[DELAY_BUFFER_SIZE] = {0};
};
