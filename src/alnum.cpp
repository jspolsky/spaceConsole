#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

#include "Util.h"
#include "alnum.h"

namespace Alnum
{
    Adafruit_AlphaNum4 alpha4[4];
    char sMain[17], sSecondary[17];
    unsigned long otm_start_time = 0L;
    unsigned long main_start_time = 0L;
    unsigned long secondary_start_time = 0L;

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
        // IF
        //      we are in a one time message AND
        //      the one time message has been visible for 
        //      more than 2 seconds
        // THEN
        //      show the main message
        //      forget about the one time message
        if (otm_start_time) {
            if (millis() > otm_start_time + 2000L) {
                writeString(sMain);
                otm_start_time = 0;
                main_start_time = millis();
            }
        }

        // IF
        //      we are showing the main message
        // AND
        //      we have now been showing it for for than
        //      4 seconds
        // THEN
        //      show the secondary message for a second
        if (main_start_time) {
            if (millis() > main_start_time + 4000L) {
                writeString(sSecondary);
                secondary_start_time = millis();
                main_start_time = 0L;
            }
        }

        // IF
        //      we are showing the secondary message
        // AND
        //      we have now been showing it for for than
        //      1 second
        // THEN
        //      show the main message again
        if (secondary_start_time) {
            if (millis() > secondary_start_time + 1000L) {
                writeString(sMain);
                secondary_start_time = 0L;
                main_start_time = millis();
            }
        }

    }

    // The message that is shown 80% of the time
    //
    void setMainMessage(const char* s) {
        strcpy(sMain, s);
        dbgprintf("Main message is %s\n", s);
        if (!otm_start_time) {
            writeString(sMain);
            main_start_time = millis();
        }
    }

    // The message that is shown 20% of the time
    //
    void setSecondaryMessage(const char* s) {
        strcpy(sSecondary, s);
        dbgprintf("Secondary message is %s\n", s);
    }

    // A quick message that is shown for 2 seconds
    //
    void showOneTime(const char* s) {
        dbgprintf("One time message is %s\n", s);
        
        writeString(s);
        otm_start_time = millis();
        main_start_time = 0L;
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