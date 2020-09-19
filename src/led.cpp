
#include <Arduino.h>
#include <OctoWS2811.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "Teensy4Controller.h"
#include "Util.h"
#include "led.h"

namespace Led
{

#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define YELLOW 0xFFFF00
#define PINK 0xFF1088
#define ORANGE 0xE05800
#define WHITE 0xFFFFFF
#define BROWN 0x654321

  uint32_t rgColors[8] = {WHITE, RED, GREEN, BLUE, YELLOW, PINK, ORANGE, BROWN};

  const int numPins = 8;
  byte pinList[numPins] = {33, 34, 35, 36, 37, 38, 39, 40};
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
    pcontroller = new CTeensy4Controller<RGB, WS2811_800kHz>(&octo, drawingMemory);

    FastLED.setBrightness(255);
    FastLED.addLeds(pcontroller, rgbarray, numPins * ledsPerStrip);
  }

  void loop(uint32_t encoder_pos)
  {

    uint32_t t = millis();
    uint8_t ixColor = ((t / 1200) % 8);
    uint32_t color = rgColors[ixColor];

    //
    // very simplistic dimming based on encoder_pos which is 0..1023
    //

    byte *rgbColor = (byte *)&color;

    for (int i = 0; i < 4; i++)
    {
      uint32_t original = rgbColor[i];
      uint32_t dimmed = original * encoder_pos / 1024;
      rgbColor[i] = (byte)(dimmed & 0xFF);
    }

    uint32_t height = (t % 1200);

    for (uint32_t i = 0; i < height; i++)
    {
      rgbarray[i] = color;
      rgbarray[i + 1200] = color;
    }
    for (uint32_t i = height; i < 1200; i++)
    {
      rgbarray[i] = CRGB::Black;
      rgbarray[i + 1200] = CRGB::Black;
    }

    FastLED.show();
  }

}; // namespace Led