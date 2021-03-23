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

    if (_showBackground) {
        // draw background grid
        juce::Rectangle<int> background(0, 0, getWidth(), getHeight());

        const int xSquares = 20;
        const int xSquarePixels = w / xSquares;

        const int ySquares = 10;
        const int ySquarePixels = h / ySquares;

        g.fillCheckerBoard(background, xSquarePixels, ySquarePixels, Colour(0xff111111), Colours::transparentBlack);

        // draw zero voltage line (x axis)
        g.drawHorizontalLine(h / 2, 0, w);
    }

    // draw trace
    for (int channel = 0; channel < _asb.getNumChannels(); channel++) {
        Colour &colour = *_colours.getUnchecked(channel);
        g.setColour(colour);

        drawChannel(g, w, h, channel, colour);
    }

    // draw line above relevant encoder
    for (int channel = 0; channel < _asb.getNumChannels(); channel++) {
        Colour &colour = *_colours.getUnchecked(channel);
        g.setColour(colour);

        const int gutterWidth = 30;
        const int gutterHeight = 25;

        const int encoderWidth = 150;
        const int encoderSpacing = 75;
        const int encoderPosition = encoderWidth + encoderSpacing;
        const int encoderStartX = gutterWidth + (encoderPosition * channel);
        const int encoderEndX = encoderStartX + encoderWidth;
        const int encoderThickness = 3;

        g.drawLine(encoderStartX,
                   h - gutterHeight,
                   encoderEndX,
                   h - gutterHeight,
                   encoderThickness);
    }

    // draw toggle background button
    g.setColour(Colours::white);
    juce::Rectangle<int> toggleBackgroundButton(w - 120, h - 45, 50, 30);
    if (_showBackground) {
        g.setColour(Colours::black);
        g.fillRect(toggleBackgroundButton);
        g.setColour(Colours::white);
    } else {
        g.fillCheckerBoard(toggleBackgroundButton, 25, 15, Colour(0xff222222), Colours::black);
    }
    g.drawRect(toggleBackgroundButton);
}

void Oscilloscope::drawChannel(Graphics &g, float w, float h, int channel, Colour colour) const {
    // buffer fills width of screen
    float step = _asb.getNumSamples() / w;

    // phase represents part of buffer to display
    float phase = 0.00f;

    // working across screen (and thru buffer)
    for (int i = 0; i < (int) w; i++) {

        // get sample at pixel, val = +/-1.0f (+/-5V)
        float val = _asb.getSample(channel, (int) phase);

        // scale val to encoder value
        const float pressedEncoderValue = _processor.getParameter(Percussa::sspLast - (4 - channel));
        const float scalingFactor = pow(2, pressedEncoderValue);
        val = val * scalingFactor;

        // center val on the screen (0.0f -> 1.0f)
        const float encoderValue = _processor.getParameter(channel);
        const float positionOffset = -encoderValue / 4;
        val = 1.0f - (val + 1.0f + positionOffset) * 0.5f;

        // scale val to screen height in pixels
        val = val * h;

        if (val == 0.5f) {
            g.setColour(Colours::transparentBlack);
        } else {
            g.setColour(colour);
        }
        g.drawLine(i, val, i + 1, val, 1.5f);

        phase += step;
    }
}

void Oscilloscope::setShowBackground(bool showBackground) {
    _showBackground = showBackground;
}

