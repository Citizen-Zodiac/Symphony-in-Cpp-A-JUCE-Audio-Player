#pragma once

#include <JuceHeader.h>
#include"PlayerGUI.h"

class MainComponent::Public juce::AudioAppComponent
{
public:
	MainComponent();
	~MainComponent();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

	void resized() override;
Private:
	PlayerGUI player1;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};