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

#pragma once

#include <assert.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class Oscilloscope : public Component {
private:
    const AudioSampleBuffer &_asb;
    CriticalSection &_lock;
    OwnedArray<Colour> _colours;
    SCP &_processor;
public:
    Oscilloscope(const AudioSampleBuffer &asb, CriticalSection &lock, SCP &processor) :
            _asb(asb), _lock(lock), _processor(processor) {

        // TODO: There's got to be a better way...
        _colours.add(new Colour (Colours::green));
        _colours.add(new Colour (Colours::cyan));
        _colours.add(new Colour (Colours::red));
        _colours.add(new Colour (Colours::yellow));
    }

private:
    void paint(Graphics &g) override;

    void drawChannel(Graphics &g, float w, float h, int channel, Colour colour) const;

    juce_UseDebuggingNewOperator
};

