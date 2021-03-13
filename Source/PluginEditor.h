/*
	Copyright (c) 2018 - Bert Schiettecatte, Noisetron LLC. 

	This software is part of the Percussa SSP's software development kit (SDK). 
	For more info about Percussa or the SSP visit http://www.percussa.com/ 
	and our forum at http://forum.percussa.com/ 

	Permission is granted to use this software under the terms of either:
	a) the GPL v2 (or any later version)
	b) the Affero GPL v3

	Details of these licenses can be found at: www.gnu.org/licenses

	This software is distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Oscilloscope.h" 

class SCOPEEditor  : public AudioProcessorEditor, public Timer
{
public:
    // TODO: Get nScopes from number of active incoming connections?
	static const int nScopes = JucePlugin_MaxNumInputChannels;
	static const int keepout = 100; 

	SCOPEEditor (SCOPE&);
	~SCOPEEditor();

	void paint (Graphics&) override;
	void resized() override;
	void timerCallback(); 

private:
	SCOPE& processor;
	bool showParamValues; 

	OwnedArray<Oscilloscope> in; 

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SCOPEEditor)
};

#endif  
