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

#define DELAY_BUFFER_SIZE (44100 * 4)

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
    
    inline int getReadIndex(int delayTime)
    {
        // convert sampleRate to ms by /1000
        int rI = writeIndex - ((mSampleRate/1000) * delayTime);
        
        if(rI < 0)
            return static_cast<int>(DELAY_BUFFER_SIZE - rI);
        
        return static_cast<int>(rI);
    }
    
    inline void setSampleRate(double sampleRate)
    {
        mSampleRate = sampleRate;
    }
//    inline void performInterpolation(float [], float [], double frac);

    
private:
    // read and write index
    int readIndex = 0;
    int writeIndex = 0;
    
    // delayTime in miliseconds(ms), hardcoded for now
    //int delayTime = 100;
    int leftDelayTime = 200;
    int rightDelayTime = 600;
    
    // feedback
    float mFeedback = 0.7f;
    
    // sampleRate
    int mSampleRate = -1;
    
    // delay buffer
    float leftDelayBuffer[DELAY_BUFFER_SIZE] = {0};
    float rightDelayBuffer[DELAY_BUFFER_SIZE] = {0};
};
