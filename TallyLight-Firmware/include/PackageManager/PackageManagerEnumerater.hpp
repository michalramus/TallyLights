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

const char CONFIG_PACKAGE_BEGIN[] = {'C', 'O', 'N', 'F', '+'}; //all config packages begin with this
