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
                       )
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
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

//    for (int channel = 0; channel < 2; ++channel)
//    {
//        //auto* channelData = buffer.getWritePointer (channel);
//
//        const int numSamples = buffer.getNumSamples();
//        for(int sample = 0; sample < numSamples; sample++)
//        {
//            // iterate through the channels sample
//            float smpl = buffer.getSample(channel, sample);
//
//        }
//
//    }
    const int numSamples = buffer.getNumSamples();
    for(int sample = 0; sample < numSamples; sample++)
    {
        // get left channel data
        const int leftChannel = 0;
        float leftSample = buffer.getSample(leftChannel, sample);
        // call delay
        
    }
    /*
     Each sample is processed individually.
     I'm not sure if we iterate through the sample first or the channels first
     
     but I do know that we pass each float of each index per channel
     the circular buffer gets updated per sample, per call to doDelayLeft() or doDelayRight()
     and we return the updated value at that sample
     
     Lets say I have it right with the outer loop at sample and the inner loop as the channel
     for(sample)
        get the data from the leftChannel at index sample; leftChannel[sample]
        we pass that data to doLeftDelay()
            // we do the delay and return a float
            // we can store that float for now
        
        get the data from the rightChannel at index sample; rightChannel[sample]
        we pass that data to doRightDelay()
            // we do the delay and return a float
            // we can store that float for now
     
        // call filter
        filter(leftFloat)
        filter(rightFloat)
        
        get write pointer to the left channel
        add it to left channel
        get write pointer to the right channe
        add it to the right channel
        
        should be good to send out this way
     */
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
}

void MBRDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MBRDelayAudioProcessor();
}
