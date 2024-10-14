#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), parameters(p.getValueTreeState())
{
    // Configuration des sliders et labels pour EQ1

    // EQ1 Fréquence
    eq1FreqSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    eq1FreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    addAndMakeVisible(eq1FreqSlider);

    eq1FreqLabel.setText("Freq", juce::dontSendNotification);
    eq1FreqLabel.attachToComponent(&eq1FreqSlider, false);
    addAndMakeVisible(eq1FreqLabel);

    // EQ1 Gain
    eq1GainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    eq1GainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    addAndMakeVisible(eq1GainSlider);

    eq1GainLabel.setText("Gain", juce::dontSendNotification);
    eq1GainLabel.attachToComponent(&eq1GainSlider, false);
    addAndMakeVisible(eq1GainLabel);

    // EQ1 Q
    eq1QSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    eq1QSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    addAndMakeVisible(eq1QSlider);

    eq1QLabel.setText("Q", juce::dontSendNotification);
    eq1QLabel.attachToComponent(&eq1QSlider, false);
    addAndMakeVisible(eq1QLabel);

    // EQ1 On/Off
    eq1OnButton.setButtonText("On");
    addAndMakeVisible(eq1OnButton);

    // Configuration des sliders et labels pour EQ2

    // EQ2 Fréquence
    eq2FreqSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    eq2FreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    addAndMakeVisible(eq2FreqSlider);

    eq2FreqLabel.setText("Freq", juce::dontSendNotification);
    eq2FreqLabel.attachToComponent(&eq2FreqSlider, false);
    addAndMakeVisible(eq2FreqLabel);

    // EQ2 Gain
    eq2GainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    eq2GainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    addAndMakeVisible(eq2GainSlider);

    eq2GainLabel.setText("Gain", juce::dontSendNotification);
    eq2GainLabel.attachToComponent(&eq2GainSlider, false);
    addAndMakeVisible(eq2GainLabel);

    // EQ2 Q
    eq2QSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    eq2QSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    addAndMakeVisible(eq2QSlider);

    eq2QLabel.setText("Q", juce::dontSendNotification);
    eq2QLabel.attachToComponent(&eq2QSlider, false);
    addAndMakeVisible(eq2QLabel);

    // EQ2 On/Off
    eq2OnButton.setButtonText("On");
    addAndMakeVisible(eq2OnButton);

    // Attachments pour lier les sliders aux paramètres
    eq1FreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        parameters, "EQ1_FREQ", eq1FreqSlider);
    eq1GainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        parameters, "EQ1_GAIN", eq1GainSlider);
    eq1QAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        parameters, "EQ1_Q", eq1QSlider);
    eq1OnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        parameters, "EQ1_ON", eq1OnButton);

    eq2FreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        parameters, "EQ2_FREQ", eq2FreqSlider);
    eq2GainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        parameters, "EQ2_GAIN", eq2GainSlider);
    eq2QAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        parameters, "EQ2_Q", eq2QSlider);
    eq2OnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        parameters, "EQ2_ON", eq2OnButton);

    // Configuration des limites des sliders
    // EQ1
    eq1FreqSlider.setRange(20.0, 20000.0, 1.0);
    eq1FreqSlider.setSkewFactorFromMidPoint(1000.0);

    eq1GainSlider.setRange(-24.0, 24.0, 0.1);
    eq1GainSlider.setSkewFactorFromMidPoint(0.0);

    eq1QSlider.setRange(0.1, 10.0, 0.1);
    eq1QSlider.setSkewFactor(1.0); // Linéaire

    // EQ2
    eq2FreqSlider.setRange(20.0, 20000.0, 1.0);
    eq2FreqSlider.setSkewFactorFromMidPoint(5000.0);

    eq2GainSlider.setRange(-24.0, 24.0, 0.1);
    eq2GainSlider.setSkewFactorFromMidPoint(0.0);

    eq2QSlider.setRange(0.1, 10.0, 0.1);
    eq2QSlider.setSkewFactor(1.0); // Linéaire

    // Définir la taille de l'éditeur
    setSize (350, 450);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Remplir l'arrière-plan
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // Dessiner le titre
    g.setColour (juce::Colours::white);
    g.setFont (20.0f);
    g.drawFittedText ("Dual Parametric EQ", getLocalBounds().removeFromTop(30), juce::Justification::centred, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // Définir les zones pour les EQs
    auto area = getLocalBounds().reduced(20);
    auto eq1Area = area.removeFromLeft(getWidth() / 2 - 30);
    auto eq2Area = area;

    // Positionnement des contrôles pour EQ1
    int sliderDiameter = 80;
    int labelHeight = 20;
    int padding = 10;

    // Arrange EQ1 sliders vertically
    int currentY = 60;

    eq1FreqSlider.setBounds(eq1Area.getX() + (eq1Area.getWidth() - sliderDiameter) / 2, currentY, sliderDiameter, sliderDiameter);
    currentY += sliderDiameter + labelHeight + padding;

    eq1GainSlider.setBounds(eq1Area.getX() + (eq1Area.getWidth() - sliderDiameter) / 2, currentY, sliderDiameter, sliderDiameter);
    currentY += sliderDiameter + labelHeight + padding;

    eq1QSlider.setBounds(eq1Area.getX() + (eq1Area.getWidth() - sliderDiameter) / 2, currentY, sliderDiameter, sliderDiameter);
    currentY += sliderDiameter + labelHeight + padding;

    eq1OnButton.setBounds(eq1Area.getX() + (eq1Area.getWidth() - 60) / 2, currentY, 60, 30);

    // Positionnement des contrôles pour EQ2
    currentY = 60;

    eq2FreqSlider.setBounds(eq2Area.getX() + (eq2Area.getWidth() - sliderDiameter) / 2, currentY, sliderDiameter, sliderDiameter);
    currentY += sliderDiameter + labelHeight + padding;

    eq2GainSlider.setBounds(eq2Area.getX() + (eq2Area.getWidth() - sliderDiameter) / 2, currentY, sliderDiameter, sliderDiameter);
    currentY += sliderDiameter + labelHeight + padding;

    eq2QSlider.setBounds(eq2Area.getX() + (eq2Area.getWidth() - sliderDiameter) / 2, currentY, sliderDiameter, sliderDiameter);
    currentY += sliderDiameter + labelHeight + padding;

    eq2OnButton.setBounds(eq2Area.getX() + (eq2Area.getWidth() - 60) / 2, currentY, 60, 30);
}
