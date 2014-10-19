#include "PluginProcessor.h"
#include "PluginEditor.h"

TempusAudioProcessorEditor::TempusAudioProcessorEditor (TempusAudioProcessor* ownerFilter)
    :	AudioProcessorEditor (ownerFilter),
		processor (ownerFilter)
{
    setSize (400, 300);
}

TempusAudioProcessorEditor::~TempusAudioProcessorEditor()
{
}

void TempusAudioProcessorEditor::paint (Graphics& /*g*/)
{
}
