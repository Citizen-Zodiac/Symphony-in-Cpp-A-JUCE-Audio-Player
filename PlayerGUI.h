#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    PlayerGUI(PlayerAudio& player);
    ~PlayerGUI() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    PlayerAudio& player;




    // GUI Elements
    juce::TextButton loadButton{ "Load File" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton loopButton{ "Loop" };
    juce::TextButton pauseButton{ "Pause" };
    juce::TextButton goToEnd{ "Go to End" };
    juce::TextButton goToStart{ "Go to Start" };

    juce::Slider volumeSlider;

    std::unique_ptr<juce::FileChooser> fileChooser;

    // Event handlers
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
