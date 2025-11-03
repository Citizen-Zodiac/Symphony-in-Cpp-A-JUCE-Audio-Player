#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    setSize(800, 200);
    // Add buttons
    for (auto* btn : { &loadButton, &restartButton , &stopButton, &playButton, &goToEnd, &goToStart,
        &muteButton, &loopButton })
    {
        btn->addListener(this);
        addAndMakeVisible(*btn);
        addAndMakeVisible(metadataLabel);
    }

    // Volume slider setup
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    // POSITION SLIDER SETUP
    positionSlider.setRange(0.0, 1.0, 0.001);
    positionSlider.setValue(0.0);
    positionSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);

    // Start timer for position updates (every 30ms)
    startTimer(30);

    // Label style
    metadataLabel.setJustificationType(juce::Justification::centred);
    metadataLabel.setText("No file loaded", juce::dontSendNotification);
    metadataLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    metadataLabel.setFont(juce::Font(16.0f, juce::Font::plain));

    // background color
    setOpaque(true);

    // Position Time labels setup
    currentTimeLabel.setText("0:00", juce::dontSendNotification);
    currentTimeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(currentTimeLabel);

    totalTimeLabel.setText("0:00", juce::dontSendNotification);
    totalTimeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(totalTimeLabel);

    // Speed Slider setup 
    addAndMakeVisible(speedSlider);
    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    speedSlider.addListener(this);

    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(speedLabel);

}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}


void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void PlayerGUI::resized()
{
    int y = 20;
    int buttonWidth = 80;
    int buttonHeight = 40;
    int sliderHeight = 25;

    loadButton.setBounds(20, y, 100, buttonHeight);
    playButton.setBounds(130, y, buttonWidth, buttonHeight);
    stopButton.setBounds(220, y, buttonWidth, buttonHeight);
    restartButton.setBounds(310, y, buttonWidth, buttonHeight);
    goToStart.setBounds(400, y, 100, buttonHeight);
    goToEnd.setBounds(510, y, 100, buttonHeight);
    loopButton.setBounds(620, y, buttonWidth, buttonHeight);
    muteButton.setBounds(710, y, 70, buttonHeight);
    metadataLabel.setBounds(20, 70, getWidth() - 40, 20);
    volumeSlider.setBounds(100, 100, getWidth() - 120, sliderHeight);
    speedLabel.setBounds(0, 130, getWidth(), 20);
    speedSlider.setBounds(getWidth() / 2 - 150, 155, 300, 20);


    int sliderY = 170;
    currentTimeLabel.setBounds(10, sliderY, 50, 25);
    positionSlider.setBounds(70, sliderY, getWidth() - 140, 25);
    totalTimeLabel.setBounds(getWidth() - 60, sliderY, 50, 25);

}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    static bool isPlaying = false;
    static bool isLooping = false;

    if (button == &loadButton)
    {
        juce::FileChooser chooser("Select audio files...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    playerAudio.loadFile(file);

                    metadataLabel.setText("Title: " + playerAudio.getTitle() +
                        " | Artist: " + playerAudio.getArtist() +
                        " | Duration: " + juce::String(playerAudio.getDuration() / 60.0, 2) + "min",
                        juce::dontSendNotification);

                }
                else
                {
                    metadataLabel.setText("No file loaded", juce::dontSendNotification);
                }
            });

    }

    else if (button == &restartButton)
    {
        // Restart playback
        playerAudio.restart();
        playButton.setButtonText("Pause");
        isPlaying = true;

    }

    else if (button == &stopButton)
    {
        // Stop playback and reset position
        playerAudio.stop();
        playerAudio.setPosition(0.0);
        playButton.setButtonText("Play");
        isPlaying = false;
    }
    else if (button == &playButton)
    {
        // Start & Pause playback
        isPlaying = !isPlaying;
        playerAudio.play(isPlaying);

        // Change button text
        playButton.setButtonText(isPlaying ? "Pause" : "Play");
    }
    else if (button == &goToEnd)
    {
        // Go to end of the track
        playerAudio.goToEnd();
        if (isPlaying)
        {
            playerAudio.stop();
            isPlaying = false;
            playButton.setButtonText("Play");
        }
        if (isLooping)
        {
            playerAudio.setPosition(0.0);
            playerAudio.play(true);
            isPlaying = true;
            playButton.setButtonText("Pause");
        }

    }
    else if (button == &goToStart)
    {
        // Go to start of the track
        playerAudio.goToStart();
    }

    else if (button == &loopButton)
    {
        // Toggle looping
        isLooping = !isLooping;
        playerAudio.setLooping(isLooping);

        // Change button text to show state
        loopButton.setButtonText(isLooping ? "Loop" : "Single");
    }

    else if (button == &muteButton)
    {
        isMuted = !isMuted;

        if (isMuted)
        {
            previousVolume = (float)volumeSlider.getValue();
            playerAudio.setGain(0.0f);
            muteButton.setButtonText("Unmute");
        }
        else
        {
            playerAudio.setGain(previousVolume);
            muteButton.setButtonText("Mute");
        }
    }


}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        float newVolume = (float)slider->getValue();

        if (isMuted && newVolume > 0.0f)
        {
            isMuted = false;
            muteButton.setButtonText("Mute");
        }

        playerAudio.setGain(newVolume);
        previousVolume = newVolume;
    }

    else if (slider == &positionSlider)
    {
        double length = playerAudio.getLength();
        double newPosition = positionSlider.getValue() * length;
        playerAudio.setPosition(newPosition);
    }
    if (slider == &speedSlider)
    {
        playerAudio.setPlaybackSpeed(speedSlider.getValue());
    }
}
juce::String PlayerGUI::formatTime(double seconds)
{
    if (seconds < 0) seconds = 0;

    int minutes = (int)seconds / 60;
    int secs = (int)seconds % 60;

    return juce::String::formatted("%d:%02d", minutes, secs);
}

void PlayerGUI::timerCallback()
{
    double length = playerAudio.getLength();
    double currentPos = playerAudio.getPosition();

    if (length > 0.0 && currentPos > 0.0)
    {
        positionSlider.setValue(currentPos / length, juce::dontSendNotification);
        currentTimeLabel.setText(formatTime(currentPos), juce::dontSendNotification);
        totalTimeLabel.setText(formatTime(length), juce::dontSendNotification);
    }
}