
#include <Arduino.h>
#include <OctoWS2811.h>

#include "led.h"

#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define YELLOW 0xFFFF00
#define PINK 0xFF1088
#define ORANGE 0xE05800
#define WHITE 0xFFFFFF
#define BROWN 0x654321

const int numPins = 8;
byte pinList[numPins] = {33, 34, 35, 36, 37, 38, 39, 40};

uint32_t rgColors[8] = {WHITE, RED, GREEN, BLUE, YELLOW, PINK, ORANGE, BROWN};

const int ledsPerStrip = 300;

// These buffers need to be large enough for all the pixels.
// The total number of pixels is "ledsPerStrip * numPins".
// Each pixel needs 3 bytes, so multiply by 3.  An "int" is
// 4 bytes, so divide by 4.  The array is created using "int"
// so the compiler will align it to 32 bit memory.
DMAMEM int displayMemory[ledsPerStrip * numPins * 3 / 4];
int drawingMemory[ledsPerStrip * numPins * 3 / 4];
const int config = WS2811_RGB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config, numPins, pinList);

void led_setup()
{
  leds.begin();
  leds.show();
}

void led_loop(uint32_t encoder_pos)
{

  uint32_t t = millis();
  uint8_t ixColor = ((t / 1200) % 8);
  uint32_t color = rgColors[ixColor];

  uint32_t height = (t % 1200);

  for (uint32_t i = 0; i < height; i++)
  {
    leds.setPixel(i, color);
    leds.setPixel(i + 1200, color);
  }
  for (uint32_t i = height; i < 1200; i++)
  {
    leds.setPixel(i, 0);
    leds.setPixel(i + 1200, 0);
  }

  leds.setPixel(encoder_pos, WHITE);
  leds.setPixel(encoder_pos + 1, WHITE);
  leds.setPixel(encoder_pos + 2, WHITE);

  leds.show();
}
