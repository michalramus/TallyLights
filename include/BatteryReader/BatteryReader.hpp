/**
 * @file BatteryReader.hpp
 * @brief Reads the battery voltage
 */

#include <Arduino.h>

#pragma once

/**
 * @brief Initializes the BatteryReader
 * 
 * @param batteryPin 
 * @param maxVoltage Voltage before voltage divider when after it voltage is 3.3V
 * @param offSet This value will be added to the voltage
 */
void BatteryReader_init(uint8_t batteryPin, float maxVoltage, float offSet);

/**
 * @brief Reads the battery voltage
 * 
 * @return float 
 */
float BatteryReader_readBatteryVoltage();

/**
 * @brief Reads the battery voltage and returns it as a string
 * 
 * @return String rounded to two decimal places
 */
String BatteryReader_readBatteryVoltageString();
