/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MBRDelayAudioProcessor::MBRDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
treeState(*this, nullptr, "PARAMETERS",
          {
    std::make_unique<juce::AudioParameterInt>("mFeedback", "Feedback", 0, 150, 15),
    std::make_unique<juce::AudioParameterInt>("mLeftDelay", "LeftDelay", 0, 4000, 200),
    std::make_unique<juce::AudioParameterInt>("mRightDelay", "RightDelay", 0, 4000, 200),
    std::make_unique<juce::AudioParameterInt>("mHighpass", "Highpass", 20, 2000, 20),
    std::make_unique<juce::AudioParameterInt>("mLowpass", "Highpass", 2000, 20000, 20000),
    std::make_unique<juce::AudioParameterFloat>("mDry", "Dry", -48.0f, 0.0f, -1.0f),
    std::make_unique<juce::AudioParameterFloat>("mWet", "Wet", -48.0f, 0.0f, -1.0f),
    std::make_unique<juce::AudioParameterInt>("mBypass", "Bypass", 0, 1, 0),
    std::make_unique<juce::AudioParameterInt>("mMono", "Mono", 0, 1, 1)
}),
        mFeedback(15), Bypass(false), mMono(false)
#endif
{
}

MBRDelayAudioProcessor::~MBRDelayAudioProcessor()
{
}

//==============================================================================
const juce::String MBRDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MBRDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MBRDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MBRDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MBRDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MBRDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MBRDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MBRDelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MBRDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void MBRDelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MBRDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // setup
    mDelayBuffer.setSampleRate(sampleRate);
    mDelayBuffer.setFeedback(mFeedback);
    mDelayBuffer.setTimeDelay("left", 200);
    mDelayBuffer.setTimeDelay("right", 200);
    mDelayBuffer.updateHighpassCutoff(20);
    mDelayBuffer.updateLowpassCutoff(20000);
    smoothedDry.reset(sampleRate, 0.01);
    smoothedWet.reset(sampleRate, 0.01);
}

void MBRDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MBRDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MBRDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    const int numSamples = buffer.getNumSamples();
    for(int sample = 0; sample < numSamples; sample++)
    {
        // get left channel data and delay it
        const int leftChannel = 0;
        float leftSample = buffer.getSample(leftChannel, sample);
        float delayedLeftSample = mDelayBuffer.doLeftDelay(leftSample);
        
        // get right channel data and delay it
        const int rightChannel = 1;
        float rightSample = buffer.getSample(rightChannel, sample);
        float delayedRightSample = mDelayBuffer.doRightDelay(rightSample);
        
        if(mMono) convertStereoToMono(delayedLeftSample, delayedRightSample);
        
        //set smoothed values
        mDryGain = smoothedDry.getNextValue();
        mWetGain = smoothedWet.getNextValue();
        
        // Add it back to the channels + the dry sample
        
        auto* channelData = buffer.getWritePointer(leftChannel);
        if(Bypass)
        {
            channelData[sample] = leftSample * juce::Decibels::decibelsToGain(mDryGain);
        } else
            channelData[sample] = (leftSample * juce::Decibels::decibelsToGain(mDryGain)) + (delayedLeftSample * juce::Decibels::decibelsToGain(mWetGain));
        
        channelData = buffer.getWritePointer(rightChannel);
        if(Bypass)
        {
            channelData[sample] = rightSample * juce::Decibels::decibelsToGain(mDryGain);
        } else
            channelData[sample] = (rightSample * juce::Decibels::decibelsToGain(mDryGain)) + (delayedRightSample * juce::Decibels::decibelsToGain(mWetGain));
    }
}

//==============================================================================
bool MBRDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MBRDelayAudioProcessor::createEditor()
{
    return new MBRDelayAudioProcessorEditor (*this);
}

//==============================================================================
void MBRDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    // (From the JUCE Tut)
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MBRDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    // From the JUCE Tut
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
    
    if(xmlState.get() != nullptr)
    {
        if(xmlState->hasTagName (treeState.state.getType()))
        {
            treeState.replaceState (juce::ValueTree::fromXml(*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MBRDelayAudioProcessor();
}

//==============================================================================
// Functions to convert audio into mono
void MBRDelayAudioProcessor::toggleMono(int m)
{
    if(m == 1) mMono = false;
    else if(m == 0) mMono = true;
}

void MBRDelayAudioProcessor::convertStereoToMono(float& leftChannelAudio, float& rightChannelAudio)
{
    leftChannelAudio += rightChannelAudio;
    rightChannelAudio = leftChannelAudio;
    
    rightChannelAudio *= 0.6f;
    leftChannelAudio  *= 0.6f;
}

// Public functions for Module mods
void MBRDelayAudioProcessor::adjustFeedback(double fb)
{
    mDelayBuffer.setFeedback(fb);
}

void MBRDelayAudioProcessor::adjustTimeDelay(std::string name, int ms)
{
    
    mDelayBuffer.setTimeDelay(name, ms);
}

void MBRDelayAudioProcessor::adjustHighpass(int hz)
{
    mDelayBuffer.updateHighpassCutoff(hz);
}

void MBRDelayAudioProcessor::adjustLowpass(int hz)
{
    mDelayBuffer.updateLowpassCutoff(hz);
}

void MBRDelayAudioProcessor::updateDry(float d)
{
    smoothedDry.setTargetValue(d);
}

void MBRDelayAudioProcessor::updateWet(float d)
{
    smoothedWet.setTargetValue(d);
}

void MBRDelayAudioProcessor::bypassToggle(int b)
{
    if(b == 0) Bypass = false;
    else if(b == 1) Bypass = true;
}
