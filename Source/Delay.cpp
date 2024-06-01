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
    mLeftLowpass.update(); // update lowpass filter
    mLeftHighpass.update(); // update highpass filter
    
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
    
    // filter signal then output
    float filteredOutput = mLeftLowpass.filter(leftOutput);
    filteredOutput = mLeftHighpass.filter(filteredOutput);
    
    return filteredOutput;
}

float Delay::doRightDelay(float rightInput)
{
    jassert(mSampleRate > 0);
    
    float rightOutput = rightInput;
    mRightLowpass.update(); // update lowpass filter
    mRightHighpass.update(); // update highpass filter
    
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

    // return filtered signal
    float filteredOutput = mRightLowpass.filter(rightOutput);
    filteredOutput = mRightHighpass.filter(filteredOutput);
    
    return filteredOutput;
}
