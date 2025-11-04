#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    // Add buttons
    for (auto* btn : { &loadButton, &restartButton , &stopButton, &playButton, &goToEnd, &goToStart,
        &muteButton, &loopButton, &addButton, &nextButton, &prevButoon, &removePlButton,&clearABButton })
    {
        addAndMakeVisible(*btn);
        btn->addListener(this);
        btn->setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        btn->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
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
    addAndMakeVisible(metadataLabel);
    metadataLabel.setJustificationType(juce::Justification::centred);
    metadataLabel.setText("No file loaded", juce::dontSendNotification);
    metadataLabel.setColour(juce::Label::textColourId, juce::Colour(0xff1a75ff));
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
    //trackList.addMouseListener(this, true);
    addAndMakeVisible(trackList);
    trackList.setModel(this);
    trackList.getHeader().addColumn("Title", 1, 250);
    trackList.getHeader().addColumn("Artist", 2, 200);
    trackList.getHeader().addColumn("Length", 3, 80);

    trackList.setColour(juce::TableListBox::backgroundColourId, juce::Colour(0xff1a1a1a));
        trackList.setOutlineThickness(1);
        trackList.setRowHeight(35);
        auto& header = trackList.getHeader();

        header.setColour(juce::TableHeaderComponent::backgroundColourId, juce::Colour(0xff181818));
        header.setColour(juce::TableHeaderComponent::textColourId, juce::Colour(0xffb3b3b3));
        header.setColour(juce::TableHeaderComponent::outlineColourId, juce::Colour(0xff1a1a1a));


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
    double length = playerAudio.getLength();

    if (loopPointA >= 0 && length > 0)
    {
        double aRatio = loopPointA / length;
        int aX = positionSlider.getX() + (int)(aRatio * positionSlider.getWidth());

        g.setColour(juce::Colours::red);
        g.drawLine(aX, positionSlider.getY() - 10, aX, positionSlider.getY() + positionSlider.getHeight() + 10, 2.0f);
        g.setColour(juce::Colours::white);
        g.drawText("A", aX - 10, positionSlider.getY() - 25, 20, 20, juce::Justification::centred);
    }

    if (loopPointB >= 0 && length > 0)
    {
        double bRatio = loopPointB / length;
        int bX = positionSlider.getX() + (int)(bRatio * positionSlider.getWidth());

        g.setColour(juce::Colours::green);
        g.drawLine(bX, positionSlider.getY() - 10, bX, positionSlider.getY() + positionSlider.getHeight() + 10, 2.0f);
        g.setColour(juce::Colours::white);
        g.drawText("B", bX - 10, positionSlider.getY() - 25, 20, 20, juce::Justification::centred);
    }
        
}

void PlayerGUI::resized()
{    
    auto area = getLocalBounds().reduced(10);
    // Top
    loadButton.setBounds(area.removeFromTop(40).removeFromLeft(130));
    area.removeFromTop(5);
    clearABButton.setBounds(area.removeFromTop(40).removeFromLeft(130));
  
    // Bottom
    auto bottomArea = area.removeFromBottom(60).reduced(10, 10);
    auto sliderArea = bottomArea.removeFromRight(250);
    volumeSlider.setBounds(sliderArea.reduced(5, 0));

    int totalButtonWidth = bottomArea.getWidth();
    int smallButtonWidth = totalButtonWidth * 0.10;
    int normalButtonWidth = (totalButtonWidth - 2 * smallButtonWidth) / 4;
    int buttonHeight = bottomArea.getHeight();
    
    loopButton.setBounds(bottomArea.removeFromLeft(smallButtonWidth).reduced(4));
    restartButton.setBounds(bottomArea.removeFromLeft(normalButtonWidth).reduced(4));
    prevButoon.setBounds(bottomArea.removeFromLeft(normalButtonWidth).reduced(4));
    playButton.setBounds(bottomArea.removeFromLeft(normalButtonWidth).reduced(4));
    nextButton.setBounds(bottomArea.removeFromLeft(normalButtonWidth).reduced(4));
    muteButton.setBounds(bottomArea.removeFromLeft(smallButtonWidth+10).reduced(4));

    //Playlist
    auto rightArea = area.removeFromRight(300);
    trackList.setBounds(rightArea.removeFromTop(250));
    
    addButton.setBounds(rightArea.removeFromTop(40).reduced(100,0).translated(-60,0));
    auto removeButtonArea = addButton.getBounds().translated(addButton.getWidth() + 10,0);
    removePlButton.setBounds(removeButtonArea);
    
    // Waves
    
    // Middle
    auto h = getHeight()/1.2 ;
    positionSlider.setBounds(270, h, getWidth() -600, 25);
    currentTimeLabel.setBounds(210, h, 60, 25);
    totalTimeLabel.setBounds(getWidth() - 330, h, 60, 25);
    goToStart.setBounds(130, h, 80, 30);
    goToEnd.setBounds(getWidth() - 270, h, 80, 30);

    metadataLabel.setBounds(250, h-30, getWidth() - 600, 25);
}

static bool isPlaying = false;
void PlayerGUI::buttonClicked(juce::Button* button)
{
    static bool checkRemovePl = false;
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
                        " | Artist: " + playerAudio.getArtist(),
                        
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
                    isPlaying = false;
                    playButton.setButtonText("Play");
                    playerAudio.play(isPlaying);

                }
                checkRemovePl = false;
            });

    }

    else if (button == &restartButton)
    {
        // Restart playback
        playerAudio.restart();
        playButton.setButtonText("Pause");
        isPlaying = true;
        loopPointA = -1.0;
        loopPointB = -1.0;
        playerAudio.clearLoopPoints();
        repaint();
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
                " | Artist: " + playlist[currentIndex].artist,
                juce::dontSendNotification);

            // Change button text
            playButton.setButtonText(isPlaying ? "Pause" : "Play");

        }
        else if(checkRemovePl==false&&playlist.empty())
        {
            isPlaying = !isPlaying;
            playerAudio.play(isPlaying);
            playButton.setButtonText(isPlaying ? "Pause" : "Play");

            metadataLabel.setText("Title: " +playerAudio.getTitle() +
                " | Artist: " + playerAudio.getArtist() ,
                juce::dontSendNotification);
        }
        else if (checkRemovePl)
        {
            metadataLabel.setText("No file loaded", juce::dontSendNotification);
            return;
        }
        
        

    }
    else if (button == &goToEnd)
    {
        // Go to end of the track
        playerAudio.goToEnd();
        if (isPlaying)
        {
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
                track.length = playerAudio.getLength()/60 ;
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
            loopPointA = -1.0;
            loopPointB = -1.0;
            playerAudio.clearLoopPoints();
            repaint();
            currentIndex = (currentIndex + 1) % playlist.size();
             playerAudio.loadFile(playlist[currentIndex].file);
         
            playButton.setButtonText("Pause");
            isPlaying = true;
            playerAudio.play(isPlaying);
            

            metadataLabel.setText("Title: " + playlist[currentIndex].title +
                " | Artist: " + playlist[currentIndex].artist,
                juce::dontSendNotification);
        
        }
        
    }
    else if (button == &prevButoon)
    {
        if (!playlist.empty())
        {
            loopPointA = -1.0;
            loopPointB = -1.0;
            playerAudio.clearLoopPoints();
            repaint();
            currentIndex = (currentIndex > 0) ? (currentIndex - 1) : (playlist.size() - 1);
            playerAudio.loadFile(playlist[currentIndex].file);

            playButton.setButtonText("Pause");
            isPlaying = true;
            playerAudio.play(isPlaying);


            metadataLabel.setText("Title: " + playlist[currentIndex].title +
                " | Artist: " + playlist[currentIndex].artist ,
                juce::dontSendNotification);

        }
    }
    else if (button == &clearABButton)
    {
        loopPointA = -1.0;
        loopPointB = -1.0;
        playerAudio.clearLoopPoints();
        repaint();
    }
    else if (button == &removePlButton)
    {
        if(!playlist.empty())
        {
            playlist.clear();
            trackList.updateContent();
            trackList.repaint();
            if (isPlaying)
            {
                playerAudio.stop();

            }
            isPlaying = false;
            playButton.setButtonText("Play");

            checkRemovePl = true;
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
        playerAudio.checkAndHandleLooping();
    }
}

int PlayerGUI::getNumRows()
{
    return playlist.size();
}
void PlayerGUI::paintRowBackground(juce::Graphics& g,int row, int width, int height, bool rowIsSelected)
{
    if (row == currentIndex)

        g.fillAll(juce::Colour(0xff1a75ff));
    else if (rowIsSelected)
        g.fillAll(juce::Colour(0xff0a3d62));
    else if (row % 2 == 0)
        g.fillAll(juce::Colour(0xff181818));
    else                       
        g.fillAll(juce::Colour(0xff202020));
}
void PlayerGUI::paintCell(juce::Graphics& g, int row, int col, int width, int height, bool selected)
{
    const auto& track = playlist[row];
    juce::String text;
    if (col == 1)
       text= track.title;
    else if (col == 2)
       text= track.artist;
    else if (col == 3)
        text = juce::String(track.length);
    
    if (row == currentIndex)
    {
        g.setColour(juce::Colours::white);
        g.setFont(juce::Font("Arial", 14.0f, juce::Font::bold));
    }
    g.setColour(selected ? juce::Colours::black : juce::Colours::white);
    g.setFont(juce::Font("Arial", 14.0f, juce::Font::plain));
    g.drawText(text, 8, 0, width - 16, height, juce::Justification::centredLeft);
}
void PlayerGUI::setCurrentlyPlayingRow(int row)
{
    currentIndex = row;
    trackList.repaint();

}

bool PlayerGUI::keyPressed(const juce::KeyPress& key)
{
    double currentPos = playerAudio.getPosition();

    
    DBG("Key pressed: " << key.getKeyCode() << " Character: " << key.getTextCharacter());

    if (key.getKeyCode() == 65 || key.getKeyCode() == 97) 
    {
        loopPointA = currentPos;
        updateABLoopPoints();
        repaint();
        DBG("A point set at: " << currentPos);
        return true;
    }
    else if (key.getKeyCode() == 66 || key.getKeyCode() == 98) 
    {
    
        loopPointB = currentPos;
        updateABLoopPoints();
        repaint();
        DBG("B point set at: " << currentPos);
        return true;
    }

    return false;
}

void PlayerGUI::updateABLoopPoints()
{

    if (loopPointA >= 0)
        playerAudio.setALoopPoint(loopPointA);

    if (loopPointB >= 0)
        playerAudio.setBLoopPoint(loopPointB);

    if (loopPointA < 0 && loopPointB < 0)
        playerAudio.clearLoopPoints();
}                  
