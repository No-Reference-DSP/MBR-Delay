/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <stdexcept>

//==============================================================================
/**
*/
class MBRDelayAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MBRDelayAudioProcessorEditor (MBRDelayAudioProcessor&);
    ~MBRDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MBRDelayAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MBRDelayAudioProcessorEditor)
};
