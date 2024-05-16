/*
  ==============================================================================

    Delay.cpp
    Created: 14 May 2024 1:32:37pm
    Author:  Juan Cocina

  ==============================================================================
*/

#include "Delay.h"

// Constructor
Delay::Delay() {
    // this is where I'll initialize data for filters and other parameters that are not
    // given by PluginProcessor
}

// Destructor
Delay::~Delay() {
    // I know I'll have to destroy the arrays that I create to keep the program memory safe
    /*
     Might look like
         delete[] leftBuffer;
         delete[] rightBuffer;
     */
}

float Delay::doLeftDelay(float leftInput)
{
    jassert(mSampleRate > 0);
    
    float leftOutput = leftInput;
    
    // calculate the readIndex
    readIndex = getReadIndex(leftDelayTime);
    
    // get the delayed signal
    leftOutput = leftDelayBuffer[readIndex];
    
    // insert new signal
    leftDelayBuffer[writeIndex] = leftInput + leftOutput * mFeedback;
    return leftOutput;
}

float Delay::doRightDelay(float rightInput)
{
    jassert(mSampleRate > 0);
    
    float rightOutput = rightInput;
    
    // calculate the readIndex
    readIndex = getReadIndex(rightDelayTime);
    
    // get the delayed signal
    rightOutput = leftDelayBuffer[readIndex];
    
    // insert new signal
    leftDelayBuffer[writeIndex] = rightInput + rightOutput * mFeedback;
    incrementWrite();
    
    return rightOutput;
}
