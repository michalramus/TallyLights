/**
 * @file PackageTypeResolver.cpp
 * @brief Implementation of PackageTypeResolver.hpp
 */

#include <Arduino.h>

#include "PackageManager/PackageTypeResolver.hpp"

#include "PackageManager/PackageManagerEnumerater.hpp"

PackageManager_PackageTypes PackageManager_ResolvePackageType(uint8_t package[], uint8_t packageSize)
{
    if(packageSize <=2)
    {
        return PackageManager_PackageTypes::incorrect;
    }

    if (packageSize > sizeof(CONFIG_PACKAGE_BEGIN)/sizeof(CONFIG_PACKAGE_BEGIN[0])) //config package
    {
        bool isConfigPackage = true;

        for (uint16_t i = 0; i < sizeof(CONFIG_PACKAGE_BEGIN)/sizeof(CONFIG_PACKAGE_BEGIN[0]); i++)
        {
            if (package[i] != CONFIG_PACKAGE_BEGIN[i])
            {
               isConfigPackage = false;
               break;
            }
        }

        if (isConfigPackage)
        {
            Serial.println("Config package");
            return PackageManager_PackageTypes::config;
        }
    }

    if ((package[1] << 8 | package[0]) == (packageSize - 2)) //tsl 5.0 - check length of package -> description in protocol docs
    {
        Serial.println("TSL 5.0 package");
        return PackageManager_PackageTypes::tsl5_0;
    }
    
    if (((package[0] >> 7) == 1) && (package[1] >> 6) == 0) // tsl 3.1 - check const bits -> description in protocol docs
    {
        Serial.println("TSL 3.1 package");
        return PackageManager_PackageTypes::tsl3_1;
    }

    Serial.println("Incorrect package - ignored");
    return PackageManager_PackageTypes::incorrect;
}
