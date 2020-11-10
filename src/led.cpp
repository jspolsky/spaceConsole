
#include <Arduino.h>
#include <OctoWS2811.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "Teensy4Controller.h"
#include "Util.h"
#include "led.h"

namespace Led
{

  static bool power = true;

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
    if (power)
    {
      fnMondrian();
      FastLED.show();
    }
    else
    {
      FastLED.showColor(0);
    }
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

  // zap a nice color from bottom to top every second.
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

  // show little pulses of nice colors
  void fnMondrian()
  {
    static uint8_t hue = 0;

    // 200, 500, 2500, 5000 are interesting options
    EVERY_N_MILLIS(200)
    {
      hue = random8();
    }

    EVERY_N_MILLIS(200)
    {

      uint16_t runlength = random16(NUM_LEDS / 10, NUM_LEDS / 5);
      uint16_t start = random16(0, NUM_LEDS - 1 - runlength);

      CRGB color = CHSV(hue, 255, 192);
      for (uint32_t i = start; i < start + runlength; i++)
      {
        All4Strips(i, color);
      }
    }

    EVERY_N_MILLIS(10)
    {
      // so
      // the last param of fadeToBlackBy can be 255 for "crisp" mondrian
      // or any power of 2 below that, to get different effects.
      fadeToBlackBy(pixels, 16 * ledsPerStrip, 4);
    }
  }

  bool togglePower(void)
  {
    return (power = !power);
  }

  uint8_t brighter()
  {
    // TODO UNIMPLEMENTED
    return 127;
  }

  uint8_t dimmer()
  {
    // TODO UNIMPLEMENTED
    return 127;
  }

  void testPattern()
  {
    // TODO UNIMPLEMENTED
  }

  void setSolidColor(CRGB rgb)
  {
    // TODO UNIMPLEMENTED
  }

}; // namespace Led