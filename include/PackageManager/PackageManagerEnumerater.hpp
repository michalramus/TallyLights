/**
 * @file PackageManagerEnumerater.hpp
 * @brief All enumerates used in the PackageManager module
 */


#pragma once

/**
 * @enum PackageManager_PackageTypes
 * @brief In what protocol was the packet sent
    */
enum class PackageManager_PackageTypes
{
    tsl3_1,
    tsl5_0,
    config,
    incorrect,
};

/**
 * @enum PackageManager_LedBrightness 
 * @brief Predefined brightnesses for the led
 */
enum class PackageManager_LedBrightness
{
    off = 0,
    oneSeventh = 36,
    half = 128,
    full = 255,
};

const char CONFIG_PACKAGE_BEGIN[] = {'C', 'O', 'N', 'F', '+'}; //all config packages begin with this
