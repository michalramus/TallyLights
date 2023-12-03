#include "BatteryReader/BatteryReader.hpp"

#include "LedDriver/LedDriver.hpp"

#include "LittleFS/LittleFSManager.hpp"
#include "LittleFS/LittleFSFileList.hpp"

#include "PackageManager/PackageExecutor/PackageExecutor.hpp"
#include "PackageManager/PackageExecutor/PackageExecutorConfigPackages.hpp"
#include "PackageManager/PackageParser/PackageParserTSL3_1.hpp"
#include "PackageManager/PackageParser/PackageParserTSL5_0.hpp"
#include "PackageManager/PackageTypeResolver.hpp"

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <FastLED.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>

// AP access data
String AP_SSID = "TallyLight-MAR-" + WiFi.macAddress();
const char *AP_PASS = "TallyMAR";

// leds
#define LEDS_PIN D1
#define NUM_LEDS 12
CRGB ledsArray[NUM_LEDS];

#define BUTTON_PIN D2

// udp
WiFiUDP udp;
uint16_t udpPort = 10373;
const uint16_t udpPacketMaxSize = 512;
uint8_t udpPacketBuffer[udpPacketMaxSize];
uint16_t udpPacketSize = 0;

void setup()
{
    Serial.begin(76800);

    // leds initialization
    FastLED.addLeds<WS2812B, LEDS_PIN, GRB>(ledsArray, NUM_LEDS);
    LedDriver_init(ledsArray, LedSegmentAddresses(3, 11, 0, 2));
    Serial.println("Leds initialized");

    // button initialization
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    Serial.println("Button initialized");

    // battery reader initialization
    BatteryReader_init(A0, 5.0, 0.0);

    // LittleFS initialization
    if (!LittleFSManager_init())
    {
        Serial.println("Critical error. Restarting...");
        LedDriver_setMode(LedDriver_LedModes::live);
        delay(1000);
        ESP.restart();
    }

    LedDriver_setMode(LedDriver_LedModes::preview);
    delay(1000);

    // wifi initialization
    if (digitalRead(BUTTON_PIN) == LOW)
    {
        // initialize wifi in AP mode
        Serial.println("WiFi AP mode");
        LedDriver_setMode(LedDriver_LedModes::configMode);

        WiFiManager wifiManager;
        wifiManager.startConfigPortal(AP_SSID.c_str(), AP_PASS);
    }
    else
    {
        // initialize wifi in STA mode
        Serial.println("WiFi STA mode");
        LedDriver_setMode(LedDriver_LedModes::wifiNotConnected);

        WiFi.begin();
        Serial.print("Connecting to WiFi");
        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print('.');
            delay(500);
        }
        Serial.println("");
    }


    LedDriver_setMode(LedDriver_LedModes::preview); // if mode change is not doubled, not working
    LedDriver_setMode(LedDriver_LedModes::preview);
    Serial.println("WiFi connected");

    delay(500);
    LedDriver_setMode(LedDriver_LedModes::off); // if mode change is not doubled, not working
    LedDriver_setMode(LedDriver_LedModes::off);

    // initialize udp
    udp.begin(udpPort);
    Serial.println("UDP initialized");

    // initialize PackageExecutor
    uint16_t tslAddress = 0;
    uint8_t tslAddressBuffer[2];
    if (LittleFSManager_readFileContent(LittleFS_FileList.tslAddressFile, tslAddressBuffer, 2) != -1)
    {
        tslAddress = (tslAddressBuffer[0] << 8) | tslAddressBuffer[1];
        PackageExecutor_SetTslAddress(tslAddress);
        Serial.println("TSL address: " + String(tslAddress));
    }
    else
    {
        Serial.println("TSL address not found, using default: 0");
        PackageExecutor_SetTslAddress(0);
        LedDriver_setMode(LedDriver_LedModes::contentLaterUse); // if mode change is not doubled, not working
        LedDriver_setMode(LedDriver_LedModes::contentLaterUse);
        delay(500);
    }

    LedDriver_setMode(LedDriver_LedModes::off); // if mode change is not doubled, not working
    LedDriver_setMode(LedDriver_LedModes::off);
}

void loop()
{
    // check wifi connection
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Wifi not connected");
        LedDriver_setMode(LedDriver_LedModes::wifiNotConnected);
        LedDriver_setBrightness(255, LedDriver_LedSegments::back);

        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print('.');
            WiFi.reconnect();
            delay(5000);
        }
        Serial.println("\nWifi connected");

        LedDriver_setMode(LedDriver_LedModes::off); // if mode change is not doubled, not working
        LedDriver_setMode(LedDriver_LedModes::off);
    }


    // UDP
    udpPacketSize = udp.parsePacket();
    if (udpPacketSize)
    {
        udpPacketSize = udp.read(udpPacketBuffer, udpPacketMaxSize);
        Serial.println("UDP packet received");

        Package package;
        String udpResponse;
        switch (PackageManager_ResolvePackageType(udpPacketBuffer, udpPacketSize))
        {
            case PackageManager_PackageTypes::tsl3_1:
                package = PackageManager_ParsePackageTSL3_1(udpPacketBuffer);
                PackageExecutor_Execute(package);

                break;

            case PackageManager_PackageTypes::tsl5_0:
                package = PackageManager_ParsePackageTSL5_0(udpPacketBuffer, udpPacketSize);
                PackageExecutor_Execute(package);

                break;
            case PackageManager_PackageTypes::config:

                udpResponse = PackageExecutor_ExecuteConfigPackage((char *)udpPacketBuffer);

                if (udpResponse != "")
                {
                    udpResponse += "\n";
                    udp.beginPacket(udp.remoteIP(), udp.remotePort());
                    udp.write(udpResponse.c_str());
                    udp.endPacket();
                }
                break;

            default:
                break;
        }

        udpPacketSize = 0; // reset packets
    }
}
