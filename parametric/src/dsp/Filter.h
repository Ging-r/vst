//
// Created by Viktor Lindström on 2026-02-05.
//

#ifndef PARAMETRICEQ_FILTER_H
#define PARAMETRICEQ_FILTER_H

#endif //PARAMETRICEQ_FILTER_H

#pragma once
#include "Biquad.h"
#include <cmath>

class Filter {
public:

    virtual void prepare(float sampleRate) {
        biquad.reset();
        fs = sampleRate;
    }

    virtual void setCutoff(float frequencyHz) {
        f0 = frequencyHz;
        oldf0 = smoothC(frequencyHz);
    }

    virtual void setResonance(float q) {
        quality = q;
        oldQuality = smoothQ(q);
    }

    virtual float process(float x) {
        updateCoefficients();
        return biquad.process(x);
    }

    float smoothQ(float newValue){
        float y;
        if(std::abs(oldQuality-newValue)<0.01f){
            return newValue;
        }else{
            y = oldQuality + ((newValue-oldQuality))/100;
        }

        return y;
    }

    float smoothC(float newValue){
        float y;

        if(std::abs(oldf0-newValue)<0.01f){
            return newValue;
        }else {
            y = oldf0 + (newValue-oldf0)/100;
        }

        return y;
    }

    virtual ~Filter() = default;
protected:
    virtual void updateCoefficients() = 0;

    float fs{};
    float f0{};
    float quality{};

    float oldQuality{0.707f};
    float oldf0{1000.0f};

    Biquad biquad;
};
