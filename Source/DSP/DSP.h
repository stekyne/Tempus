#ifndef DSPDSP_H_INCLUDED
#define DSPDSP_H_INCLUDED

#include <math.h>

namespace DSP
{

class OnePole
{
public:
    OnePole (double Fc) 
    { 
        z1 = 0.0; setFc (Fc); 
    }

    void setFc (double Fc)
    {
        b1 = exp (-2.0 * M_PI * Fc);
        a0 = 1.0 - b1;
    }

    double process (float in)
    {
        return z1 = in * a0 + z1 * b1;
    }

    void reset ()
    {
        a0 = 1.0; b1 = z1 = 0.0;
    }

private:
    double a0 {1.0}, b1 {0.0}, z1 {0.0};
};

}

#endif
