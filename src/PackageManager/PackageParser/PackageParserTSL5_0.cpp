/**
 * @file PackageParserTSL5_0.cpp
 * @brief Implementation of PackageParserTSL5_0.hpp
 */

#include "PackageManager/PackageParser/PackageParserTSL5_0.hpp"


/**
 * @brief Resolves led mode from raw value from package
 *
 * @param rawModeValue
 * @return LedDriver_LedModes
 */
LedDriver_LedModes PackageManager_ResolveLedModeTSL5_0(uint8_t rawModeValue);


Package PackageManager_ParsePackageTSL5_0(uint8_t package[], uint16_t packageSize)
{
    Package packageParsed;

    //address
    packageParsed.address = package[7] << 8 | package[6];

    //led enable
    if ((package[8] & 0b11) > 0)
    {
        packageParsed.backLedEnable = true;
    }
    else
    {
        packageParsed.backLedEnable = false;
    }

    if ((package[8] & 0b1100) > 0)
    {
        packageParsed.frontLedEnable = true;
    }
    else
    {
        packageParsed.frontLedEnable = false;
    }

    //led mode
    packageParsed.frontLedMode = PackageManager_ResolveLedModeTSL5_0(package[8] & 0b1100 >> 2);
    packageParsed.backLedMode = PackageManager_ResolveLedModeTSL5_0(package[8] & 0b11);

    //brightness
    switch ((package[8] & 0b11000000) >> 6)
    {
        case 0:
            packageParsed.brightness = LedDriver_LedBrightness::off;
            break;
        case 1:
            packageParsed.brightness = LedDriver_LedBrightness::oneSeventh;
            break;
        case 2:
            packageParsed.brightness = LedDriver_LedBrightness::half;
            break;
        case 3:
            packageParsed.brightness = LedDriver_LedBrightness::full;
            break;

        default:
            packageParsed.brightness = LedDriver_LedBrightness::full;
            break;
    }

    return packageParsed;
}

LedDriver_LedModes PackageManager_ResolveLedModeTSL5_0(uint8_t rawModeValue)
{
    switch (rawModeValue)
    {
        case 0:
            return LedDriver_LedModes::off;
            break;
        case 1:
            return LedDriver_LedModes::live;
            break;
        case 2:
            return LedDriver_LedModes::preview;
            break;
        case 3:
            return LedDriver_LedModes::contentLaterUse;
            break;
        default:
            return LedDriver_LedModes::off;
            break;
    }
}
