#include "PluginProcessor.h"
#include "PluginEditor.h"

TempusAudioProcessorEditor::TempusAudioProcessorEditor (TempusAudioProcessor* ownerFilter)
    :	AudioProcessorEditor (ownerFilter),
		processor (ownerFilter)
{
    setSize (600, 480);
}

TempusAudioProcessorEditor::~TempusAudioProcessorEditor()
{
}

void TempusAudioProcessorEditor::paint (Graphics& g)
{
    g.setColour (Colours::white);
    g.fillRect (0, 0, getWidth (), 35);
}
