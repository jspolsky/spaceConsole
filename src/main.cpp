
#include <Arduino.h>
#include "led.h"
#include "encoder.h"

void setup()
{

  Serial.begin(512000);
  while (!Serial)
    ;

  led_setup();
  encoder_setup();
}

void loop()
{
  uint32_t encoder_pos = encoder_loop();
  led_loop(encoder_pos);
}
