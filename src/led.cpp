
#include <Arduino.h>
#include <OctoWS2811.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "Teensy4Controller.h"
#include "Util.h"
#include "led.h"
#include "palettes.h"

namespace Led
{

  const CRGB *rgColors = rgPalettes[0];

  const int numPins = 17;
  byte pinList[numPins] = {25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 14};
  const int ledsPerStrip = 300;
  CRGB rgbarray[numPins * ledsPerStrip];

  // These buffers need to be large enough for all the pixels.
  // The total number of pixels is "ledsPerStrip * numPins".
  // Each pixel needs 3 bytes, so multiply by 3.  An "int" is
  // 4 bytes, so divide by 4.  The array is created using "int"
  // so the compiler will align it to 32 bit memory.
  DMAMEM int displayMemory[ledsPerStrip * numPins * 3 / 4];
  int drawingMemory[ledsPerStrip * numPins * 3 / 4];
  OctoWS2811 octo(ledsPerStrip, displayMemory, drawingMemory, WS2811_RGB | WS2811_800kHz, numPins, pinList);
  CTeensy4Controller<RGB, WS2811_800kHz> *pcontroller;

  void setup()
  {
    octo.begin();
    pcontroller = new CTeensy4Controller<RGB, WS2811_800kHz>(&octo);

    FastLED.setBrightness(128);
    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setTemperature(DirectSunlight);
    FastLED.addLeds(pcontroller, rgbarray, numPins * ledsPerStrip);
  }

  void loop()
  {

    uint32_t t = millis();
    uint8_t ixColor = ((t / 1200) % 512);
    CRGB color = rgColors[ixColor];
    uint32_t height = (t % 1200);

    if (height > 1190)
      height = 1199;

    for (uint32_t i = 0; i < height; i++)
    {
      rgbarray[i] = color;
      rgbarray[i + 1200] = color;
      rgbarray[i + 2400] = color;
      rgbarray[i + 3600] = color;
    }
    // for (uint32_t i = height; i < 1200; i++)
    // {
    //   rgbarray[i] = CRGB::White;
    //   rgbarray[i + 1200] = CRGB::White;
    //   rgbarray[i + 2400] = CRGB::White;
    //   rgbarray[i + 3600] = CRGB::White;
    // }

    rgbarray[4800] = color;                         // 4800 is the first button ... for now
    rgbarray[4801] = rgColors[(ixColor + 1) % 512]; // 4801 is the next button ... preview the upcoming color :)

    FastLED.show();
  }

  void setBrightness(uint8_t brightness)
  {
    FastLED.setBrightness(brightness);
  }

  uint16_t getFPS()
  {
    return FastLED.getFPS();
  }

}; // namespace Led