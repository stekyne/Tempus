#ifndef DSPLFO_H_INCLUDED
#define DSPLFO_H_INCLUDED

#include "DSP.h"

template <typename FunctionType, int TableLength>
class LFO
{
public:
    LFO () 
        :   lfoIndex (0.f),
            sampleRate (0.f)
    {
        LFO::initialise ();
    }

    ~LFO () {}

    void setup (float sampleRate)
    {
        this->sampleRate = sampleRate;
    }

    float nextSample (float freq)
    {
        if (sampleRate == 0.f) return 0.f;

        const float frac = lfoIndex - (int)lfoIndex;
        const float a = table[(int)lfoIndex];
        const float b = table[(int)lfoIndex + 1];

        const float incr = freq * TableLength / sampleRate;
        lfoIndex += incr;

        while (lfoIndex >= TableLength)
            lfoIndex -= TableLength;

        while (lfoIndex < 0)
            lfoIndex += TableLength;

        return a + frac * (b - a);
    }

private:
    // Set up the table data for this template type
    static void initialise ()
    {
        static bool isInitialised = false;

        if (isInitialised == false)
        {
            isInitialised = true;

            const double div = 1.f / (double)TableLength;
            const double eq = 2.0 * DSP::PI_DOUBLE * div;

            for (int n = 0; n < TableLength; n++)
            {
                table[n] = func (n * eq);
            }
        }
    }

    static float table[TableLength];
    static FunctionType func;
    float lfoIndex, sampleRate;
};

template<typename FunctionType, int Size>
float LFO<FunctionType, Size>::table[Size] = {0};

template<typename FunctionType, int Size>
FunctionType LFO<FunctionType, Size>::func;

struct SinLFO 
{
    float operator () (double phase)
    {
        return (float)abs (cos (phase));
    }
};

#endif
