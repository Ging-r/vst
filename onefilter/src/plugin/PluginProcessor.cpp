#include "PluginProcessor.h"
#include "PluginEditor.h"

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
        "CUTOFF",
        "Cutoff Freq",
        juce::NormalisableRange<float>(20.0f, 20000.0f, 0.1f, 0.5f),
        1000.0f
    ));

    auto qRange = juce::NormalisableRange<float>(0.01f, 30.0f, 0.001f);
    qRange.setSkewForCentre(0.707f);

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "QUALITY",
        "Q",
        qRange,
        0.707f
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
    leftFilter.prepare(static_cast<float>(sampleRate));
    rightFilter.prepare(static_cast<float>(sampleRate));
}

void AudioPluginAudioProcessor::releaseResources() {
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

float oldQ = 0.0f;
float oldCutOff = 0.0f;
void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                             juce::MidiBuffer &midiMessages) {
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    float newCutoff = *apvts.getRawParameterValue("CUTOFF");
    float newQ = *apvts.getRawParameterValue("QUALITY");

    //if(std::abs(oldCutOff-newCutoff)<0.01f){
        leftFilter.setCutoff(newCutoff);
        rightFilter.setCutoff(newCutoff);
        oldCutOff = newCutoff;

    //}
    //if(std::abs(oldQ-newQ)<0.01f){
        leftFilter.setResonance(newQ);
        rightFilter.setResonance(newQ);
        oldQ = newQ;
    //}
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());


    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto *channelData = buffer.getWritePointer(channel);
        auto &filter = (channel == 0) ? leftFilter : rightFilter;

        for (int i = 0; i < buffer.getNumSamples(); ++i)
            channelData[i] = filter.process(channelData[i]);
    }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const {
    return true;
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
