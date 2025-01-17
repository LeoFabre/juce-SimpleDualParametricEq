#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

//==============================================================================
class AudioPluginAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();

    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;

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

    void setCurrentProgram(int index) override;

    const juce::String getProgramName(int index) override;

    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;

    void setStateInformation(const void *data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState &getValueTreeState() { return parameters; }

    template<typename ValueT>
    juce::NormalisableRange<ValueT> logRange(ValueT min, ValueT max)
    {
        ValueT rng{std::log(max / min)};
        return {
            min, max,
            [=](ValueT min, ValueT, ValueT v) { return std::exp(v * rng) * min; },
            [=](ValueT min, ValueT, ValueT v) { return std::log(v / min) / rng; }
        };
    }

private:
    juce::AudioProcessorValueTreeState parameters;

    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> eq1Filter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> eq2Filter;

    void updateFilters();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessor)
};
