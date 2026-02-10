#pragma once

#include "PluginProcessor.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
    juce::Slider lowcutoffSlider;
    juce::Slider lowqualitySlider;

    juce::Slider highcutoffSlider;
    juce::Slider highqualitySlider;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> cutoffAttachment;
    std::unique_ptr<SliderAttachment> qualityAttachment;

    AudioPluginAudioProcessor& processorRef;
};
