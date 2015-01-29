#ifndef DSPDELAY_H_INCLUDED
#define DSPDELAY_H_INCLUDED

// Needs to be defined for MSVC otherwise M_PI is undefined
#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>
#include <cassert>

#include "DSP.h"
#include "../Controller/Parameters.h"

template <typename PanType>
struct VarDelayLine
{
    void initialise (float sampleRate, int maxDelayInSamples)
    {
        this->sampleRate = sampleRate;
        this->maxDelayInSamples = maxDelayInSamples;

        delayLineFilter.reset ();

        assert (enabledParam != nullptr &&
                volumeParam != nullptr &&
                delayTimeParam != nullptr &&
                panParam != nullptr &&
                feedbackParam != nullptr &&
                modSpeedParam != nullptr &&
                modAmountParam != nullptr);
    }

    void process (float* inputBuffer, int blockSize, float* delay)
    {
        // If the delay line was previously active but now disabled, clear the delay history
        if (enabledParam->getValue () < 0.5f && isEnabled == true)
        {
            zeromem (delay, sizeof (float) * maxDelayInSamples);
        }

        // Processor is disabled so return without further processing
        isEnabled = enabledParam->getValue () > 0.5f;
        if (isEnabled == false)
            return;

        for (int i = 0; i < blockSize; ++i)
        {
            auto delayTime = delayTimeParam->getValue ();

            // Smooth incoming delay time, avoid big jumps which might glitch the sound
            delayTimeInSamples = (float)(delayLineFilter.process (delayTime) * sampleRate);

            // Check delay time is within range
            assert (delayTimeInSamples >= 0.f);
            assert (delayTimeInSamples < maxDelayInSamples);

            if (delayTimeInSamples > maxDelayInSamples)
                delayTimeInSamples = (float)maxDelayInSamples;

            readIndex = validateReadPosition (writeIndex - delayTimeInSamples);
            readIndexFraction = readIndex - (long)readIndex;

            // Ensure we are not reading from or writing to the buffer outside of the valid range
            assert (readIndex >= 0);
            assert (readIndex < maxDelayInSamples);
            assert (writeIndex >= 0);
            assert (writeIndex < maxDelayInSamples);

            // Interpolate between adjacent samples if reading pointer is fractional
            const int readPositionIndex = (int)readIndex;
            const float interpSample = delay[readPositionIndex] +
                                        (readIndexFraction * (delay[readPositionIndex + 1] -
                                        delay[readPositionIndex]));

            // 'writeSample' is the current input plus delay feedback and will be written into the delay buffer
            // TODO should convert to soft clipper instead of hard
            float writeSample = inputBuffer[i] + (interpSample * feedbackParam->getValue ());
            if (writeSample > 1.f) writeSample = 1.f;
            else if (writeSample < -1.f) writeSample = -1.f;
            
            // Write the sample back into the delay and output the delayed sample generated
            delay[writeIndex] = writeSample;
            inputBuffer[i] = interpSample * volumeParam->getValue () * 
                             PanType::process (panParam->getValue ());

            // Validate the writeIndex and ensure it stays in range
            writeIndex = writeIndex != maxDelayInSamples - 1 ? writeIndex + 1 : 0;
        }
    }

    void setExternalParameters (DelayEnabledParam* enabledParam,
                                DelayVolumeParam* volumeParam,
                                DelayTimeAmountParam* delayTimeParam,
                                DelayPanParam* panParam,
                                DelayFeedbackParam* feedbackParam,
                                DelayModSpeedParam* modSpeedParam,
                                DelayModAmountParam* modAmountParam)
    {
        this->enabledParam = enabledParam;
        this->volumeParam = volumeParam;
        this->delayTimeParam = delayTimeParam;
        this->panParam = panParam;
        this->feedbackParam = feedbackParam;
        this->modSpeedParam = modSpeedParam;
        this->modAmountParam = modAmountParam;
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
    DSP::OnePole delayLineFilter {0.003f};
    DelayEnabledParam* enabledParam;
    DelayVolumeParam* volumeParam;
    DelayTimeAmountParam* delayTimeParam;
    DelayPanParam* panParam;
    DelayFeedbackParam* feedbackParam;
    DelayModSpeedParam* modSpeedParam;
    DelayModAmountParam* modAmountParam;

    float readIndex {0.f}, delayTimeInSamples {0.f}, readIndexFraction {0.f}, sampleRate {0.f};
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

    void initialise (float sampleRate, int maxDelayInSamples)
    {
        this->maxDelayInSamples = maxDelayInSamples;

        delayLeft.initialise (sampleRate, maxDelayInSamples);
        delayRight.initialise (sampleRate, maxDelayInSamples);

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

        delayLeft.process (bufferL, blockSize, delayHistory[0]);
        delayRight.process (bufferR, blockSize, delayHistory[1]);
    }

    void setExternalParameters (DelayEnabledParam* enabledParam,
                                DelayVolumeParam* volumeParam,
                                DelayTimeAmountParam* delayTimeParam,
                                DelayPanParam* panParam,
                                DelayFeedbackParam* feedbackParam,
                                DelayModSpeedParam* modSpeedParam,
                                DelayModAmountParam* modAmountParam)
    {
        delayLeft.setExternalParameters (enabledParam, volumeParam, delayTimeParam, panParam, 
                                         feedbackParam, modSpeedParam, modAmountParam);
        delayRight.setExternalParameters (enabledParam, volumeParam, delayTimeParam, panParam, 
                                          feedbackParam, modSpeedParam, modAmountParam);
    }

private:
    VarDelayLine<DSP::PanLeft> delayLeft;
    VarDelayLine<DSP::PanRight> delayRight;
    float** delayHistory = nullptr;
    int maxDelayInSamples {0};
};

#endif
