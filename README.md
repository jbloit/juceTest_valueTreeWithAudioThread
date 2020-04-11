This is some toy code to try and make sense of best practices when writing/reading data from the message thread and the audio thread, as is typically needed in audio applications.
The UI updates some variables read in the audio callback, and the callback also reads or writes these values (buffer count and state).
The data is stored in a ValueTree node.

Importantly, this code is tested with the Thread Sanitizer and seems to behave ok. (I wasn't yet able to run the thread Sanitizer within a plugin/host architecture).

The app doesn't produce sound, I should maybe at least add in a sine wave to make sure it doesn't glitch.


## Thread safety
I'm using a CriticalSection to ensure thread safety when reading/writing values. I'm using a try lock on the audio callback. I'm using this approach based on Thibaut Carpentier's paper: "Synchronisation de données inter-processus dans les applications audio temps réel: qu'est-ce qui débloque?" (JIM 2018).

## Realtime safety
I'm using an AsyncUpdater so that the UI updates from the ValueTree listener happen asynchronously on the message thread after being updated on the audio thread.

## Warning
This code only reflects my understanding of some good practices that I have read on the Juce forums, in the Juce codebase, and from Thibaut Carpentier's paper.
