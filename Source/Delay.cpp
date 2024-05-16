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
    
    // insert into the delayBuffer
    leftDelayBuffer[writeIndex] = leftOutput;
    
    // incremement the writePointer
    incrementWrite();
    
    // calculate the read index
    readIndex = getReadIndex();
    
    // interpolation is something we will do later
    
    // return the data at buffer[readIndex]
    return leftDelayBuffer[readIndex] + leftInput;
}

float Delay::doRightDelay(float rightInput)
{
    jassert(mSampleRate > 0);
    
    float rightOutput = rightInput;
    
    // insert into the delayBuffer
    rightDelayBuffer[writeIndex] = rightOutput;
    
    // calculate the read index
    readIndex = getReadIndex();
    
    // interpolation is something we will do later
    
    // return the data at buffer[readIndex] + out original input data
    return rightDelayBuffer[readIndex] + rightInput;
}
