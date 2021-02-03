#include <Arduino.h>
#include "Util.h"
#include "buttons.h"

namespace Buttons
{
    void setup()
    {
        for (int i = 5; i < 10; i++)
            pinMode(i, INPUT_PULLUP);
    }

    void loop()
    {
        static uint8_t btnLast[5] = {1, 1, 1, 1, 1};
        for (int i = 5; i < 10; i++)
        {
            uint8_t btn = digitalRead(i);
            if (btn != btnLast[i - 5])
            {
                dbgprintf("button %d %d\n", 10 - i, 1 - btn);
                btnLast[i - 5] = btn;
            }
        }
    }
} // namespace Buttons