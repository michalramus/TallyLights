/**
 * @file LedDriver.hpp
 * @brief Driver for the LEDs
 */

#include "LedEnumerates.hpp"
#include "LedSegmentAddresses.hpp"

#include <FastLED.h>

#pragma once

/**
 * @brief Initialize the LedDriver module
 * 
 * @param leds 
 * @param config 
 */
void LedDriver_init(CRGB *leds, LedSegmentAddresses config);

/**
 * @brief Set the mode of the LEDs
 * 
 * @param mode 
 */
void LedDriver_setMode(LedDriver_LedModes mode);

/**
 * @brief Set the brightness of the LEDs
 * 
 * @param brightness 
 * @param segment 
 */
void LedDriver_setBrightness(uint8_t brightness, LedDriver_LedSegments segment);
