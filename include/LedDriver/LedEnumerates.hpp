/**
 * @file LedEnumerates.hpp
 * @brief All enumerates used by the LedDriver module
 */

#pragma once

/**
 * @enum LedDriver_LedModes
 * @brief Possible modes that the LEDs can be in
 */
enum class LedDriver_LedModes
{
    off,
    preview,
    live,
    contentLaterUse,
    wifiNotConnected,
    configMode,
};

/**
 * @enum LedDriver_LedSegments
 * @brief Led segments that can be controlled
 */
enum class LedDriver_LedSegments
{
    front,
    back,
};

/**
 * @enum LedDriver_LedBrightness 
 * @brief Predefined brightnesses for the led
 */
enum class LedDriver_LedBrightness
{
    off = 0,
    oneSeventh = 36,
    half = 128,
    full = 255,
};
