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
    
    // calculate the readIndex as a float
    float readIndex = getReadIndex(leftDelayTime);
    int readAsInt = static_cast<int>(readIndex);
    int readAsIntNext = readAsInt + 1 >= DELAY_BUFFER_SIZE ? 0 : readAsInt+1;
    
    // get the values from the buffer + frac
    float bufferValOne = leftDelayBuffer[readAsInt];
    float bufferValTwo = leftDelayBuffer[readAsIntNext];
    float frac = readIndex - readAsInt;
    
    // perform interpolation and get delayedSignal
    leftOutput = linearInterpolation(bufferValOne, bufferValTwo, frac);
    
    // insert new signal
    leftDelayBuffer[writeIndex] = leftInput + leftOutput * mFeedback;
    return leftOutput;
}

float Delay::doRightDelay(float rightInput)
{
    jassert(mSampleRate > 0);
    
    float rightOutput = rightInput;
    
    // calculate the readIndex as a float
    float readIndex = getReadIndex(rightDelayTime);
    int readAsInt = static_cast<int>(readIndex);
    int readAsIntNext = readAsInt + 1 >= DELAY_BUFFER_SIZE ? 0 : readAsInt+1;
    
    // get the values from the buffer + frac
    float bufferValOne = rightDelayBuffer[readAsInt];
    float bufferValTwo = rightDelayBuffer[readAsIntNext];
    float frac = readIndex - readAsInt;
    
    // perform interlpoation and get delayed signal
    rightOutput = linearInterpolation(bufferValOne, bufferValTwo, frac);
    
    // insert new signal
    rightDelayBuffer[writeIndex] = rightInput + rightOutput * mFeedback;
    
    // move write pointer only in right channel pipeline
    incrementWrite();
    
    return rightOutput;
}
