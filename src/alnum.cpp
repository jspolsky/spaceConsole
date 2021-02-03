#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

#include "alnum.h"

namespace Alnum
{
    Adafruit_AlphaNum4 alpha4[4];

    void setup()
    {
        for (int i = 0; i < 4; i++)
        {
            (alpha4[i] = Adafruit_AlphaNum4())
                .begin(0x70 + i);
        }
    }

    void loop()
    {
    }

    void writeString(const char *s)
    {
        int i = 0;
        while (i < 16 && *s)
        {
            alpha4[i / 4].writeDigitAscii(i % 4, *s);
            i++;
            s++;
        }
        while (i < 16)
        {
            alpha4[i / 4].writeDigitAscii(i % 4, ' ');
            i++;
        }

        for (int q = 0; q < 4; q++)
        {
            alpha4[q].writeDisplay();
        }
    }

}; // namespace Alnum