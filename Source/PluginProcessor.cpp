#include "PluginProcessor.h"
#include "PluginEditor.h"

TempusAudioProcessor::TempusAudioProcessor()
{
    addParameter (masterVolume = new MainVolumeParam);
    addParameter (dryVolume = new DryVolumeParam);
    addParameter (wetVolume = new WetVolumeParam);

    for (int i = 0; i < MAX_NUM_TAPS; ++i)
    {
        addParameter (delayEnabled[i] = new DelayEnabledParam);
        addParameter (delayVolume[i] = new DelayVolumeParam);
        addParameter (delayAmount[i] = new DelayTimeAmountParam);
        addParameter (delayPan[i] = new DelayPanParam);
        addParameter (delayFeedback[i] = new DelayFeedbackParam);
        addParameter (delayModSpeed[i] = new DelayModSpeedParam);
        addParameter (delayModAmount[i] = new DelayModAmountParam);

        delayLine[i].setExternalParameters (delayEnabled[i], delayVolume[i], delayAmount[i], delayPan[i],
                                            delayFeedback[i], delayModSpeed[i], delayModAmount[i]);
    }

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
    return getParameters ().size ();
}

float TempusAudioProcessor::getParameter (int index)
{
    jassert (index < getNumParameters ());
    //std::lock_guard<std::mutex> lock (paramLock);
    return getParameters ().getUnchecked (index)->getValue ();
}

void TempusAudioProcessor::setParameter (int index, float newValue)
{
    jassert (index < getNumParameters ());
    //std::lock_guard<std::mutex> lock (paramLock);
    getParameters ().getUnchecked (index)->setValue (newValue);
}

const String TempusAudioProcessor::getParameterName (int index)
{
    jassert (index < getNumParameters ()); 
    //std::lock_guard<std::mutex> lock (paramLock);
    return getParameters ().getUnchecked (index)->getName (512);
}

const String TempusAudioProcessor::getParameterText (int index)
{
    jassert (index < getNumParameters ());
    //std::lock_guard<std::mutex> lock (paramLock);
    const auto value = getParameters ().getUnchecked (index)->getValue ();
    return getParameters ().getUnchecked (index)->getText (value, 512);
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
	return 1;
}

int TempusAudioProcessor::getCurrentProgram()
{
	return 0;
}

void TempusAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const String TempusAudioProcessor::getProgramName (int /*index*/)
{
	return String::empty;
}

void TempusAudioProcessor::changeProgramName (int /*index*/, const String& /*newName*/)
{
}

void TempusAudioProcessor::prepareToPlay (double sampleRate, int /*samplesPerBlock*/)
{
	const auto maxDelayInSamples = (int)ceil (MAX_DELAY_IN_SECONDS * sampleRate);

	for (int i = 0; i < MAX_NUM_TAPS; ++i)
	{
		delayLine[i].initialise ((float)sampleRate, maxDelayInSamples);
	}
}

void TempusAudioProcessor::releaseResources ()
{
}

void TempusAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& /*midiMessages*/)
{
    auto outputBuffer = buffer.getArrayOfWritePointers ();
	const auto numChans = buffer.getNumChannels ();

    jassert (numChans > 0 && numChans <= 2);

    for (int tap = 0; tap < MAX_NUM_TAPS; ++tap)
        delayLine[tap].process (outputBuffer, buffer.getNumSamples ());

    buffer.applyGain (masterVolume->getValue ());

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

void TempusAudioProcessor::getStateInformation (MemoryBlock& /*destData*/)
{
}

void TempusAudioProcessor::setStateInformation (const void* /*data*/, int /*sizeInBytes*/)
{
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new TempusAudioProcessor();
}
