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
    void fnThrobBrightness();
    void geneticAlgorithm();
    void testPattern();
    void setSolidColor(CRGB);
    void Mirror4Strips();
    void setButtonStatus(int);

    void fnNoise();
    void fnOneSecondSweep();
    void fnMondrian();
    void fnTestPattern();
}; // namespace Led