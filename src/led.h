#pragma once

namespace Led
{
    void setup();
    void loop();
    void setBrightness(uint8_t brightness);
    uint16_t getFPS();

    void fnOneSecondSweep();
    void fnMondrian();
    void fnPaletteTester();
}; // namespace Led