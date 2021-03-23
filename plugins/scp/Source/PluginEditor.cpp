// see header file for license 

#include "PluginProcessor.h"
#include "PluginEditor.h"

SCPEditor::SCPEditor (SCP& p)
    : AudioProcessorEditor (&p), processor (p)
{
	// set this to true to see the parameter values update 
	// in the vst plugin GUI (editor) when turning encoders/
	// pushing buttons 
	showParamValues = false;

	showBackground = true;
	showBackgroundButtonPressed = false;

    scope = new Oscilloscope(processor.inBuffer, processor.lock, processor, showBackground);
    addAndMakeVisible(scope);

	setSize (1600, 480);
	startTimer(50); 
}

SCPEditor::~SCPEditor()
{
    stopTimer();
}

void SCPEditor::timerCallback()
{
    // Toggle scope background when Left Shift button is pressed
    // TODO: This sometimes doesn't get called while the button is pressed...
    // TODO: Move to adding an event onto a queue in processor to be consumed here
    if (processor.getParameter(Percussa::sspSwShiftL) > 0.5f && !showBackgroundButtonPressed) {
        showBackgroundButtonPressed = true;
        showBackground = !showBackground;
        scope->setShowBackground(showBackground);
    } else if (processor.getParameter(Percussa::sspSwShiftL) < 0.5f && showBackgroundButtonPressed) {
        showBackgroundButtonPressed = false;
    };

	scope->repaint();

	// repaint our own canvas as well 
	repaint(); 
}

void SCPEditor::paint(Graphics& g)
{
	g.fillAll (Colours::black);

	int w=getWidth();
	int h=getHeight();

	if (showParamValues) { 
		h -= keepout; 
	}

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

void SCPEditor::resized()
{
	int w=getWidth(); 
	int h=getHeight();

	if (showParamValues) { 
		h -= keepout; 
	}

    scope->setBounds(0, 0, w, h);
}
