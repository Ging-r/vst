//
// Created by Viktor Lindström on 2026-02-05.
//
#include "LowPassFilter.h"
#include <cmath>

void LowPassFilter::setResonance(float q) {
    quality = q;
    oldQuality = smoothQ(q);
    updateCoefficients();
}

void LowPassFilter::setCutoff(float frequencyHz) {
    f0 = frequencyHz;
    oldf0 = smoothC(frequencyHz);
    updateCoefficients();
}

float LowPassFilter::process(float x) {
    //updateCoefficients();
    return biquad.process(x);
}

void LowPassFilter::prepare(float sampleRate) {
    biquad.reset();
    fs = sampleRate;
    updateCoefficients();
}

float LowPassFilter::smoothQ(float newValue){
    if(std::abs(oldQuality-newValue)<0.01f){
        return newValue;
    }else{
        const float y = oldQuality + ((newValue-oldQuality))/100;
        oldQuality = y;
    }

    return oldQuality;
}
float LowPassFilter::smoothC(float newValue){

    if(std::abs(oldf0-newValue)<0.01f){
        return newValue;
    }else {
        const float y = oldf0 + (newValue-oldf0)/100;
        oldf0 = y;
    }

    return oldf0;
}

void LowPassFilter::updateCoefficients() {
    const float w0 = 2 * static_cast<float>(M_PI) * oldf0 / fs;
    const float sineW0 = std::sinf(w0);
    const float cosineW0 = std::cos(w0);


    const float alpha = sineW0 / ((2 * oldQuality)+0.00000001f);

    const float b0 = (1 - cosineW0) / 2;
    const float b1 = 1 - cosineW0;
    const float b2 = (1 - cosineW0) / 2;
    const float a0 = 1 + alpha;
    const float a1 = -2 * cosineW0;
    const float a2 = 1 - alpha;
    const float a0inv = 1/a0;

    biquad.setCoefficients(b0*a0inv, b1*a0inv, b2*a0inv, 1.0f, a1*a0inv, a2*a0inv);
}


