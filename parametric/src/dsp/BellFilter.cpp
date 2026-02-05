//
// Created by Viktor Lindström on 2026-02-05.
//
#include "Filter.h"
#include <cmath>

class BellFilter final : public Filter {
    void updateCoefficients() override {
        const auto A = static_cast<float>(std::sqrt(std::pow(10, (dbGain / 40))));
        const float w0 = 2 * static_cast<float>(M_PI) * f0 / fs;
        const float alpha = std::sin(w0) / (2 * quality);

        const float b0 = 1 + alpha * A;
        const float b1 = -2 * cos(w0);
        const float b2 = 1 - alpha * A;
        const float a0 = 1 + alpha / A;
        const float a1 = -2 * cos(w0);
        const float a2 = 1 - alpha / A;
        const float a0inv = 1/a0;

        biquad.setCoefficients(b0*a0inv, b1*a0inv, b2*a0inv, 1.0f, a1*a0inv,a2*a0inv);
    }
    void setDbGain(float gain) {
        dbGain = gain;
        updateCoefficients();
    }

private:
    float dbGain{0.0f};
};
