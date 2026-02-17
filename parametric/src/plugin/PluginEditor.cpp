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
    lowqualitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false,50,20);
    addAndMakeVisible(lowqualitySlider);

    highcutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    highcutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(highcutoffSlider);

    highqualitySlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    highqualitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(highqualitySlider);

    bellfreqSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bellfreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(bellfreqSlider);

    bellqSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bellqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(bellqSlider);

    bellGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bellGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(bellGainSlider);




    lowcutoffAttachment = std::make_unique<SliderAttachment>(
       processorRef.apvts,
       "LOWPASSCUTOFF",
       lowcutoffSlider
       );

    lowqualityAttachment = std::make_unique<SliderAttachment>(
        processorRef.apvts,
        "LOWPASSQUALITY",
        lowqualitySlider
        );
    highcutoffAttachment = std::make_unique<SliderAttachment>(
       processorRef.apvts,
       "HIGHPASSCUTOFF",
       highcutoffSlider
       );

    highqualityAttachment = std::make_unique<SliderAttachment>(
        processorRef.apvts,
        "HIGHPASSQUALITY",
        highqualitySlider
        );

     bellfreqAttachment = std::make_unique<SliderAttachment>(
        processorRef.apvts,
        "BELL1CUTOFF",
        bellfreqSlider
        );

     bellqAttachment = std::make_unique<SliderAttachment>(
        processorRef.apvts,
        "BELL1Q",
        bellqSlider
        );

     bellgainAttachment = std::make_unique<SliderAttachment>(
        processorRef.apvts,
        "BELL1GAIN",
        bellGainSlider
        );



    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600,400);
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
    highcutoffSlider.setBounds(50,180,100,100);
    highqualitySlider.setBounds(50,20,100,100);
    lowcutoffSlider.setBounds(450, 180, 100, 100);
    lowqualitySlider.setBounds(450, 20, 100, 100);
    bellfreqSlider.setBounds(250, 180,100,100);
    bellGainSlider.setBounds(250 , 20, 100, 100);
    bellqSlider.setBounds(250, 300, 100, 100);


}
