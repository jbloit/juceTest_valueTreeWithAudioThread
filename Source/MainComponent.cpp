/*
 ==============================================================================
 
 This file was auto-generated!
 
 ==============================================================================
 */

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    
    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    model = ValueTree(IDs::DATA_MODEL);
    model.setProperty(IDs::processorState, State::OFF, nullptr);
    model.addListener(this);
    
    myButton.setButtonText("click this");
    addAndMakeVisible(myButton);
    myButton.addListener(this);
    
    logLabel.setText("Log", NotificationType::dontSendNotification);
    addAndMakeVisible(logLabel);
    
    stateLabel.setText("OFF", NotificationType::dontSendNotification);
    addAndMakeVisible(stateLabel);
    
    
    {
        const ScopedTryLock myScopedTryLock (objectLock);
        if (myScopedTryLock.isLocked())
        {
            cachedBufferCount.referTo(model, IDs::bufferCount, nullptr);
            cachedState.referTo(model, IDs::processorState, nullptr);
        }
        else
        {
            DBG("Failed to aquire lock in CONSTRUCTOR");
        }
    }

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double newSampleRate)
{
    sampleRate = newSampleRate;
    expectedSamplesPerBlock = samplesPerBlockExpected;
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!
    
    {
        const ScopedTryLock myScopedTryLock (objectLock);
        if (myScopedTryLock.isLocked())
        {
            if (cachedState == State::ON)
            {
                cachedBufferCount.setValue(cachedBufferCount + 1, nullptr);
            }
        }
        else
        {
            DBG("Failed to aquire lock in AUDIO callback");
        }
        
    }
    
    bufferToFill.clearActiveBufferRegion();
    auto originalPhase = phase;
    
    for (auto chan = 0; chan < bufferToFill.buffer->getNumChannels(); ++chan)
    {
        phase = originalPhase;
        
        auto* channelData = bufferToFill.buffer->getWritePointer (chan, bufferToFill.startSample);
        
        for (auto i = 0; i < bufferToFill.numSamples ; ++i)
        {
            channelData[i] = amplitude.get() * std::sin (phase.get());
            
            // increment the phase step for the next sample
            phase = std::fmod (phase.get() + phaseDelta.get(), MathConstants<float>::twoPi);
        }
    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    
    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    // You can add your drawing code here!
}

void MainComponent::resized()
{
    auto area = getLocalBounds();

    auto buttonArea = area.reduced(getWidth()/4, getHeight()/4);
    
    auto logArea = buttonArea.removeFromBottom(buttonArea.getHeight()/2);
    
    myButton.setBounds(buttonArea);
    
    logLabel.setBounds(logArea);
}


void MainComponent::buttonClicked (Button*)

{
    {
        const ScopedTryLock myScopedTryLock (objectLock);
        if (myScopedTryLock.isLocked())
        {

            if (cachedState == State::ON) cachedState = State::OFF;
            else cachedState = State::ON;
            
            cachedBufferCount = 0;

        }
        else
        {
            DBG("Failed to aquire lock in GUI (button event) callback");
        }
    }
}



void MainComponent::handleAsyncUpdate()
{
    
    {
        const ScopedTryLock myScopedTryLock (objectLock);
        if (myScopedTryLock.isLocked())
        {
            
            logLabel.setText(" bufferCount :" + String(cachedBufferCount) + " | state :" + String(cachedState), NotificationType::dontSendNotification);
        }
        else
        {
            DBG("Failed to aquire lock in VT callback");
        }
    }

}

void MainComponent::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged,
                               const Identifier& property)
{
                triggerAsyncUpdate();
}

// Mouse handling..
void MainComponent::mouseDown (const MouseEvent& e)
{
    mouseDrag (e);
}

void MainComponent::mouseDrag (const MouseEvent& e)
{
    lastMousePosition = e.position;
    
    frequency = (getHeight() - e.y) * 10.0f;
    amplitude = jmin (0.9f, 0.2f * e.position.x / getWidth());
    
    phaseDelta = (float) (MathConstants<double>::twoPi * frequency.get() / sampleRate);
    
    repaint();
}

void MainComponent::mouseUp (const MouseEvent&)
{
    amplitude = 0.0f;
    repaint();
}
