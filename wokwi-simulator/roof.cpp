#define FASTLED_INTERNAL
#include <FastLED.h>
#include "roof.h"

namespace Roof
{

#define NUM_LEDS 1200

// LEDs pin
#define DATA_PIN 3
// LED brightness
#define BRIGHTNESS 255


// Define the array of leds
CRGB leds[NUM_LEDS];


  void setup()
  {
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
  }

  void loop()
  {

    uint32_t base = 75;
    const uint32_t d1 = 41;
    const uint32_t d2 = 34;

    while (base <= 975)
    {

      uint8_t brightness = 255;
      CRGB col = CRGB::White;

      for (uint32_t i = base; i < base + d1; i++)
      {
        leds[i] = col.scale8(brightness);
        brightness -= 4;
      }

      brightness = 255;
      col = CRGB::Red;

      for (uint32_t i = base + d1; i < base + d1 + d2; i++)
      {
        leds[i] = col.scale8(brightness);
        brightness -= 4;
      }

      base += d1 + d2;

      brightness = 255;
      col = CRGB::Green;

      for (uint32_t i = base; i < base + d1; i++)
      {
        leds[i] = col.scale8(brightness);
        brightness -= 4;
      }

      brightness = 255;
      col = CRGB::Blue;
      for (uint32_t i = base + d1; i < base + d1 + d2; i++)
      {
        leds[i] = col.scale8(brightness);
        brightness -= 4;
      }

      base += d1 + d2;
      brightness = 255;
      col = CRGB::Magenta;

      for (uint32_t i = base; i < base + d1; i++)
      {
        leds[i] = col.scale8(brightness);
        brightness -= 4;
      }

      brightness = 255;
      col = CRGB::Cyan;

      for (uint32_t i = base + d1; i < base + d1 + d2; i++)
      {
        leds[i] = col.scale8(brightness);
        brightness -= 4;
      }

      base += 150;
    }


    FastLED.delay(10);
  }
}
