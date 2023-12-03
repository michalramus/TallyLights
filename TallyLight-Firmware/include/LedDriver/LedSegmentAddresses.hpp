/**
 * @file LedSegmentAddresses.hpp
 * @brief Struct that holds the indices of the LEDs on the strip needed for the initialization of the LedDriver
*/

#include <cstdint>

#pragma once

/**
 * @struct LedSegmentAddresses
 * @brief Struct that holds the indices of the LEDs on the strip needed for the initialization of the LedDriver
 * @details Indexing starts at 0
 */
struct LedSegmentAddresses
{
    LedSegmentAddresses (uint16_t frontBeginIndex, uint16_t frontEndIndex, uint16_t backBeginIndex, uint16_t backEndIndex) :
        frontBeginIndex(frontBeginIndex),
        frontEndIndex(frontEndIndex),
        backBeginIndex(backBeginIndex),
        backEndIndex(backEndIndex)
    {}

    uint16_t frontBeginIndex; 
    uint16_t frontEndIndex;
    uint16_t backBeginIndex;
    uint16_t backEndIndex;
};
