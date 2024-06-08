/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <stdexcept>

#include "FeedbackLAF.h"
#include "DelayLAF.h"
#include "FilterLAF.h"
#include "MixLAF.h"
#include "BypassLAF.h"
#include "MonoLAF.h"

//==============================================================================
/**
*/
class MBRDelayAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    MBRDelayAudioProcessorEditor (MBRDelayAudioProcessor&);
    ~MBRDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    // overriding the Slider Listener
    inline void sliderValueChanged (juce::Slider *slider) override
    {
        if(slider == &mFeedbackKnob)
        {
            audioProcessor.adjustFeedback(mFeedbackKnob.getValue());
        }
        else if(slider == &mLeftDelayKnob)
        {
            audioProcessor.adjustTimeDelay("left", mLeftDelayKnob.getValue());
        }
        else if(slider == &mRightDelayKnob)
        {
            audioProcessor.adjustTimeDelay("right", mRightDelayKnob.getValue());
        }
        else if(slider == &mHighpassFilter)
        {
            audioProcessor.adjustHighpass(mHighpassFilter.getValue());
        }
        else if(slider == &mLowpassFilter)
        {
            audioProcessor.adjustLowpass(mLowpassFilter.getValue());
        }
        else if(slider == &mDryKnob)
        {
            audioProcessor.updateDry(mDryKnob.getValue());
        }
        else if(slider == &mWetKnob)
        {
            audioProcessor.updateWet(mWetKnob.getValue());
        }
        else if(slider == &mBypass)
        {
            audioProcessor.bypassToggle(mBypass.getValue());
        }
        else if(slider == &mMonoSwitch)
        {
            audioProcessor.toggleMono(mMonoSwitch.getValue());
        }
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MBRDelayAudioProcessor& audioProcessor;
    
    
    // Feedback Components
    FeedbackLAF fbLookAndFeel;
    juce::Slider mFeedbackKnob;
    
    // Time Delay Components
    DelayLAF tdLookAndFeel;
    juce::Slider mLeftDelayKnob;
    juce::Slider mRightDelayKnob;
    
    // Filter Components
    FilterLAF ftrLookAndFeel;
    juce::Slider mHighpassFilter;
    juce::Slider mLowpassFilter;
    
    // Mix Components
    MixLAF mixLookAndFeel;
    juce::Slider mDryKnob;
    juce::Slider mWetKnob;
    
    // Bypass Slider
    BypassLAF bpLookAndFeel;
    juce::Slider mBypass;
    
    // Mono Slider
    MonoLAF monoLookAndFeel;
    juce::Slider mMonoSwitch;
    
public:
    // Tree State Data
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> leftDelayValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rightDelayValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highpassValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowpassValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wetValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bypassValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> monoValue;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MBRDelayAudioProcessorEditor)
};
