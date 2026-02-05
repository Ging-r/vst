//
// Created by Viktor Lindström on 2026-02-05.
//
#include "LowPassFilter.h"
#include <cmath>

void LowPassFilter::setResonance(float q) {
    quality = q;
    updateCoefficients();
}

void LowPassFilter::setCutoff(float frequencyHz) {
    f0 = frequencyHz;
    updateCoefficients();
}

float LowPassFilter::process(float x) {
    updateCoefficients();
    return biquad.process(x);
}

void LowPassFilter::prepare(float sampleRate) {
    biquad.reset();
    fs = sampleRate;
    updateCoefficients();
}

void LowPassFilter::updateCoefficients() {
    const float w0 = 2 * static_cast<float>(M_PI) * f0 / fs;
    const float sineW0 = std::sinf(w0);
    const float cosineW0 = std::cos(w0);

    const float alpha = sineW0 / (2 * quality);

    const float b0 = (1 - cosineW0) / 2;
    const float b1 = 1 - cosineW0;
    const float b2 = (1 - cosineW0) / 2;
    const float a0 = 1 + alpha;
    const float a1 = -2 * cosineW0;
    const float a2 = 1 - alpha;

    biquad.setCoefficients(b0, b1, b2, a0, a1, a2);
}


