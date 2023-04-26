#include <remote.h>
#include <Util.h>
#define SUPPRESS_ERROR_MESSAGE_FOR_BEGIN
#include <IRremote.h>

#define pinIRReceiver 12

namespace Remote
{
    IRrecv irrecv(pinIRReceiver);
    
    // if loop() returns true, check these two values:
    unsigned long lastResult;   // which button was pressed
    bool fRepeat;               // whether it was an autorepeat caused by holding it down

    void setup()
    {
        IrReceiver.begin(pinIRReceiver, ENABLE_LED_FEEDBACK);
    }

    bool loop()
    {

        if (IrReceiver.decode())
        {
            if (IrReceiver.decodedIRData.address != 0 ||
                IrReceiver.decodedIRData.command != 0) {

                dbgprintf("IR 0x%x\n", IrReceiver.decodedIRData.command);

                lastResult = IrReceiver.decodedIRData.command;
                fRepeat = IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT;
            }

            IrReceiver.resume();
            return true;
        }
        else
        {
            return false;
        }
    }

} // namespace Remote
