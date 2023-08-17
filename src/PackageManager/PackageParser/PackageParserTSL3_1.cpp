/**
 * @file PackageParserTSL3_1.cpp
 * @brief Implementation of PackageParserTSL3_1.hpp
 */

#include "PackageManager/PackageParser/PackageParserTSL3_1.hpp"

Package PackageManager_ParsePackageTSL3_1(uint8_t package[])
{
    Package packageParsed;

    packageParsed.address = package[0] & 0x7F;

    switch ((package[1] & 0b11))
    {
        case 0:
            packageParsed.frontLedEnable = false;
            packageParsed.backLedEnable = false;
            packageParsed.frontLedMode = LedDriver_LedModes::off;
            packageParsed.backLedMode = LedDriver_LedModes::off;
            break;

        case 1:
            packageParsed.frontLedEnable = true;
            packageParsed.backLedEnable = true;
            packageParsed.frontLedMode = LedDriver_LedModes::preview;
            packageParsed.backLedMode = LedDriver_LedModes::preview;
            break;

        case 2:
            packageParsed.frontLedEnable = true;
            packageParsed.backLedEnable = true;
            packageParsed.frontLedMode = LedDriver_LedModes::live;
            packageParsed.backLedMode = LedDriver_LedModes::live;
            break;

        default:
            packageParsed.frontLedEnable = false;
            packageParsed.backLedEnable = false;
            packageParsed.frontLedMode = LedDriver_LedModes::off;
            packageParsed.backLedMode = LedDriver_LedModes::off;
            break;
    }

    // brightness
    switch ((package[1] & 0b110000) >> 4)
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
