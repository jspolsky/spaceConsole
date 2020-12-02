
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

  uint8_t brightnessOptions[15] = {2, 4, 8, 12, 16, 24, 32, 48, 56, 64, 72, 96, 128, 168, 255};
  static uint8_t brightnessIndex = 14;

  enum mode_t
  {
    mode_singleColor,
    mode_genetic,
    mode_testPattern
  };

  static mode_t mode = mode_testPattern;

  static CRGB singleColorMode = CRGB::Black;
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
  // Pin 14 is for five RGB pushbuttons on the control panel.

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

    FastLED.setBrightness(brightnessOptions[brightnessIndex]);

    //FastLED.setCorrection(TypicalLEDStrip);
    //FastLED.setTemperature(DirectSunlight);
    FastLED.addLeds(pcontroller, pixels, numPins * ledsPerStrip);
  }

  void loop()
  {
    if (power)
    {
      if (mode == mode_genetic)
      {
        // fnMondrian();
        fnOneSecondSweep();
        FastLED.show();
      }
      else if (mode == mode_singleColor)
      {
        FastLED.showColor(singleColorMode);
      }
      else if (mode == mode_testPattern)
      {
        fnTestPattern();
        FastLED.show();
      }
    }
    else
    {
      FastLED.showColor(0);
    }
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

    EVERY_N_MILLIS(10)
    {
      // optional variant adds a gentle little fade at the top
      fadeToBlackBy(pixels, 16 * ledsPerStrip, 8);
    }

    for (uint16_t i = 4800; i < 4805; i++)
      pixels[i] = CHSV(hue, 255, 128); // 4800 is the first button ... for now
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

  void fnTestPattern()
  {
    static uint8_t hue = 0;

    EVERY_N_MILLISECONDS(100)
    {
      hue = (hue + 1) & 0xFF;
    }

    for (int i = 0; i < 1200; i++)
    {
      pixels[i] = CHSV(32, 255, 196);
      pixels[i + 1200] = CHSV(96, 255, 196);
      pixels[i + 2400] = CHSV(160, 255, 196);
      pixels[i + 3600] = CHSV(224, 255, 196);
    }

    for (uint8_t level = 0; level < 4; level++) {

      uint8_t block_size = 6 * (level+1);
      uint16_t bounce_pos = cubicwave8((millis() / 32) & 0xFF) * (300 - block_size) / 256;

      for (uint16_t i = 0; i < block_size; i++) 
      {
        for (uint16_t j = 0; j < 4; j++) {

          if (bounce_pos + i < 300) {
            pixels[j*1200 + level*300 + bounce_pos + i] = (i % 6) ? CRGB::White: CRGB::Black;
          }

        }
      }      
      

      for (int i = 0; i < 4; i++)
      { 
        pixels[i*1200 + level*300] = pixels[i*1200 + level*300 +299] = CHSV(hue, 255, 255);
      }

    }
  }

  bool togglePower(void)
  {
    return (power = !power);
  }

  uint8_t brighter()
  {
    uint8_t result;

    brightnessIndex = min(14, brightnessIndex + 1);
    FastLED.setBrightness(result = brightnessOptions[brightnessIndex]);
    return result;
  }

  uint8_t dimmer()
  {
    uint8_t result;

    if (brightnessIndex > 0)
    {
      brightnessIndex--;
    }

    FastLED.setBrightness(result = brightnessOptions[brightnessIndex]);
    return result;
  }

  void geneticAlgorithm()
  {
    mode = mode_genetic;
  }

  void testPattern()
  {
    mode = mode_testPattern;
  }

  void setSolidColor(CRGB rgb)
  {
    singleColorMode = rgb;
    mode = mode_singleColor;
  }

}; // namespace Led