#ifndef DSPDELAY_H_INCLUDED
#define DSPDELAY_H_INCLUDED

#include "DSP.h"
#include <math.h>

struct VarDelayLine
{
    VarDelayLine () :
        maxDelayInSamples (0),
        writeIndex (0),
        interpSample (0.f),
        readIndex (0.f),
        delayTimeInSamples (0.f),
        readIndexFraction (0.f),
        sampleRate (0.f),
        delayInSecs (0.f), 
        feedback (0.f), 
        pan (0.f), 
        volume (0.f), 
        modSpeed (0.f), 
        modAmount (0.f),
        isEnabled (false)
    {}

    ~VarDelayLine () 
    {}

    void initialise (float sampleRate, int maxDelayInSamples)
    {
        this->sampleRate = sampleRate;
        this->maxDelayInSamples = maxDelayInSamples;
    }

    void process (float* inputBuffer, int blockSize, float* delay)
    {
        delayTimeInSamples = delayInSecs * sampleRate;

        if (delayTimeInSamples > maxDelayInSamples)
            delayTimeInSamples = (float)maxDelayInSamples;

        for (int i = 0; i < blockSize; i++)
        {
            readIndex = writeIndex - delayTimeInSamples;
            readIndex = (readIndex >= 0 ?
                         (readIndex < maxDelayInSamples ?
                            readIndex : readIndex - maxDelayInSamples) :
                            readIndex + maxDelayInSamples);

            readIndexFraction = readIndex - (int)readIndex;

            interpSample = delay[(int)readIndex] + 
                            readIndexFraction * (delay[(int)readIndex + 1] - 
                            delay[(int)readIndex]);

            delay[writeIndex] = inputBuffer[i] + (interpSample * feedback);
            inputBuffer[i] = interpSample;
            writeIndex = (writeIndex != maxDelayInSamples - 1 ? 
                            writeIndex + 1 : 0);
        }
    }

public:
    float delayInSecs, feedback, pan, volume, modSpeed, modAmount;
    bool isEnabled;

private:
    int maxDelayInSamples, writeIndex;
    float interpSample, readIndex, delayTimeInSamples, 
          readIndexFraction, sampleRate;
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

    void setVolume (float volume)
    {
        delayLine[0].volume = volume;
        delayLine[1].volume = volume;
    }

    float getVolume ()
    {
        return delayLine[0].volume;
    }

    void setPanning (float panning)
    {
        this->panning = panning;

        delayLine[0].pan = sinf ((1.0f - panning) * DSP::PI_HALFf);
        delayLine[1].pan = sinf (panning * DSP::PI_HALFf);
    }

    float getPanning ()
    {
        return panning;
    }

    bool isEnabled () const
    {
        return delayLine[0].isEnabled;
    }

    void initialise (float sampleRate, int maxDelayInSamples)
    {
        delayLine[0].initialise (sampleRate, maxDelayInSamples);
        delayLine[1].initialise (sampleRate, maxDelayInSamples);

        delayHistory = new float*[2];
        delayHistory[0] = new float[maxDelayInSamples];
        delayHistory[1] = new float[maxDelayInSamples];

        zeromem (delayHistory[0], maxDelayInSamples);
        zeromem (delayHistory[1], maxDelayInSamples);
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
    float panning = 0.5f;
};

#endif
