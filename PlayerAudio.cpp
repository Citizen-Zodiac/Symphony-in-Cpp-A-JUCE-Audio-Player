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
        // ?? Disconnect old source first
        transportSource.stop();
        transportSource.setSource(nullptr);
        readerSource.reset();

        // Create new reader source
        readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
        transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);

        // metadata //
        auto values = reader->metadataValues;
        title = values["title"];
        artist = values["artist"];
        duration = reader->lengthInSamples / reader->sampleRate;

        if (title.isEmpty())
            title = file.getFileNameWithoutExtension();
        if (artist.isEmpty())
            artist = "Unknown Artist";

        return true;
    }
    return false;
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

