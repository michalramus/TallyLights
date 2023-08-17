/**
 * @file PackageExecutor.hpp
 * @brief Execute parsed packages
 */

#include <PackageManager/Package.hpp>

#include <cstdint>

#pragma once

/**
 * @brief Initialize the package executor
 * 
 * @param address The address of the device
 */
void PackageExecutor_Init(uint16_t address);

/**
 * @brief Execute a package
 * 
 * @param package The package to execute
 */
void PackageExecutor_Execute(Package package);
