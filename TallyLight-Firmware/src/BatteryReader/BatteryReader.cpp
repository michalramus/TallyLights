#include "BatteryReader/BatteryReader.hpp"

uint8_t _batteryPin;
float _maxVoltage;
float _offSet;

void BatteryReader_init(uint8_t batteryPin, float maxVoltage, float offSet) 
{
    _batteryPin = batteryPin;
    _maxVoltage = maxVoltage;
    _offSet = offSet;
    Serial.println("BatteryReader: pin " + String(_batteryPin) + ", maxVoltage " + String(_maxVoltage) + ", offSet " + String(_offSet));

    pinMode(_batteryPin, INPUT);
}

float BatteryReader_readBatteryVoltage()
{
    float voltage = analogRead(_batteryPin) * _maxVoltage / 1023.0 + _offSet;
    Serial.print("Battery voltage: ");
    Serial.println(voltage);
    return voltage;
}

String BatteryReader_readBatteryVoltageString()
{
    String voltage = String(BatteryReader_readBatteryVoltage(), 2);
    return voltage + "V";
}
