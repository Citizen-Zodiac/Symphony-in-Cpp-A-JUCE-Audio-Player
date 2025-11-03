#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"
#include<string>
#include <vector>
struct TrackMetadata
{
    juce::String title;
    juce::String artist;
    double length = 0.0;
    juce::File file;
};

class PlayerGUI :
    public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer,
    public juce::TableListBoxModel,
    public juce::ChangeListener

{

public:
    PlayerGUI();
    ~PlayerGUI() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback()override;
    bool keyPressed(const juce::KeyPress& key) override;
   
    int PlayerGUI::getNumRows() override;
    void PlayerGUI::paintRowBackground( juce::Graphics& g,int row, int width, int height, bool selected) override;
    void PlayerGUI::paintCell(juce::Graphics& g, int row, int col, int width, int height, bool selected) override;
    void PlayerGUI::setCurrentlyPlayingRow(int row);

private:
    
    PlayerAudio playerAudio;
    juce::TableListBox trackList;
    std::vector<TrackMetadata> playlist;
    int currentIndex = 0;
    double loopPointA = -1.0;      
    double loopPointB = -1.0;       
    void updateABLoopPoints();


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
    juce::TextButton removePlButton{ "Remove PL" };
    juce::TextButton clearABButton{ "Clear A-B" };

    bool isMuted = false;
    float previousVolume = 0.5f;

    juce::Slider volumeSlider;
    juce::Slider positionSlider;
    juce::Label currentTimeLabel;
    juce::Label totalTimeLabel;
    //Time Helper Function
    juce::String formatTime(double seconds);  

    std::unique_ptr<juce::FileChooser> fileChooser;

    juce::Label metadataLabel;
    // Event handlers
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    juce::Slider speedSlider;
    juce::Label speedLabel;


    juce::AudioThumbnailCache thumbnailCache{ 10 };
    juce::AudioThumbnail thumbnail{ 1000, playerAudio.getFormatManager(), thumbnailCache };

    bool fileLoaded = false;
    double waveformPosition = 0.0;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    int waveformTop = 0;
    int waveformHeight = 0;
    int waveformX = 0;
    int waveformWidth = 0;





    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
