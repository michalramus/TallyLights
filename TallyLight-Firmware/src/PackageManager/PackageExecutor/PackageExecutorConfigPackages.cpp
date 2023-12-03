/**
 * @file PackageExecutorConfigPackages.cpp
 * @brief Implementation of the PackageExecutorConfigPackages 
 */

#include "PackageManager/PackageExecutor/PackageExecutorConfigPackages.hpp"
#include "PackageManager/PackageManagerEnumerater.hpp"
#include "PackageManager/PackageExecutor/PackageExecutor.hpp"

#include "LittleFS/LittleFSManager.hpp"
#include "LittleFS/LittleFSFileList.hpp"

#include "BatteryReader/BatteryReader.hpp"

#include "LedDriver/LedDriver.hpp"

#include "OTA/OTA.hpp"

String PackageExecutor_ExecuteConfigPackage(char package[])
{
    String convertedPackage = String(package);
    convertedPackage = convertedPackage.substring(sizeof(CONFIG_PACKAGE_BEGIN));

    if (convertedPackage.indexOf("BAT") != -1)
    {
        return BatteryReader_readBatteryVoltageString();
    }

    if (convertedPackage.indexOf("GetTSLAddress") != -1)
    {
        Serial.println("Reading TSL Address");
        uint8_t tslAddressBuffer[2];
        if (LittleFSManager_readFileContent(LittleFS_FileList.tslAddressFile, tslAddressBuffer, 2) == -1)
        {
            return "TSLAddress not found - please set it first";
        }

        uint16_t tslAddress = (tslAddressBuffer[0] << 8) | tslAddressBuffer[1];

        return String("TSL Address: " + String(tslAddress));
    }

    if (convertedPackage.indexOf("SetTSLAddress+") != -1)
    {
        convertedPackage = convertedPackage.substring(convertedPackage.indexOf("+") + 1);
        uint16_t tslAddress = convertedPackage.toInt();

        PackageExecutor_SetTslAddress(tslAddress);

        uint8_t tslAddressConv[2]; 
        tslAddressConv[0] = tslAddress >> 8;
        tslAddressConv[1] = tslAddress & 0xFF;

        LittleFSManager_replaceFileContent(LittleFS_FileList.tslAddressFile, tslAddressConv, 2);

        Serial.println("TSL Address changed to: " + String(tslAddress));
        return String("TSL Address changed to: " + String(tslAddress));
    }

    if (convertedPackage.indexOf("OTAMode") != -1)
    {
        LedDriver_setBrightness(255, LedDriver_LedSegments::back);
        LedDriver_setMode(LedDriver_LedModes::configMode);
        OTA_enterIntoOTA();

        return "OTA Mode";
    }

    Serial.println("Incorrect command");
    return "Incorrect command";
}
