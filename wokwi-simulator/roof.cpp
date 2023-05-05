#define FASTLED_INTERNAL
#include <FastLED.h>
#include "roof.h"

namespace Roof
{

  #define NUM_LEDS 1200

  // Define the array of leds
  CRGB leds[NUM_LEDS];

  const uint32_t ring_endpoints[6][4] =
  { { 225, 299, 525, 599 },
    { 150, 224, 450, 524 },
    { 75, 149, 375, 449 },
    { 825, 899, 1125, 1199 },
    { 750, 824, 1050, 1124 },
    { 675, 749, 975, 1049 }
  };

  void SetLed(uint32_t i, CRGB color) {
    leds[i] = color;
  }

  void SetRingColor(ring_t ring, CRGB color) {

    for (uint32_t i = ring_endpoints[ring][0]; i <= ring_endpoints[ring][1]; i++)
      SetLed(i, color);
    for (uint32_t i = ring_endpoints[ring][2]; i <= ring_endpoints[ring][3]; i++)
      SetLed(i, color);

  }
  
  void setup()
  {
    const uint32_t DATA_PIN = 3;
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(255);
  }

  void test()
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

  }

  void loop()
  {

    static uint8_t hue = 0;
    ring_t ring = 0;

    EVERY_N_MILLIS(10) {
      hue += 1;
    }

    SetRingColor(0, CHSV(hue, 255, 255));
    SetRingColor(1, CHSV(hue + 20, 255, 255));
    SetRingColor(2, CHSV(hue + 30, 255, 255));
    SetRingColor(3, CHSV(hue + 50, 255, 255));
    SetRingColor(4, CHSV(hue + 70, 255, 255));
    SetRingColor(5, CHSV(hue + 90, 255, 255));
    

    FastLED.delay(1);
  }
}
