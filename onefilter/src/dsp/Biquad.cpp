//
// Created by Viktor Lindström on 2026-02-05.
//
#include "Biquad.h"
#include <cmath>

void Biquad::setCoefficients(const float b0_, const float b1_, const float b2_,
                             const float a0_, const float a1_, const float a2_) {
    b0 = b0_;
    b1 = b1_;
    b2 = b2_;
    a0 = a0_;
    a1 = a1_;
    a2 = a2_;
}

void Biquad::reset() {
    s1 = 0.0f;
    s2 = 0.0f;
}

float Biquad::process(float x) {
    x += 1.0e-25f;

    const float y = b0 * x + s1;

    const float feedback = std::tanh(y);


    s1 = s2 + b1 * x - a1 * feedback;
    s2 =      b2 * x - a2 * feedback;

    return y;
}

