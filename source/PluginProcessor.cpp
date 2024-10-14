#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       parameters(*this, nullptr, juce::Identifier("PARAMETERS"),
       {
           // EQ1 Parameters
           std::make_unique<juce::AudioParameterFloat>("EQ1_FREQ", "EQ1 Frequency", 20.0f, 20000.0f, 1000.0f),
           std::make_unique<juce::AudioParameterFloat>("EQ1_GAIN", "EQ1 Gain", -24.0f, 24.0f, 0.0f),
           std::make_unique<juce::AudioParameterFloat>("EQ1_Q", "EQ1 Q", 0.1f, 10.0f, 1.0f),
           std::make_unique<juce::AudioParameterBool>("EQ1_ON", "EQ1 On", true),

           // EQ2 Parameters
           std::make_unique<juce::AudioParameterFloat>("EQ2_FREQ", "EQ2 Frequency", 20.0f, 20000.0f, 5000.0f),
           std::make_unique<juce::AudioParameterFloat>("EQ2_GAIN", "EQ2 Gain", -24.0f, 24.0f, 0.0f),
           std::make_unique<juce::AudioParameterFloat>("EQ2_Q", "EQ2 Q", 0.1f, 10.0f, 1.0f),
           std::make_unique<juce::AudioParameterBool>("EQ2_ON", "EQ2 On", true),
       })
#endif
{
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Préparation des filtres EQ
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();

    eq1Filter.prepare(spec);
    eq1Filter.reset();
    eq2Filter.prepare(spec);
    eq2Filter.reset();

    // Initialisation des filtres avec les paramètres actuels
    updateFilters();
}

void AudioPluginAudioProcessor::releaseResources()
{
    // Libération des ressources si nécessaire
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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
#endif

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Efface les canaux de sortie non utilisés
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Met à jour les filtres au cas où les paramètres ont changé
    updateFilters();

    // Création d'un processeur DSP pour traiter le buffer
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    // Appliquer EQ1 si activé
    bool eq1On = parameters.getRawParameterValue("EQ1_ON")->load();
    if (eq1On)
        eq1Filter.process(context);

    // Appliquer EQ2 si activé
    bool eq2On = parameters.getRawParameterValue("EQ2_ON")->load();
    if (eq2On)
        eq2Filter.process(context);
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Sauvegarde de l'état des paramètres
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Restaure l'état des paramètres
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState && xmlState->hasTagName(parameters.state.getType()))
    {
        parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
        updateFilters();
    }
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}

// Mise à jour des filtres EQ en fonction des paramètres
void AudioPluginAudioProcessor::updateFilters()
{
    // EQ1
    auto eq1Freq = parameters.getRawParameterValue("EQ1_FREQ")->load();
    auto eq1Gain = parameters.getRawParameterValue("EQ1_GAIN")->load();
    auto eq1Q = parameters.getRawParameterValue("EQ1_Q")->load();
    bool eq1On = parameters.getRawParameterValue("EQ1_ON")->load();

    if (eq1On) {
        auto coeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), eq1Freq, eq1Q,
            juce::Decibels::decibelsToGain(eq1Gain));
        *eq1Filter.coefficients = *coeffs;
    } else {
        // Réinitialiser le filtre (état interne)
        eq1Filter.reset();
    }

    // EQ2
    auto eq2Freq = parameters.getRawParameterValue("EQ2_FREQ")->load();
    auto eq2Gain = parameters.getRawParameterValue("EQ2_GAIN")->load();
    auto eq2Q = parameters.getRawParameterValue("EQ2_Q")->load();
    bool eq2On = parameters.getRawParameterValue("EQ2_ON")->load();

    if (eq2On) {
        auto coeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), eq2Freq, eq2Q,
            juce::Decibels::decibelsToGain(eq2Gain));
        *eq2Filter.coefficients = *coeffs;
    } else {
        // Réinitialiser le filtre (état interne)
        eq2Filter.reset();
    }
}
