//
//  Delay.hpp
//  MBR-Delay - VST3
//
//  Created by Juan Cocina on 5/9/24.
//

#ifndef Delay_hpp
#define Delay_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <stdexcept>
#include <JuceHeader.h>

using namespace std;

class Delay {
public:
    Delay();
    ~Delay();
    
    float[] doLeftDelay();
    float[] doRightDelay();
    
    inline void setBufferSize();
    inline void setSampleRate();
    inline void performInterpolation();
    
    
private:
    int max_buffer_size;
    int readPointer;
    int writePointer;
    
    // what are some parts of the delay line that need to be established on
    // construction?
//    float circularBufferLeft[];
//    float circularBufferRight[];
};
#endif /* Delay_hpp */
