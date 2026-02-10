#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "juce_core/juce_core.h"
#include <memory>

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
          .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
          .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      ),
      apvts(*this, nullptr, "Parameters", createParameterLayout()) {
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor() {
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "LOWPASSCUTOFF",
        "Cutoff Freq",
        juce::NormalisableRange<float>(20.0f, 20000, 1.0f, 0.5f),
        1000.0f
    ));

    auto lowQRange = juce::NormalisableRange<float>(0.01f, 5.0f, 0.01f);
    lowQRange.setSkewForCentre(0.707f); // Sets 0.707 to be exactly at 12 o'clock

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "LOWPASSQUALITY",
        "Q",
        lowQRange,
        0.707f
    ));

layout.add(std::make_unique<juce::AudioParameterFloat>(
        "HIGHPASSCUTOFF",
        "Cutoff Freq",
        juce::NormalisableRange<float>(20.0f, 20000, 1.0f, 0.5f),
        1000.0f
    ));

    auto highQRange = juce::NormalisableRange<float>(0.01f, 5.0f, 0.01f);
    highQRange.setSkewForCentre(0.707f); // Sets 0.707 to be exactly at 12 o'clock

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "HIGHPASSQUALITY",
        "Q",
        highQRange,
        0.707f
    ));
layout.add(std::make_unique<juce::AudioParameterFloat>(
        "BELL1CUTOFF",
        "Cutoff Freq",
        juce::NormalisableRange<float>(20.0f, 20000, 1.0f, 0.5f),
        1000.0f
    ));

    auto qRangeBell = juce::NormalisableRange<float>(0.01f, 5.0f, 0.01f);
    qRangeBell.setSkewForCentre(0.707f); // Sets 0.707 to be exactly at 12 o'clock

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "BELL1Q",
        "Q",
        qRangeBell,
        0.707f
    ));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                "BELL1GAIN",
                "Gain",
                juce::NormalisableRange<float>(-25.0f,25.0f,0.01f,1.0f),
                0.0f
                ));

    return layout;
}

const juce::String AudioPluginAudioProcessor::getName() const {
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::isMidiEffect() const {
    return false;
}
bool AudioPluginAudioProcessor::acceptsMidi() const {
    return false;
}
bool AudioPluginAudioProcessor::producesMidi() const {
    return false;
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms() {
    return 1;
}

int AudioPluginAudioProcessor::getCurrentProgram() {
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram(int index) {
    juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index) {
    juce::ignoreUnused(index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index, const juce::String &newName) {
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    // Use this method as the place to do any pre-playback
    // initialisation that you need.
    leftLPF.prepare(static_cast<float>(sampleRate));
    rightLPF.prepare(static_cast<float>(sampleRate));
}

void AudioPluginAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
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
void AudioPluginAudioProcessor::setLowPassFilter(LowPassFilter filter, float cutoff, float q){
    filter.setCutoff(cutoff);
    filter.setResonance(q);
}
void AudioPluginAudioProcessor::setHighPassFilter(HighPassFilter filter, float cutoff, float q){
    filter.setCutoff(cutoff);
    filter.setResonance(q);
}
void AudioPluginAudioProcessor::setBellFilter(BellFilter filter, float freq, float q, float gain){
    filter.setCutoff(freq);
    filter.setResonance(q);
    filter.setDbGain(gain);
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                             juce::MidiBuffer &midiMessages) {
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    float LowNewCutoff = *apvts.getRawParameterValue("LOWPASSCUTOFF");
    float LowNewQ = *apvts.getRawParameterValue("LOWPASSQUALITY");

    float HighNewCutoff = *apvts.getRawParameterValue("HIGHPASSCUTOFF");
    float HighNewQ = *apvts.getRawParameterValue("HIGHPASSCUTOFF");

    float Bell1NewFreq = *apvts.getRawParameterValue("BELL1CUTOFF");
    float Bell1NewQ = *apvts.getRawParameterValue("BELL1Q");
    float Bell1NewGain = *apvts.getRawParameterValue("BELL1GAIN");

    setLowPassFilter(leftLPF, LowNewCutoff, LowNewQ);
    setLowPassFilter(rightLPF, LowNewCutoff, LowNewQ);

    setHighPassFilter(leftHPFilter, HighNewCutoff, HighNewQ);
    setHighPassFilter(rightHPFilter, HighNewCutoff, HighNewQ);

    setBellFilter(leftBellFilter1, Bell1NewFreq, Bell1NewQ, Bell1NewGain);
    setBellFilter(rightBellFilter1, Bell1NewFreq, Bell1NewQ, Bell1NewGain);


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());


    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto *channelData = buffer.getWritePointer(channel);
        auto &filter = (channel == 0) ? leftLPF : rightLPF;

        for (int i = 0; i < buffer.getNumSamples(); ++i)
            channelData[i] = filter.process(channelData[i]);
    }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *AudioPluginAudioProcessor::createEditor() {
    return new AudioPluginAudioProcessorEditor(*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {
    if (const std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes)); xmlState != nullptr) {
        if (xmlState->hasTagName(apvts.state.getType())) {
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin.
juce::AudioProcessor * JUCE_CALLTYPE createPluginFilter() {
    return new AudioPluginAudioProcessor();
}
