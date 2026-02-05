//
// Created by Viktor Lindström on 2026-02-04.
//

#pragma once

class Biquad {
public:
    void setCoefficients(float b0_, float b1_, float b2_,
                         float a0_, float a1_, float a2_);

    float process(float x);

    void reset();

private:
    float b0{}, b1{}, b2{};
    float a0{}, a1{}, a2{};
    float s1{0.0f};
    float s2{0.0f};
};
