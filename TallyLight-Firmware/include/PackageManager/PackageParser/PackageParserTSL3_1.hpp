/**
 * @file PackageParserTSL3_1.hpp
 * @brief Package parser for TSL3.1
 */

#include "PackageManager/Package.hpp"

#include <cstdint>

#pragma once

/**
 * @brief Parses a package from TSL3.1
 * @param package The package to parse
 * @return The parsed package
 */
Package PackageManager_ParsePackageTSL3_1(uint8_t package[]);
