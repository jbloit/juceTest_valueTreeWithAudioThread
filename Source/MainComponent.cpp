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
            // If we get here, then our attempt at locking failed because another thread had already locked it..
        }
    }

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.
    
    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.
    
    // For more details, see the help for AudioProcessor::prepareToPlay()
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
            // If we get here, then our attempt at locking failed because another thread had already locked it..
        }
        
    }
    bufferToFill.clearActiveBufferRegion();
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
            // If we get here, then our attempt at locking failed because another thread had already locked it..
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
            // If we get here, then our attempt at locking failed because another thread had already locked it..
        }
    }

}

void MainComponent::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged,
                               const Identifier& property)
{
                triggerAsyncUpdate();
}
