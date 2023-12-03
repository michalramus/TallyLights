/**
 * @file LittleFSManager.hpp
 * @brief Manages the LittleFS
 */

#include <LittleFS.h>

#pragma once

/**
 * @brief Initializes the LittleFSManager
 *
 * @return true if LittleFSManager was initialized successfully
 */
bool LittleFSManager_init();

/**
 * @brief Reads content of file
 *
 * @param buffer
 * @return uint16_t Size of file or -1 if file not found
 */
int32_t LittleFSManager_readFileContent(const char *filename, uint8_t *buffer, uint16_t bufferSize);

/**
 * @brief Replace whole file content by buffer
 * If file not exists, it will be created
 *
 * @param buffer
 * @param bufferSize
 */
void LittleFSManager_replaceFileContent(const char *filename, const uint8_t *buffer, uint16_t bufferSize);
