/**
 * @file PackageExecutor.cpp
 * @brief Implementation of PackageExecutor.hpp
 */

#include "PackageManager/PackageExecutor/PackageExecutor.hpp"

#include "LedDriver/LedDriver.hpp"

//--------------------variables--------------------//
uint16_t _address = 0;
//--------------------functions--------------------//

void PackageExecutor_SetTslAddress(uint16_t address = 0) 
{
    _address = address;
    Serial.println("TSL address set to: " + String(address));
}

void PackageExecutor_Execute(Package package)
{
    if (package.address != _address)
        return;


    if (package.backLedEnable)
    {
        LedDriver_setBrightness((uint8_t)package.brightness, LedDriver_LedSegments::back);
        LedDriver_setMode(package.backLedMode);
    }
    else
    {
        LedDriver_setBrightness(0, LedDriver_LedSegments::back);
    }

    if (package.frontLedEnable)
    {
        LedDriver_setBrightness((uint8_t)package.brightness, LedDriver_LedSegments::front);
        LedDriver_setMode(package.frontLedMode);
    }
    else
    {
        LedDriver_setBrightness(0, LedDriver_LedSegments::front);
    }
}
