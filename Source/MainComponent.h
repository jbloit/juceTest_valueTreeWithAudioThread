/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ValueTreeIDs.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/


enum State {
    OFF,
    ON
};


class MainComponent   : public AudioAppComponent, Button::Listener, ValueTree::Listener, AsyncUpdater
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...

    TextButton myButton;
    void buttonClicked (Button*) override;
    
    CriticalSection objectLock;
    int someValue = 333;

    Array<float> someArray;
    
    State state = State::OFF;
    int bufferCount;
    
    ValueTree model;
    
    Label logLabel;
    Label stateLabel;
    
    CachedValue<int> cachedBufferCount;
    CachedValue<int> cachedState;
    
    void handleAsyncUpdate() override;
    
    void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged,
                                   const Identifier& property) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
