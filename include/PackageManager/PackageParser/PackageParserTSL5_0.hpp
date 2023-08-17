/**
 * @file PackageParserTSL5_0.hpp
 * @brief Package parser for TSL5.0
 */

#include "PackageManager/Package.hpp"

#include <cstdint>

#pragma once

/**
 * @brief Parses a package from TSL5.0
 * @param package The package to parse
 * @return The parsed package
 */
Package PackageManager_ParsePackageTSL5_0(uint8_t package[], uint16_t packageSize);
