#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()
{
    transportSource.stop();
    transportSource.setSource(nullptr);
    readerSource.reset();
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources()
{
    resampleSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    if (auto* reader = formatManager.createReaderFor(file))
    {
        transportSource.stop();
        transportSource.setSource(nullptr);
        readerSource.reset();

        readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
        transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);

        auto& metadata = reader->metadataValues;

        title = "";
        artist = "";

        if (metadata.containsKey("Title")) title = metadata["Title"];
        else if (metadata.containsKey("TITLE")) title = metadata["TITLE"];

        if (metadata.containsKey("Artist")) artist = metadata["Artist"];
        else if (metadata.containsKey("ARTIST")) artist = metadata["ARTIST"];

     
        if (title.isEmpty() && artist.isEmpty())
        {
            parseMetadataFromFilename(file.getFileNameWithoutExtension());
        }
        
        else if (title.isEmpty() && !artist.isEmpty())
        {
            title = file.getFileNameWithoutExtension();
        }
        else if (artist.isEmpty() && !title.isEmpty())
        {
            artist = "Unknown Artist";
        }

        duration = reader->lengthInSamples / reader->sampleRate;

        return true;
    }
    return false;
}


void PlayerAudio::parseMetadataFromFilename(const juce::String& filename)
{
    juce::String cleanName = filename;

    if (cleanName.contains(".") && cleanName.indexOf(".") < 3)
    {
        int dotPos = cleanName.indexOf(".");
        juce::String potentialNumber = cleanName.substring(0, dotPos).trim();

        if (potentialNumber.containsOnly("0123456789"))
        {
            cleanName = cleanName.substring(dotPos + 1).trim();
        }
    }

    if (cleanName.contains(" - "))
    {
        int dashPos = cleanName.indexOf(" - ");
        artist = cleanName.substring(0, dashPos).trim();
        title = cleanName.substring(dashPos + 3).trim(); 
    }
    else
    {
        title = cleanName;
        artist = "Unknown Artist";
    }
}
void PlayerAudio::restart()
{
    transportSource.setPosition(0.0);
    transportSource.start();
}

void PlayerAudio::stop()
{
    transportSource.stop();
    transportSource.setPosition(0.0);
}

void PlayerAudio::play(bool c)
{
    if (c)
    {
        transportSource.start();
    }
    else
    {
        transportSource.stop();
    }
}


void PlayerAudio::goToEnd()
{
    transportSource.setPosition(transportSource.getLengthInSeconds());
}

void PlayerAudio::goToStart()
{
    transportSource.setPosition(0.0);
}

void PlayerAudio::setLooping(bool shouldloop)
{
    if (readerSource != nullptr)
        readerSource->setLooping(shouldloop);
}

void PlayerAudio::setPlaybackSpeed(double speed)
{
    playbackSpeed = juce::jlimit(0.5, 2.0, speed);
    resampleSource.setResamplingRatio(playbackSpeed);
}


void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}

void PlayerAudio::setPosition(double position)
{
    transportSource.setPosition(position);
}

double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLength() const
{
    return transportSource.getLengthInSeconds();
}


void PlayerAudio::setALoopPoint(double time)
{
    loopPointA = time;
    abLoopEnabled = (loopPointA >= 0 && loopPointB >= 0);
}

void PlayerAudio::setBLoopPoint(double time)
{
    loopPointB = time;
    abLoopEnabled = (loopPointA >= 0 && loopPointB >= 0);
}

void PlayerAudio::clearLoopPoints()
{
    loopPointA = -1.0;
    loopPointB = -1.0;
    abLoopEnabled = false;
}

void PlayerAudio::checkAndHandleLooping()
{
    if (abLoopEnabled && readerSource != nullptr)
    {
        double currentPos = getPosition();

        if (loopPointA > loopPointB && loopPointB >= 0)
        {
            std::swap(loopPointA, loopPointB);
        }

        if (currentPos >= loopPointB && loopPointB >= 0)
        {
            transportSource.setPosition(loopPointA);
        }
        else if (currentPos < loopPointA && loopPointA >= 0)
        {
            transportSource.setPosition(loopPointA);
        }
    }
}
