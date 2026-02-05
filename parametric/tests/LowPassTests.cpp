//
// Created by Viktor Lindström on 2026-02-05.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <cmath>
#include <vector>
#include "../src/dsp/LowPassFilter.h"

// Helper to generate a sine wave chunk
std::vector<float> generateSineWave(float frequency, float sampleRate, int numSamples) {
    std::vector<float> buffer(numSamples);
    for (int i = 0; i < numSamples; ++i) {
        float t = i / sampleRate;
        buffer[i] = std::sin(2.0f * static_cast<float>(M_PI) * frequency * t);
    }
    return buffer;
}

TEST_CASE("LowPassFilter: Initialization and Parameters", "[filter]") {
    LowPassFilter filter;

    SECTION("Can prepare without crashing") {
        REQUIRE_NOTHROW(filter.prepare(44100.0f));
    }

    SECTION("Handles parameter updates safely") {
        filter.prepare(48000.0f);
        REQUIRE_NOTHROW(filter.setCutoff(20000.0f));
        REQUIRE_NOTHROW(filter.setResonance(5.0f));

        float out = filter.process(0.5f);
        REQUIRE_FALSE(std::isnan(out));
        REQUIRE_FALSE(std::isinf(out));
    }
}

TEST_CASE("LowPassFilter: Signal Processing", "[filter]") {
    LowPassFilter filter;
    float sampleRate = 44100.0f;
    filter.prepare(sampleRate);

    SECTION("DC Gain should be approx 1.0 (Transparency at 0Hz)") {
        // A low pass filter should let a constant signal pass through unchanged
        filter.setCutoff(1000.0f);
        filter.setResonance(0.707f);

        // Feed "1.0" for 1000 samples to let the filter settle
        float output = 0.0f;
        for(int i = 0; i < 1000; ++i) {
            output = filter.process(1.0f);
        }

        // Catch2 Approx wrapper for float comparison
        REQUIRE(output == Catch::Approx(1.0f).margin(0.01f));
    }

    SECTION("Should attenuate frequencies above cutoff") {
        float cutoff = 500.0f;
        float testFreq = 5000.0f; // Way above cutoff

        filter.setCutoff(cutoff);
        filter.setResonance(0.707f);

        auto sineWave = generateSineWave(testFreq, sampleRate, 1000);
        float maxAmplitude = 0.0f;

        for (float sample : sineWave) {
            float out = std::abs(filter.process(sample));
            if (out > maxAmplitude) maxAmplitude = out;
        }

        // The output amplitude should be significantly less than input (1.0)
        // At 10x cutoff, a 12dB/oct filter should be very quiet.
        REQUIRE(maxAmplitude < 0.2f);
    }

    SECTION("Silence should result in silence") {
        filter.prepare(44100.0f);
        for(int i=0; i<100; ++i) {
            REQUIRE(filter.process(0.0f) == Catch::Approx(0.0f).margin(0.0001f));
        }
    }
}