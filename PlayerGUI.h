#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    PlayerAudio playerAudio;


    // GUI Elements
    juce::TextButton loadButton{ "Load File" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton goToEnd{ "Go to End" };
    juce::TextButton goToStart{ "Go to Start" };
    juce::TextButton loopButton{ "Single" };
    juce::TextButton muteButton{ "Mute" };

    bool isMuted = false;
    float previousVolume = 0.5f;

    juce::Slider volumeSlider;

    std::unique_ptr<juce::FileChooser> fileChooser;

    juce::Label metadataLabel;
    // Event handlers
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
