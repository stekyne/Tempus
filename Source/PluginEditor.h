#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class TempusAudioProcessor;

class TempusAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    TempusAudioProcessorEditor (TempusAudioProcessor* ownerFilter);
    ~TempusAudioProcessorEditor();

    void paint (Graphics& g);

private:
	TempusAudioProcessor* processor;
};

#endif
