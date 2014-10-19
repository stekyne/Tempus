#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Controller\Parameters.cpp"

TempusAudioProcessor::TempusAudioProcessor() : 
	masterVolume (1.0f)
{
    //addParameter ()
}

TempusAudioProcessor::~TempusAudioProcessor()
{
}

const String TempusAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

int TempusAudioProcessor::getNumParameters()
{
	return Parameters::NumParameters;
}

float TempusAudioProcessor::getParameter (int index)
{
	jassert (index < static_cast<int> (NumParameters));
	return parameters[index].getValue ();
}

void TempusAudioProcessor::setParameter (int index, float newValue)
{
	jassert (index < static_cast<int> (NumParameters));
	parameters[index].setValue (newValue);
}

const String TempusAudioProcessor::getParameterName (int index)
{
	jassert (index < static_cast<int> (NumParameters));
	return parameters[index].getName ();
}

const String TempusAudioProcessor::getParameterText (int index)
{
	jassert (index < static_cast<int> (NumParameters));
	return parameters[index].getValueAsString ();
}

String TempusAudioProcessor::getParameterName (int parameterIndex, int maximumStringLength)
{
    
    return String::empty;
}

const String TempusAudioProcessor::getInputChannelName (int channelIndex) const
{
	return String (channelIndex + 1);
}

const String TempusAudioProcessor::getOutputChannelName (int channelIndex) const
{
	return String (channelIndex + 1);
}

bool TempusAudioProcessor::isInputChannelStereoPair (int) const
{
	return true;
}

bool TempusAudioProcessor::isOutputChannelStereoPair (int) const
{
	return true;
}

bool TempusAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
	return true;
   #else
	return false;
   #endif
}

bool TempusAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
	return true;
   #else
	return false;
   #endif
}

bool TempusAudioProcessor::silenceInProducesSilenceOut() const
{
	return false;
}

double TempusAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int TempusAudioProcessor::getNumPrograms()
{
	return 0;
}

int TempusAudioProcessor::getCurrentProgram()
{
	return 0;
}

void TempusAudioProcessor::setCurrentProgram (int index)
{
}

const String TempusAudioProcessor::getProgramName (int index)
{
	return String::empty;
}

void TempusAudioProcessor::changeProgramName (int index, const String& newName)
{
}

void TempusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	const auto maxDelayInSamples = (int)ceil (MAX_DELAY_IN_SECONDS * sampleRate);

	for (int i = 0; i < MAX_NUM_TAPS; ++i)
	{
		delayLine[i].initialise (sampleRate, maxDelayInSamples);
	}
}

void TempusAudioProcessor::releaseResources ()
{
}

void TempusAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	auto inputBuffer = buffer.getArrayOfChannels ();
	const auto numChans = buffer.getNumChannels ();

	jassert (numChans <= 2);

	for (int tap = 0; tap < MAX_NUM_TAPS; ++tap)
	{
		if (delayLine[tap].isEnabled () == false)
			continue;

		delayLine[tap].process (inputBuffer, buffer.getNumSamples ());
	}

	buffer.applyGain (masterVolume);

	for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
		buffer.clear (i, 0, buffer.getNumSamples());
}

bool TempusAudioProcessor::hasEditor() const
{
	return true;
}

AudioProcessorEditor* TempusAudioProcessor::createEditor()
{
	return new TempusAudioProcessorEditor (this);
}

void TempusAudioProcessor::getStateInformation (MemoryBlock& destData)
{
}

void TempusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new TempusAudioProcessor();
}
