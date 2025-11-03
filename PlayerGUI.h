#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"
#include<string>
#include <vector>

struct TrackMetadata
{
    juce::String title;
    juce::String artist;
    double duration = 0;
    juce::File file;
};

class PlayerGUI : 
    public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::ListBoxModel
{

public:
    PlayerGUI();
    ~PlayerGUI() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    void paint(juce::Graphics& g) override;
    void resized() override;

   
    int PlayerGUI::getNumRows() override;
    void PlayerGUI::paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool selected) override;
    //void PlayerGUI::listBoxItemClicked(int row, const juce::MouseEvent&) override;

    void PlayerGUI::mouseDown(const juce::MouseEvent& event) override;
private:
    PlayerAudio playerAudio;

    juce::ListBox trackList;
    std::vector<TrackMetadata> playlist;
    int currentIndex = 0;

    // GUI Elements
    juce::TextButton loadButton{ "Load File" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton goToEnd{ "Go to End" };
    juce::TextButton goToStart{ "Go to Start" };
    juce::TextButton loopButton{ "Single" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton addButton{ "Add" };
    juce::TextButton nextButton{ "Next" };
    juce::TextButton prevButoon{ "Prev" };

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
