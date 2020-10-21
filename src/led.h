#pragma once

namespace Led
{
    void setup();
    void loop();
    void setBrightness(uint8_t brightness);
    uint16_t getFPS();
    void nextPattern();

    void fnOneSecondSweep();
    void fnMondrian();
    void fnPaletteTester();
}; // namespace Led