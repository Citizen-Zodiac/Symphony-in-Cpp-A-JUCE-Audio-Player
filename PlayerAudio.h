#pragma once
#include<JuceHeader.h>

class PlayerAudio
{

Public:
	PlayerAudio();
	~PlayerAudio();
	
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;
	
	bool loadFile(const juce::File& file);
	void play();
	void stop();
	void setGain(float gain);
	void setPosition(double position);
	double getPosition() const;
	double getLength() const;




Private:
	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};