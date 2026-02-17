#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "../dsp/LowPassFilter.cpp"
#include "../dsp/HighPassFilter.cpp"
#include "../dsp/BellFilter.cpp"

//==============================================================================
class AudioPluginAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

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


    void setLowPassFilter(LowPassFilter *filter, float cutoff, float q);
    void setHighPassFilter(HighPassFilter *filter, float cutoff, float q);
    void setBellFilter(BellFilter *filter, float freq, float q, float gain);

    juce::AudioProcessorValueTreeState apvts;
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
    LowPassFilter leftLPF;
    LowPassFilter rightLPF;

    HighPassFilter leftHPF;
    HighPassFilter rightHPF;

    BellFilter leftBellFilter1;
    BellFilter rightBellFilter1;

    BellFilter leftBellFilter2;
    BellFilter rightBellFilter2;

    BellFilter leftBellFilter3;
    BellFilter rightBellFilter3;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
};
