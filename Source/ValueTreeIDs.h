/*
  ==============================================================================

    ValueTreeIDs.h
    Created: 10 Apr 2020 2:59:40pm
    Author:  julien@macmini

  ==============================================================================
*/


#pragma once


namespace IDs
{
    
#define DECLARE_ID(name) const juce::Identifier name (#name);
    
    DECLARE_ID (DATA_MODEL)
    DECLARE_ID (processorState)
    DECLARE_ID (bufferCount)
#undef DECLARE_ID
    
}
