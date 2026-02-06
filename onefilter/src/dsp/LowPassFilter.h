//
// Created by Viktor Lindström on 2026-02-04.
//

#pragma once
#include "Biquad.h"

class LowPassFilter {
public:
    void prepare(float sampleRate);

    void setCutoff(float frequencyHz);

    void setResonance(float q);

    float process(float x);

private:
    void updateCoefficients();
    float smoothQ(float newValue);
    float smoothC(float newValue);

    float fs{};
    float f0{1000.0f};
    float quality{0.707f};

    float oldf0{1000.0f};
    float oldQuality{0.707f};
    Biquad biquad;
};
