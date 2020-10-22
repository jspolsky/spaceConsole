
#include <Arduino.h>
#include <OctoWS2811.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "Teensy4Controller.h"
#include "Util.h"
#include "led.h"

namespace Led
{

  //
  // Which of the physical LEDs that are present are going to
  // get patterns drawn on them?
  //

  const uint32_t FIRST_LED = 0;  // LAB
  const uint32_t NUM_LEDS = 300; // LAB

  // const uint32_t FIRST_LED = 400;  // ANTENNA
  // const uint32_t NUM_LEDS = 800;   // ANTENNA

  //
  // Physical arrangement of LEDs
  //

  // Pins 25-40 go out to the antenna. Each group of four sequential pins
  // represent one line from the bottom to the top, so, for example, pins 25 - 28
  // are a single line of 1200 LEDs, addressed as pixels[0..1199].
  //
  // Pin 14 is for RGB pushbuttons on the control panel.

  const int numPins = 17;
  byte pinList[numPins] = {25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 14};
  const int ledsPerStrip = 300;
  CRGB pixels[numPins * ledsPerStrip];

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

    FastLED.setBrightness(255);
    //FastLED.setCorrection(TypicalLEDStrip);
    //FastLED.setTemperature(DirectSunlight);
    FastLED.addLeds(pcontroller, pixels, numPins * ledsPerStrip);
  }

  void loop()
  {
    fnOneSecondSweep();
    //fnMondrian();
    //fnPaletteTester();
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

  void All4Strips(uint32_t i, CRGB color)
  {
    pixels[i + FIRST_LED] =
        pixels[i + FIRST_LED + 1200] =
            pixels[i + FIRST_LED + 2400] =
                pixels[i + FIRST_LED + 3600] = color;
  }

  //
  // for now, all functions named "fn" are loop() implementations. Each one implements a different
  // pattern / style / gene.
  //

  void fnOneSecondSweep()
  {

    static uint32_t split = 0;
    static uint32_t last_split = 0;
    static uint8_t hue = random8();

    last_split = split;
    split = (millis() % 1000) * NUM_LEDS / 1000;

    if (last_split > split)
    {
      // finish the top
      for (uint32_t i = last_split; i < NUM_LEDS; i++)
      {
        All4Strips(i, CHSV(hue, 255, 128));
      }

      hue += random(27, 102);
    }

    for (uint32_t i = 0; i < split; i++)
    {
      All4Strips(i, CHSV(hue, 255, 128));
    }

    pixels[4800] =
        pixels[4801] = CHSV(hue, 255, 128); // 4800 is the first button ... for now
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
        All4Strips(i, CHSV(hue, 255, 128));
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

    for (uint32_t i = 0; i < 300; i++)
      All4Strips(i, CRGB(0x444444));

    for (uint32_t i = 0; i < 60; i++)
      All4Strips(i, CHSV(baseHue, 255, 64)); // orig
    for (uint32_t i = 60; i < 120; i++)
      All4Strips(i, CHSV(baseHue, 255, 128)); // brighter
    for (uint32_t i = 120; i < 180; i++)
      All4Strips(i, CHSV(baseHue, 255, 192)); // brighter!
    for (uint32_t i = 180; i < 240; i++)
      All4Strips(i, CHSV(baseHue, 192, 212)); // desaturate
    for (uint32_t i = 240; i < 300; i++)
      All4Strips(i, CHSV(baseHue + 128, 255, 64)); // opposite

    All4Strips(chaser, CRGB(0x444444));
    All4Strips(chaser, CRGB(0x444444));
    All4Strips(chaser, CRGB(0x444444));

    EVERY_N_MILLISECONDS(10)
    {
      baseHue++;
      chaser = chaser + 1;
      if (chaser > 297)
        chaser = 0;
    }
  }

}; // namespace Led