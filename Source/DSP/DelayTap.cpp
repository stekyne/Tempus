#include "DelayTap.h"
#include "../JuceLibraryCode/JuceHeader.h"

DelayTap::DelayTap ()
	:	tap_time (0.f),
		feedback(0.f),
		volume (0.7f),
		pan (0.5f),
        modSpeed (0.f), 
        modAmount (0.f),
        sampleRate (44100),
        isEnabled (false)
{
    lfo.setup (44100);
}

void DelayTap::setTapTime (float tap_time)
{
	this->tap_time = tap_time;
}

void DelayTap::setFeedback (float fdbk)
{
	this->feedback = fdbk;
}

void DelayTap::setVolume (float volume)
{
	this->volume = volume;
}

void DelayTap::setPan (float pan)
{
	this->pan = pan;
}

void DelayTap::setModSpeed (float speedInHz)
{
    this->modSpeed = speedInHz;
}

void DelayTap::setModAmount (float amountNormalised)
{
    this->modAmount = amountNormalised;
}

float DelayTap::getTapTime () const
{  
	return tap_time;
}

float DelayTap::getFeedback () const
{
	return feedback;
}

float DelayTap::getVolume () const
{
	return volume;
}

float DelayTap::getPan () const
{
	return pan;
}

float DelayTap::getModSpeed () const
{
    return modSpeed;
}

float DelayTap::getModAmount () const
{
    return modAmount;
}

void DelayTap::setTapEnabled (bool isEnabled)
{
	this->isEnabled = isEnabled;
}

bool DelayTap::isTapEnabled () const
{
	return isEnabled;
}

void DelayTap::setSampleRate (float sampleRate)
{
    this->sampleRate = sampleRate;
    lfo.setup (sampleRate);
}

float DelayTap::getSampleRate () const
{
    return sampleRate;
}

void DelayTap::applyLFO ()
{
    if (modAmount != 0.f)
    {
        const auto lfoSample = lfo.nextSample (modSpeed);
        tap_time = tap_time + (tap_time * lfoSample * modAmount);
    }
}