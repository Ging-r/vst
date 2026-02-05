//
// Created by Viktor Lindström on 2026-02-05.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../src/dsp/LowPassFilter.h"

TEST_CASE("Lowpass passes DC") {
    LowPassFilter f;
    f.prepare(48000.0f);
    f.setCutoff(1000.0f);
    f.setResonance(0.707f);

    float y = 0.0f;
    for (int i = 0; i < 5000; ++i)
        y = f.process(1.0f);

    REQUIRE(y == Approx(1.0f).margin(1e-3f));
}
