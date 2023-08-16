/**
 * @file LedSegment.hpp
 * @brief Characterize current state of a segment of LEDs
 */

#include <FastLED.h>
#include <cstdint>

#pragma once

/**
 * @struct LedSegment
 * @brief Characterize current state of a segment of LEDs
 */
struct LedSegment
{
    uint16_t beginIndex;
    uint16_t endIndex;
    uint8_t brightness = 255;
    CRGB color = CRGB(0, 0, 0); ///< not scaled by brightness
};
