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
    void sliderValueChanged (juce::Slider *slider) override
    {
        if(slider == &mFeedbackKnob)
        {
            audioProcessor.adjustFeedback(mFeedbackKnob.getValue());
        }
        if(slider == &mLeftDelayKnob)
        {
            audioProcessor.adjustTimeDelay("left", mLeftDelayKnob.getValue());
            if(mLinkButton.getToggleState())
            {
                mRightDelayKnob.setValue(mLeftDelayKnob.getValue());
            }
        }
        if(slider == &mRightDelayKnob)
        {
            audioProcessor.adjustTimeDelay("right", mRightDelayKnob.getValue());
            if(mLinkButton.getToggleState())
            {
                mLeftDelayKnob.setValue(mRightDelayKnob.getValue());
            }
        }
        if(slider == &mHighpassFilter)
        {
            audioProcessor.adjustHighpass(mHighpassFilter.getValue());
        }
        if(slider == &mLowpassFilter)
        {
            audioProcessor.adjustLowpass(mLowpassFilter.getValue());
        }
        if(slider == &mDryKnob)
        {
            audioProcessor.updateDry(mDryKnob.getValue());
        }
        if(slider == &mWetKnob)
        {
            audioProcessor.updateWet(mWetKnob.getValue());
        }
        if(slider == &mBypass)
        {
            audioProcessor.bypassToggle(mBypass.getValue());
        }
        if(slider == &mMonoSwitch)
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
    
    // Link button
    juce::ImageButton mLinkButton;
    
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
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> linkValue;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MBRDelayAudioProcessorEditor)
};
