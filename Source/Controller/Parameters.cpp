#ifndef TEMPUS_PARAMETERS_CPP_INCLUDED
#define TEMPUS_PARAMETERS_CPP_INCLUDED

#include <array>
#include "Parameters.h"

static std::array<Parameter, NumParameters> parameters
{{
    {Delay_1_Enabled,   1.f,    "D#1 Active",   ParameterType::Bool},
    {Delay_1_Volume,    0.7f,   "D#1 Volume",   ParameterType::Normalized},
    {Delay_1_Delay,     0.2f,   "D#1 Delay(s)", ParameterType::MilliSeconds},
    {Delay_1_Pan,       0.5f,   "D#1 Pan",      ParameterType::Normalized},
    {Delay_1_Feedback,  0.f,    "D#1 Feedback", ParameterType::Normalized},
    {Delay_1_ModSpeed,  1.f,    "D#1 Mod. Speed", ParameterType::Hertz},
    {Delay_1_ModAmount, 0.f,    "D#1 Mod Amount", ParameterType::Normalized},

    {Delay_2_Enabled,   0.f,    "D#2 Active",   ParameterType::Bool},
    {Delay_2_Volume,    0.7f,   "D#2 Volume",   ParameterType::Normalized},
    {Delay_2_Delay,     0.2f,   "D#2 Delay(s)", ParameterType::MilliSeconds},
    {Delay_2_Pan,       0.5f,   "D#2 Pan",      ParameterType::Normalized},
    {Delay_2_Feedback,  0.f,    "D#2 Feedback", ParameterType::Normalized},
    {Delay_2_ModSpeed,  1.f,    "D#2 Mod. Speed", ParameterType::Hertz},
    {Delay_2_ModAmount, 0.f,    "D#2 Mod Amount", ParameterType::Normalized},

    {Delay_3_Enabled,   1.f,    "D#3 Active",   ParameterType::Bool},
    {Delay_3_Volume,    0.7f,   "D#3 Volume",   ParameterType::Normalized},
    {Delay_3_Delay,     0.2f,   "D#3 Delay(s)", ParameterType::MilliSeconds},
    {Delay_3_Pan,       0.5f,   "D#3 Pan",      ParameterType::Normalized},
    {Delay_3_Feedback,  0.f,    "D#3 Feedback", ParameterType::Normalized},
    {Delay_3_ModSpeed,  1.f,    "D#3 Mod. Speed", ParameterType::Hertz},
    {Delay_3_ModAmount, 0.f,    "D#3 Mod Amount", ParameterType::Normalized},

    {Delay_4_Enabled,   0.f,    "D#4 Active",   ParameterType::Bool},
    {Delay_4_Volume,    0.7f,   "D#4 Volume",   ParameterType::Normalized},
    {Delay_4_Delay,     0.2f,   "D#4 Delay(s)", ParameterType::MilliSeconds},
    {Delay_4_Pan,       0.5f,   "D#4 Pan",      ParameterType::Normalized},
    {Delay_4_Feedback,  0.f,    "D#4 Feedback", ParameterType::Normalized},
    {Delay_4_ModSpeed,  1.f,    "D#4 Mod. Speed", ParameterType::Hertz},
    {Delay_4_ModAmount, 0.f,    "D#4 Mod Amount", ParameterType::Normalized},

    {Delay_5_Enabled,   0.f,    "D#5 Active",   ParameterType::Bool},
    {Delay_5_Volume,    0.7f,   "D#5 Volume",   ParameterType::Normalized},
    {Delay_5_Delay,     0.2f,   "D#5 Delay(s)", ParameterType::MilliSeconds},
    {Delay_5_Pan,       0.5f,   "D#5 Pan",      ParameterType::Normalized},
    {Delay_5_Feedback,  0.f,    "D#5 Feedback", ParameterType::Normalized},
    {Delay_5_ModSpeed,  1.f,    "D#5 Mod. Speed", ParameterType::Hertz},
    {Delay_5_ModAmount, 0.f,    "D#5 Mod Amount", ParameterType::Normalized},

    {Delay_6_Enabled,   0.f,    "D#6 Active",   ParameterType::Bool},
    {Delay_6_Volume,    0.7f,   "D#6 Volume",   ParameterType::Normalized},
    {Delay_6_Delay,     0.2f,   "D#6 Delay(s)", ParameterType::MilliSeconds},
    {Delay_6_Pan,       0.5f,   "D#6 Pan",      ParameterType::Normalized},
    {Delay_6_Feedback,  0.f,    "D#6 Feedback", ParameterType::Normalized},
    {Delay_6_ModSpeed,  1.f,    "D#6 Mod. Speed", ParameterType::Hertz},
    {Delay_6_ModAmount, 0.f,    "D#6 Mod Amount", ParameterType::Normalized},

    {Delay_7_Enabled,   0.f,    "D#7 Active",   ParameterType::Bool},
    {Delay_7_Volume,    0.7f,   "D#7 Volume",   ParameterType::Normalized},
    {Delay_7_Delay,     0.2f,   "D#7 Delay(s)", ParameterType::MilliSeconds},
    {Delay_7_Pan,       0.5f,   "D#7 Pan",      ParameterType::Normalized},
    {Delay_7_Feedback,  0.f,    "D#7 Feedback", ParameterType::Normalized},
    {Delay_7_ModSpeed,  1.f,    "D#7 Mod. Speed", ParameterType::Hertz},
    {Delay_7_ModAmount, 0.f,    "D#7 Mod Amount", ParameterType::Normalized},

    {Delay_8_Enabled,   0.f,    "D#8 Active",   ParameterType::Bool},
    {Delay_8_Volume,    0.7f,   "D#8 Volume",   ParameterType::Normalized},
    {Delay_8_Delay,     0.2f,   "D#8 Delay(s)", ParameterType::MilliSeconds},
    {Delay_8_Pan,       0.5f,   "D#8 Pan",      ParameterType::Normalized},
    {Delay_8_Feedback,  0.f,    "D#8 Feedback", ParameterType::Normalized},
    {Delay_8_ModSpeed,  1.f,    "D#8 Mod. Speed", ParameterType::Hertz},
    {Delay_8_ModAmount, 0.f,    "D#8 Mod Amount", ParameterType::Normalized},

    {MasterVolume, 1.f, "Master Volume", ParameterType::Normalized},
    {DryMix, 1.f, "Dry Mix", ParameterType::Normalized},
    {WetMix, 0.5f, "Wet Mix", ParameterType::Normalized},
}};

#endif