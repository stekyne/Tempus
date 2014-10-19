/*
	Below is the algorithm that this class is based upon
	rp   = *p - vdt;											// calculate where to read from, (current write position - the delay time)
	rp	 = (rp >= 0 ? (rp < mdt ? rp : rp - mdt) : rp + mdt);	// boundary check, is the read pointer looking at a valid buffer point
	rpi  = (int)rp;												// take the integer value of the read pointer
	frac = rp - rpi;											// take the fractional value of the read pointer
	next = (rpi != mdt-1 ? delay[rpi+1] : delay[0]);			// get the next sample, check if the next sample is outside the buffer
	out  =  delay[rpi] + frac*(next - delay[rpi]);				// calculate the output sample using linear interpolation
	delay[*p] = sig[i];											// update delay line
	sig[i] = out;												// output delay sample
	*p = (*p != mdt-1 ? *p+1 : 0);								// increment write position, check for end of buffer
*/
#include "VDelayLine.h"

VDelayLine::VDelayLine ()
	:	maxDelay (10.f),
		sampleRate (44100),
		dry (1.f),
        wet (0.7f),
        maxDelayInSamples (0),
        blockSize (0)
{
	writePos[0] = writePos[1] = 0;
    delay[0] = delay[1] = nullptr;
}

VDelayLine::~VDelayLine ()
{
    delete[] delay[0];
    delete[] delay[1];

    for (int i = 0; i < MAX_TAPS; ++i)
        delete[] buffers[i];
}

void VDelayLine::initialise (float sampleRate, int blockSize, float maxDelay)
{
	this->sampleRate = sampleRate;
    this->blockSize = blockSize;
    this->maxDelay = maxDelay;

    maxDelayInSamples = (int)ceil (maxDelay * sampleRate);

    delay[0] = new float[maxDelayInSamples];
    delay[1] = new float[maxDelayInSamples];

    for (int i = 0; i < MAX_TAPS; ++i)
    {
        buffers[i] = new float[2 * blockSize];
        taps[i].setSampleRate (sampleRate);
    }
    
    clearBuffers ();
}

void VDelayLine::clearBuffers ()
{
    memset (delay[0], 0, sizeof (float) * maxDelayInSamples);
    memset (delay[1], 0, sizeof (float) * maxDelayInSamples);

    for (int i = 0; i < MAX_TAPS; ++i)
        memset (buffers[i], 0, sizeof (float) * blockSize * 2);
}

float VDelayLine::getMaxDelay () const
{
	return maxDelay;
}

float VDelayLine::getTapDelay (int tapNum) const
{
    jassert (tapNum < MAX_TAPS);
	return taps[tapNum].getTapTime ();
}

void VDelayLine::setTapDelay (int tapNum, float delayTime)
{
    jassert (tapNum < MAX_TAPS);
	taps[tapNum].setTapTime (delayTime > maxDelay ? maxDelay : delayTime);
}

bool VDelayLine::isTapEnabled (int tapNum) const
{
    jassert (tapNum < MAX_TAPS);
	return taps[tapNum].isTapEnabled ();
}

void VDelayLine::enableTap (int tapNum, bool isEnabled)
{
    jassert (tapNum < MAX_TAPS);
	taps[tapNum].setTapEnabled (isEnabled);
}

float VDelayLine::getDryMix () const
{
    return dry;
}

void VDelayLine::setDryMix (float mix) 
{ 
    this->dry = mix; 
}

float VDelayLine::getWetMix () const
{
    return wet;
}

void VDelayLine::setWetMix (float mix)
{
    this->wet = mix;
}

float VDelayLine::getVolume (int tapNum) const
{ 
    jassert (tapNum < MAX_TAPS);
    return taps[tapNum].getVolume ();
}

void VDelayLine::setVolume (int tapNum, float value)
{ 
    jassert (tapNum < MAX_TAPS);
    this->taps[tapNum].setVolume (value);
}

float VDelayLine::getFeedback (int tapNum) const
{ 
    jassert (tapNum < MAX_TAPS);
    return taps[tapNum].getFeedback ();
}

void VDelayLine::setFeedback (int tapNum, float value)
{ 
    jassert (tapNum < MAX_TAPS);
    this->taps[tapNum].setFeedback (value);
}

float VDelayLine::getPan (int tapNum) const
{ 
    jassert (tapNum < MAX_TAPS);
    return taps[tapNum].getPan ();
}

void VDelayLine::setPan (int tapNum, float value)
{ 
    jassert (tapNum < MAX_TAPS);
    this->taps[tapNum].setPan (value);
}

float VDelayLine::getModSpeed (int tapNum) const
{ 
    jassert (tapNum < MAX_TAPS);
    return taps[tapNum].getModSpeed ();
}

void VDelayLine::setModSpeed (int tapNum, float value)
{ 
    jassert (tapNum < MAX_TAPS);
    this->taps[tapNum].setModSpeed (value);
}

float VDelayLine::getModAmount (int tapNum) const
{ 
    jassert (tapNum < MAX_TAPS);
    return taps[tapNum].getModAmount ();
}

void VDelayLine::setModAmount (int tapNum, float value)
{ 
    jassert (tapNum < MAX_TAPS);
    this->taps[tapNum].setModAmount (value);
}

void VDelayLine::process (AudioSampleBuffer &buffer)
{
    // Check if delayline has been initialised
    if (delay[0] == nullptr || delay[1] == nullptr) return;
    jassert (blockSize == buffer.getNumSamples ());

	float* buff_in[2] = { buffer.getSampleData (0),		// left
						  buffer.getSampleData (1) };	// right channel

	// Run the current buffer through the multitap delay
	// This will put data into each of the taps own audio buffer
	// So that further processing can be done
	for (int chan = 0; chan < 2; chan++)
	{
        // Calculate offset in array to find the next channel position
        const int offset = (chan == 0) ? 0 : chan * blockSize;

		for (int i = 0; i < buffer.getNumSamples (); ++i)
		{
			double out = 0.0;

			for (int tapNum = 0; tapNum < MAX_TAPS; tapNum++)
			{
                if (!taps[tapNum].isTapEnabled ())
                    continue;

                // Update tap delay times
                taps[tapNum].applyLFO ();
                readPos = writePos[chan] - (taps[tapNum].getTapTime () * sampleRate);

				// Ensure the next read position is within bounds
				if (readPos >= 0)
				{
                    readPos = (readPos < maxDelayInSamples ? 
                                    readPos : readPos - maxDelayInSamples);
				}
				else
				{
                    readPos = readPos + maxDelayInSamples;
				}

                jassert (readPos >= 0 && readPos < maxDelayInSamples);

				readPos_int = (int)std::truncf (readPos);
				readPos_frac = readPos - readPos_int;

				// Boundary check the next sample index
                if (readPos_int != maxDelayInSamples - 1)
				{
					next = delay[chan][readPos_int + 1];
				}
				else
				{
					next = delay[chan][0];
				}

				// Use linear interpolation to calculate the next sample
                buffers[tapNum][i + offset] = (float)(delay[chan][readPos_int] +
													readPos_frac * 
													(next - delay[chan][readPos_int]));

                // Apply further processing on this tap channel
                // ..

                const float panning = (chan == 0 ?
                                       sinf ((1.0f - taps[tapNum].getPan ()) * DSP::PI_HALFf) :
                                       sinf (taps[tapNum].getPan () * DSP::PI_HALFf));

				// Accumulate the output sample
                out += buffers[tapNum][i + offset] * 
                        taps[tapNum].getFeedback () * 
                        panning;
			}

			delay[chan][writePos[chan]] = (float)(buff_in[chan][i]);

            if (++writePos[chan] >= maxDelayInSamples)
                writePos[chan] = 0;

            float tapSamples = 0.f;

            // Write out the processed samples to the buffer given by the host
            for (int tapNum = 0; tapNum < MAX_TAPS; ++tapNum)
            {
                tapSamples += buffers[tapNum][i + offset] *
                                taps[tapNum].getVolume ();
            }

            buff_in[chan][i] = tapSamples + (float)out;
		}
	}
}
