#include "PlayerGUI.h"

PlayerGUI::PlayerGUI(PlayerAudio& p) : player(p)
{
    // Add buttons
    for (auto* btn : { &loadButton, &restartButton , &stopButton, &playButton,&pauseButton, &loopButton, &goToEnd, &goToStart,
        })
    {
        btn->addListener(this);
        addAndMakeVisible(*btn);
    }

    // Volume slider setup
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);


}

PlayerGUI::~PlayerGUI() {}


void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void PlayerGUI::resized()
{
    int y = 20;
    int buttonWidth = 80;
    int buttonHeight = 40;

  
    loadButton.setBounds(20, y, 100, buttonHeight);
    playButton.setBounds(130, y, buttonWidth, buttonHeight);
    pauseButton.setBounds(220, y, buttonWidth, buttonHeight);
    stopButton.setBounds(310, y, buttonWidth, buttonHeight);
    loopButton.setBounds(400, y, buttonWidth, buttonHeight);
    goToStart.setBounds(490, y, 100, buttonHeight);
    goToEnd.setBounds(600, y, 100, buttonHeight);

    volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
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
                    player.loadFile(file);
                }
            });
    }

    else if (button == &restartButton)
    {
        // Restart playback
        player.restart();
    }
    else if (button == &stopButton)
    {
        // Stop playback and reset position
        player.stop();
        player.setPosition(0.0);
    }
    else if (button == &playButton)
    {
        // Start playback
        player.play();
    }
    else if (button == &pauseButton)
    {
        // Pause playback
        player.pause();
    }
    else if (button == &goToEnd)
    {
        // Go to end of the track
        player.goToEnd();
    }
    else if (button == &goToStart)
    {
        // Go to start of the track
        player.goToStart();
    }
    else if (button == &loopButton)
    {
        // Toggle looping
        static bool isLooping = false;
        isLooping = !isLooping;
        player.setLooping(isLooping);

        // Change button text to show state
        loopButton.setButtonText(isLooping ? "Loop" : "Single");
    }

}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        player.setGain((float)slider->getValue());
    }
}
