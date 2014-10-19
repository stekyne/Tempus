#ifndef TEMPUS_PARAMETERS_H_INCLUDED
#define TEMPUS_PARAMETERS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

// TODO convert to decibels instead of normalized
struct MainVolumeParam : public AudioProcessorParameter
{
    virtual float getValue () const { return value; }
    virtual void setValue (float newValue) { value = newValue; }
    virtual float getDefaultValue () const { return 1.f; }
    virtual String getName (int /*maximumStringLength*/) const { return "Main Volume"; }
    virtual String getLabel () const { return "dBs"; }
    virtual String getText (float value, int /*maximumStringLength*/) const
    {
        return String::formatted ("%.2f dBs", value);
    }
    virtual float getValueForText (const String& text) const
    {
        const auto index = text.indexOf (0, "dBs");
        const auto subString = text.substring (index);
        return subString.getFloatValue ();
    }

private:
    float value {1.f};
};

struct DryVolumeParam : public AudioProcessorParameter
{
    virtual float getValue () const { return value; }
    virtual void setValue (float newValue) { value = newValue; }
    virtual float getDefaultValue () const { return 1.f; }
    virtual String getName (int /*maximumStringLength*/) const { return "Dry Volume"; }
    virtual String getLabel () const { return "dBs"; }
    virtual String getText (float value, int /*maximumStringLength*/) const
    {
        return String::formatted ("%.2f dBs", value);
    }
    virtual float getValueForText (const String& text) const
    {
        const auto index = text.indexOf (0, "dBs");
        const auto subString = text.substring (index);
        return subString.getFloatValue ();
    }

private:
    float value {1.f};
};

struct WetVolumeParam : public AudioProcessorParameter
{
    virtual float getValue () const { return value; }
    virtual void setValue (float newValue) { value = newValue; }
    virtual float getDefaultValue () const { return 1.f; }
    virtual String getName (int /*maximumStringLength*/) const { return "Wet Volume"; }
    virtual String getLabel () const { return "dBs"; }
    virtual String getText (float value, int /*maximumStringLength*/) const
    {
        return String::formatted ("%.2f dBs", value);
    }
    virtual float getValueForText (const String& text) const
    {
        const auto index = text.indexOf (0, "dBs");
        const auto subString = text.substring (index);
        return subString.getFloatValue ();
    }

private:
    float value {1.f};
};

struct DelayEnabledParam : public AudioProcessorParameter
{
    DelayEnabledParam () : delayNum (++instanceCount) {}
    virtual float getValue () const { return isEnabled; }
    virtual void setValue (float newValue)
    {
        if (newValue > 0.5f) isEnabled = true;
        else isEnabled = false;
    }
    virtual float getDefaultValue () const { return false; }
    virtual String getName (int maximumStringLength) const
    {
        return String::formatted ("Delay#%d Enabled", delayNum);
    }
    virtual String getLabel () const { return String::empty; }
    virtual String getText (float value, int /*maximumStringLength*/) const
    {
        return isEnabled ? String::formatted ("Delay #%d Enabled", delayNum)
            : String::formatted ("Delay#%d Disabled", delayNum);
    }
    virtual float getValueForText (const String& text) const
    {
        if (text.contains ("Enabled"))
            return 1.f;
        return 0.f;
    }

private:
    bool isEnabled {false};
    int delayNum {1};
    static int instanceCount;
};

struct DelayVolumeParam : public AudioProcessorParameter
{
    DelayVolumeParam () : delayNum (++instanceCount) {}
    virtual float getValue () const { return value; }
    virtual void setValue (float newValue) { value = newValue; }
    virtual float getDefaultValue () const { return 1.f; }
    virtual String getName (int /*maximumStringLength*/) const { 
        return String::formatted ("Delay#%d Volume", delayNum); 
    }
    virtual String getLabel () const { return "dBs"; }
    virtual String getText (float value, int /*maximumStringLength*/) const
    {
        return String::formatted ("%.2f dBs", value);
    }
    virtual float getValueForText (const String& text) const
    {
        const auto index = text.indexOf (0, "dBs");
        const auto subString = text.substring (index);
        return subString.getFloatValue ();
    }

private:
    float value {1.f};
    int delayNum {1};
    static int instanceCount;
};

struct DelayTimeAmountParam : public AudioProcessorParameter
{
    DelayTimeAmountParam () : delayNum (++instanceCount) {}
    virtual float getValue () const { return value; }
    virtual void setValue (float newValue) { value = newValue; }
    virtual float getDefaultValue () const { return 0.2f; }
    virtual String getName (int maximumStringLength) const { 
        return String::formatted ("Delay#%d Amount (s)", delayNum); 
    }
    virtual String getLabel () const { return "ms"; }
    virtual String getText (float value, int /*maximumStringLength*/) const
    {
        return String::formatted ("%.2f ms", value);
    }
    virtual float getValueForText (const String& text) const
    {
        const auto index = text.indexOf (0, "ms");
        const auto subString = text.substring (index);
        return subString.getFloatValue ();
    }

private:
    float value {.2f};
    int delayNum {1};
    static int instanceCount;
};

struct DelayPanParam : public AudioProcessorParameter
{
    DelayPanParam () : delayNum (++instanceCount) {}
    virtual float getValue () const { return value; }
    virtual void setValue (float newValue) { newValue = value; }
    virtual float getDefaultValue () const { return 0.5f; }
    virtual String getName (int maximumStringLength) const {
        return String::formatted ("Delay#%d Pan", delayNum);
    }
    virtual String getLabel () const { return String::empty; }
    virtual String getText (float value, int /*maximumStringLength*/) const
    {
        return String::formatted ("%.2f", value);
    }
    virtual float getValueForText (const String& text) const
    {
        return text.getFloatValue ();
    }

private:
    float value {.5f};
    int delayNum {1};
    static int instanceCount;
};

struct DelayFeedbackParam : public AudioProcessorParameter
{
    DelayFeedbackParam () : delayNum (++instanceCount) {}
    virtual float getValue () const { return value; }
    virtual void setValue (float newValue) { value = newValue; }
    virtual float getDefaultValue () const { return 0.f; }
    virtual String getName (int maximumStringLength) const {
        return String::formatted ("Delay#%d Feedback", delayNum);
    }
    virtual String getLabel () const { return "%%"; }
    virtual String getText (float value, int /*maximumStringLength*/) const
    {
        return String::formatted ("%.2f %%", value);
    }
    virtual float getValueForText (const String& text) const
    {
        const auto index = text.indexOf (0, "%%");
        const auto subString = text.substring (index);
        return subString.getFloatValue ();
    }

private:
    float value {0.f};
    int delayNum {1};
    static int instanceCount;
};

struct DelayModSpeedParam : public AudioProcessorParameter
{
    DelayModSpeedParam () : delayNum (++instanceCount) {}
    virtual float getValue () const { return value; }
    virtual void setValue (float newValue) { value = newValue; }
    virtual float getDefaultValue () const { return 2.f; }
    virtual String getName (int maximumStringLength) const {
        return String::formatted ("Delay#%d Mod. Speed", delayNum);
    }
    virtual String getLabel () const { return "Hz"; }
    virtual String getText (float value, int /*maximumStringLength*/) const
    {
        return String::formatted ("%.2f Hz", value);
    }
    virtual float getValueForText (const String& text) const
    {
        const auto index = text.indexOf (0, "Hz");
        const auto subString = text.substring (index);
        return subString.getFloatValue ();
    }

private:
    float value {2.f};
    int delayNum {1};
    static int instanceCount;
};

struct DelayModAmountParam : public AudioProcessorParameter
{
    DelayModAmountParam () : delayNum (++instanceCount) {}
    virtual float getValue () const { return value; }
    virtual void setValue (float newValue) { newValue = value; }
    virtual float getDefaultValue () const { return 0.f; }
    virtual String getName (int maximumStringLength) const
    {
        return String::formatted ("Delay#%d Mod. Amount", delayNum);
    }
    virtual String getLabel () const { return String::empty; }
    virtual String getText (float value, int /*maximumStringLength*/) const
    {
        return String::formatted ("%.2f", value);
    }
    virtual float getValueForText (const String& text) const
    {
        return text.getFloatValue ();
    }
    
private:
    float value {0.f};
    int delayNum {1};
    static int instanceCount;
};

#endif