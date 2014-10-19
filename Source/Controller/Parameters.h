#ifndef TEMPUS_PARAMETERS_H_INCLUDED
#define TEMPUS_PARAMETERS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

enum Parameters
{
    Delay_1_Enabled,
    Delay_1_Volume,
    Delay_1_Delay,
    Delay_1_Pan,
    Delay_1_Feedback,
    Delay_1_ModSpeed,
    Delay_1_ModAmount,

    Delay_2_Enabled,
    Delay_2_Volume,
    Delay_2_Delay,
    Delay_2_Pan,
    Delay_2_Feedback,
    Delay_2_ModSpeed,
    Delay_2_ModAmount,

    Delay_3_Enabled,
    Delay_3_Volume,
    Delay_3_Delay,
    Delay_3_Pan,
    Delay_3_Feedback,
    Delay_3_ModSpeed,
    Delay_3_ModAmount,

    Delay_4_Enabled,
    Delay_4_Volume,
    Delay_4_Delay,
    Delay_4_Pan,
    Delay_4_Feedback,
    Delay_4_ModSpeed,
    Delay_4_ModAmount,

    Delay_5_Enabled,
    Delay_5_Volume,
    Delay_5_Delay,
    Delay_5_Pan,
    Delay_5_Feedback,
    Delay_5_ModSpeed,
    Delay_5_ModAmount,

    Delay_6_Enabled,
    Delay_6_Volume,
    Delay_6_Delay,
    Delay_6_Pan,
    Delay_6_Feedback,
    Delay_6_ModSpeed,
    Delay_6_ModAmount,

    Delay_7_Enabled,
    Delay_7_Volume,
    Delay_7_Delay,
    Delay_7_Pan,
    Delay_7_Feedback,
    Delay_7_ModSpeed,
    Delay_7_ModAmount,

    Delay_8_Enabled,
    Delay_8_Volume,
    Delay_8_Delay,
    Delay_8_Pan,
    Delay_8_Feedback,
    Delay_8_ModSpeed,
    Delay_8_ModAmount,

    MasterVolume,
    DryMix,
    WetMix,

    NumParameters,
    Empty
};

// Used for printing the parameter value
enum ParameterType
{
    Bool,           // Switch, on/off
    Hertz,          // Parameter represents frequency
    Normalized,     // 0.0 - 1.0 value
    MilliSeconds    // float number that represents time in ms
};

struct Parameter
{
    Parameter (Parameters param, float defaultValue, String _name, 
               ParameterType _type) :
        id (param),
        value (defaultValue),
        name (_name),
        type (_type)
    {}

    inline int getID () const
    {
        return id;
    }

    inline float getValue ()
    {
        hasChanged = false;
        return value;
    }

    inline void setValue (float newValue)
    {
        value = newValue;
        hasChanged = true;
    }

    inline String getName () const
    {
        return name;
    }

    String getValueAsString () const
    {
        switch (type)
        {
        case ParameterType::Bool:
            return value == 0.0 ? "On" : "Off";

        case ParameterType::Hertz:
            return String::formatted ("%.2f Hz", value);

        case ParameterType::Normalized:
            return String::formatted ("%.2f", value);

        case ParameterType::MilliSeconds:
            return String::formatted ("%.2f ms", value);

        default: 
            return String (value);
        }
    }

private:
    String name = String::empty;
    float value = 0.f;
    ParameterType type;
    Parameters id;
    bool hasChanged = false;
};

#endif