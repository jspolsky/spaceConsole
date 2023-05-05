#define FASTLED_INTERNAL
#include <FastLED.h>
#include "roof.h"
#include "jetpack.h"

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

  void loop()
  {

    Jetpack::loop();    

    FastLED.delay(1);
  }
}
