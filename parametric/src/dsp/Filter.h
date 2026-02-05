//
// Created by Viktor Lindström on 2026-02-05.
//

#ifndef PARAMETRICEQ_FILTER_H
#define PARAMETRICEQ_FILTER_H

#endif //PARAMETRICEQ_FILTER_H

#pragma once
#include "Biquad.h"

class Filter {
public:

    virtual void prepare(float sampleRate) {
        biquad.reset();
        fs = sampleRate;
        updateCoefficients();
    }

    virtual void setCutoff(float frequencyHz) {
        f0 = frequencyHz;
        updateCoefficients();
    };

    virtual void setResonance(float q) {
        quality = q;
        updateCoefficients();
    }

    virtual float process(float x) {
        return biquad.process(x);
    }

    virtual ~Filter() = default;
protected:
    virtual void updateCoefficients();

    float fs{};
    float f0{};
    float quality{};

    Biquad biquad;
};
