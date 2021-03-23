// see header file for license 

#include "Oscilloscope.h"
#include "Percussa.h"

void Oscilloscope::paint(Graphics &g) {
    const ScopedLock sl(_lock);

    auto w = (float) getWidth();
    auto h = (float) getHeight();

    float fontSize = 0.05f * h;
    Font f(Font::getDefaultMonospacedFontName(), fontSize, Font::plain);
    g.setFont(f);
    g.setColour(Colours::white);

    // draw background grid
    juce::Rectangle<int> background(0, 0, getWidth(), getHeight());

    const int xSquares = 20;
    const int xSquarePixels = w / xSquares;

    const int ySquares = 10;
    const int ySquarePixels = h / ySquares;

    g.fillCheckerBoard(background, xSquarePixels, ySquarePixels, Colour(0x66666666), Colours::transparentBlack);

    // draw zero voltage line (x axis)
    g.drawHorizontalLine(h / 2, 0, w);

    // draw channels
    for (int channel = 0; channel < _asb.getNumChannels(); channel++) {
        Colour &colour = *_colours.getUnchecked(channel);
        g.setColour(colour);

        // draw line above relevant encoder
        const int gutterWidth = 30;
        const int gutterHeight = 30;

        const int encoderWidth = 150;
        const int encoderSpacing = 75;
        const int encoderPosition = encoderWidth + encoderSpacing;
        const int encoderStartX = gutterWidth + (encoderPosition * channel);
        const int encoderEndX = encoderStartX + encoderWidth;
        const int encoderThickness = 5;

        g.drawLine(encoderStartX,
                   h - gutterHeight,
                   encoderEndX,
                   h - gutterHeight,
                   encoderThickness);

        // draw trace line
        drawChannel(g, w, h, channel, colour);
    }
}

void Oscilloscope::drawChannel(Graphics &g, float w, float h, int channel, Colour colour) const {
    // buffer fills width of screen
    float step = _asb.getNumSamples() / w;

    // phase represents part of buffer to display
    float phase = 0.00f;

    // working across screen (and thru buffer)
    for (int i = 0; i < (int) w; i++) {
        // get sample at pixel, val = +/-1.0 (+/-5V)
        float val = _asb.getSample(channel, (int) phase);

        // if val goes off the edge, don't display
        if (val <= -1.00f || val >= 1.00f) val = 0.00f;

        val = 1.0f - (val + 1.0f + (_processor.getParameter(channel) / 4)) * 0.5f;

        // scale val to screen height in pixels
        val = val * h;

        if (val > h) val = h;
        else if (val < 0.0f) val = 0.0f;

        if (val == 0.5f) {
            g.setColour(Colours::transparentBlack);
        } else {
            g.setColour(colour);
        }
        g.drawLine(i, val, i + 1, val, 1.5f);

        phase += step;
    }
}

