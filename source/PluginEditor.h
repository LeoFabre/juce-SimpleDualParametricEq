#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_graphics/juce_graphics.h>
#include "PluginProcessor.h"

//==============================================================================
class AudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Référence au processeur
    AudioPluginAudioProcessor& processorRef;

    // ValueTreeState pour lier les paramètres
    juce::AudioProcessorValueTreeState& parameters;

    // Sliders et Labels pour EQ1
    juce::Slider eq1FreqSlider;
    juce::Slider eq1GainSlider;
    juce::Slider eq1QSlider;
    juce::ToggleButton eq1OnButton;

    juce::Label eq1FreqLabel;
    juce::Label eq1GainLabel;
    juce::Label eq1QLabel;
    // Pas de label séparé pour le bouton On/Off

    // Sliders et Labels pour EQ2
    juce::Slider eq2FreqSlider;
    juce::Slider eq2GainSlider;
    juce::Slider eq2QSlider;
    juce::ToggleButton eq2OnButton;

    juce::Label eq2FreqLabel;
    juce::Label eq2GainLabel;
    juce::Label eq2QLabel;
    // Pas de label séparé pour le bouton On/Off

    // Attachments pour lier les sliders aux paramètres
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> eq1FreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> eq1GainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> eq1QAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eq1OnAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> eq2FreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> eq2GainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> eq2QAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eq2OnAttachment;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
