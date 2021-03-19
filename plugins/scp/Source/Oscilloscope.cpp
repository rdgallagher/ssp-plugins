// see header file for license 

#include "Oscilloscope.h"

void Oscilloscope::paint(Graphics &g) {
    const ScopedLock sl(_lock);

    auto w = (float) getWidth();
    auto h = (float) getHeight();

    Font f(Font::getDefaultMonospacedFontName(), 0.075f * h, Font::plain);
    g.setFont(f);
    g.setColour(Colours::white);

    juce::Rectangle<int> background(0, 0, getWidth(), getHeight());

    // draw grid
    const int xSquares = 20;
    const int xSquarePixels = w / xSquares;

    const int ySquares = 10;
    const int ySquarePixels = h / ySquares;

    g.fillCheckerBoard(background, xSquarePixels, ySquarePixels, Colour(0x66666666), Colours::transparentBlack);

    // draw axes
    // - y (voltage)
    g.drawVerticalLine(0, 0, h);
    // TODO: Draw voltage markers at square divisions

    // - x (time)
    g.drawHorizontalLine(h / 2, 0, w);
    // TODO: The below code adds timing information to the x-axis, but is it useful?
//    const float bufferLengthSeconds = _asb.getNumSamples() / _sampleRate;
//    const float xSquareSeconds = bufferLengthSeconds / xSquares;
//    for (int square = 2; square < xSquares; square += 2) {
//        float squareTimeMillis = square * xSquareSeconds * 1000;
//        char squareTimeDisplay[4];
//        snprintf(squareTimeDisplay, 4, "%.2f", squareTimeMillis);
//        g.drawSingleLineText(String(squareTimeMillis) + "ms", square * xSquarePixels, h / 2,
//                             Justification::centred);
//    }

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

    // TODO: Is this useful for changing scale?
    float timescale = 1.0f;

    // working across screen (and thru buffer)
    for (int i = 0; i < (int) w; i++) {
        // TODO: what V does +/-1.0 represent here?
        // get sample at pixel, val = +/-1.0
        float val = _asb.getSample(channel, (int) phase) * 0.75f; // TODO: Why *0.75?

        // limit val to +/-1.0
        if (val < -1.00f) val = -1.00f;
        if (val > 1.00f) val = 1.00f;

        // center val on 0.5 and scale to 0-1 (ie. +/-0.5)
        val = 1.0f - (val + 1.0f) * 0.5f;

        // scale val to screen height in pixels
        val = val * h;

        if (val > h) val = h;
        else if (val < 0.0f) val = 0.0f;

        if (val == 0.5f) {
            g.setColour(Colours::transparentBlack);
        } else {
            g.setColour(colour);
        }
        g.drawLine(i * timescale, val, i * timescale + 1, val, 2.0f);

        phase += step;
    }
}

