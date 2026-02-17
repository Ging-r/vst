//
// Created by Viktor Lindström on 2026-02-05.
//
#include "Filter.h"
#include <cmath>

class BellFilter final : public Filter {
    void updateCoefficients() override {
        const float A = static_cast<float>(std::pow(10, (oldDbGain / 40)));
        const float w0 = 2 * static_cast<float>(M_PI) * oldf0 / fs;
        const float sineW0 = std::sinf(w0);
        const float cosineW0 = std::cos(w0);

        const float alpha = sineW0 / (2 * oldQuality);

        const float b0 = 1 + alpha * A;
        const float b1 = -2 * cosineW0;
        const float b2 = 1 - alpha * A;
        const float a0 = 1 + alpha / A;
        const float a1 = -2 * cosineW0;
        const float a2 = 1 - alpha / A;
        const float a0inv = 1/a0;

        biquad.setCoefficients(b0*a0inv, b1*a0inv, b2*a0inv, 1.0f, a1*a0inv,a2*a0inv);
    }
    public: void setDbGain(float gain) {
        dbGain = gain;
        oldDbGain = smoothDb(gain);
    }

    float smoothDb(float gain){
        float y;
        if(std::abs(oldDbGain-gain)<0.01f){
            return gain;
        }else {
            y = oldDbGain + (gain-oldDbGain)/100;
        }
        return y;
 
    }

private:
    float dbGain{0.0f};
    float oldDbGain{0.0f};
};
