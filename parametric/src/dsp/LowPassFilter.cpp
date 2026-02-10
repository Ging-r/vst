//
// Created by Viktor Lindström on 2026-02-05.
//
#include "Filter.h"
#include <cmath>
class LowPassFilter final : public Filter {
    void updateCoefficients() override {
        const float w0 = 2 * static_cast<float>(M_PI) * oldf0 / fs;
        const float sineW0 = std::sinf(w0);
        const float cosineW0 = std::cos(w0);


        const float alpha = sineW0 / (2 * oldQuality);

        const float b0 = (1 - cosineW0) / 2;
        const float b1 = 1 - cosineW0;
        const float b2 = (1 - cosineW0) / 2;
        const float a0 = 1 + alpha;
        const float a1 = -2 * cosineW0;
        const float a2 = 1 - alpha;
        const float a0inv = 1/a0;

        biquad.setCoefficients(b0*a0inv, b1*a0inv, b2*a0inv, 1.0f, a1*a0inv, a2*a0inv);
    }
};


