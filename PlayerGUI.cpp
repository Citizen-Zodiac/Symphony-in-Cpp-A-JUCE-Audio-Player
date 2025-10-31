#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
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

    // Label style
    metadataLabel.setJustificationType(juce::Justification::centred);
    metadataLabel.setText("No file loaded", juce::dontSendNotification);
    metadataLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    metadataLabel.setFont(juce::Font(16.0f, juce::Font::plain));

    // background color
    setOpaque(true);
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
    loadButton.setBounds(20, y, 100, 40);
    restartButton.setBounds(140, y, 80, 40);
    stopButton.setBounds(240, y, 80, 40);
    playButton.setBounds(340, y, 80, 40);
    goToEnd.setBounds(440, y, 80, 40);
    goToStart.setBounds(540, y, 80, 40);
    loopButton.setBounds(640, y, 80, 40);
    muteButton.setBounds(740, y, 60, 40);

    metadataLabel.setBounds(20, 70, getWidth() - 40, 20);
    volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
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

        if (!isMuted)
        {
            previousVolume = (float)volumeSlider.getValue();
            playerAudio.setGain(0.0f);
            volumeSlider.setValue(0.0f, juce::dontSendNotification);
            muteButton.setButtonText("Unmute");
        }
        else
        {
            playerAudio.setGain(previousVolume);
            volumeSlider.setValue(previousVolume, juce::dontSendNotification);
            muteButton.setButtonText("Mute");
        }
        isMuted = !isMuted;
    }

}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        if (!isMuted)
            previousVolume = (float)slider->getValue();
        playerAudio.setGain((float)slider->getValue());
    }
}