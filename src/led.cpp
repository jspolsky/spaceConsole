
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

  const CRGB *rgColors;

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
    nextPattern();

    octo.begin();
    pcontroller = new CTeensy4Controller<RGB, WS2811_800kHz>(&octo);

    FastLED.setBrightness(128);
    //FastLED.setCorrection(TypicalLEDStrip);
    //FastLED.setTemperature(DirectSunlight);
    FastLED.addLeds(pcontroller, rgbarray, numPins * ledsPerStrip);
  }

  void loop()
  {
    //fnOneSecondSweep();
    //fnMondrian();
    fnPaletteTester();
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

  void nextPattern()
  {
    uint8_t ixPalette = random8() % NUM_PALETTES;
    rgColors = rgPalettes[ixPalette];

    dbgprintf("New palette %d\n", ixPalette);
  }

  void All4Strips(uint32_t i, CRGB color)
  {
    rgbarray[i] =
        rgbarray[i + 1200] =
            rgbarray[i + 2400] =
                rgbarray[i + 3600] = color;
  }

  //
  // for now, all functions named "fn" are loop() implementations. Each one implements a different
  // pattern / style / gene.
  //

  void fnOneSecondSweep()
  {
    uint32_t t = millis();
    uint8_t ixColor = ((t / 1200) % 4);
    CRGB color = rgColors[ixColor];
    uint32_t height = (t % 1200);

    if (height > 1190)
      height = 1199;

    for (uint32_t i = 0; i < height; i++)
    {
      All4Strips(i, color);
    }

    rgbarray[4800] = color;                       // 4800 is the first button ... for now
    rgbarray[4801] = rgColors[(ixColor + 1) % 4]; // 4801 is the next button ... preview the upcoming color :)
  }

  void fnMondrian()
  {
    static uint8_t ixColorFromPalette = 0; // current color, 0..3
    static uint32_t length = random(10, 100);
    static uint32_t startingPosition = random(900, 1200 - length);
    static uint8_t hue = random(0, 255);

    for (uint32_t i = 900; i < 1200; i++)
    {
      if (i >= startingPosition && i < (startingPosition + length))
      {
        All4Strips(i, rgColors[ixColorFromPalette]);
      }
      else
      {
        All4Strips(i, CRGB::Black);
      }
    }

    EVERY_N_MILLISECONDS(100)
    {
      ixColorFromPalette = (ixColorFromPalette + 1) % 4;
      length = random(10, 100);
      startingPosition = random(900, 1200 - length);
      hue = random(0, 255);
    }
  }

  void fnPaletteTester()
  {
    static uint8_t baseHue = 0;
    static uint32_t chaser = 0;

    for (uint32_t i = 900; i < 1200; i++)
      All4Strips(i, CRGB(0x444444));

    for (uint32_t i = 900; i < 960; i++)
      All4Strips(i, CHSV(baseHue, 255, 64)); // orig
    for (uint32_t i = 960; i < 1020; i++)
      All4Strips(i, CHSV(baseHue, 255, 128)); // brighter
    for (uint32_t i = 1020; i < 1080; i++)
      All4Strips(i, CHSV(baseHue, 255, 192)); // brighter!
    for (uint32_t i = 1080; i < 1140; i++)
      All4Strips(i, CHSV(baseHue, 192, 212)); // desaturate
    for (uint32_t i = 1140; i < 1200; i++)
      All4Strips(i, CHSV(baseHue + 128, 255, 64)); // opposite

    All4Strips(chaser + 900, CRGB(0x444444));
    All4Strips(chaser + 901, CRGB(0x444444));
    All4Strips(chaser + 902, CRGB(0x444444));

    EVERY_N_MILLISECONDS(10)
    {
      baseHue++;
      chaser = chaser + 1;
      if (chaser > 297)
        chaser = 0;
    }
  }

}; // namespace Led