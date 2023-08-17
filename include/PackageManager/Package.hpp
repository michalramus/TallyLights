/**
 * @file Package.hpp
 * @brief Package that was parsed 
 */

#include "PackageManagerEnumerater.hpp"
#include "LedDriver/LedEnumerates.hpp"

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
    LedDriver_LedModes mode;
    PackageManager_LedBrightness brightness;
};

