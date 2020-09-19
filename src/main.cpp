
#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "Util.h"
#include "led.h"
#include "encoder.h"
#include "oled.h"

char *itoa(int value, char *str, int base);

void setup()
{

  Util::setup();
  Led::setup();
  Encoder::setup();
  OLED::setup();
  OLED::status(0, "Space Console 1.0");
}

void loop()
{
  uint32_t absolute;
  Encoder::loop(absolute);
  Led::loop(absolute);

  char s[16];
  itoa(absolute, s, 10);
  EVERY_N_MILLISECONDS(500)
  {
    // updating the oled is super slow
    OLED::status(1, s);
  }
}
