#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);

    lowcutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lowcutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(lowcutoffSlider);

    lowqualitySlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lowqualitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(lowqualitySlider);


    cutoffAttachment = std::make_unique<SliderAttachment>(
       processorRef.apvts,
       "CUTOFF",
       lowcutoffSlider
       );

    qualityAttachment = std::make_unique<SliderAttachment>(
        processorRef.apvts,
        "QUALITY",
        lowqualitySlider
        );

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void AudioPluginAudioProcessorEditor::resized()
{
    lowcutoffSlider.setBounds(50,80,100,120);
    lowqualitySlider.setBounds(200,80,100,120);
}
