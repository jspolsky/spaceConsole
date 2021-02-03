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

    // returns 0 if nothing happened or 1-5 if a button has just been pressed
    int loop()
    {
        static uint8_t btnLast[5] = {1, 1, 1, 1, 1};
        for (int i = 5; i < 10; i++)
        {
            uint8_t btn = digitalRead(i);
            if (btn != btnLast[i - 5])
            {
                btnLast[i - 5] = btn;
                if (btn == 0)
                    return (10 - i);
            }
        }

        return 0;
    }
} // namespace Buttons