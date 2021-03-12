// see header file for license 

#include "PluginProcessor.h"
#include "PluginEditor.h"

DATAEditor::DATAEditor (DATA& p)
    : AudioProcessorEditor (&p), processor (p)
{
	// set this to true to see the parameter values update 
	// in the vst plugin GUI (editor) when turning encoders/
	// pushing buttons 
	showParamValues = false; 

	for (int i=0; i<nScopes; i++) { 
		Oscilloscope* o = new Oscilloscope(
			processor.inBuffer, processor.lock, i);  
		o->setInfo(String(i+1));
		o->setInfoCol(Colours::white); 
		addAndMakeVisible(o);
		in.add(o);
	}

	setSize (1600, 480);
	startTimer(50); 
}

DATAEditor::~DATAEditor()
{
}

void DATAEditor::timerCallback()
{ 
	for (int i=0; i<nScopes; i++) { 
		in[i]->repaint(); 
	}

	// repaint our own canvas as well 
	repaint(); 
}

void DATAEditor::paint(Graphics& g)
{
	g.fillAll (Colours::black);

	// show plugin name in the background 
	Font f(Font::getDefaultMonospacedFontName(), 
		0.9f*getHeight(), Font::plain);
	g.setFont(f);
	g.setColour(Colour(30, 30, 30));

	int w=getWidth(); 
	int h=getHeight();

	if (showParamValues) { 
		h -= keepout; 
	}

	g.drawFittedText(
		JucePlugin_Name,
		0, 0,
		w, h,
		Justification::centred,
		1);

	// paint parameter values 
	if (showParamValues) { 

		Font f(Font::getDefaultMonospacedFontName(), 
			24, Font::plain);
		g.setFont(f);
		g.setColour(Colours::white); 

		// first row 
		int x = 10; 
		for (int i=Percussa::sspEnc1; i<Percussa::sspSw1; i++) { 
			g.drawSingleLineText(
				String::formatted("%4.2f", processor.getParameter(i)), 
				x, getHeight()-keepout+20); 
			x += 150;  
		}

		// second row 
		x = 10; 
		for (int i=Percussa::sspSw1; i<Percussa::sspSwLeft; i++) { 
			g.drawSingleLineText(
				String::formatted("%4.2f", processor.getParameter(i)), 
				x, getHeight()-keepout+40); 
			x += 150;  
		}

		// third row 
		x = 10; 
		for (int i=Percussa::sspSwLeft; i<Percussa::sspOutEn1; i++) { 
			g.drawSingleLineText(
				String::formatted("%4.2f", processor.getParameter(i)), 
				x, getHeight()-keepout+60); 
			x += 150;  
		}

		// fourth row 
		x = 10; 
		for (int i=Percussa::sspOutEn1; i<Percussa::sspInEn1; i++) { 
			g.drawSingleLineText(
				String::formatted("%1.1f", processor.getParameter(i)), 
				x, getHeight()-keepout+80); 
			x += 50;  
		}

		// fifth row 
		x = 10; 
		for (int i=Percussa::sspInEn1; i<Percussa::sspLast; i++) { 
			g.drawSingleLineText(
				String::formatted("%1.1f", processor.getParameter(i)), 
				x, getHeight()-keepout+100); 
			x += 50;  
		}
	} 
}

void DATAEditor::resized()
{
	int w=getWidth(); 
	int h=getHeight();

	if (showParamValues) { 
		h -= keepout; 
	}

	int scopeWidth=w/(nScopes/2);
	int scopeHeight=h/2; 

	for (int col=0; col<in.size(); col++) { 

		Oscilloscope* o = in[col]; 
		assert(o);

		int scopeX, scopeY;
		if (col < nScopes / 2) {
		    scopeX = col * scopeWidth;
		    scopeY = 0;
		} else {
            scopeX = (col - (nScopes /2)) * scopeWidth;
		    scopeY = h/2;
		}

		o->setBounds(
			scopeX,
			scopeY,
			scopeWidth, 
			scopeHeight); 
	}	
}
