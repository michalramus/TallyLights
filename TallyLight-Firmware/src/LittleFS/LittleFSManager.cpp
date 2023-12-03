/**
 * @file LittleFSManager.cpp
 * @brief Manages the LittleFS
 */

#include <LittleFS.h>

#include "LittleFS/LittleFSManager.hpp"

bool LittleFSManager_init()
{
    bool initialized = LittleFS.begin();

    if (!initialized)
    {
        Serial.println("LittleFS initialization failed.");
        return false;
    }

    Serial.println("LittleFS initialized.");
    return true;
}

int32_t LittleFSManager_readFileContent(const char *filename, uint8_t *buffer, uint16_t bufferSize)
{
    Serial.println("Reading file " + String(filename));
    File file = LittleFS.open(filename, "r");

    if (!file)
    {
        Serial.println("File " + String(filename) + " not found");
        return -1;
    }

    int32_t fileSize = file.size();

    if (fileSize > bufferSize)
    {
        Serial.println("Buffer too small");
        return -1;
    }

    file.read(buffer, fileSize);
    file.close();

    Serial.println("Reading completed");
    return fileSize;
}

void LittleFSManager_replaceFileContent(const char *filename, const uint8_t *buffer, uint16_t bufferSize)
{
    Serial.println("Writing to file " + String(filename));
    File file = LittleFS.open(filename, "w");

    file.write(buffer, bufferSize);
    file.close();
    Serial.println("Writing completed");
}
