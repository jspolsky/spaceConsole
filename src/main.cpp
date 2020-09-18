
#include <Arduino.h>
#include "Util.h"
#include "led.h"
#include "encoder.h"
#include "oled.h"

void setup()
{

  Util::setup();
  Led::setup();
  Encoder::setup();
  OLED::setup();
  OLED::status(0, "Space Console 1.0");

  pinMode(13, OUTPUT);
}

void loop()
{
  uint32_t absolute;
  Encoder::loop(absolute);
  Led::loop(absolute);

  char s[16];
  itoa(absolute, s, 10);
  OLED::status(1, s);
}
