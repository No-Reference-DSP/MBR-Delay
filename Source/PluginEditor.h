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
    
public:
    // Tree State Data
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> leftDelayValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rightDelayValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MBRDelayAudioProcessorEditor)
};
