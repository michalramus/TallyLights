/**
 * @file OTA.hpp
 * @brief 
 */

#include <cstdint>

#pragma once

/**
 * @brief Enter into OTA mode
 * @param port 
 * @warning This function is infinite
 */
void OTA_enterIntoOTA(uint16_t port = 8266, const char *password = "TallyMAR_OTA");
