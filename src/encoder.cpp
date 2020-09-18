#include <Arduino.h>
#include "encoder.h"

namespace Encoder
{

    const int PIN_CS = 10;
    const int PIN_SCK = 13;
    const int PIN_MISO = 12;

    void setup()
    {

        pinMode(PIN_CS, OUTPUT);
        pinMode(PIN_SCK, OUTPUT);
        pinMode(PIN_MISO, INPUT);

        digitalWrite(PIN_SCK, HIGH);
        digitalWrite(PIN_CS, HIGH);
    }

    // this noop loop takes about 250 ns, as measured on Teensy 4.1.

    void noop()
    {
        for (uint32_t i = 0; i < 50; i++)
            __asm__("nop\n\t");
    }

    void loop(uint32_t &absolute)
    {
        static uint32_t lastVals[8] = {0, 0, 0, 0, 0, 0, 0, 0};

        bool validResultFound = false;
        int pos = 0;

        bool bits[6];

        while (!validResultFound)
        {
            digitalWrite(PIN_CS, LOW);

            pos = 0;
            int csm = 0;

            for (int i = 0; i < 10; i++)
            {

                digitalWrite(PIN_SCK, LOW);
                noop();
                noop();
                noop();
                digitalWrite(PIN_SCK, HIGH);
                noop();
                noop();
                noop();

                bool bit = digitalRead(PIN_MISO) ? 1 : 0;

                noop();
                noop();

                csm += bit; // keep count for checksum

                pos <<= 1;
                pos |= bit;
            }

            for (int i = 0; i < 6; i++)
            {

                digitalWrite(PIN_SCK, LOW);
                noop();
                noop();
                noop();

                digitalWrite(PIN_SCK, HIGH);
                noop();
                noop();
                noop();

                bits[i] = digitalRead(PIN_MISO);
                csm += bits[i];

                noop();
                noop();
            }

            digitalWrite(PIN_SCK, LOW);
            digitalWrite(PIN_SCK, HIGH);

            if ((csm & 1) == 0)
            {
                validResultFound = true;
                for (int i = 0; i < 7; i++)
                    lastVals[i] = lastVals[i + 1];
                lastVals[7] = pos;
            }

            digitalWrite(PIN_CS, HIGH);
        }
        uint32_t sum = 0;
        for (int i = 0; i < 8; i++)
            sum += lastVals[i];
        absolute = (sum / 8);
    }
}; // namespace Encoder