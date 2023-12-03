/**
 * @file Package.hpp
 * @brief Package that was parsed
 */

#include "LedDriver/LedEnumerates.hpp"
#include "PackageManagerEnumerater.hpp"

#include <cstdint>

#pragma once

/**
 * @struct Package
 * @brief Package that was parsed
 */
struct Package
{
    uint16_t address;
    bool frontLedEnable;
    bool backLedEnable;
    LedDriver_LedModes frontLedMode;
    LedDriver_LedModes backLedMode;
    LedDriver_LedBrightness brightness;
};
