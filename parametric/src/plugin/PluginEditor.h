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

    juce::Slider bellfreqSlider;
    juce::Slider bellqSlider;
    juce::Slider bellGainSlider;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> lowcutoffAttachment;
    std::unique_ptr<SliderAttachment> lowqualityAttachment;
    std::unique_ptr<SliderAttachment> highqualityAttachment;
    std::unique_ptr<SliderAttachment> highcutoffAttachment;

    std::unique_ptr<SliderAttachment> bellfreqAttachment;
    std::unique_ptr<SliderAttachment> bellqAttachment;
    std::unique_ptr<SliderAttachment> bellgainAttachment;
    AudioPluginAudioProcessor& processorRef;
};
