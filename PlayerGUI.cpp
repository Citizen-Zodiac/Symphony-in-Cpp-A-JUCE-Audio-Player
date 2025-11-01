#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    // Add buttons
    for (auto* btn : { &loadButton, &restartButton , &stopButton, &playButton, &goToEnd, &goToStart,
        &muteButton, &loopButton, &addButton, &nextButton, &prevButoon })
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
    metadataLabel.setColour(juce::Label::textColourId, juce::Colours::darkblue);
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


    // Playlist
    addAndMakeVisible(trackList);
    trackList.setModel(this);
    trackList.addMouseListener(this, true);
 
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
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::cyan);
}

void PlayerGUI::resized()
{
   
        int y = 20;
        int buttonWidth = 80;
        int buttonHeight = 40;


        loadButton.setBounds(20, y, 100, buttonHeight);
        playButton.setBounds(130, y, buttonWidth, buttonHeight);
        stopButton.setBounds(220, y, buttonWidth, buttonHeight);
        restartButton.setBounds(310, y, buttonWidth, buttonHeight);
        goToStart.setBounds(400, y, 100, buttonHeight);
        goToEnd.setBounds(510, y, 100, buttonHeight);
        loopButton.setBounds(620, y, buttonWidth, buttonHeight);
        muteButton.setBounds(710, y, 70, buttonHeight);
        metadataLabel.setBounds(20, 70, getWidth() - 40, 20);
        volumeSlider.setBounds(20, 100, getWidth() - 40, 20);

        int sliderY = 140;
        positionSlider.setBounds(70, sliderY, getWidth() - 140, 25);
        currentTimeLabel.setBounds(10, sliderY, 50, 25);
        totalTimeLabel.setBounds(getWidth() - 60, sliderY, 50, 25);
    
    addButton.setBounds(840, y, 50, 30);
    nextButton.setBounds(940, y, 40, 30);
    prevButoon.setBounds(1040, y, 50, 30);

    trackList.setBounds(10,200, 500, 200);


    metadataLabel.setBounds(20, 70, getWidth() - 40, 20);
    volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
}

static bool isPlaying = false;
void PlayerGUI::buttonClicked(juce::Button* button)
{
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
                if(!playlist.empty())
                {
                    playlist.clear();
                    trackList.updateContent();
                    trackList.repaint();
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
        if (!playlist.empty())
        {
            
            playerAudio.loadFile(playlist[currentIndex].file);
             // Start & Pause playback
            isPlaying = !isPlaying;
            playerAudio.play(isPlaying);

            metadataLabel.setText("Title: " + playlist[currentIndex].title +
                " | Artist: " + playlist[currentIndex].artist +
                " | Duration: " + juce::String(playlist[currentIndex].duration / 60.0, 2) + "min",
                juce::dontSendNotification);

            // Change button text
            playButton.setButtonText(isPlaying ? "Pause" : "Play");

        }
        else
        {
            isPlaying = !isPlaying;
            playerAudio.play(isPlaying);
            playButton.setButtonText(isPlaying ? "Pause" : "Play");

            metadataLabel.setText("Title: " +playerAudio.getTitle() +
                " | Artist: " + playerAudio.getArtist() +
                " | Duration: " + juce::String(playerAudio.getDuration() / 60.0, 2) + "min",
                juce::dontSendNotification);
        }

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
    else if (button == &addButton)
    {
       
        static  double pos;
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
                    pos = playerAudio.getPosition();
                    playerAudio.loadFile(file);
                }

                else
                {
                    metadataLabel.setText("No file loaded",juce::dontSendNotification);
                }
                TrackMetadata track;
                track.title = playerAudio.getTitle();
                track.artist = playerAudio.getArtist();
                track.duration = playerAudio.getDuration();
                track.file = file;
                playlist.push_back(track);
                trackList.updateContent();
                 
               if (isPlaying)
                {
                    playerAudio.loadFile(playlist[currentIndex].file);
                    playerAudio.setPosition(pos);
                    playerAudio.play(isPlaying);
                }
                
            });


    }
    else if (button == &nextButton)
    {

        if (!playlist.empty())
        {
            currentIndex = (currentIndex + 1) % playlist.size();
             playerAudio.loadFile(playlist[currentIndex].file);
         
            playButton.setButtonText("Pause");
            isPlaying = true;
            playerAudio.play(isPlaying);
            

            metadataLabel.setText("Title: " + playlist[currentIndex].title +
                " | Artist: " + playlist[currentIndex].artist +
                " | Duration: " + juce::String(playlist[currentIndex].duration / 60.0, 2) + "min",
                juce::dontSendNotification);
        
        }
        
    }
    else if (button == &prevButoon)
    {
        if (!playlist.empty())
        {
            currentIndex = (currentIndex > 0) ? (currentIndex - 1) : (playlist.size() - 1);
            playerAudio.loadFile(playlist[currentIndex].file);

            playButton.setButtonText("Pause");
            isPlaying = true;
            playerAudio.play(isPlaying);


            metadataLabel.setText("Title: " + playlist[currentIndex].title +
                " | Artist: " + playlist[currentIndex].artist +
                " | Duration: " + juce::String(playlist[currentIndex].duration / 60.0, 2) + "min",
                juce::dontSendNotification);

        }
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
    else if (slider == &positionSlider)
    {
        double length = playerAudio.getLength();
        double newPosition = positionSlider.getValue() * length;
        playerAudio.setPosition(newPosition);
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

int PlayerGUI::getNumRows()
{
    return playlist.size();
}
void PlayerGUI::paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool selected)
{
    if (selected)
        
        g.fillAll(juce::Colours::white);
        g.setColour(juce::Colours::orange);
        g.setFont(16.0f);
        g.drawText(playlist[row].title, 5, 0, width, height, juce::Justification::centredLeft);

   
    
}

void PlayerGUI::mouseDown(const juce::MouseEvent& event)
{   
    if (event.mods.isRightButtonDown())
        return;
    
    int row = trackList.getRowContainingPosition(event.x, event.y);

    if (row >= 0 && row < playlist.size())

    {
        playerAudio.loadFile(playlist[row].file);

        if (currentIndex != row)
        {
            currentIndex = row;
            isPlaying = !isPlaying;

            playerAudio.play(isPlaying);
            playButton.setButtonText(isPlaying ? "Pause" : "Play");
        }

        if (!isPlaying)
        {
            isPlaying = !isPlaying;
            playerAudio.play(isPlaying);
            playButton.setButtonText(isPlaying ? "Pause" : "Play");
        }
        else if (isPlaying)
        {
            isPlaying = !isPlaying;
            playerAudio.play(isPlaying);
            playButton.setButtonText(isPlaying ? "Pause" : "Play");

        }

        metadataLabel.setText("Title: " + playlist[currentIndex].title +
            " | Artist: " + playlist[currentIndex].artist +
            " | Duration: " + juce::String(playlist[currentIndex].duration / 60.0, 2) + "min",
            juce::dontSendNotification);


    }
      
}
