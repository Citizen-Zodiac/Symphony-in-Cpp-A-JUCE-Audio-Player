#pragma once
#include <JuceHeader.h>

class PlayerAudio : public juce::AudioSource
{
public:
    PlayerAudio();
    ~PlayerAudio() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    bool loadFile(const juce::File& file);
    void restart();
    void stop();

    void setLooping(bool shouldloop);
    void play(bool ch);

    void goToStart();
    void goToEnd();

    void setGain(float gain);
    void setPosition(double position);
    double getPosition() const;
    double getLength() const;

    juce::String getTitle() const { return title; }
    juce::String getArtist() const { return artist; }
    double getDuration() const { return duration; }

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::String title;
    juce::String artist;
    double duration = 0.0;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
