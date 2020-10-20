#pragma once

namespace Led
{
    void setup();
    void loop();
    void setBrightness(uint8_t brightness);
    uint16_t getFPS();
}; // namespace Led