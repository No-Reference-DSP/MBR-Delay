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

float Delay::doLeftDelay(float leftChannelData)
{
    // processing sample by sample
    float left = leftChannelData;
    
    // insert into the delayBuffer
    leftDelayBuffer[writeIndex] = left + leftChannelData;
    
    // incremement the writePointer
    incrementWrite();
    
    // calculate the read index
    // we'll keep it simple for now
    incrementRead();
    
    // interpolation is something we will do later
    
    // return the data at buffer[readIndex]
    cout << "left - modified: " << leftDelayBuffer[readIndex] + leftChannelData << endl;
    DBG("left - modified");
    DBG(leftDelayBuffer[readIndex]+leftChannelData);
    return leftDelayBuffer[readIndex] + leftChannelData;
}

float Delay::doRightDelay(float rightChannelData)
{
    // processing sample by sample
    float right = rightChannelData;
    
    // insert into the delayBuffer
    rightDelayBuffer[writeIndex] = right + rightChannelData;
    
    // we only need to call increments for one channel to do both channels
    // interpolation is something we will do later
    
    // return the data at buffer[readIndex]
    return rightDelayBuffer[readIndex] + rightChannelData;
}
