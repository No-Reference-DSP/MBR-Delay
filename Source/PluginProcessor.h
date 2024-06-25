/*
 
***
** MBR-Delay Plugin
** Copyright (C) 2024 No Reference DSP
***
 This file is part of MBR-Delay
 MBR-Delay is free software: you can redistribute it and/or modify it under the terms of the GNU General
 Public License as published by the Free Software Foundation, either version 3 of the License, or (at
 your option) any later version.

 MBR-Delay is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
 implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License along with MBR-Delay.
 If not, see <https://www.gnu.org/licenses/>.
 
*/
#pragma once

#include <JuceHeader.h>
#include "Delay.h"
#include <string>

//==============================================================================
/**
*/
class MBRDelayAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MBRDelayAudioProcessor();
    ~MBRDelayAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //===========================================
    // creating functions to convert audio into mono
    void toggleMono(int m);
    void convertStereoToMono(float& leftChannelAudio, float& rightChannelAudio);
    
    // Public functions for Delay mods
    void adjustFeedback(double fb);
    void adjustTimeDelay(std::string name, int ms);
    void adjustHighpass(int hz);
    void adjustLowpass(int hz);
    void updateDry(float d);
    void updateWet(float d);
    void bypassToggle(int i);
    
    // Processor State Tree
    juce::AudioProcessorValueTreeState treeState;

private:
    // Feedback vars
    int mFeedback;
//
//    // Time Delay vars
//    int mLeftDelay;
//    int mRightDelay;
//
//    // Filter vars
//    int mHighpass;
//    int mLowpass;
    
    // Delay Variables
    Delay mDelayBuffer;
    float mDryGain = 1.0f;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedDry;
    
    float mWetGain = 1.0f;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedWet;
    
    // Mono Switch Variables
    bool mMono;
    
    // Bypass
    bool Bypass;
    
    // link
    bool mLink;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MBRDelayAudioProcessor)
};
