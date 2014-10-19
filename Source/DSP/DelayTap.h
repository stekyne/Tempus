#ifndef DSPDELAYTAP_H_INCLUDED
#define DSPDELAYTAP_H_INCLUDED

#include "LFO.h"

struct DelayTap
{
	DelayTap ();
	~DelayTap () {}

	void setTapTime (float time);
	void setFeedback (float feedback);
	void setVolume (float volume);
	void setPan (float pan);
	void setModSpeed (float speed);
	void setModAmount (float amount);

	float getTapTime () const;
	float getFeedback () const;
	float getVolume () const;
	float getPan () const;
	float getModSpeed () const;
	float getModAmount () const;

	void setTapEnabled (bool isEnabled);
	bool isTapEnabled () const;

    void setSampleRate (float sampleRate);
    float getSampleRate () const;

    void applyLFO ();

private:
    LFO<SinLFO, 2050> lfo;
    float tap_time, feedback, pan, volume, modSpeed, modAmount, sampleRate;
	bool isEnabled;
};

#endif