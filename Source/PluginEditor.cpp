/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MBRDelayAudioProcessorEditor::MBRDelayAudioProcessorEditor (MBRDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Tree State attatchments
    feedbackValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "mFeedback", mFeedbackKnob);
    leftDelayValue =        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "mLeftDelay", mLeftDelayKnob);
    rightDelayValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "mRightDelay", mRightDelayKnob);
    
    
    // Feedback components
    mFeedbackKnob.setLookAndFeel(&fbLookAndFeel);
    mFeedbackKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mFeedbackKnob.setTextBoxStyle(juce::Slider::TextBoxRight, false, 47, 21);
    mFeedbackKnob.setTextValueSuffix("%");
    mFeedbackKnob.setRotaryParameters(3.92699, 8.63938, true);
    mFeedbackKnob.setRange(0, 150, 1); // will need to be multiplied by 0.01 before being used as FB
    mFeedbackKnob.setValue(15);
    mFeedbackKnob.addListener(this);
    addAndMakeVisible(mFeedbackKnob);
    
    // Left Time Delay Components
    mLeftDelayKnob.setLookAndFeel(&tdLookAndFeel);
    mLeftDelayKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mLeftDelayKnob.setTextBoxStyle(juce::Slider::TextBoxRight, false, 48, 22);
    mLeftDelayKnob.setTextValueSuffix("ms");
    mLeftDelayKnob.setRotaryParameters(3.92699, 8.63938, true);
    mLeftDelayKnob.setRange(0, 4000, 1); // ms, 4 second delay time?
    mLeftDelayKnob.setValue(200);
    mLeftDelayKnob.addListener(this);
    addAndMakeVisible(mLeftDelayKnob);
    
    // Right Time Delay Components
    mRightDelayKnob.setLookAndFeel(&tdLookAndFeel);
    mRightDelayKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mRightDelayKnob.setTextBoxStyle(juce::Slider::TextBoxRight, false, 48, 22);
    mRightDelayKnob.setTextValueSuffix("ms");
    mRightDelayKnob.setRotaryParameters(3.92699, 8.63938, true);
    mRightDelayKnob.setRange(0, 4000, 1); // same as above
    mRightDelayKnob.setValue(200);
    mRightDelayKnob.addListener(this);
    addAndMakeVisible(mRightDelayKnob);
    
    
    setSize (900, 400);
}

MBRDelayAudioProcessorEditor::~MBRDelayAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

//==============================================================================
void MBRDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//
//    g.setColour (juce::Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    
    // Draw background
    auto backgroundImage = juce::ImageCache::getFromMemory(BinaryData::backgroundWithModules_png, BinaryData::backgroundWithModules_pngSize);
    g.drawImage(backgroundImage, getLocalBounds().toFloat());

}

void MBRDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    mFeedbackKnob.setBounds(146, 64, 102, 110);
    mLeftDelayKnob.setBounds(327, 64, 122, 84);
    mRightDelayKnob.setBounds(327, 207, 122, 84);
}
