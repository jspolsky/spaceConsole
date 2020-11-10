#pragma once

namespace Led
{
    void setup();
    void loop();
    void setBrightness(uint8_t brightness);
    uint16_t getFPS();
    bool togglePower();
    uint8_t brighter();
    uint8_t dimmer();
    void testPattern();
    void setSolidColor(CRGB);

    void fnOneSecondSweep();
    void fnMondrian();
    void fnPaletteTester();
}; // namespace Led