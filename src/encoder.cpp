#include <Arduino.h>
#include "encoder.h"

const int PIN_CS = 10;
const int PIN_SCK = 13;
const int PIN_MISO = 12;

void encoder_setup()
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

uint32_t encoder_loop()
{
    bool validResultFound = false;
    int pos = 0;

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

            int bit = digitalRead(PIN_MISO);

            noop();
            noop();

            csm += bit; // keep count for checksum

            pos <<= 1;
            pos |= bit;
        }

        int bits[6];

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
        }

        digitalWrite(PIN_CS, HIGH);
    }
    return pos;
}
