#pragma once

#include <OctoWS2811.h>
#include <FastLED.h>
#include <Arduino.h>
#include <Util.h>

template <EOrder RGB_ORDER = RGB,
          uint8_t CHIP = WS2811_800kHz>
class CTeensy4Controller : public CPixelLEDController<RGB_ORDER, 8, 0xFF>
{
    OctoWS2811 *pocto;
    uint8_t *drawingMemory;

public:
    CTeensy4Controller(OctoWS2811 *_pocto, int *_drawingMemory)
        : pocto(_pocto),
          drawingMemory((uint8_t *)_drawingMemory){

          };

    virtual void init() {}
    virtual void showPixels(PixelController<RGB_ORDER, 8, 0xFF> &pixels)
    {

        // uint8_t *p = drawingMemory;
        // while (pixels.has(1))
        // {
        //     *p++ = pixels.loadAndScale0();
        //     *p++ = pixels.loadAndScale1();
        //     *p++ = pixels.loadAndScale2();
        //     pixels.stepDithering();
        //     pixels.advanceData();
        // }

        uint32_t i = 0;
        while (pixels.has(1))
        {
            uint8_t r = pixels.loadAndScale0();
            uint8_t g = pixels.loadAndScale1();
            uint8_t b = pixels.loadAndScale2();
            pocto->setPixel(i++, r, g, b);
            pixels.stepDithering();
            pixels.advanceData();
        }

        pocto->show();
    }
};
