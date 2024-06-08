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
    feedbackValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "mFeedback", mFeedbackKnob);
    
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
    leftDelayValue =        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "mLeftDelay", mLeftDelayKnob);
    
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
    rightDelayValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "mRightDelay", mRightDelayKnob);
    
    // Highpass Filter
    mHighpassFilter.setLookAndFeel(&ftrLookAndFeel);
    mHighpassFilter.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mHighpassFilter.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 69, 25);
    mHighpassFilter.setTextValueSuffix("Hz");
    mHighpassFilter.setRotaryParameters(3.92699, 8.63938, true);
    mHighpassFilter.setRange(20, 2000, 1);
    mHighpassFilter.setValue(20);
    mHighpassFilter.addListener(this);
    addAndMakeVisible(mHighpassFilter);
    highpassValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "mHighpass", mHighpassFilter);
    
    // Lowpass Filter
    mLowpassFilter.setLookAndFeel(&ftrLookAndFeel);
    mLowpassFilter.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mLowpassFilter.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 69, 25);
    mLowpassFilter.setTextValueSuffix("Hz");
    mLowpassFilter.setRotaryParameters(3.92699, 8.63938, true);
    mLowpassFilter.setRange(2000, 20000, 1);
    mLowpassFilter.setValue(20000);
    mLowpassFilter.addListener(this);
    addAndMakeVisible(mLowpassFilter);
    lowpassValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "mLowpass", mLowpassFilter);
    
    // Dry
    mDryKnob.setLookAndFeel(&mixLookAndFeel);
    mDryKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mDryKnob.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 48, 21);
    mDryKnob.setTextValueSuffix("dB");
    mDryKnob.setRotaryParameters(3.92699, 8.63938, true);
    mDryKnob.setRange(-48.0f, 0.0f, 0.1);
    mDryKnob.setValue(-1);
    mDryKnob.addListener(this);
    addAndMakeVisible(mDryKnob);
    dryValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "mDry", mDryKnob);
    
    // Wet
    mWetKnob.setLookAndFeel(&mixLookAndFeel);
    mWetKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mWetKnob.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 48, 21);
    mWetKnob.setTextValueSuffix("db");
    mWetKnob.setRotaryParameters(3.92699, 8.63938, true);
    mWetKnob.setRange(-48.f, 0.0f, 0.1);
    mWetKnob.setValue(-1);
    mWetKnob.addListener(this);
    addAndMakeVisible(mWetKnob);
    wetValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "mWet", mWetKnob);
    
    // Bypass
    mBypass.setLookAndFeel(&bpLookAndFeel);
    mBypass.setSliderStyle(juce::Slider::LinearHorizontal);
    mBypass.setTextBoxStyle(juce::Slider::NoTextBox, true, 0,0);
    mBypass.setRange(0, 1, 1);
    mBypass.setValue(0);
    mBypass.addListener(this);
    addAndMakeVisible(mBypass);
    bypassValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "mBypass", mBypass);
    
    // Mono
    mMonoSwitch.setLookAndFeel(&monoLookAndFeel);
    mMonoSwitch.setSliderStyle(juce::Slider::LinearHorizontal);
    mMonoSwitch.setTextBoxStyle(juce::Slider::NoTextBox, true, 0,0);
    mMonoSwitch.setRange(0, 1, 1);
    mMonoSwitch.setValue(1);
    mMonoSwitch.addListener(this);
    addAndMakeVisible(mMonoSwitch);
    monoValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "mMono", mMonoSwitch);
    
    
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
    
    auto bypassBackground = juce::ImageCache::getFromMemory(BinaryData::BypassBackground_png, BinaryData::BypassBackground_pngSize);
    g.drawImage(bypassBackground, 170, 298, 73, 33, 0, 0, 73, 33);
    
}

void MBRDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    mFeedbackKnob.setBounds(146, 64, 102, 110);
    mLeftDelayKnob.setBounds(327, 64, 122, 84);
    mRightDelayKnob.setBounds(327, 207, 122, 84);
    mHighpassFilter.setBounds(499, 218, 123, 108);
    mLowpassFilter.setBounds(499, 88, 123, 108);
    mDryKnob.setBounds(769, 73, 73, 112);
    mWetKnob.setBounds(769, 210, 73, 112);
    mBypass.setBounds(170, 298, 73, 33);
    mMonoSwitch.setBounds(343, 323, 50, 12);
}
