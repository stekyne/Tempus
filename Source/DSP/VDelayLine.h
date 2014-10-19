#ifndef DSPVDELAYLINE_H_INCLUDED
#define DSPVDELAYLINE_H_INCLUDED

#include "DelayTap.h"
#include "../JuceLibraryCode/JuceHeader.h"

#define MAX_TAPS 8

class VDelayLine
{
public:
	VDelayLine ();
    ~VDelayLine ();

    void initialise (float samplerate, int blockSize, float maxDelay);
    void clearBuffers ();

	void setTapDelay (int tapNum, float delayTime);
	float getTapDelay (int tapNum) const;
    float getMaxDelay () const;
	bool isTapEnabled (int tapNum) const;
	void enableTap (int tapNum, bool isEnabled);

	float getDryMix () const;
	void setDryMix (float amount);
    float getWetMix () const;
    void setWetMix (float amount);

	float getVolume (int tapNum) const;
	float getFeedback (int tapNum) const;
	float getPan (int tapNum) const;
	float getModSpeed (int tapNum) const;
	float getModAmount (int tapNum) const;

	void setVolume (int tapNum, float volume);
	void setFeedback (int tapNum, float feedback);
	void setPan (int tapNum, float pan);
	void setModSpeed (int tapNum, float modSpeed);
	void setModAmount (int tapNum, float modAmount);

	void process (AudioSampleBuffer& buffer);

private:
    DelayTap taps[MAX_TAPS];
    float* buffers[MAX_TAPS];
    float lastSample[MAX_TAPS];
	float readPos, readPos_frac, next;
	float sampleRate, dry, wet, maxDelay;
    int readPos_int, writePos[2], blockSize, maxDelayInSamples;
    float* delay[2];
};

#endif
