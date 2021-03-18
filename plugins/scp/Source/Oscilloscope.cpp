// see header file for license 

#include "Oscilloscope.h"

void Oscilloscope::paint(Graphics &g) {
    const ScopedLock sl(_lock);

    auto w = (float) getWidth();
    auto h = (float) getHeight();

    Font f(Font::getDefaultMonospacedFontName(), 0.075f * h, Font::plain);
    g.setFont(f);

    juce::Rectangle<int> background(0, 0, getWidth(), getHeight());

    // draw grid
    g.fillCheckerBoard(background, getWidth() / 20, getHeight() / 10, Colour(0x66666666), Colours::black);

    // draw axes
    g.setColour(Colours::white);
    g.drawVerticalLine(0, 0, h);
    g.drawHorizontalLine(h / 2, 0, w);

    for (int channel = 0; channel < _asb.getNumChannels(); channel++) {
        Colour &colour = *_colours.getUnchecked(channel);
        g.setColour(colour);

        // draw legend
        g.drawSingleLineText("-" + String(channel + 1), 25 + (50 * channel), h - 30);

        // draw line
        drawChannel(g, w, h, channel, colour);
    }
}

void Oscilloscope::drawChannel(Graphics &g, float w, float h, int channel, Colour colour) const {
    float step = _asb.getNumSamples() / w;
    float phase = 0.00f;

    for (int i = 0; i < (int) w; i++) {
        float val = _asb.getSample(channel, (int) phase) * 0.75f;

        if (val < -1.00f) val = -1.00f;
        if (val > 1.00f) val = 1.00f;

        val = 1.0f - (val + 1.0f) * 0.5f;
        val = val * h;

        if (val > h) val = h;
        else if (val < 0.0f) val = 0.0f;

        if (val == 0.5f) {
            g.setColour(Colours::transparentBlack);
        } else {
            g.setColour(colour);
        }
        g.drawLine(i, val, i + 1, val, 2.0f);

        phase += step;
    }
}

