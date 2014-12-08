#ifndef DSPDELAY_H_INCLUDED
#define DSPDELAY_H_INCLUDED

// Needs to be defined for MSVC otherwise M_PI is undefined
#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>

#include "DSP.h"

struct VarDelayLine
{
    void initialise (float sampleRate, int maxDelayInSamples)
    {
        this->sampleRate = sampleRate;
        this->maxDelayInSamples = maxDelayInSamples;

        delayLineFilter.reset ();
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
        if (shouldClearDelay)
        {
            zeromem (delay, sizeof (float) * maxDelayInSamples);
            shouldClearDelay = false;
        }

        for (int i = 0; i < blockSize; ++i)
        {
            // Smooth incoming delay time, avoid big jumps which might glitch   
            delayTimeInSamples =
                delayLineFilter.process (delayInSecs) * sampleRate;

            // Check delay time is within range
            assert (delayTimeInSamples >= 0.f);
            assert (delayTimeInSamples < maxDelayInSamples);

            if (delayTimeInSamples > maxDelayInSamples)
                delayTimeInSamples = (float)maxDelayInSamples;

            readIndex = validateReadPosition (writeIndex - delayTimeInSamples);
            readIndexFraction = readIndex - (long)readIndex;

            // Ensure we are not reading or writing from/to the buffer outside of the valid range
            assert (readIndex >= 0);
            assert (readIndex < maxDelayInSamples);
            assert (writeIndex >= 0);
            assert (writeIndex < maxDelayInSamples);

            // Interpolate between adjacent samples if reading pointer is fractional
            const int readPositionIndex = (int)readIndex;
            const float interpSample = delay[readPositionIndex] +
                                        (readIndexFraction * (delay[readPositionIndex + 1] -
                                        delay[readPositionIndex]));

            // writeSample will be written into the delay buffer
            float writeSample = inputBuffer[i] + (interpSample * feedback);

            // Clamp the write sample to +- 1.f
            if (writeSample > 1.f) writeSample = 1.f;
            else if (writeSample < -1.f) writeSample = -1.f;
            delay[writeIndex] = writeSample;

            // Output the delayed sample generated
            inputBuffer[i] = interpSample * volume * pan;

            // Validate the writeIndex and ensure it stays in range
            writeIndex = (writeIndex != maxDelayInSamples - 1 ? 
                            writeIndex + 1 : 0);
        }
    }

private:
    // Ensure the read position is above 0 and less than the maximum delay amount
    inline float validateReadPosition (float readPosition)
    {
        if (readPosition >= 0.f)
        {
            if (readPosition < maxDelayInSamples)
                return readPosition;
            
            // Read position has overshot, wrap back around
            return readPosition - maxDelayInSamples;
        }

        // Read position has undershot, wrap forward
        return readPosition + maxDelayInSamples;
    }

private:
    DSP::OnePole delayLineFilter {0.0003f};
    float delayInSecs {0.f}, feedback {0.f}, pan {0.f}, volume {0.f}, 
          modSpeed {0.f}, modAmount {0.f}, readIndex {0.f}, delayTimeInSamples {0.f}, 
          readIndexFraction {0.f}, sampleRate {0.f};
    long maxDelayInSamples {0}, writeIndex {0};
    bool isEnabled {false}, shouldClearDelay {false};
};

struct StereoVarDelayLine
{
public:
    StereoVarDelayLine () = default;

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
        const auto leftPan = sinf ((1.0f - pan) * M_PI_2);
        const auto rightPan = sinf (pan * M_PI_2);

        delayLine[0].updateParameters (delayAmount, feedBack, leftPan, volume, 
                                       modFrequency, modAmount, isEnabled);

        delayLine[1].updateParameters (delayAmount, feedBack, rightPan, volume, 
                                       modFrequency, modAmount, isEnabled);
    }

    void initialise (float sampleRate, int maxDelayInSamples)
    {
        this->maxDelayInSamples = maxDelayInSamples;

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

        assert (blockSize <= maxDelayInSamples);

        delayLine[0].process (bufferL, blockSize, delayHistory[0]);
        delayLine[1].process (bufferR, blockSize, delayHistory[1]);
    }

private:
    float** delayHistory = nullptr;
    VarDelayLine delayLine[2];
    int maxDelayInSamples {0};
};

#endif
