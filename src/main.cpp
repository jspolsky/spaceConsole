
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
  OLED::status(0, "Space Console 2.0");
}

void loop()
{

  char s[16];

  uint32_t absolute;

  Encoder::loop(absolute);
  uint8_t brightness = (absolute / 4);
  //Led::setBrightness(brightness);

  Led::loop();

  EVERY_N_MILLISECONDS(1000)
  {
    itoa(Led::getFPS(), s, 10);
    OLED::status(1, s);
    //    Serial.println(absolute);
  }
}
