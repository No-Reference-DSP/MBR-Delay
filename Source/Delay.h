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
#include <string>
#include "ButterworthLowpass.h"
#include "ButterworthHighpass.h"

#define DELAY_BUFFER_SIZE (192000 * 2)

using namespace std;

class Delay {
public:
    Delay();
    ~Delay();
    
    float doLeftDelay(float leftChannelData);
    float doRightDelay(float rightChannelData);
    
    // Setters
    void setSampleRate(double sampleRate);
    void setFeedback(double fb);
    void setTimeDelay(std::string name, int ms);
    void updateHighpassCutoff(int hz);
    void updateLowpassCutoff(int hz);
    void reset();
    void clearBuffers();
    
    inline void incrementWrite()
    {
        if(++writeIndex >= DELAY_BUFFER_SIZE)
            writeIndex = 0;
    }
    
    inline void updateTimeDelay(){
        leftDelayTime = mSmoothedLeftDelayTime.getNextValue();
        rightDelayTime = mSmoothedRightDelayTime.getNextValue();
    }

    // Getters
    inline float getReadIndex(int delayTime)
    {
        // convert sampleRate to ms by /1000
        float readIndex = writeIndex - ((mSampleRate/1000) * delayTime);
        
        if(readIndex < 0)
            return DELAY_BUFFER_SIZE + readIndex;
        
        return readIndex;
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
    int writeIndex;
    
    // delay times + smoothing
    float leftDelayTime;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> mSmoothedLeftDelayTime;
    float rightDelayTime;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> mSmoothedRightDelayTime;
    
    // feedback + smoothing
    float mFeedback;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> mSmoothedFeedback;
    
    // sampleRate
    float mSampleRate;
    
    // delay buffer
    float leftDelayBuffer[DELAY_BUFFER_SIZE] = {0};
    float rightDelayBuffer[DELAY_BUFFER_SIZE] = {0};
    
    // filters
    ButterworthLowpass mLeftLowpass;
    ButterworthLowpass mRightLowpass;
    
    ButterworthHighpass mLeftHighpass;
    ButterworthHighpass mRightHighpass;
};
