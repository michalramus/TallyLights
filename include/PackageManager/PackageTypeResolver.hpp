/**
 * @file PackageTypeResolver.hpp
 * @brief Resolves package type
 */

#include "PackageManagerEnumerater.hpp"

#include <cstdint>

#pragma once

/**
 * @brief Resolves package type
 * 
 * @param package 
 * @param packageSize 
 * @return PackageManager_PackageTypes 
 */
PackageManager_PackageTypes PackageManager_ResolvePackageType(uint8_t package[], uint8_t packageSize);
