#ifndef DSPDELAY_H_INCLUDED
#define DSPDELAY_H_INCLUDED

#include <math.h>
#include <cassert>
#include "DSP.h"

struct VarDelayLine
{
    VarDelayLine ()
    {}

    ~VarDelayLine () 
    {}

    void initialise (float sampleRate, int maxDelayInSamples)
    {
        this->sampleRate = sampleRate;
        this->maxDelayInSamples = maxDelayInSamples;
    }

    void updateParameters (float delayInSeconds, float feedBack, float pan, 
                           float volume, float modSpeed, float modAmount, 
                           bool isEnabled)
    {
        this->delayInSecs = delayInSeconds;
        this->feedback = feedBack;
        this->pan = pan;
        this->volume = volume;
        this->modSpeed = modSpeed;
        this->modAmount = modAmount;
        
        // If previously disabled, clear delay history
        if (this->isEnabled == false && isEnabled == true)
            shouldClearDelay = true;

        this->isEnabled = isEnabled;
    }

    void process (float* inputBuffer, int blockSize, float* delay)
    {
        delayTimeInSamples = delayInSecs * sampleRate;

        if (delayTimeInSamples > maxDelayInSamples)
            delayTimeInSamples = (float)maxDelayInSamples;

        if (shouldClearDelay)
        {
            zeromem (delay, sizeof (float) * maxDelayInSamples);
            shouldClearDelay = false;
        }

        for (int i = 0; i < blockSize; i++)
        {
            readIndex = writeIndex - delayTimeInSamples;
            readIndex = (readIndex >= 0 ?
                         (readIndex < maxDelayInSamples ?
                            readIndex : readIndex - maxDelayInSamples) :
                            readIndex + maxDelayInSamples);

            readIndexFraction = readIndex - (int)readIndex;

            assert (readIndex >= 0);
            assert (readIndex < maxDelayInSamples);
            assert (writeIndex >= 0);
            assert (writeIndex < maxDelayInSamples);

            const float interpSample = delay[(int)readIndex] + 
                                        readIndexFraction * (delay[(int)readIndex + 1] - 
                                        delay[(int)readIndex]);

            float writeSample = inputBuffer[i] + (interpSample * feedback);

            // Clamp the write sample to +- 1.f
            if (writeSample > 1.f) writeSample = 1.f;
            if (writeSample < -1.f) writeSample = -1.f;

            delay[writeIndex] = writeSample;
            inputBuffer[i] = interpSample * volume * pan;
            writeIndex = (writeIndex != maxDelayInSamples - 1 ? 
                            writeIndex + 1 : 0);
        }
    }

private:
    float delayInSecs {0.f}, feedback {0.f}, pan {0.f}, volume {0.f}, 
          modSpeed {0.f}, modAmount {0.f}, readIndex {0.f}, delayTimeInSamples {0.f}, 
          readIndexFraction {0.f}, sampleRate {0.f};
    int maxDelayInSamples {0}, writeIndex {0};
    bool isEnabled {false}, shouldClearDelay {false};
};

struct StereoVarDelayLine
{
public:
    StereoVarDelayLine ()
    {}

    ~StereoVarDelayLine ()
    {
        if (delayHistory == nullptr)
            return;

        delete[] delayHistory[0];
        delete[] delayHistory[1];
        delete[] delayHistory;
    }

    void updateParameters (float volume, float pan, bool isEnabled, float delayAmount,
                           float feedBack, float modAmount, float modFrequency)
    {
        const auto leftPan = sinf ((1.0f - pan) * DSP::PI_HALFf);
        const auto rightPan = sinf (pan * DSP::PI_HALFf);

        delayLine[0].updateParameters (delayAmount, feedBack, leftPan, volume, 
                                       modFrequency, modAmount, isEnabled);

        delayLine[1].updateParameters (delayAmount, feedBack, rightPan, volume, 
                                       modFrequency, modAmount, isEnabled);
    }

    void initialise (float sampleRate, int maxDelayInSamples)
    {
        delayLine[0].initialise (sampleRate, maxDelayInSamples);
        delayLine[1].initialise (sampleRate, maxDelayInSamples);

        delayHistory = new float*[2];
        delayHistory[0] = new float[maxDelayInSamples];
        delayHistory[1] = new float[maxDelayInSamples];

        zeromem (delayHistory[0], maxDelayInSamples * sizeof (float));
        zeromem (delayHistory[1], maxDelayInSamples * sizeof (float));
    }

    void process (float** buffers, int blockSize)
    {
        auto bufferL = buffers[0];
        auto bufferR = buffers[1];

        delayLine[0].process (bufferL, blockSize, delayHistory[0]);
        delayLine[1].process (bufferR, blockSize, delayHistory[1]);
    }

private:
    float** delayHistory = nullptr;
    VarDelayLine delayLine[2];
};

#endif
