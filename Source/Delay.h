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
#include <math.h>
#include "ButterworthLowpass.h"
#include "ButterworthHighpass.h"

#define DELAY_BUFFER_SIZE (44100 * 4)

using namespace std;

class Delay {
public:
    Delay();
    ~Delay();
    
    float doLeftDelay(float leftChannelData);
    float doRightDelay(float rightChannelData);
    
    // Setters
    inline void incrementWrite()
    {
        if(++writeIndex >= DELAY_BUFFER_SIZE)
            writeIndex = 0;
    }
    
    inline void setSampleRate(double sampleRate)
    {
        mSampleRate = sampleRate;
        
        // for the filters
        mLeftLowpass.setSampleRate(sampleRate);
        mRightLowpass.setSampleRate(sampleRate);
        
        mLeftHighpass.setSampleRate(sampleRate);
        mRightHighpass.setSampleRate(sampleRate);
    }
    
    void reset()
    {
        writeIndex = 0;
        leftDelayTime = 200;
        rightDelayTime = 200;
        mFeedback = 0.7f;
        
        memset(leftDelayBuffer, 0, DELAY_BUFFER_SIZE-1);
        memset(rightDelayBuffer, 0, DELAY_BUFFER_SIZE-1);
        
        // reset filters as well
        mLeftLowpass.reset();
        mRightLowpass.reset();
        
        mLeftHighpass.reset();
        mRightHighpass.reset();
    }
    
    // Getters
    inline float getReadIndex(int delayTime)
    {
        // convert sampleRate to ms by /1000
        float rI = writeIndex - ((mSampleRate/1000) * delayTime);
        
        if(rI < 0)
            return DELAY_BUFFER_SIZE + rI;
        
        return rI;
    }
    
    inline float linearInterpolation(float bufferValOne, float bufferValTwo, float frac)
    {
        // linear interpolation [Dat 97]: y(n) = x(n-[M+1])frac + x(n-M)(1-frac)
        // referenced from the book: DAFX: Digital Audio Effects, by Udo Zolzer
        // bufferValOne = n-M, bufferValTwo = n - [M+1]
        return (bufferValTwo * frac) + (bufferValOne * (1 - frac));
    }
    
    
private:
    // write index
    int writeIndex = 0;
    
    // delayTime in miliseconds(ms), hardcoded for now
    //int delayTime = 100;
    int leftDelayTime = 600;
    int rightDelayTime = 600;
    
    // feedback
    float mFeedback = 0.3f;
    
    // sampleRate
    float mSampleRate = -1.0f;
    
    // delay buffer
    float leftDelayBuffer[DELAY_BUFFER_SIZE] = {0};
    float rightDelayBuffer[DELAY_BUFFER_SIZE] = {0};
    
    // filters
    ButterworthLowpass mLeftLowpass;
    ButterworthLowpass mRightLowpass;
    
    ButterworthHighpass mLeftHighpass;
    ButterworthHighpass mRightHighpass;
};
