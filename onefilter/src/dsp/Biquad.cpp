//
// Created by Viktor Lindström on 2026-02-05.
//
#include "Biquad.h"

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
    x1 = x2 = 0.0f;
    y1 = y2 = 0.0f;
}

float Biquad::process(const float x) {
    const float y = (b0) * x + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;

    x2 = x1;
    x1 = x;

    y2 = y1;
    y1 = y;

    return y;
}

