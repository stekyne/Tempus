#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include <mutex>
#include "../JuceLibraryCode/JuceHeader.h"
#include "DSP\Delay.h"
#include "Controller\Parameters.h"

class TempusAudioProcessor  : public AudioProcessor
{
public:
    TempusAudioProcessor();
    ~TempusAudioProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    int getNumParameters () override;
    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;
    const String getParameterName (int index) override;
    const String getParameterText (int index) override;

    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    const float MAX_DELAY_IN_SECONDS = 10.f;
    static const int MAX_NUM_TAPS = 1;
    
    //std::mutex paramLock;
    StereoVarDelayLine delayLine[MAX_NUM_TAPS];

    MainVolumeParam* masterVolume;
    DryVolumeParam* dryVolume;
    WetVolumeParam* wetVolume;

    DelayEnabledParam* delayEnabled[MAX_NUM_TAPS];
    DelayVolumeParam* delayVolume[MAX_NUM_TAPS];
    DelayTimeAmountParam* delayAmount[MAX_NUM_TAPS];
    DelayPanParam* delayPan[MAX_NUM_TAPS];
    DelayFeedbackParam* delayFeedback[MAX_NUM_TAPS];
    DelayModSpeedParam* delayModSpeed[MAX_NUM_TAPS];
    DelayModAmountParam* delayModAmount[MAX_NUM_TAPS];
};

#endif
