//
//  Delay.hpp
//  MBR-Delay - VST3
//
//  Created by Juan Cocina on 5/9/24.
//

#ifndef Delay_hpp
#define Delay_hpp

#include <JuceHeader.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <stdexcept>

using namespace std;

class Delay {
public:
    Delay();
    ~Delay();
    
    float doLeftDelay(float leftChannelData);
    float doRightDelay();
    
    inline void setBufferSize();
    inline void setSampleRate();
    inline void performInterpolation();
    
    
private:
    const int maxBufferSize;
    int readPointer;
    int writePointer;
    
    // what are some parts of the delay line that need to be established on
    // construction?
    
    // Declaring these two arrays is undefined behavior, need fix
//    float leftCircularBuffer[];
//    float rightCircularBuffer[];
};
#endif /* Delay_hpp */
