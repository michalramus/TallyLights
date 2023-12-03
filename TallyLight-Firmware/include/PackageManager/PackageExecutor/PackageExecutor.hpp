/**
 * @file PackageExecutor.hpp
 * @brief Execute parsed packages
 */

#include <PackageManager/Package.hpp>

#include <cstdint>

#pragma once

/**
 * @brief Set the Tsl Address
 * 
 * @param address The address of the device
 */
void PackageExecutor_SetTslAddress(uint16_t address);

/**
 * @brief Execute a package
 * 
 * @param package The package to execute
 */
void PackageExecutor_Execute(Package package);
