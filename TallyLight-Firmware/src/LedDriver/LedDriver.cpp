/**
 * @file LedDriver.cpp
 * @brief Implementation of the LedDriver module
 */

#include "LedDriver/LedDriver.hpp"

#include "LedDriver/LedColors.hpp"
#include "LedDriver/LedEnumerates.hpp"
#include "LedDriver/LedSegment.hpp"
#include "LedDriver/LedSegmentAddresses.hpp"

#include <FastLED.h>

//---------------------------- variables ----------------------------
CRGB *leds = nullptr;

LedSegment frontSegment = LedSegment();
LedSegment backSegment = LedSegment();

//---------------------------- functions prototypes ----------------------------
void LedDriver_updateSegment(LedSegment frontSegment, LedSegment backSegment);

//---------------------------- functions ----------------------------

void LedDriver_init(CRGB *leds, LedSegmentAddresses config)
{
    ::leds = leds;

    frontSegment.beginIndex = config.frontBeginIndex;
    frontSegment.endIndex = config.frontEndIndex;
    backSegment.beginIndex = config.backBeginIndex;
    backSegment.endIndex = config.backEndIndex;

    LedDriver_setMode(LedDriver_LedModes::off);
    Serial.println("LedDriver initialized");
}

void LedDriver_setMode(LedDriver_LedModes mode)
{
    switch (mode)
    {
        case LedDriver_LedModes::off:
            frontSegment.color = LedDriver_offColor;
            backSegment.color = LedDriver_offColor;
            LedDriver_updateSegment(frontSegment, backSegment);
            Serial.println("LedMode: off");
            break;

        case LedDriver_LedModes::preview:
            frontSegment.color = LedDriver_previewColor;
            backSegment.color = LedDriver_previewColor;
            LedDriver_updateSegment(frontSegment, backSegment);
            Serial.println("LedMode: preview");
            break;

        case LedDriver_LedModes::live:
            frontSegment.color = LedDriver_liveColor;
            backSegment.color = LedDriver_liveColor;
            LedDriver_updateSegment(frontSegment, backSegment);
            Serial.println("LedMode: live");
            break;

        case LedDriver_LedModes::contentLaterUse:
            frontSegment.color = LedDriver_contentLaterUseColor;
            backSegment.color = LedDriver_contentLaterUseColor;
            LedDriver_updateSegment(frontSegment, backSegment);
            Serial.println("LedMode: contentLaterUse");
            break;

        case LedDriver_LedModes::wifiNotConnected:
            frontSegment.color = LedDriver_offColor;
            backSegment.color = LedDriver_wifiNotConnectedColor;
            LedDriver_updateSegment(frontSegment, backSegment);
            Serial.println("LedMode: wifiNotConnected");
            break;

        case LedDriver_LedModes::configMode:
            frontSegment.color = LedDriver_offColor;
            backSegment.color = LedDriver_configModeColor;
            LedDriver_updateSegment(frontSegment, backSegment);
            Serial.println("LedMode: configMode");
            break;


        default:
            break;
    }
}

void LedDriver_setBrightness(uint8_t brightness, LedDriver_LedSegments segment)
{
    switch (segment)
    {
        case LedDriver_LedSegments::front:
            frontSegment.brightness = brightness;
            LedDriver_updateSegment(frontSegment, backSegment);
            Serial.println("LedBrightness: front - " + String(brightness));
            break;
            
        case LedDriver_LedSegments::back:
            backSegment.brightness = brightness;
            LedDriver_updateSegment(frontSegment, backSegment);
            Serial.println("LedBrightness: back - " + String(brightness));
            break;

        default:
            break;
    }
}

//---------------------------- private functions ----------------------------

void LedDriver_updateSegment(LedSegment frontSegment, LedSegment backSegment)
{
    for (uint16_t i = frontSegment.beginIndex; i <= frontSegment.endIndex; i++)
    {
        leds[i] = frontSegment.color;
        leds[i].subtractFromRGB(255 - frontSegment.brightness);
    }

    for (uint16_t i = backSegment.beginIndex; i <= backSegment.endIndex; i++)
    {
        leds[i] = backSegment.color;
        leds[i].subtractFromRGB(255 - backSegment.brightness);
    }

    FastLED.show();
}
